.386
.model flat,stdcall

; Win32 API
EXTRN ExitProcess:PROC
EXTRN MessageBoxA:PROC

; OpenAL API
include openal32.inc
includelib openal32.lib

; uFMOD (OpenAL)
include oalufmod.inc

.CODE

; Let's place the stream right inside the code section.
xm_length EQU 905
xm LABEL BYTE
	db 045h,078h,074h,065h,06Eh,064h,065h,064h,020h,04Dh,06Fh,064h,075h,06Ch,065h,03Ah
	db 020h,073h,06Fh,066h,074h,020h,06Dh,061h,06Eh,069h,061h,063h,02Dh,06Dh,069h,06Eh
	db 069h,06Dh,061h,06Ch,000h,01Ah,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,004h,001h,034h,000h,000h,000h
	db 020h,000h,000h,000h,002h,000h,00Dh,000h,001h,000h,001h,000h,00Ah,000h,091h,000h
	db 000h,001h,002h,003h,004h,005h,006h,007h,000h,001h,002h,003h,004h,005h,006h,007h
	db 008h,009h,00Ah,00Bh,008h,009h,00Ch,00Bh,008h,009h,00Ah,00Bh,008h,009h,00Ch,00Bh
	db 009h,000h,000h,000h,000h,004h,000h,001h,000h,083h,016h,001h,080h,080h,02Eh,001h
	db 000h,00Eh,060h,080h,03Ah,001h,000h,00Eh,062h,081h,061h,083h,035h,001h,009h,000h
	db 000h,000h,000h,004h,000h,001h,000h,083h,016h,001h,080h,080h,02Eh,001h,000h,00Eh
	db 060h,080h,035h,001h,000h,00Eh,062h,081h,061h,083h,038h,001h,009h,000h,000h,000h
	db 000h,004h,000h,001h,000h,083h,016h,001h,080h,080h,02Eh,001h,000h,00Eh,060h,080h
	db 038h,001h,000h,00Eh,062h,080h,083h,033h,001h,009h,000h,000h,000h,000h,006h,000h
	db 001h,000h,083h,016h,001h,080h,080h,02Eh,001h,000h,00Eh,060h,080h,033h,001h,000h
	db 00Eh,061h,081h,061h,083h,035h,001h,083h,00Dh,001h,083h,036h,001h,080h,083h,036h
	db 001h,009h,000h,000h,000h,000h,004h,000h,001h,000h,083h,00Fh,001h,080h,080h,02Eh
	db 001h,000h,00Eh,060h,080h,036h,001h,000h,00Eh,062h,081h,061h,083h,033h,001h,009h
	db 000h,000h,000h,000h,006h,000h,001h,000h,083h,00Fh,001h,080h,080h,02Eh,001h,000h
	db 00Eh,060h,080h,033h,001h,000h,00Eh,061h,081h,061h,083h,02Eh,001h,083h,012h,001h
	db 083h,033h,001h,080h,083h,035h,001h,009h,000h,000h,000h,000h,006h,000h,001h,000h
	db 083h,016h,001h,080h,080h,02Eh,001h,000h,00Eh,060h,080h,035h,001h,000h,00Eh,061h
	db 081h,061h,083h,02Eh,001h,083h,00Dh,001h,083h,031h,001h,080h,083h,02Eh,001h,009h
	db 000h,000h,000h,000h,008h,000h,001h,000h,083h,012h,001h,098h,00Ah,001h,083h,019h
	db 001h,088h,00Ah,083h,01Eh,001h,081h,061h,083h,012h,001h,080h,083h,014h,001h,080h
	db 083h,01Bh,001h,080h,083h,020h,001h,080h,083h,014h,001h,080h,009h,000h,000h,000h
	db 000h,008h,000h,001h,000h,083h,012h,001h,081h,061h,083h,019h,001h,080h,083h,01Eh
	db 001h,080h,083h,012h,001h,080h,083h,019h,001h,083h,031h,001h,083h,01Eh,001h,080h
	db 083h,012h,001h,083h,031h,001h,083h,019h,001h,080h,009h,000h,000h,000h,000h,008h
	db 000h,001h,000h,083h,014h,001h,083h,033h,001h,083h,01Bh,001h,080h,083h,020h,001h
	db 083h,031h,001h,083h,014h,001h,080h,083h,01Bh,001h,083h,030h,001h,083h,020h,001h
	db 080h,083h,014h,001h,083h,031h,001h,083h,01Bh,001h,080h,009h,000h,000h,000h,000h
	db 008h,000h,001h,000h,083h,016h,001h,083h,030h,001h,083h,01Dh,001h,083h,031h,001h
	db 083h,022h,001h,083h,035h,001h,083h,016h,001h,098h,00Ah,001h,083h,01Dh,001h,088h
	db 00Ah,083h,022h,001h,081h,061h,083h,016h,001h,080h,083h,01Dh,001h,080h,009h,000h
	db 000h,000h,000h,008h,000h,001h,000h,083h,016h,001h,080h,083h,01Dh,001h,080h,083h
	db 022h,001h,080h,083h,016h,001h,080h,083h,018h,001h,080h,083h,01Dh,001h,080h,083h
	db 011h,001h,080h,083h,018h,001h,080h,009h,000h,000h,000h,000h,008h,000h,001h,000h
	db 083h,016h,001h,083h,030h,001h,083h,01Dh,001h,083h,031h,001h,083h,019h,001h,083h
	db 02Eh,001h,083h,016h,001h,098h,00Ah,001h,083h,01Dh,001h,088h,00Ah,083h,019h,001h
	db 081h,061h,083h,016h,001h,080h,083h,01Dh,001h,080h,0F1h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,027h,001h,000h,012h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,040h,000h,008h
	db 000h,02Ch,000h,00Eh,000h,008h,000h,018h,000h,016h,000h,020h,000h,008h,000h,02Dh
	db 000h,00Dh,000h,032h,000h,004h,000h,03Ch,000h,007h,000h,044h,000h,004h,000h,05Ah
	db 000h,000h,000h,064h,000h,000h,000h,06Eh,000h,000h,000h,000h,000h,020h,000h,00Ah
	db 000h,028h,000h,01Eh,000h,018h,000h,032h,000h,020h,000h,03Ch,000h,020h,000h,046h
	db 000h,020h,000h,050h,000h,020h,000h,05Ah,000h,020h,000h,064h,000h,020h,000h,06Eh
	db 000h,020h,000h,078h,000h,020h,000h,082h,000h,020h,000h,009h,006h,001h,002h,004h
	db 002h,003h,005h,001h,000h,000h,000h,000h,000h,080h,000h,00Ch,000h,000h,000h,000h
	db 000h,000h,000h,00Ch,000h,000h,000h,040h,000h,001h,080h,0F9h,000h,0BFh,000h,0C3h
	db 000h,00Ah,000h,057h,000h,06Eh,000h,023h,000h

