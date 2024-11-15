/* Enable Unicode for optimal NT/XP performance */
// #define UNICODE

#include <windows.h>
#include <commctrl.h>
#include "play_ds.h"

double scale = 3.1 * VU_NUM_BARS / 16;

#ifdef __GNUC__
	/*
	   Looks like we're using GNU C.
	   Place GCC specific stuff here.

	   int rescale(int x){ return ( (!x) ? 0 : (scale * log10(x)) ); }
	      Let's try to optimize the above function
	      using some inline assembly code.
	*/
	int rescale(int x){
		asm(	"test %%eax,%%eax\n\t"
			"pushl %%eax\n\t"
			"jz rescale_R\n\t"
			"fldlg2\n\t"
			"fildl (%%esp)\n\t"
			"fyl2x\n\t"
			"fldl (_scale)\n\t"
			"fmulp %%st,%%st(1)\n\t"
			"fistpl (%%esp)\n\t"
			"rescale_R:\n\t"
			"popl %%eax"
			: "=a" (x)
			: "a" (x)
		);
		return x;
	}
#else
	/*
	   Looks like we're using Visual C.
	   Place MSVC specific stuff here.

	   The very usage of floating point arithmetics makes
	   the VC compiler refer to the CRT library. The
	   following declaration prevents the CRT from being
	   linked to our exe. This makes the exe a bit smaller.
	*/
	int _fltused;
	/*
	   int rescale(int x){ return ( (!x) ? 0 : (scale * log10(x)) ); }
	      Let's try to optimize the above function
	      using some inline assembly code.
	*/
	int __declspec(naked) __fastcall rescale(int x){
		__asm{
			test ecx,ecx
			push ecx
			jz rescale_R
			fldlg2
			fild DWORD PTR [esp]
			fyl2x
			fld QWORD PTR [scale]
			fmulp st(1),st(0)
			fistp DWORD PTR [esp]
	rescale_R:
			pop eax
			ret
		}
	}
#endif

typedef struct{
	BITMAPINFOHEADER bmiHeader;
	unsigned int bmiColors[6];
} VU_BITMAPINFO;

long int __stdcall WndProc(HWND,UINT,WPARAM,LPARAM);
void vol_fade(unsigned int);
void Play(HWND, CHR*);
void Stop(HWND);
void SetEffects(HWND,int);
void UpdateEffects(int);
int DS_init(HWND);

/* Structure used to pick an XM file name through GetOpenFileName. */
CHR buffer[640], szTtl[256];
const CHR szFilter[] = { 'X','M',' ','f','i','l','e','s',' ','(','*','.','x','m',')',0,'*','.','x','m',0,0 };
OPENFILENAME openfile = {
		sizeof(OPENFILENAME), 0, 0, szFilter,
		0, 0, 0, buffer, sizeof(buffer)/sizeof(CHR) - 1, szTtl, sizeof(szTtl) / sizeof(CHR) - 1,
		0, 0, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
		0, 0, 0, 0, 0, 0
	};

/* Main window class. */
const CHR szXMPlayerCLS[] = { 'c','l','s','X','M','P','l','a','y','e','r',0 };
WNDCLASSEX wndclex = {
		sizeof(WNDCLASSEX), 0, &WndProc,
		0, 0, 0, 0, 0, 0,
		0, szXMPlayerCLS, 0
	};

/* 3 buttons in toolbar. */
TBBUTTON tbbtn[] = {
		{0, IDC_PLAY,   TBSTATE_ENABLED, 0,             0, 0},
		{2, IDC_PAUSE,  0,               TBSTYLE_CHECK, 0, 0},
		{3, IDC_MUTING, 0,               TBSTYLE_CHECK, 0, 0}
	};

/* VU meter bitmap bits */
unsigned char dibits[VU_WIDTH * 14];
VU_BITMAPINFO pbmi = {
	{
		sizeof(BITMAPINFOHEADER), /* biSize                           */
		VU_WIDTH, 28,             /* biWidth, biHeight                */
		1, 4,                     /* biPlanes, biBitCount             */
		BI_RGB,                   /* biCompression                    */
		sizeof(dibits),           /* biSizeImage                      */
		0, 0,                     /* biXPelsPerMeter, biYPelsPerMeter */
		6, 6                      /* biClrUsed, biClrImportant        */
	},
	0x808080,
	0,
	0xC0C0C0, /* silver */
	0x7FFF00, /* green  */
	0xFFFF00, /* yellow */
	0xFF0000  /* red    */
};

