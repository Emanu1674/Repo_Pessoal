format MS COFF

; Win32 API
extrn '__imp__MessageBoxA@16' as MessageBox:DWORD
extrn '__imp__ExitProcess@4' as ExitProcess:DWORD

; uFMOD (WINMM)
include 'ufmod.inc'

section '.text' code readable executable

xm file 'mini.xm'
xm_length = $ - xm
MsgCaption db "FASM",0
MsgBoxText db "uFMOD ruleZ!",0

PUBLIC _start
_start:
	xor edi,edi

	; Start playback.
	push XM_MEMORY
	push xm_length
	push xm
	call uFMOD_PlaySong

	; Wait for user input.
	push edi
	push MsgCaption
	push MsgBoxText
	push edi
	call [MessageBox]

	; Stop playback.
	push edi
	push edi
	push edi
	call uFMOD_PlaySong

	push edi
	call [ExitProcess]