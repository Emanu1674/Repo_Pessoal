/* Set to actual sampling rate in Hz. (48KHz for default uFMOD build.) */
#define uFMOD_MixRate 48000

#define BG_COLOR    0xDCDCDC /* background color */
#define VU_NUM_BARS 16       /* number of bars in VU meter */

/* VU meter width in pixels */
#define VU_WIDTH ((8 * VU_NUM_BARS + 1) & -2)

#include "dsufmod.h"

#ifdef UNICODE
	typedef unsigned short CHR;
#else
	typedef unsigned char CHR;
#endif

#define IDD_DIALOG_1 300
#define ID_TBAR      200
#define ID_VOLUME    201
#define IDS_INFO     202
#define ID_EFF0      203
#define ID_EFF1      204
#define ID_EFF2      205
#define IDS_BUDDY_L  206
#define ID_PARAM     207
#define IDS_BUDDY_R  208
#define MAINICON     400
#define ID_BMP       402

#define IDC_PLAY     300
#define IDC_STOP     301
#define IDC_PAUSE    302
#define IDC_MUTING   303

typedef long int (__stdcall *LPDSCREATE)(void*, void*, void*);

#undef INTERFACE
#define INTERFACE IDSFX
DECLARE_INTERFACE_(IDSFX, IUnknown){
	STDMETHOD(QueryInterface)   (THIS_ void*, void**) PURE;
	STDMETHOD_(ULONG,AddRef)    (THIS) PURE;
	STDMETHOD_(ULONG,Release)   (THIS) PURE;
	STDMETHOD(SetAllParameters) (THIS_ void*) PURE;
	STDMETHOD(GetAllParameters) (THIS_ void*) PURE;
};

#define IDSFX_SetAllParameters(p,a) p->lpVtbl->SetAllParameters(p,a)
#define IDSFX_GetAllParameters(p,a) p->lpVtbl->GetAllParameters(p,a)

