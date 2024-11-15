; MINI_DS1.ASM
; ------------
; A pure FASM no-linker example without macros
; and standard includes. Only for real coders ;)

format PE gui 4.0
entry start

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

section '.code' code readable executable

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

; Error message
szError    db 'DirectX error',0

MsgCaption db 'FASM',0
MsgBoxText db 'uFMOD ruleZ!',0

start:
	mov esi,lpDS
	xor ebx,ebx

	; Request an instance of IDirectSound
	push ebx      ; pUnkOuter <= 0
	push esi      ; ppDS
	push ebx      ; lpcGuidDevice <= 0
	call [DirectSoundCreate]
	test eax,eax
	js ds_error

	; It is important to set the cooperative level to at least
	; DSSCL_PRIORITY prior to creating 16-bit stereo buffers.
	push 2	      ; DSSCL_PRIORITY
	call [GetForegroundWindow]
	push eax      ; hWnd
	mov eax,[esi]
	mov ecx,[eax]
	push eax      ; this
	call DWORD [ecx + 24] ; IDirectSound::SetCooperativeLevel
	test eax,eax
	js ds_error

	; Create a secondary sound buffer.
	mov eax,[esi]
	push ebx      ; pUnkOuter <= 0
	push lpDSBuf
	push bufDesc
	mov ecx,[eax]
	push eax      ; this
	call DWORD [ecx + 12] ; IDirectSound::CreateSoundBuffer
	test eax,eax
	js ds_error

	; Start playback.
	push DWORD [lpDSBuf]
	push XM_MEMORY
	push xm_length
	push xm
	call uFMOD_DSPlaySong
	test eax,eax
	js ds_error

	; Wait for user input.
	push ebx     ; MB_OK
	push MsgCaption
	push MsgBoxText
	push ebx
	call [MessageBox]

	; Stop playback.
	push ebx
	push ebx
	push ebx
	push ebx
	call uFMOD_DSPlaySong

cleanup:
	; Release DirectSound instance and free all buffers.
	mov eax,[esi]
	test eax,eax
	jz exit
	mov ecx,[eax]
	push eax
	call DWORD [ecx + 8]  ; IDirectSound::Release

exit:
	push ebx
	call [ExitProcess]

ds_error:
	; Report an error condition.
	push 10h    ; MB_ICONSTOP
	push ebx    ; "Error"
	push szError
	push ebx
	call [MessageBox]
	jmp cleanup

; Imports:
align 4
data import
	dd 0,0,0,RVA kernel32_name,RVA kernel32_table
	dd 0,0,0,RVA user32_name,RVA user32_table
	dd 0,0,0,RVA dsound_name,RVA dsound_table
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

dsound_table:
	DirectSoundCreate dd 80000001h ; DSOUND.#1
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
dsound_name db 'DSOUND.DLL',0

	; Include the whole uFMOD sources here. (Right after
	; your main code to avoid naming conflicts, but stil
	; inside your code section.)
	macro PUBLIC symbol {} ; hide all publics
	macro thnk nm { nm equ [nm] }
	include '..\..\..\..\ufmodlib\src\fasm.asm'

lpDS	dd ? ; -> IDirectSound
lpDSBuf dd ? ; -> IDirectSoundBuffer