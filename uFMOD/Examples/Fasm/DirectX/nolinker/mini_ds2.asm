; MINI_DS2.ASM
; ------------
; A pure FASM no-linker example using
; extended programming headers (aka win32ax.inc)

include 'C:\tools\fasm\include\win32ax.inc'

; uFMOD setup:
UF_OUTPUT equ DIRECTSOUND ; Set output driver to DIRECTSOUND (WINMM, OPENAL, DIRECTSOUND)
UF_FREQ   equ 48000	  ; Set sampling rate to 48KHz       (22050, 44100, 48000)
UF_RAMP   equ STRONG	  ; Select STRONG interpolation      (NONE, WEAK, STRONG)
UF_UFS	  equ ANSI	  ; Select ANSI encoding             (ANSI, UNICODE)
UD_MODE   equ UNSAFE	  ; Select UNSAFE mode               (NORMAL, UNSAFE, BENCHMARK)
VB6	  equ 0 	  ; Disable VisualBasic 6 extensions
PBASIC	  equ 0 	  ; Disable PureBasic extensions
EBASIC	  equ 0 	  ; Disable Emergence BASIC extensions
BLITZMAX  equ 0 	  ; Disable BlitzMax extensions
NOLINKER  equ 1 	  ; Select "no linker" mode
UF_FMT	  equ 0 	  ; Disable DLL entry point

; uFMOD constants:
XM_RESOURCE	  = 0
XM_MEMORY	  = 1
XM_FILE 	  = 2
XM_NOLOOP	  = 8
XM_SUSPENDED	  = 16
uFMOD_BUFFER_SIZE = 10000h
uFMOD_MIN_VOL	  = 0
uFMOD_MAX_VOL	  = 25
uFMOD_DEFAULT_VOL = 25

.code
	; Request an instance of IDirectSound
	invoke DirectSoundCreate, 0, lpDS, 0
	test eax,eax
	js ds_error

	; It is important to set the cooperative level to at least
	; DSSCL_PRIORITY prior to creating 16-bit stereo buffers.
	invoke GetForegroundWindow
	mov edx,[lpDS]
	mov ecx,[edx]
	; IDirectSound::SetCooperativeLevel
	stdcall DWORD [ecx + 24], edx, eax, 2 ; DSCL_PRIORITY
	test eax,eax
	js ds_error

	; Create a secondary sound buffer.
	mov eax,[lpDS]
	mov ecx,[eax]
	; IDirectSound::CreateSoundBuffer
	stdcall DWORD [ecx + 12], eax, bufDesc, lpDSBuf, 0
	test eax,eax
	js ds_error

	; Start playback.
	stdcall uFMOD_DSPlaySong, xm, xm_length, XM_MEMORY, [lpDSBuf]
	test eax,eax
	js ds_error

	; Wait for user input.
	invoke MessageBox, HWND_DESKTOP, "uFMOD ruleZ!", "FASM", MB_OK

	; Stop playback.
	stdcall uFMOD_DSPlaySong, 0, 0, 0, 0

cleanup:
	; Release DirectSound instance and free all buffers.
	mov eax,[lpDS]
	test eax,eax
	jz exit
	mov ecx,[eax]
	; IDirectSound::Release
	stdcall DWORD [ecx + 8], eax

exit:
	invoke ExitProcess, 0

ds_error:
	; Report an error condition.
	invoke MessageBox, HWND_DESKTOP, "DirectX error", 0, MB_ICONSTOP
	jmp cleanup

xm file '..\mini.xm'
xm_length = $ - xm

; Optimization:
; This header file is suitable for mini.xm track only!
; If you change the track, update the optimization header.
; (Use the standard eff.inc file for a general purpose player app.)
include 'mini.eff.inc'

; Wave format descriptor used to configure the DirectSound buffer
pcm	dd 20001h  ; wFormatTag <= WAVE_FORMAT_PCM, nChannels <= 2
	dd UF_FREQ
	dd UF_FREQ*4
	dd 100004h ; wBitsPerSample <= 16, nBlockAlign <= 4
	dd 0	   ; cbSize <= 0 (no extra info)

; DirectSound buffer descriptor
bufDesc dd 20	   ; dwSize  <= DSBUFFERDESC1
		   ; (for older DirectX versions compatibility)
	dd 14000h  ; dwFlags <= DSBCAPS_STICKYFOCUS OR DSBCAPS_GETCURRENTPOSITION2
	dd uFMOD_BUFFER_SIZE ; dwBufferBytes
	dd 0
	dd pcm	   ; lpwfxFormat

; Imports:
data import
	library kernel32,'KERNEL32.DLL',\
		user32,'USER32.DLL',\
		dsound,'DSOUND.DLL'
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
	import dsound,\
		DirectSoundCreate,1
end data

	; Include the whole uFMOD sources here. (Right after
	; your main code to avoid naming conflicts, but stil
	; inside your code section.)
	macro PUBLIC symbol {} ; hide all publics
	macro thnk nm { nm equ [nm] }
	include '..\..\..\..\ufmodlib\src\fasm.asm'

lpDS	dd ? ; -> IDirectSound
lpDSBuf dd ? ; -> IDirectSoundBuffer