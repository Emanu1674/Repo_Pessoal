/*
  JMP2PAT.C
  ---------
  Sometimes it makes sense merging various XM tracks
  sharing the same instruments in a single XM file.
  This example program uses such an XM file actually
  containing 3 tracks and the uFMOD_Jump2Pattern()
  function to play all 3 tracks in the same file.

  BLITZXMK.XM tracked by Kim (aka norki):
    [00:07] - track #1
    [08:10] - track #2
    [11:13] - track #3

  Enable Unicode for optimal NT/XP performance */
// #define UNICODE

#ifdef UNICODE
	typedef unsigned short CHR;
#else
	typedef unsigned char CHR;
#endif

/* Common IDs */
#include "Jmp2Pat.h"

/* Win32 API */
#include <windows.h>

/* uFMOD (WINMM) */
#include "ufmod.h"

unsigned int paused, vals[] = { 0, 8, 11 }; /* Preset pattern indexes */
HINSTANCE hInst;

const CHR szPause[]  = { '&','P','a','u','s','e',0 };
const CHR szResume[] = { '&','R','e','s','u','m','e',0 };
const CHR szErr[]    = { 'U','n','a','b','l','e',' ','t','o',' ',
                         's','t','a','r','t',' ','p','l','a','y','b','a','c','k',0 };

int __stdcall DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_COMMAND:
			switch(wParam){
				case IDCANCEL: EndDialog(hDlg, 0);
				break;
				case IDB_PAUSE_RESUME:
					/* Pause/Resume playback */
					if(paused){
						uFMOD_Resume();
						SetDlgItemText(hDlg, IDB_PAUSE_RESUME, szPause);
					}else{
						uFMOD_Pause();
						SetDlgItemText(hDlg, IDB_PAUSE_RESUME, szResume);
					}
					paused ^= 1;
					break;
				default:
					/* Jump to a specific pattern index */
					uMsg = wParam - IDB_1;
					if(uMsg < 3) uFMOD_Jump2Pattern(vals[uMsg]);
			}
			return 1;
		case WM_INITDIALOG:
			PostMessage(hDlg, WM_SETICON, ICON_BIG, (unsigned int)LoadIcon(hInst, (CHR*)IDI_MAIN));
			return 1;
	}
	return 0;
}

void start(void){

	/* Start playback */
	if(uFMOD_PlaySong((char*)1, 0, XM_RESOURCE)){

		/* Load the GUI */
		hInst = GetModuleHandle(0);
		DialogBoxParam(hInst, (CHR*)IDD_MAIN, 0, &DlgProc, 0);

		/* Stop playback */
		uFMOD_StopSong();
	}else
		MessageBox(0, szErr, 0, MB_ICONSTOP);
	ExitProcess(0);
}
