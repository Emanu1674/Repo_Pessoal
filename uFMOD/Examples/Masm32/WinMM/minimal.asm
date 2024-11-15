.386
.model flat,stdcall

; Win32 API
proto_stdcall_arg1 typedef PROTO :DWORD
proto_stdcall_arg4 typedef PROTO :DWORD,:DWORD,:DWORD,:DWORD
externdef _imp__ExitProcess@4:PTR proto_stdcall_arg1
ExitProcess equ <_imp__ExitProcess@4>
externdef _imp__MessageBoxA@16:PTR proto_stdcall_arg4
MessageBox equ <_imp__MessageBoxA@16>
includelib kernel32.lib
includelib user32.lib
includelib winmm.lib

; uFMOD (WINMM)
include ufmod.inc

.CODE

externdef MsgBoxText:BYTE,MsgCaption:BYTE

; Let's place the stream right inside the code section.
xm_length EQU 905
xm LABEL BYTE
xm_unused_000 LABEL BYTE

; *** The following 60 bytes are not used and
; *** we'll substitute them with useful data
; (the actual size and location of such gaps
; may be found out using the Eff utility)

start:
	; EDI = 0
	xor edi,edi

	; Start playback.
	invoke uFMOD_PlaySong,OFFSET xm,xm_length,XM_MEMORY

	; Wait for user input.
	invoke MessageBox,edi,OFFSET MsgBoxText,OFFSET MsgCaption,edi

	; Stop playback.
	invoke uFMOD_PlaySong,edi,edi,edi

	invoke ExitProcess,edi