/* Pointers to DirectSound interfaces. */
IDirectSoundBuffer *lpDSBuffer;
IDirectSound *lpDS;
IDSFX *lpDSFX;

unsigned int actual_volume = uFMOD_DEFAULT_VOL, nxt_timer, old_useconds;
int stopped, paused, muting, width, height, isDX_old;
RECT vu_rc;
unsigned int vol_R, vol_L, LR_rc_x, LR_rc_y, time_rc_top, time_rc_left;
HBITMAP hBMP, hVU;
HBRUSH hBrBG;
HFONT hFnt;
HWND hTBar, hVolm, hEff0, hEff1, hEff2, hBuddy_left, hParam, hBuddy_right, hFocus;
HINSTANCE hInstance;
HANDLE hDSoundLib;
CHR szCurTime[]          = { '0','0',':','0','0',':','0','0' };
const CHR szDSOUND[]     = { 'd','s','o','u','n','d',0 };
const CHR szDefaultTtl[] = { 'X','M',' ','P','l','a','y','e','r',0 };
const CHR szL[]          = { 'L' };
const CHR szR[]          = { 'R' };
const CHR szEmpty[]      = { 0 };
const CHR szErr1[]       = { 'D','i','r','e','c','t','S','o','u','n','d',' ','e','r','r','o','r',0 };

void FillVUBars(unsigned int volume, unsigned char *pdibits){
unsigned int i, cColor, *dest, *src;
unsigned char *p_dibits;
	/* Fill 1 scan line */
	p_dibits = pdibits;
	for(i = 0; i < VU_NUM_BARS; i++){
		cColor = 0x33;     /* green  */
		if(i >= volume)
			cColor = 0x22; /* silver */
		else if(i >= VU_NUM_BARS * 3 / 4)
			cColor = 0x55; /* red    */
		else if(i >= VU_NUM_BARS / 2)
			cColor = 0x44; /* yellow */
		*p_dibits++ = cColor & 0x0F;
		*p_dibits++ = cColor;
		*p_dibits++ = cColor & 0xF0;
		*p_dibits++ = 0x11;
	}
	/* Fill 10 more scan lines */
	src = (unsigned int*)pdibits;
	dest = src + VU_NUM_BARS;
	for(i = 0; i < VU_NUM_BARS;     i++) *dest++ = *src++;
	src = (unsigned int*)pdibits;
	for(i = 0; i < VU_NUM_BARS * 2; i++) *dest++ = *src++;
	src = (unsigned int*)pdibits;
	for(i = 0; i < VU_NUM_BARS * 4; i++) *dest++ = *src++;
	src = (unsigned int*)pdibits;
	for(i = 0; i < VU_NUM_BARS * 3; i++) *dest++ = *src++;
}

/* Draw VU meter */
void RedrawVU(HDC hDC){
HDC hMemDC = CreateCompatibleDC(hDC);
HBITMAP hOldBmp;
	/* R channel: */
	FillVUBars(vol_R, dibits + VU_WIDTH / 2);
	/* L channel: */
	FillVUBars(vol_L, dibits + VU_WIDTH * 8);
	SetDIBits(hDC, hVU, 0, 28, dibits, (void*)&pbmi, DIB_RGB_COLORS);
	hOldBmp = SelectObject(hMemDC, hVU);
	BitBlt(hDC, vu_rc.left, 1, VU_WIDTH - 2, 28, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBmp);
	DeleteDC(hMemDC);
}

/* Draw playback time indicator. */
#ifdef BENCHMARK
	CHR szTSC[10], szFmt[] = { '%','8','d',0 };
#endif
void RedrawTimeP(HDC hDC){
HFONT hFntOld = SelectObject(hDC, hFnt);
	SetBkColor(hDC, pbmi.bmiColors[1]);
	/* HH:MM:SS */
	SetTextColor(hDC, 0x404040);
	TextOut(hDC, time_rc_left, time_rc_top, szCurTime, 8);
	/* TSC indicator. */
#ifdef BENCHMARK
	SetTextColor(hDC, 0x000080);
	wsprintf(szTSC, szFmt, uFMOD_tsc >> 10); /* divide the TSC by 1024 */
	TextOut(hDC, time_rc_left, 16, szTSC, 8);
#endif
	SelectObject(hDC, hFntOld);
}

