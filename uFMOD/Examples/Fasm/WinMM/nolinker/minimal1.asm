; MINIMAL1.ASM
; ------------
; A pure FASM no-linker example without macros
; and standard includes. Only for real coders ;)

format PE gui 4.0
entry start

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

section '.code' code readable executable

xm file '..\mini.xm'
xm_length = $ - xm

; Optimization:
; This header file is suitable for mini.xm track only!
; If you change the track, update the optimization header.
; (Use the standard eff.inc file for a general purpose player app.)
include 'mini.eff.inc'

caption db 'FASM',0
message db 'uFMOD ruleZ!',0

start:
	xor edi,edi

	; Start playback
	push XM_MEMORY
	push xm_length
	push xm
	call uFMOD_PlaySong

	; Popup a message box to let the XM play until user input
	push edi
	push caption
	push message
	push edi
	call [MessageBox]

	; Stop playback
	push edi
	push edi
	push edi
	call uFMOD_PlaySong

	; Exit
	push edi
	call [ExitProcess]

; Imports:
align 4
data import
	dd 0,0,0,RVA kernel32_name,RVA kernel32_table
	dd 0,0,0,RVA user32_name,RVA user32_table
	dd 0,0,0,RVA winmm_name,RVA winmm_table
	dd 0,0,0,0,0
end data

kernel32_table:
	ExitProcess	    dd RVA _ExitProcess
	WaitForSingleObject dd RVA _WaitForSingleObject
	CloseHandle	    dd RVA _CloseHandle
	CreateThread	    dd RVA _CreateThread
	SetThreadPriority   dd RVA _SetThreadPriority
	HeapAlloc	    dd RVA _HeapAlloc
	HeapCreate	    dd RVA _HeapCreate
	HeapDestroy	    dd RVA _HeapDestroy
	Sleep		    dd RVA _Sleep
	CreateFile	    dd RVA _CreateFile
	ReadFile	    dd RVA _ReadFile
	SetFilePointer	    dd RVA _SetFilePointer
	FindResource	    dd RVA _FindResource
	LoadResource	    dd RVA _LoadResource
	SizeofResource	    dd RVA _SizeofResource
	dd 0

user32_table:
	MessageBox dd RVA _MessageBox
	dd 0

winmm_table:
	waveOutGetPosition     dd RVA _waveOutGetPosition
	waveOutOpen	       dd RVA _waveOutOpen
	waveOutPrepareHeader   dd RVA _waveOutPrepareHeader
	waveOutReset	       dd RVA _waveOutReset
	waveOutUnprepareHeader dd RVA _waveOutUnprepareHeader
	waveOutWrite	       dd RVA _waveOutWrite
	waveOutClose	       dd RVA _waveOutClose
	dd 0

kernel32_name db 'KERNEL32.DLL',0
align 2
_ExitProcess dw 0
	db 'ExitProcess',0
align 2
_WaitForSingleObject dw 0
	db 'WaitForSingleObject',0
align 2
_CloseHandle dw 0
	db 'CloseHandle',0
align 2
_CreateThread dw 0
	db 'CreateThread',0
align 2
_SetThreadPriority dw 0
	db 'SetThreadPriority',0
align 2
_HeapAlloc dw 0
	db 'HeapAlloc',0
align 2
_HeapCreate dw 0
	db 'HeapCreate',0
align 2
_HeapDestroy dw 0
	db 'HeapDestroy',0
align 2
_Sleep dw 0
	db 'Sleep',0
align 2
_CreateFile dw 0
	db 'CreateFile'
if UF_UFS eq ANSI
	dw 'A'
else
	dw 'W'
end if
align 2
_ReadFile dw 0
	db 'ReadFile',0
align 2
_SetFilePointer dw 0
	db 'SetFilePointer',0
align 2
_FindResource dw 0
	db 'FindResourceA',0
align 2
_LoadResource dw 0
	db 'LoadResource',0
align 2
_SizeofResource dw 0
	db 'SizeofResource',0
align 2
user32_name db 'USER32.DLL',0
align 2
_MessageBox dw 0
	db 'MessageBoxA',0
align 2
winmm_name db 'WINMM.DLL',0
align 2
_waveOutGetPosition dw 0
	db 'waveOutGetPosition',0
align 2
_waveOutOpen dw 0
	db 'waveOutOpen',0
align 2
_waveOutPrepareHeader dw 0
	db 'waveOutPrepareHeader',0
align 2
_waveOutReset dw 0
	db 'waveOutReset',0
align 2
_waveOutUnprepareHeader dw 0
	db 'waveOutUnprepareHeader',0
align 2
_waveOutWrite dw 0
	db 'waveOutWrite',0
align 2
_waveOutClose dw 0
	db 'waveOutClose',0

	; Include the whole uFMOD sources here. (Right after
	; your main code to avoid naming conflicts, but still
	; inside your code section.)
	macro PUBLIC symbol {} ; hide all publics
	macro thnk nm { nm equ [nm] }
	include '..\..\..\..\ufmodlib\src\fasm.asm'