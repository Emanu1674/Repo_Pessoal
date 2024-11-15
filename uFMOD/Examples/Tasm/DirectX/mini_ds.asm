; Set to actual sampling rate in Hz. (48KHz is the default value.)
uFMOD_MixRate = 48000

.386
.model flat,stdcall

; Win32 API
EXTRN ExitProcess:PROC
EXTRN MessageBoxA:PROC
EXTRN GetForegroundWindow:PROC
EXTRN DirectSoundCreate:PROC

; uFMOD (DirectX)
include dsufmod.inc
includelib dsound.lib

.DATA

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

; Wave format descriptor used to configure the DirectSound buffer
pcm	dd 20001h ; wFormatTag <= WAVE_FORMAT_PCM, nChannels <= 2
	dd uFMOD_MixRate
	dd uFMOD_MixRate*4
	dd 100004h ; wBitsPerSample <= 16, nBlockAlign <= 4
	dd 0 ; cbSize <= 0 (no extra info)

; DirectSound buffer descriptor
bufDesc dd 20 ; DSBUFFERDESC1
	; (for older DirectX versions compatibility)
	dd 14000h ; dwFlags <= DSBCAPS_STICKYFOCUS OR DSBCAPS_GETCURRENTPOSITION2
	dd uFMOD_BUFFER_SIZE ; dwBufferBytes
	dd 0,pcm ; lpwfxFormat

; Error message
szError db "DirectX error",0

MsgCaption db "TASM",0
MsgBoxText db "uFMOD ruleZ!",0

lpDS    dd 0 ; -> IDirectSound
lpDSBuf dd 0 ; -> IDirectSoundBuffer

.CODE

start:
	mov esi,OFFSET lpDS
	xor ebx,ebx

	; Request an instance of IDirectSound
	call DirectSoundCreate, ebx, esi, ebx
	test eax,eax
	js ds_error

	; It is important to set the cooperative level to at least
	; DSSCL_PRIORITY prior to creating 16-bit stereo buffers.
	call GetForegroundWindow
	mov edx,[esi]
	mov ecx,[edx]
	call DWORD PTR [ecx+24], edx, eax, 2
	test eax,eax
	js ds_error

	; Create a secondary sound buffer.
	; IDirectSound::CreateSoundBuffer
	mov eax,[esi]
	mov ecx,[eax]
	call DWORD PTR [ecx+12], eax, OFFSET bufDesc, OFFSET lpDSBuf, ebx
	test eax,eax
	js ds_error

	; Start playback.
	call uFMOD_DSPlaySong, OFFSET xm, xm_length, XM_MEMORY, lpDSBuf
	test eax,eax
	js ds_error

	; Wait for user input.
	call MessageBoxA, ebx, OFFSET MsgBoxText, OFFSET MsgCaption, ebx

	; Stop playback.
	call uFMOD_DSPlaySong, ebx, ebx, ebx, ebx

cleanup:
	; Release DirectSound instance and free all buffers.
	mov eax,[esi]
	test eax,eax
	jz exit
	; IDirectSound::Release
	mov ecx,[eax]
	call DWORD PTR [ecx+8], eax

exit:
	call ExitProcess, ebx

ds_error:
	; Report an error condition.
	call MessageBoxA, ebx, OFFSET szError, ebx, 10h
	jmp cleanup

END start