void start(void){
SIZE size;
MSG msg;
HDC hDC;
HWND hW;
HFONT hFntOld;
int x, y;
unsigned int uaux, useconds, uminutes, uhours, vol_newL, vol_newR, *p_dibits;
CHR *cur, *ttl;
	/* Initialize OLE COM. */
	CoInitialize(0);
	/* Center the main window on screen. */
	hFnt = GetStockObject(ANSI_FIXED_FONT);
	hDC = GetDC(0);
	pbmi.bmiColors[1] = (GetDeviceCaps(hDC, COLORRES) < 16) ? 0xC0C0C0 : BG_COLOR;
	hFntOld = SelectObject(hDC, hFnt);
	GetTextExtentPoint32(hDC, szCurTime, 8, &size);
	SelectObject(hDC, hFntOld);
	/* VU meter bitmap. */
	hVU = CreateCompatibleBitmap(hDC, VU_WIDTH, 28);
	ReleaseDC(0, hDC);
	/* Draw the grid */
	p_dibits = (unsigned int*)dibits;
	for(x = 0; x < VU_NUM_BARS;     x++) *p_dibits++ = 0x11000000;
	p_dibits += VU_NUM_BARS * 11;
	for(x = 0; x < VU_NUM_BARS;     x++) *p_dibits++ = 0x11000000;
	for(x = 0; x < VU_NUM_BARS * 2; x++) *p_dibits++ = 0x11111111;
	for(x = 0; x < VU_NUM_BARS;     x++) *p_dibits++ = 0x11000000;
	p_dibits += VU_NUM_BARS * 11;
	for(x = 0; x < VU_NUM_BARS;     x++) *p_dibits++ = 0x11000000;
	/* Playback time bounding rect. */
	time_rc_left   = 156;
#ifdef BENCHMARK
	time_rc_top    = 2;
#else
	time_rc_top    = (30 - size.cy) >> 1;
#endif
	/* "L" & "R" labels coords. */
	LR_rc_x        = time_rc_left + size.cx + 8;
	LR_rc_y        = (16 - size.cy) >> 1;
	/* VU bounding rect. */
	vu_rc.left     = LR_rc_x + size.cx / 8 + 4;
	vu_rc.top      = 1;
	vu_rc.right    = vu_rc.left + VU_WIDTH;
	vu_rc.bottom   = 29;
	width = vu_rc.left + VU_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2;
	height = 30 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME) * 2;
	x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	/* Fill out the main window class structure. */
	hInstance = GetModuleHandle(0);
	wndclex.hIcon = LoadIcon(hInstance, (CHR*)MAINICON);
	wndclex.hCursor = LoadCursor(0, IDC_ARROW);
	hBrBG = CreateSolidBrush(pbmi.bmiColors[1]);
	wndclex.hbrBackground = hBrBG;
	if(RegisterClassEx(&wndclex)){
		/* Create the main window. */
		hW = CreateWindowEx(WS_EX_ACCEPTFILES, szXMPlayerCLS, szDefaultTtl,
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX,
				x, y, width, height, 0, 0, hInstance, 0);
		if(hW){
			msg.message = 0;
			do{
				/* Process some messages. */
				while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
					if(!IsDialogMessage(hW, &msg))
						DispatchMessage(&msg);
				/* Update playback time. */
				useconds = uFMOD_GetTime() / 1000;
				if(useconds != old_useconds){
					if(useconds >= nxt_timer){
						/* Show file name in window caption. */
						ttl = uFMOD_GetTitle();
						cur = ttl;
						uaux = 0;
						/* If it contains only spaces, assume no real title. */
						while(*cur && *cur++ != ' ') uaux++;
						if(!uaux) ttl = szTtl;
						SetWindowText(hW, ttl);
						nxt_timer = 0xFFFFFFFF;
					}
					old_useconds = useconds;
					uminutes = useconds / 60;
					useconds = useconds % 60;
					uhours = uminutes / 60 % 24;
					uminutes = uminutes % 60;
					cur = szCurTime;
					*cur++ = uhours / 10 + '0';
					*cur++ = uhours % 10 + '0';
					cur++;
					*cur++ = uminutes / 10 + '0';
					*cur++ = uminutes % 10 + '0';
					cur++;
					*cur++ = useconds / 10 + '0';
					*cur   = useconds % 10 + '0';
					hDC = GetDC(hW);
					RedrawTimeP(hDC);
					ReleaseDC(hW, hDC);
				}
				/* Update VU meter. */
				uaux = uFMOD_GetStats();
				vol_newR = rescale(uaux & 0xFFFF);
				vol_newL = rescale(uaux >> 16);
				if(vol_newR != vol_R || vol_newL != vol_L){
					vol_R = vol_newR;
					vol_L = vol_newL;
					hDC = GetDC(hW);
					RedrawVU(hDC);
					ReleaseDC(hW, hDC);
				}
				/* Doze off to let the CPU cool down ;-) */
				Sleep(5);
			}while(msg.message != WM_QUIT);
			DeleteObject(hBMP);
		}
	}
	DeleteObject(hVU);
	/* Release the DirectSound instance. */
	if(lpDS) IDirectSound_Release(lpDS);
	CoUninitialize();
	if(hDSoundLib) FreeLibrary(hDSoundLib);
	ExitProcess(0);
}