#ifndef DSBCAPS_CTRLFX
	/*
	   DirectX 8 SDK or later not found.
	   So, need to define and even redefine some of the header data.
	   But it's always a better idea to update the SDK!
	*/
	#define DSBCAPS_CTRLFX 0x200

	typedef struct{
		int dwSize;
		int dwFlags;
		int dwBufferBytes;
		int dwReserved;
		WAVEFORMATEX* lpwfxFormat;
		GUID guid3DAlgorithm;
	} DSBUFFERDESC_DX8;
	#define DSBUFFERDESC DSBUFFERDESC_DX8

	typedef struct{
		int dwSize;
		int dwFlags;
		GUID guidDSFXClass;
		void* dwReserved1;
		void* dwReserved2;
	} DSEFFECTDESC;

	typedef struct{
		int dwRateHz;
		int dwWaveShape;
	} DSFXGargle;

	typedef struct{
		float fCenter;
		float fBandwidth;
		float fGain;
	} DSFXParamEq;

	#undef INTERFACE
	#define INTERFACE IDirectSoundBuffer_8
	DECLARE_INTERFACE_(IDirectSoundBuffer_8, IUnknown){
		STDMETHOD(QueryInterface)     (THIS_ void*, void**) PURE;
		STDMETHOD_(ULONG,AddRef)      (THIS) PURE;
		STDMETHOD_(ULONG,Release)     (THIS) PURE;
		STDMETHOD(GetCaps)            (THIS_ void*) PURE;
		STDMETHOD(GetCurrentPosition) (THIS_ int*, int*) PURE;
		STDMETHOD(GetFormat)          (THIS_ WAVEFORMATEX*, int, int*) PURE;
		STDMETHOD(GetVolume)          (THIS_ int*) PURE;
		STDMETHOD(GetPan)             (THIS_ int*) PURE;
		STDMETHOD(GetFrequency)       (THIS_ int*) PURE;
		STDMETHOD(GetStatus)          (THIS_ int*) PURE;
		STDMETHOD(Initialize)         (THIS_ void*, DSBUFFERDESC*) PURE;
		STDMETHOD(Lock)               (THIS_ int, int, void**, int*, void**, int*, int) PURE;
		STDMETHOD(Play)               (THIS_ int, int, int) PURE;
		STDMETHOD(SetCurrentPosition) (THIS_ int) PURE;
		STDMETHOD(SetFormat)          (THIS_ WAVEFORMATEX*) PURE;
		STDMETHOD(SetVolume)          (THIS_ int) PURE;
		STDMETHOD(SetPan)             (THIS_ int) PURE;
		STDMETHOD(SetFrequency)       (THIS_ int) PURE;
		STDMETHOD(Stop)               (THIS) PURE;
		STDMETHOD(Unlock)             (THIS_ void*, int, void*, int) PURE;
		STDMETHOD(Restore)            (THIS) PURE;
		STDMETHOD(SetFX)              (THIS_ int, DSEFFECTDESC*, int*) PURE;
		STDMETHOD(AcquireResources)   (THIS_ int, int, int*) PURE;
		STDMETHOD(GetObjectInPath)    (THIS_ GUID*, int, GUID*, void**) PURE;
	};

	#define IDirectSoundBuffer_Play(p,a,b,c) (*(*(IDirectSoundBuffer_8*)p).lpVtbl).Play(p,a,b,c)
	#define IDirectSoundBuffer_Stop(p) (*(*(IDirectSoundBuffer_8*)p).lpVtbl).Stop(p)
	#define IDirectSoundBuffer8_SetFX(p,a,b,c) (*(*(IDirectSoundBuffer_8*)p).lpVtbl).SetFX(p,a,b,c)
	#define IDirectSoundBuffer8_GetObjectInPath(p,a,b,c,d) (*(*(IDirectSoundBuffer_8*)p).lpVtbl).GetObjectInPath(p,a,b,c,d)

	#ifndef DSBPLAY_LOOPING
		/*
		   DirectX 5 SDK or later not found.
		   So, need to define and even redefine some basic header data.
		*/
		#define DSBPLAY_LOOPING             1
		#define DSBCAPS_PRIMARYBUFFER       1
		#define DSSCL_PRIORITY              2
		#define DSBCAPS_STICKYFOCUS         0x4000
		#define DSBCAPS_GETCURRENTPOSITION2 0x10000

		#undef INTERFACE
		#define INTERFACE IDirectSound
		DECLARE_INTERFACE_(IDirectSound, IUnknown){
			STDMETHOD(QueryInterface)       (THIS_ void*, void**) PURE;
			STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
			STDMETHOD_(ULONG,Release)       (THIS) PURE;
			STDMETHOD(CreateSoundBuffer)    (THIS_ DSBUFFERDESC_DX8*, IDirectSoundBuffer_8**, void*) PURE;
			STDMETHOD(GetCaps)              (THIS_ void*) PURE;
			STDMETHOD(DuplicateSoundBuffer) (THIS_ IDirectSoundBuffer_8*, IDirectSoundBuffer_8**) PURE;
			STDMETHOD(SetCooperativeLevel)  (THIS_ HWND, int) PURE;
			STDMETHOD(Compact)              (THIS) PURE;
			STDMETHOD(GetSpeakerConfig)     (THIS_ int*) PURE;
			STDMETHOD(SetSpeakerConfig)     (THIS_ int) PURE;
			STDMETHOD(Initialize)           (THIS_ GUID*) PURE;
		};
		#define IDirectSound_Release(p) (*(*(IDirectSound*)p).lpVtbl).Release(p)
		#define IDirectSound_CreateSoundBuffer(p,a,b,c) (*(*(IDirectSound*)p).lpVtbl).CreateSoundBuffer(p,a,b,c)
		#define IDirectSound_SetCooperativeLevel(p,a,b) (*(*(IDirectSound*)p).lpVtbl).SetCooperativeLevel(p,a,b)

		#define IDirectSoundBuffer IDirectSoundBuffer_8

		#define IDirectSoundBuffer_Play(p,a,b,c) (*(*(IDirectSoundBuffer_8*)p).lpVtbl).Play(p,a,b,c)
		#define IDirectSoundBuffer_SetFormat(p,a) (*(*(IDirectSoundBuffer_8*)p).lpVtbl).SetFormat(p,a)
		#define IDirectSoundBuffer_Stop(p) (*(*(IDirectSoundBuffer_8*)p).lpVtbl).Stop(p)

	#endif

#endif
