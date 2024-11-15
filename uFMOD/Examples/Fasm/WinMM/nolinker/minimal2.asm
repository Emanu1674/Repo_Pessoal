; MINIMAL2.ASM
; ------------
; A pure FASM no-linker example using
; extended programming headers (aka win32ax.inc)

include 'C:\tools\fasm\include\win32ax.inc'

; uFMOD setup:
UF_OUTPUT equ WINMM  ; Set output driver to WINMM       (WINMM, OPENAL, DIRECTSOUND)
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
	; Start playback
	stdcall uFMOD_PlaySong, xm, xm_length, XM_MEMORY

	; Popup a message box to let the XM play until user input
	invoke MessageBox, HWND_DESKTOP, "uFMOD ruleZ!", "FASM", MB_OK

	; Stop playback
	stdcall uFMOD_PlaySong, 0, 0, 0

	; Exit
	invoke ExitProcess, 0

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
		winmm,'WINMM.DLL'
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
		MessageBox,'MessageBoxA'
	import winmm,\
		waveOutGetPosition,'waveOutGetPosition',\
		waveOutOpen,'waveOutOpen',\
		waveOutPrepareHeader,'waveOutPrepareHeader',\
		waveOutReset,'waveOutReset',\
		waveOutUnprepareHeader,'waveOutUnprepareHeader',\
		waveOutWrite,'waveOutWrite',\
		waveOutClose,'waveOutClose'
end data

	; Include the whole uFMOD sources here. (Right after
	; your main code to avoid naming conflicts, but still
	; inside your code section.)
	macro PUBLIC symbol {} ; hide all publics
	macro thnk nm { nm equ [nm] }
	include '..\..\..\..\ufmodlib\src\fasm.asm'