; MINI_AL2.ASM
; ------------
; A pure FASM no-linker example using
; extended programming headers (aka win32ax.inc)

include 'C:\tools\fasm\include\win32ax.inc'

; uFMOD setup:
UF_OUTPUT equ OPENAL ; Set output driver to OPENAL      (WINMM, OPENAL, DIRECTSOUND)
UF_FREQ   equ 48000  ; Set sampling rate to 48KHz       (22050, 44100, 48000)
UF_RAMP   equ STRONG ; Select STRONG interpolation      (NONE, WEAK, STRONG)
UF_UFS	  equ ANSI   ; Select ANSI encoding             (ANSI, UNICODE)
UD_MODE   equ UNSAFE ; Select UNSAFE mode               (NORMAL, UNSAFE, BENCHMARK)
VB6	  equ 0      ; Disable VisualBasic 6 extensions
PBASIC	  equ 0      ; Disable PureBasic extensions
EBASIC	  equ 0      ; Disable Emergence BASIC extensions
BLITZMAX  equ 0      ; Disable BlitzMax extensions
NOLINKER  equ 1      ; Select "no linker" mode
UF_FMT	  equ 0      ; Disable DLL entry point

; uFMOD constants:
XM_RESOURCE	  = 0
XM_MEMORY	  = 1
XM_FILE 	  = 2
XM_NOLOOP	  = 8
XM_SUSPENDED	  = 16
uFMOD_MIN_VOL	  = 0
uFMOD_MAX_VOL	  = 25
uFMOD_DEFAULT_VOL = 25

.code
	; ESI = device
	; EDI = context
	; EBP = &source
	xor edi,edi
	mov ebp,source

	; Reset the error state.
	cinvoke alGetError

	; Open the preferred device.
	cinvoke alcOpenDevice, 0
	test eax,eax
	xchg eax,esi
	jz oal_error

	; Create a context and make it current.
	cinvoke alcCreateContext, esi, 0
	xchg eax,edi
	cinvoke alcMakeContextCurrent, edi

	; Generate 1 source for playback.
	cinvoke alGenSources, 1, ebp

	; Detect a possible error.
	cinvoke alGetError
	test eax,eax
	jnz oal_error

	; Start playback.
	stdcall uFMOD_OALPlaySong, xm, xm_length, XM_MEMORY, DWORD [ebp]
	test eax,eax
	jz oal_error

	; Wait for user input.
	invoke MessageBox, HWND_DESKTOP, "uFMOD ruleZ!", "FASM", MB_OK

	; Stop playback.
	stdcall uFMOD_OALPlaySong, 0, 0, 0, 0

cleanup:
	; Release the current context and destroy it (the source gets destroyed as well).
	cinvoke alcMakeContextCurrent, 0
	cinvoke alcDestroyContext, edi

	; Close the actual device.
	cinvoke alcCloseDevice, esi

exit:
	invoke ExitProcess, 0

oal_error:
	; Report an error.
	invoke MessageBox, HWND_DESKTOP, "Failed to initialize OpenAL", 0, MB_ICONSTOP
	jmp cleanup

xm file '..\mini.xm'
xm_length = $ - xm

; Optimization:
; This header file is suitable for mini.xm track only!
; If you change the track, update the optimization header.
; (Use the standard eff.inc file for a general purpose player app.)
include 'mini.eff.inc'

; Imports:
data import
	library kernel32,'KERNEL32.DLL',\
		user32,'USER32.DLL',\
		openal32,'OPENAL32.DLL'
	import kernel32,\
		ExitProcess,'ExitProcess',\
		WaitForSingleObject,'WaitForSingleObject',\
		CloseHandle,'CloseHandle',\
		CreateThread,'CreateThread',\
		SetThreadPriority,'SetThreadPriority',\
		HeapAlloc,'HeapAlloc',\
		HeapCreate,'HeapCreate',\
		HeapDestroy,'HeapDestroy',\
		Sleep,'Sleep',\
		CreateFile,'CreateFileA',\ ; set to CreateFileW for Unicode support
		ReadFile,'ReadFile',\
		SetFilePointer,'SetFilePointer',\
		FindResource,'FindResourceA',\
		LoadResource,'LoadResource',\
		SizeofResource,'SizeofResource'
	import user32,\
		MessageBox,'MessageBoxA',\
		GetForegroundWindow,'GetForegroundWindow'
	import openal32,\
		alBufferData,'alBufferData',\
		alDeleteBuffers,'alDeleteBuffers',\
		alGenBuffers,'alGenBuffers',\
		alGenSources,'alGenSources',\
		alGetError,'alGetError',\
		alGetSourcei,'alGetSourcei',\
		alSourcei,'alSourcei',\
		alSourcePlay,'alSourcePlay',\
		alSourceQueueBuffers,'alSourceQueueBuffers',\
		alSourceStop,'alSourceStop',\
		alSourceUnqueueBuffers,'alSourceUnqueueBuffers',\
		alcCloseDevice,'alcCloseDevice',\
		alcCreateContext,'alcCreateContext',\
		alcDestroyContext,'alcDestroyContext',\
		alcMakeContextCurrent,'alcMakeContextCurrent',\
		alcOpenDevice,'alcOpenDevice'
end data

	; Include the whole uFMOD sources here. (Right after
	; your main code to avoid naming conflicts, but stil
	; inside your code section.)
	macro PUBLIC symbol {} ; hide all publics
	macro thnk nm { nm equ [nm] }
	include '..\..\..\..\ufmodlib\src\fasm.asm'

source dd ? ; OpenAL source