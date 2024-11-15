// C++ wrapper for embedding unmanaged uFMOD code in .NET
// NOTE: ufmod.obj should be compiled in UNICODE flavour!

extern "C" {
	int __stdcall uFMOD_PlaySong(void*, int, int);
	void __stdcall uFMOD_Jump2Pattern(unsigned int);
	void __stdcall uFMOD_Pause();
	void __stdcall uFMOD_Resume();
	unsigned int __stdcall uFMOD_GetStats();
	unsigned int __stdcall uFMOD_GetRowOrder();
	unsigned int __stdcall uFMOD_GetTime();
	wchar_t* __stdcall uFMOD_GetTitle();
	void __stdcall uFMOD_SetVolume(unsigned int);
}

#define XM_RESOURCE       0
#define XM_MEMORY         1
#define XM_FILE           2
#define XM_NOLOOP         8
#define XM_SUSPENDED      16
#define uFMOD_MIN_VOL     0
#define uFMOD_MAX_VOL     25
#define uFMOD_DEFAULT_VOL uFMOD_MAX_VOL

#include <vcclr.h>

// Remove CRT dependency in VC++ 2008
#pragma warning(disable:4483)
void __clrcall __identifier(".cctor")(){}

// Remove MSCore dependency
extern "C" void __stdcall _CorDllMain(int, int, int){}

namespace uFMOD
{
	ref class I_uFMOD
	{
	public:
		static int PlayFile(System::String^ filename, int flags){
			pin_ptr<const wchar_t> ws = PtrToStringChars(filename);
			return uFMOD_PlaySong((void*)ws, 0, XM_FILE | flags);
		}
		static int PlayRes(int id, int flags){ return uFMOD_PlaySong((void*)id, 0, XM_RESOURCE | flags); }
		static int PlayMem(unsigned char* p, int len, int flags){ return uFMOD_PlaySong(p, len, XM_MEMORY | flags); }
		static void Stop(){ uFMOD_PlaySong(0, 0, 0); }
		static void Pause(){ uFMOD_Pause(); }
		static void Resume(){ uFMOD_Resume(); }
		static unsigned int GetStats(){ return uFMOD_GetStats(); }
		static unsigned int GetRowOrder(){ return uFMOD_GetRowOrder(); }
		static System::String^ GetTitle(){
			wchar_t c[24], *p1, *p2;
			p1 = uFMOD_GetTitle();
			p2 = c;
			while(*p1) *p2++ = *p1++;
			*p2 = 0;
			return gcnew System::String(c);
		}
		static void SetVolume(unsigned int vol){ uFMOD_SetVolume(vol); }
		static void Jump2Pattern(unsigned int pat){ uFMOD_Jump2Pattern(pat); }
	};
}