/* Main window message processing function. */
long int __stdcall WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
COLORMAP cmap;
TBADDBITMAP tbadd;
PAINTSTRUCT paint;
HDC hDC;
HFONT hFntOld;
HPEN hPenOld;
unsigned int uaux;
CHR *cur;
	switch(uMsg){
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &paint);
			/* Draw VU meter.                */
			RedrawVU(hDC);
			/* Draw playback time indicator. */
			RedrawTimeP(hDC);
			/* Draw "L" & "R" labels.        */
			hFntOld = SelectObject(hDC, hFnt);
			SetBkColor(hDC, pbmi.bmiColors[1]);
			SetTextColor(hDC, 0x404040);
			TextOut(hDC, LR_rc_x, LR_rc_y, szL, 1);
			TextOut(hDC, LR_rc_x, LR_rc_y + 15, szR, 1);
			SelectObject(hDC, hFntOld);
			/* Draw some static separators. */
			if(!isDX_old){
				hPenOld = SelectObject(hDC, CreatePen(PS_SOLID, 1, 0xC0C0C0));
				MoveToEx(hDC, 0, 32, 0);
				LineTo(hDC, width, 32);
				DeleteObject(SelectObject(hDC, GetStockObject(WHITE_PEN)));
				MoveToEx(hDC, 0, 33, 0);
				LineTo(hDC, width, 33);
				SelectObject(hDC, hPenOld);
			}
			EndPaint(hWnd, &paint);
			break;
		case WM_CTLCOLORSTATIC:
			SetBkColor((HDC)wParam, pbmi.bmiColors[1]);
			return (long)hBrBG;
		case WM_CREATE:
			/* Get a DirectSound instance. */
			if(DS_init(hWnd)){
				MessageBox(0, szErr1, 0, MB_ICONSTOP | MB_SYSTEMMODAL);
				return -1; /* destroy the window */
			}
			openfile.hwndOwner = hWnd;
			/* Create some child windows. */
			hTBar = CreateWindowEx(WS_EX_NOPARENTNOTIFY, TOOLBARCLASSNAME, szEmpty,
					WS_TABSTOP | WS_CHILD | TBSTYLE_TOOLTIPS | CCS_NODIVIDER |
					CCS_NOPARENTALIGN | CCS_NORESIZE | WS_VISIBLE | 0x800,
					4, 4, 70, 24, hWnd, (HMENU)ID_TBAR, hInstance, 0);
			SendMessage(hTBar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
			cmap.from = 0xFF00FF;
			cmap.to = GetSysColor(COLOR_BTNFACE);
			hBMP = CreateMappedBitmap(hInstance, ID_BMP, 0, &cmap, 1);
			tbadd.hInst = 0;
			tbadd.nID = (int)hBMP;
			SendMessage(hTBar, TB_ADDBITMAP,  5, (long)&tbadd);
			SendMessage(hTBar, TB_ADDBUTTONS, 3, (long)&tbbtn);
			hVolm = CreateWindowEx(WS_EX_NOPARENTNOTIFY, TRACKBAR_CLASS, szEmpty,
					WS_TABSTOP | WS_CHILD | WS_VISIBLE | TBS_NOTICKS,
					74, 4, 78, 24, hWnd, (HMENU)ID_VOLUME, hInstance, 0);
			SendMessage(hVolm, TBM_SETRANGE,    0, 0x640000);
			SendMessage(hVolm, TBM_SETPAGESIZE, 0, 5);
			SendMessage(hVolm, TBM_SETPOS,      1, 100);
			/* Enable Drag&Drop support. */
			DragAcceptFiles(hWnd, 1);
			break;
		case WM_CLOSE:
			uFMOD_StopSong();
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_DROPFILES:
			Stop(hWnd);
			/* Get the recently dropped file name. */
			DragQueryFile((HANDLE)wParam, 0, buffer, sizeof(buffer) / sizeof(CHR) - 1);
			DragFinish((HANDLE)wParam);
			/* Play it. */
			Play(hWnd, buffer);
			break;
		case WM_COMMAND:
			switch(wParam){
				case ID_EFF0:
				case ID_EFF1:
				case ID_EFF2:
					SetEffects(hWnd, wParam);
					break;
				case IDC_PLAY:
					buffer[0] = 0;
					if(GetOpenFileName(&openfile)){
						/*
						   Strip the file extension, if any.
						   (filename without extension is
						   displayed in the caption area if
						   the current song has no title.)
						*/
						cur = szTtl;
						while(*cur) cur++;
						while(cur > szTtl && *cur != '.') cur--;
						if(cur > szTtl) *cur = 0;
						/* Play the selected file. */
						Play(hWnd, buffer);
					}
					break;
				case IDC_STOP:
					Stop(hWnd);
					break;
				case IDC_PAUSE:
					if(!paused){
						vol_fade(actual_volume >> 1); /* Fade before pausing. */
						uFMOD_Pause();
					}else{
						uFMOD_Resume();
						if(!muting) vol_fade(actual_volume); /* Restore volume. */
					}
					paused ^= 1;
					break;
				case IDC_MUTING:
					muting = SendMessage(hTBar, TB_ISBUTTONCHECKED, IDC_MUTING, 0);
					if(muting){
						/* Sound off (muting) */
						vol_fade(0);
						SendMessage(hTBar, TB_CHANGEBITMAP, IDC_MUTING, 4);
					}else{
						/* Sound on. */
						uaux = SendMessage(hVolm, TBM_GETPOS, 0, 0);
						actual_volume = uaux * uFMOD_MAX_VOL / 100;
						vol_fade(actual_volume);
						cur = buffer;
						/* VOL: nn% */
						*cur++ = 'V';
						*cur++ = 'O';
						*cur++ = 'L';
						*cur++ = ':';
						*cur++ = ' ';
						if(uaux / 100) *cur++ = uaux / 100 + '0';
						*cur++ = uaux % 100 / 10 + '0';
						*cur++ = uaux % 100 % 10 + '0';
						*cur++ = '%';
						*cur = 0;
						SetWindowText(hWnd, buffer);
						SendMessage(hTBar, TB_CHANGEBITMAP, IDC_MUTING, 3);
						/* Restore the caption in about 2 sec. */
						nxt_timer = old_useconds + 2;
					}
			}
			break;
		case WM_HSCROLL:
			if((HWND)lParam == hParam)
				/* Update sound effects settings. */
				UpdateEffects(SendMessage(hParam, TBM_GETPOS, 0, 0));
			else{
				/* +/- volume. */
				SendMessage(hTBar, TB_CHECKBUTTON, IDC_MUTING, 0);
				SendMessage(hWnd, WM_COMMAND, IDC_MUTING, 0);
			}
			break;
		case WM_ACTIVATE:
			if((wParam & 0xFFFF) == WA_INACTIVE) hFocus = GetFocus();
			else{
				if(!hFocus) hFocus = hVolm;
				SetFocus(hFocus);
			}
			break;
		case WM_NOTIFY:
			/* Manage tooltips. */
			if((*(NMHDR*)lParam).code == TTN_NEEDTEXT){
				(*(TOOLTIPTEXT*)lParam).hinst = hInstance;
				(*(TOOLTIPTEXT*)lParam).lpszText = (CHR*)(*(TOOLTIPTEXT*)lParam).hdr.idFrom;
				break;
			}
		default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

/* Volume fade. */
unsigned int current_volume = uFMOD_DEFAULT_VOL;
void vol_fade(unsigned int new_volume){
int delta = (current_volume > new_volume) ? -1 : 1;
	while(current_volume != new_volume){
		current_volume += (unsigned int)delta;
		uFMOD_SetVolume(current_volume);
		Sleep(5);
	}
}

/* Start playing an XM file. */
const CHR szErr0[] = { 'U','n','a','b','l','e',' ','t','o',' ','s','t','a','r','t',' ','p','l','a','y','b','a','c','k',0 };
void Play(HWND hWnd, CHR* filename){
	if(uFMOD_DSPlaySong(filename, 0, XM_FILE, lpDSBuffer) < 0)
		MessageBox(hWnd, szErr0, 0, MB_ICONSTOP);
	else{
		stopped = 0;
		/* Make the LOAD button become STOP. */
		SendMessage(hTBar, TB_CHANGEBITMAP, IDC_PLAY, 1);
		SendMessage(hTBar, TB_SETCMDID, 0, IDC_STOP);
		SendMessage(hTBar, TB_ENABLEBUTTON, IDC_PAUSE, 1);
		SendMessage(hTBar, TB_ENABLEBUTTON, IDC_MUTING, 1);
		/* Update window caption ASAP. */
		nxt_timer = 0;
	}
}

/* Stop playback. */
void Stop(HWND hWnd){
	/* Fade before stopping. */
	vol_fade(0);
	uFMOD_StopSong();
	stopped = 1;
	/* Restore volume. */
	current_volume = actual_volume;
	uFMOD_SetVolume(actual_volume);
	/* Make the STOP button become LOAD. */
	SendMessage(hTBar, TB_CHANGEBITMAP, IDC_STOP, 0);
	SendMessage(hTBar, TB_SETCMDID, 0, IDC_PLAY);
	paused = 0;
	SendMessage(hTBar, TB_SETSTATE, IDC_PAUSE, 0);
	SendMessage(hTBar, TB_ENABLEBUTTON, IDC_MUTING, 0);
	/* Reset window caption to default. */
	SetWindowText(hWnd, szDefaultTtl);
}

/* Set / remove sound effects. */
GUID ID_DSFX_STANDARD_GARGLE   = { 0xDAFD8210,0x5711,0x4B91,0x9F,0xE3,0xF7,0x5B,0x7A,0xE2,0x79,0xBF };
GUID ID_IDirectSoundFXGargle8  = { 0xD616F352,0xD622,0x11CE,0xAA,0xC5,0x00,0x20,0xAF,0x0B,0x99,0xA3 };
GUID ID_DSFX_STANDARD_PARAMEQ  = { 0x120CED89,0x3BF4,0x4173,0xA1,0x32,0x3C,0xB4,0x06,0xCF,0x32,0x31 };
GUID ID_IDirectSoundFXParamEq8 = { 0xC03CA9FE,0xFE90,0x4204,0x80,0x78,0x82,0x33,0x4C,0xD1,0x77,0xDA };
int current_effect;
const CHR szM15dB[] = { '-','1','5','d','B',0 };
const CHR szP15dB[] = { '+','1','5','d','B',0 };
const CHR sz1Hz[]   = { '1','H','z',0 };
const CHR sz1KHz[]  = { '1','K','H','z',0 };
void SetEffects(HWND hWnd,int effect){
DSEFFECTDESC dsEffect;
int dwAux, i = 0, new_height = height;
	/* Always stop playback before applying effects! */
	if(!stopped){
		vol_fade(actual_volume >> 1);
		IDirectSoundBuffer_Stop(lpDSBuffer);
	}
	SendMessage(hEff0, BM_SETCHECK, 0, 0);
	SendMessage(hEff1, BM_SETCHECK, 0, 0);
	SendMessage(hEff2, BM_SETCHECK, 0, 0);
	lpDSFX = 0;
	dsEffect.dwSize = sizeof(DSEFFECTDESC);
	dsEffect.dwFlags = 0;
	dsEffect.dwReserved1 = 0;
	dsEffect.dwReserved2 = 0;
	IDirectSoundBuffer8_SetFX((void*)lpDSBuffer, 0, 0, 0);
	current_effect = effect;
	switch(effect){
		case ID_EFF1:
			/* Apply the "equalizer" effect. */
			dsEffect.guidDSFXClass = ID_DSFX_STANDARD_PARAMEQ;
			if(IDirectSoundBuffer8_SetFX((void*)lpDSBuffer, 1, &dsEffect, &dwAux) >= 0){
				SendMessage(hEff1, BM_SETCHECK, 1, 0);
				if(IDirectSoundBuffer8_GetObjectInPath((void*)lpDSBuffer,
						&ID_DSFX_STANDARD_PARAMEQ, 0, &ID_IDirectSoundFXParamEq8, (void**)&lpDSFX) >= 0){
					SendMessage(hParam, TBM_SETRANGE,    0, 0x1E0001);
					SendMessage(hParam, TBM_SETPAGESIZE, 0, 5);
					SendMessage(hParam, TBM_SETPOS,      1, 15); /* default gain = 0.0f */
					SetWindowText(hBuddy_left,  szM15dB);
					SetWindowText(hBuddy_right, szP15dB);
					new_height += 28;
					i = 1;
				}
			}else
				SendMessage(hEff0, BM_SETCHECK, 1, 0);
			break;
		case ID_EFF2:
			/* Apply the "gargle" effect. */
			dsEffect.guidDSFXClass = ID_DSFX_STANDARD_GARGLE;
			if(IDirectSoundBuffer8_SetFX((void*)lpDSBuffer, 1, &dsEffect, &dwAux) >= 0){
				SendMessage(hEff2, BM_SETCHECK, 1, 0);
				if(IDirectSoundBuffer8_GetObjectInPath((void*)lpDSBuffer,
						&ID_DSFX_STANDARD_GARGLE, 0, &ID_IDirectSoundFXGargle8, (void**)&lpDSFX) >= 0){
					SendMessage(hParam, TBM_SETRANGE,    0, 0x3E80001);
					SendMessage(hParam, TBM_SETPAGESIZE, 0, 100);
					SendMessage(hParam, TBM_SETPOS,      1, 20); /* default rate of modulation */
					SetWindowText(hBuddy_left,  sz1Hz);
					SetWindowText(hBuddy_right, sz1KHz);
					new_height += 28;
					i = 1;
				}
				break;
			}
		default:
			SendMessage(hEff0, BM_SETCHECK, 1, 0);
	}
	/* Resume playback. */
	if(!stopped){
		IDirectSoundBuffer_Play(lpDSBuffer, 0, 0, DSBPLAY_LOOPING);
		vol_fade(actual_volume);
	}
	/* Show/hide extra param slider. */
	EnableWindow(hParam, i);
	SetWindowPos(hWnd, 0, 0, 0, width, new_height,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}

/* Update sound effects. */
DSFXGargle dsfx_gargle;
DSFXParamEq dsfx_parameq = { 80, 12 }; /* f = 80 Hz, BW = 12 semitones */
void UpdateEffects(int value){
void* dsfx;
	if(!lpDSFX) return;
	switch(current_effect){
		case ID_EFF1:
			dsfx = &dsfx_parameq;
			dsfx_parameq.fGain = (float)(value - 15);
			break;
		case ID_EFF2:
			dsfx = &dsfx_gargle;
			dsfx_gargle.dwRateHz = value;
			break;
		default: return;
	}
	IDSFX_SetAllParameters(lpDSFX, dsfx);
}

/* Recommended DirectSound settings: */

IDirectSoundBuffer* lpDSBufferPrimary;
DSBUFFERDESC dsbuffdesc = {
		sizeof(DSBUFFERDESC),
		DSBCAPS_PRIMARYBUFFER,
	};
WAVEFORMATEX pcm = {
		WAVE_FORMAT_PCM, /* linear PCM */
		2,               /* stereo     */
		uFMOD_MixRate,
		uFMOD_MixRate * 4,
		4,16             /* 16-bit     */
	};

/* Initialize DirectSound. */
CHR szSTATIC[]    = { 's','t','a','t','i','c',0 };
CHR szBUTTON[]    = { 'b','u','t','t','o','n',0 };
CHR szOff[]       = { 'O','f','f',0 };
CHR szEqualizer[] = { 'E','q','u','a','l','i','z','e','r',0 };
CHR szGargle[]    = { 'G','a','r','g','l','e',0 };
int DS_init(HWND hWnd){
LPDSCREATE DS_Create;
	/* Get a pointer to IDirectSound. */
	hDSoundLib = LoadLibrary(szDSOUND);
	if(!hDSoundLib) return -1;
	DS_Create = (LPDSCREATE)GetProcAddress(hDSoundLib, "DirectSoundCreate8");
	if(!DS_Create){
		DS_Create = (LPDSCREATE)GetProcAddress(hDSoundLib, (char*)1);
		if(!DS_Create) return -1;
		/* For compatibility with DirectX 7 and earlier. */
		dsbuffdesc.dwSize = 20;
		isDX_old++;
	}
	if(DS_Create(0, &lpDS, 0) < 0) return -1;
	/*
	   It is important to set the cooperative level to at least
	   DSSCL_PRIORITY prior to creating 16-bit stereo buffers.
	*/
	if(IDirectSound_SetCooperativeLevel(lpDS, hWnd, DSSCL_PRIORITY) < 0)
		return -1;
	/* Get the primary buffer. */
	if(IDirectSound_CreateSoundBuffer(lpDS, (void*)&dsbuffdesc, &lpDSBufferPrimary, 0) < 0)
		return -1;
	/* Set format to PCM / 48 KHz / 16-bit / stereo. */
	if(IDirectSoundBuffer_SetFormat(lpDSBufferPrimary, &pcm) < 0)
		return -1;
	/* Create the secondary buffer. */
	dsbuffdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 |
			/*
			   Some earlier DirectX versions didn't
			   track the current position correctly
			   when this flag is not specified.
			*/
			DSBCAPS_STICKYFOCUS;
			/*
			   Enable playback even if the main window
			   is not active, but currently active
			   window don't have exclusive access to
			   DirectSound.
			*/
	/* Enable sound effects. */
	if(!isDX_old) dsbuffdesc.dwFlags |= DSBCAPS_CTRLFX;
	dsbuffdesc.dwBufferBytes = uFMOD_BUFFER_SIZE;
	dsbuffdesc.lpwfxFormat = &pcm;
	if(IDirectSound_CreateSoundBuffer(lpDS, (void*)&dsbuffdesc, &lpDSBuffer, 0) < 0)
		return -1;
	if(!isDX_old){
		/* Create the sound effects controls. */
		hEff0 = CreateWindowEx(WS_EX_NOPARENTNOTIFY, szBUTTON, szOff,
				WS_GROUP | WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
				8 + 6, 40, 60, 16, hWnd, (HMENU)ID_EFF0, hInstance, 0);
		SendMessage(hEff0, BM_SETCHECK, 1, 0);
		hEff1 = CreateWindowEx(WS_EX_NOPARENTNOTIFY, szBUTTON, szEqualizer,
				WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
				8 + 68, 40, 106, 16, hWnd, (HMENU)ID_EFF1, hInstance, 0);
		hEff2 = CreateWindowEx(WS_EX_NOPARENTNOTIFY, szBUTTON, szGargle,
				WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
				8 + 174, 40, 74, 16, hWnd, (HMENU)ID_EFF2, hInstance, 0);
		hBuddy_left = CreateWindowEx(WS_EX_NOPARENTNOTIFY, szSTATIC, szEmpty,
				WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE | SS_NOPREFIX,
				8 + 8, 62, 50, 22, hWnd, (HMENU)IDS_BUDDY_L, hInstance, 0);
		hParam = CreateWindowEx(WS_EX_NOPARENTNOTIFY, TRACKBAR_CLASS, szEmpty,
				WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_DISABLED | TBS_NOTICKS,
				8 + 58, 62, 132, 24, hWnd, (HMENU)ID_PARAM, hInstance, 0);
		hBuddy_right = CreateWindowEx(WS_EX_NOPARENTNOTIFY, szSTATIC, szEmpty,
				WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP | SS_CENTERIMAGE | SS_NOPREFIX,
				8 + 190, 62, 66, 22, hWnd, (HMENU)IDS_BUDDY_R, hInstance, 0);
		SendMessage(hBuddy_left,  WM_SETFONT, (int)hFnt, 0);
		SendMessage(hBuddy_right, WM_SETFONT, (int)hFnt, 0);
		height += 30;
		SetWindowPos(hWnd, 0, 0, 0, width, height,
				SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	return 0;
}