; Error message
szError    db "Failed to initialize OpenAL",0
MsgCaption db "TASM",0
MsgBoxText db "uFMOD ruleZ!",0

start:
	; EBX = 0
	; ESI = device
	; EDI = context
	; EBP = &source
	xor ebx,ebx
	xor edi,edi
	mov ebp,esp

	; Open the preferred device.
	call alcOpenDevice, ebx
	test eax,eax
	xchg eax,esi
	jz oal_error

	; Create a context and make it current.
	call alcCreateContext, esi
	xchg eax,edi
	call alcMakeContextCurrent, edi

	; Generate 1 source for playback.
	call alGenSources, 1, ebp

	; Detect a possible error.
	call alGetError
	test eax,eax
	jnz oal_error

	; Start playback.
	call uFMOD_OALPlaySong, OFFSET xm, xm_length, XM_MEMORY, DWORD PTR [ebp]
	test eax,eax
	jz oal_error

	; Wait for user input.
	call MessageBoxA, ebx, OFFSET MsgBoxText, OFFSET MsgCaption, ebx

	; Stop playback.
	call uFMOD_OALPlaySong, ebx, ebx, ebx, ebx

cleanup:
	; Release the current context and destroy it (the source gets destroyed as well).
	call alcMakeContextCurrent, ebx
	call alcDestroyContext, edi

	; Close the actual device.
	call alcCloseDevice, esi

exit:
	mov esp,ebp ; fix stack
	call ExitProcess, ebx

oal_error:
	; Report an error.
	call MessageBoxA, ebx, OFFSET szError, ebx, 10h
	jmp cleanup

END start