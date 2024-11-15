; Setup actual sampling rate in Hz. (48KHz is the default value.)
uFMOD_MixRate EQU 48000

.386
.model flat,stdcall

; Win32 API
proto_stdcall_arg0 typedef PROTO
proto_stdcall_arg1 typedef PROTO :DWORD
proto_stdcall_arg3 typedef PROTO :DWORD,:DWORD,:DWORD
proto_stdcall_arg4 typedef PROTO :DWORD,:DWORD,:DWORD,:DWORD
externdef _imp__ExitProcess@4:PTR proto_stdcall_arg1
ExitProcess equ <_imp__ExitProcess@4>
externdef _imp__MessageBoxA@16:PTR proto_stdcall_arg4
MessageBox equ <_imp__MessageBoxA@16>
externdef _imp__GetForegroundWindow@0:PTR proto_stdcall_arg0
GetForegroundWindow equ <_imp__GetForegroundWindow@0>
externdef _imp__DirectSoundCreate@12:PTR proto_stdcall_arg3
DirectSoundCreate equ <_imp__DirectSoundCreate@12>
includelib kernel32.lib
includelib user32.lib
includelib dsound.lib

; uFMOD (DirectX)
include dsufmod.inc

.DATA

; Let's place the stream right inside the data section.
xm_length EQU 905
xm LABEL BYTE
xm_unused_000 LABEL BYTE

; *** The following 60 bytes are not used and
; *** we'll substitute them with useful data
; (the actual size and location of such gaps
; may be found out using the Eff utility)

; Wave format descriptor used to configure the DirectSound buffer
pcm	dd 20001h          ; wFormatTag <= WAVE_FORMAT_PCM, nChannels <= 2
	dd uFMOD_MixRate
	dd uFMOD_MixRate*4
	dd 100004h         ; wBitsPerSample <= 16, nBlockAlign <= 4
	dd 0               ; cbSize <= 0 (no extra info)

; DirectSound buffer descriptor
bufDesc dd 20 ; DSBUFFERDESC1
	          ; (for older DirectX versions compatibility)
	          ; (для совместимости со старыми версиями DirectX)
	dd 14000h ; dwFlags <= DSBCAPS_STICKYFOCUS OR DSBCAPS_GETCURRENTPOSITION2
	dd uFMOD_BUFFER_SIZE ; dwBufferBytes
	dd 0
	dd pcm    ; lpwfxFormat

lpDS    dd 0 ; -> IDirectSound
lpDSBuf dd 0 ; -> IDirectSoundBuffer

; Error message
szError db "DirectSound",0

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

.CODE

start:
	; EBX = 0
	; ESI = IDirectSound
	mov esi,OFFSET lpDS
	xor ebx,ebx

	; Request an instance of IDirectSound.
	invoke DirectSoundCreate,ebx,esi,ebx
	test eax,eax
	js ds_error

	; It is important to set the cooperative level to at least
	; DSSCL_PRIORITY prior to creating 16-bit stereo buffers.
	invoke GetForegroundWindow
	push 2   ; DSSCL_PRIORITY
	push eax ; hWnd
	mov eax,[esi]
	mov ecx,[eax]
	push eax ; this
	call DWORD PTR [ecx+24] ; IDirectSound::SetCooperativeLevel
	test eax,eax
	js ds_error

	; Create a secondary sound buffer.
	mov eax,[esi]
	push ebx ; pUnkOuter <= 0
	push OFFSET lpDSBuf
	push OFFSET bufDesc
	mov ecx,[eax]
	push eax ; this
	call DWORD PTR [ecx+12] ; IDirectSound::CreateSoundBuffer
	test eax,eax
	js ds_error

	; Start playback.
	invoke uFMOD_DSPlaySong,OFFSET xm,xm_length,XM_MEMORY,lpDSBuf
	test eax,eax
	js ds_error

	; Wait for user input.
	invoke MessageBox,ebx,OFFSET MsgBoxText,OFFSET MsgCaption,ebx

	; Stop playback.
	invoke uFMOD_DSPlaySong,ebx,ebx,ebx,ebx

cleanup:
	; Release DirectSound instance and free all buffers.
	mov eax,[esi]
	test eax,eax
	jz exit
	mov ecx,[eax]
	push eax
	call DWORD PTR [ecx+8] ; IDirectSound::Release

exit:
	invoke ExitProcess,ebx

ds_error:
	; Report an error.
	invoke MessageBox,ebx,OFFSET szError,ebx,10h
	jmp cleanup

END start