org xm_unused_000 + 60
	db 034h,000h,000h,000h,020h,000h,000h,000h,002h,000h,00Dh,000h,001h,000h,001h,000h
	db 00Ah,000h,091h,000h,000h,001h,002h,003h,004h,005h,006h,007h,000h,001h,002h,003h
	db 004h,005h,006h,007h,008h,009h,00Ah,00Bh,008h,009h,00Ch,00Bh,008h,009h,00Ah,00Bh
	db 008h,009h,00Ch,00Bh,009h,000h,000h,000h,000h,004h,000h,001h,000h,083h,016h,001h
	db 080h,080h,02Eh,001h,000h,00Eh,060h,080h,03Ah,001h,000h,00Eh,062h,081h,061h,083h
	db 035h,001h,009h,000h,000h,000h,000h,004h,000h,001h,000h,083h,016h,001h,080h,080h
	db 02Eh,001h,000h,00Eh,060h,080h,035h,001h,000h,00Eh,062h,081h,061h,083h,038h,001h
	db 009h,000h,000h,000h,000h,004h,000h,001h,000h,083h,016h,001h,080h,080h,02Eh,001h
	db 000h,00Eh,060h,080h,038h,001h,000h,00Eh,062h,080h,083h,033h,001h,009h,000h,000h
	db 000h,000h,006h,000h,001h,000h,083h,016h,001h,080h,080h,02Eh,001h,000h,00Eh,060h
	db 080h,033h,001h,000h,00Eh,061h,081h,061h,083h,035h,001h,083h,00Dh,001h,083h,036h
	db 001h,080h,083h,036h,001h,009h,000h,000h,000h,000h,004h,000h,001h,000h,083h,00Fh
	db 001h,080h,080h,02Eh,001h,000h,00Eh,060h,080h,036h,001h,000h,00Eh,062h,081h,061h
	db 083h,033h,001h,009h,000h,000h,000h,000h,006h,000h,001h,000h,083h,00Fh,001h,080h
	db 080h,02Eh,001h,000h,00Eh,060h,080h,033h,001h,000h,00Eh,061h,081h,061h,083h,02Eh
	db 001h,083h,012h,001h,083h,033h,001h,080h,083h,035h,001h,009h,000h,000h,000h,000h
	db 006h,000h,001h,000h,083h,016h,001h,080h,080h,02Eh,001h,000h,00Eh,060h,080h,035h
	db 001h,000h,00Eh,061h,081h,061h,083h,02Eh,001h,083h,00Dh,001h,083h,031h,001h,080h
	db 083h,02Eh,001h,009h,000h,000h,000h,000h,008h,000h,001h,000h,083h,012h,001h,098h
	db 00Ah,001h,083h,019h,001h,088h,00Ah,083h,01Eh,001h,081h,061h,083h,012h,001h,080h
	db 083h,014h,001h,080h,083h,01Bh,001h,080h,083h,020h,001h,080h,083h,014h,001h,080h
	db 009h,000h,000h,000h,000h,008h,000h,001h,000h,083h,012h,001h,081h,061h,083h,019h
	db 001h,080h,083h,01Eh,001h,080h,083h,012h,001h,080h,083h,019h,001h,083h,031h,001h
	db 083h,01Eh,001h,080h,083h,012h,001h,083h,031h,001h,083h,019h,001h,080h,009h,000h
	db 000h,000h,000h,008h,000h,001h,000h,083h,014h,001h,083h,033h,001h,083h,01Bh,001h
	db 080h,083h,020h,001h,083h,031h,001h,083h,014h,001h,080h,083h,01Bh,001h,083h,030h
	db 001h,083h,020h,001h,080h,083h,014h,001h,083h,031h,001h,083h,01Bh,001h,080h,009h
	db 000h,000h,000h,000h,008h,000h,001h,000h,083h,016h,001h,083h,030h,001h,083h,01Dh
	db 001h,083h,031h,001h,083h,022h,001h,083h,035h,001h,083h,016h,001h,098h,00Ah,001h
	db 083h,01Dh,001h,088h,00Ah,083h,022h,001h,081h,061h,083h,016h,001h,080h,083h,01Dh
	db 001h,080h,009h,000h,000h,000h,000h,008h,000h,001h,000h,083h,016h,001h,080h,083h
	db 01Dh,001h,080h,083h,022h,001h,080h,083h,016h,001h,080h,083h,018h,001h,080h,083h
	db 01Dh,001h,080h,083h,011h,001h,080h,083h,018h,001h,080h,009h,000h,000h,000h,000h
	db 008h,000h,001h,000h,083h,016h,001h,083h,030h,001h,083h,01Dh,001h,083h,031h,001h
	db 083h,019h,001h,083h,02Eh,001h,083h,016h,001h,098h,00Ah,001h,083h,01Dh,001h,088h
	db 00Ah,083h,019h,001h,081h,061h,083h,016h,001h,080h,083h,01Dh,001h,080h,0F1h,000h
	db 000h,000h
xm_unused_001 LABEL BYTE

; The following 23 bytes are not used.
; So, let's place the MessageBox text and caption instead.
MsgBoxText db "uFMOD ruleZ!",0
MsgCaption db "MASM32",0

org xm_unused_001 + 23
	db 001h,000h,012h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,040h,000h,008h,000h,02Ch,000h,00Eh,000h
	db 008h,000h,018h,000h,016h,000h,020h,000h,008h,000h,02Dh,000h,00Dh,000h,032h,000h
	db 004h,000h,03Ch,000h,007h,000h,044h,000h,004h,000h,05Ah,000h,000h,000h,064h,000h
	db 000h,000h,06Eh,000h,000h,000h,000h,000h,020h,000h,00Ah,000h,028h,000h,01Eh,000h
	db 018h,000h,032h,000h,020h,000h,03Ch,000h,020h,000h,046h,000h,020h,000h,050h,000h
	db 020h,000h,05Ah,000h,020h,000h,064h,000h,020h,000h,06Eh,000h,020h,000h,078h,000h
	db 020h,000h,082h,000h,020h,000h,009h,006h,001h,002h,004h,002h,003h,005h,001h,000h
	db 000h,000h,000h,000h,080h,000h,00Ch,000h,000h,000h,000h,000h,000h,000h,00Ch,000h
	db 000h,000h,040h,000h,001h,080h,0F9h,000h,0BFh,000h,0C3h,000h,00Ah,000h,057h,000h
	db 06Eh,000h,023h,000h

END start
