; MINI_AL1.ASM
; ------------
; A pure FASM no-linker example without macros
; and standard includes. Only for real coders ;)

format PE gui 4.0
entry start

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

section '.code' code readable executable

xm file '..\mini.xm'
xm_length = $ - xm

; Optimization:
; This header file is suitable for mini.xm track only!
; If you change the track, update the optimization header.
; (Use the standard eff.inc file for a general purpose player app.)
include 'mini.eff.inc'

; Error message
szError    db "Failed to initialize OpenAL",0
MsgCaption db "FASM",0
MsgBoxText db "uFMOD ruleZ!",0

start:
	; EBX = 0
	; ESI = device
	; EDI = context
	; EBP = &source
	xor ebx,ebx
	xor edi,edi
	mov ebp,source

	; Reset the error state.
	call [alGetError]

	; Open the preferred device.
	push ebx
	call [alcOpenDevice]
	test eax,eax
	xchg eax,esi
	jz oal_error

	; Create a context and make it current.
	push ebx
	push esi
	call [alcCreateContext]
	xchg eax,edi
	push edi
	call [alcMakeContextCurrent]

	; Generate 1 source for playback.
	push ebp
	push 1
	call [alGenSources]
	add esp,24

	; Detect a possible error.
	call [alGetError]
	test eax,eax
	jnz oal_error

	; Start playback.
	push DWORD [ebp]
	push XM_MEMORY
	push xm_length
	push xm
	call uFMOD_OALPlaySong
	test eax,eax
	jz oal_error

	; Wait for user input.
	push ebx ; MB_OK
	push MsgCaption
	push MsgBoxText
	push ebx
	call [MessageBox]

	; Stop playback.
	push ebx
	push ebx
	push ebx
	push ebx
	call uFMOD_OALPlaySong

cleanup:
	; Release the current context and destroy it (the source gets destroyed as well).
	push ebx
	call [alcMakeContextCurrent]
	push edi
	call [alcDestroyContext]

	; Close the actual device.
	push esi
	call [alcCloseDevice]
	add esp,12

exit:
	push ebx
	call [ExitProcess]

oal_error:
	; Report an error.
	push 10h ; MB_ICONSTOP
	push ebx ; "Error"
	push szError
	push ebx
	call [MessageBox]
	jmp cleanup

; Imports:
align 4
data import
	dd 0,0,0,RVA kernel32_name,RVA kernel32_table
	dd 0,0,0,RVA user32_name,RVA user32_table
	dd 0,0,0,RVA openal32_name,RVA openal32_table
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
	dd 00

user32_table:
	MessageBox dd RVA _MessageBox
	GetForegroundWindow dd RVA _GetForegroundWindow
	dd 0

openal32_table:
	alBufferData	       dd RVA _alBufferData
	alDeleteBuffers        dd RVA _alDeleteBuffers
	alGenBuffers	       dd RVA _alGenBuffers
	alGenSources	       dd RVA _alGenSources
	alGetError	       dd RVA _alGetError
	alGetSourcei	       dd RVA _alGetSourcei
	alSourcei	       dd RVA _alSourcei
	alSourcePlay	       dd RVA _alSourcePlay
	alSourceQueueBuffers   dd RVA _alSourceQueueBuffers
	alSourceStop	       dd RVA _alSourceStop
	alSourceUnqueueBuffers dd RVA _alSourceUnqueueBuffers
	alcCloseDevice	       dd RVA _alcCloseDevice
	alcCreateContext       dd RVA _alcCreateContext
	alcDestroyContext      dd RVA _alcDestroyContext
	alcMakeContextCurrent  dd RVA _alcMakeContextCurrent
	alcOpenDevice	       dd RVA _alcOpenDevice
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
_GetForegroundWindow dw 0
	db 'GetForegroundWindow',0
align 2
openal32_name db 'OPENAL32.DLL',0
align 2
_alBufferData dw 0
	db 'alBufferData',0
align 2
_alDeleteBuffers dw 0
	db 'alDeleteBuffers',0
align 2
_alGenBuffers dw 0
	db 'alGenBuffers',0
align 2
_alGenSources dw 0
	db 'alGenSources',0
align 2
_alGetError dw 0
	db 'alGetError',0
align 2
_alGetSourcei dw 0
	db 'alGetSourcei',0
align 2
_alSourcei dw 0
	db 'alSourcei',0
align 2
_alSourcePlay dw 0
	db 'alSourcePlay',0
align 2
_alSourceQueueBuffers dw 0
	db 'alSourceQueueBuffers',0
align 2
_alSourceStop dw 0
	db 'alSourceStop',0
align 2
_alSourceUnqueueBuffers dw 0
	db 'alSourceUnqueueBuffers',0
align 2
_alcCloseDevice dw 0
	db 'alcCloseDevice',0
align 2
_alcCreateContext dw 0
	db 'alcCreateContext',0
align 2
_alcDestroyContext dw 0
	db 'alcDestroyContext',0
align 2
_alcMakeContextCurrent dw 0
	db 'alcMakeContextCurrent',0
align 2
_alcOpenDevice dw 0
	db 'alcOpenDevice',0

	; Include the whole uFMOD sources here. (Right after
	; your main code to avoid naming conflicts, but stil
	; inside your code section.)
	macro PUBLIC symbol {} ; hide all publics
	macro thnk nm { nm equ [nm] }
	include '..\..\..\..\ufmodlib\src\fasm.asm'

source dd ? ; OpenAL source