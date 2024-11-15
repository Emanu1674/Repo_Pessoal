; Win32 API
EXTERN __imp__MessageBoxA@16
%define MessageBox [__imp__MessageBoxA@16]
EXTERN __imp__ExitProcess@4
%define ExitProcess [__imp__ExitProcess@4]

; OpenAL API
%include "openal32.inc"

; uFMOD (OpenAL)
%include "oalufmod.inc"

section .text

xm         incbin "mini.xm"
xm_length  equ $-xm

; Error message
szError    db "Failed to initialize OpenAL",0
MsgCaption db "NASM",0
MsgBoxText db "uFMOD ruleZ!",0

GLOBAL _start
_start:
	; EBX = 0
	; ESI = device
	; EDI = context
	; EBP = &source
	xor ebx,ebx
	xor edi,edi
	mov ebp,esp

	; Open the preferred device.
	push ebx
	call alcOpenDevice
	test eax,eax
	xchg eax,esi
	jz NEAR oal_error

	; Create a context and make it current.
	push esi
	call alcCreateContext
	xchg eax,edi
	push edi
	call alcMakeContextCurrent

	; Generate 1 source for playback.
	push ebp
	push 1
	call alGenSources

	; Detect a possible error.
	call alGetError
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
	call MessageBox

	; Stop playback.
	push ebx
	push ebx
	push ebx
	push ebx
	call uFMOD_OALPlaySong

cleanup:
	; Release the current context and destroy it (the source gets destroyed as well).
	push ebx
	call alcMakeContextCurrent
	push edi
	call alcDestroyContext

	; Close the actual device.
	push esi
	call alcCloseDevice

exit:
	mov esp,ebp ; fix stack
	push ebx
	call ExitProcess

oal_error:
	; Report an error.
	push 10h ; MB_ICONSTOP
	push ebx ; "Error"
	push szError
	push ebx
	call MessageBox
	jmp cleanup
