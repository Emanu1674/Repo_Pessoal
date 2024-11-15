; Set to actual sampling rate in Hz. (48KHz is the default value.)
uFMOD_MixRate = 48000

format MS COFF

; Win32 API
extrn '__imp__MessageBoxA@16' as MessageBox:DWORD
extrn '__imp__ExitProcess@4' as ExitProcess:DWORD
extrn '__imp__GetForegroundWindow@0' as GetForegroundWindow:DWORD
extrn '__imp__DirectSoundCreate@12' as DirectSoundCreate:DWORD

; uFMOD (DirectX)
include 'dsufmod.inc'

section '.data' data readable writeable

xm file 'mini.xm'
xm_length = $ - xm

; Wave format descriptor used to configure the DirectSound buffer
pcm        dd 20001h          ; wFormatTag <= WAVE_FORMAT_PCM, nChannels <= 2
           dd uFMOD_MixRate
           dd uFMOD_MixRate*4
           dd 100004h         ; wBitsPerSample <= 16, nBlockAlign <= 4
           dd 0               ; cbSize <= 0 (no extra info)

; DirectSound buffer descriptor
bufDesc    dd 20     ; dwSize  <= DSBUFFERDESC1
                     ; (for older DirectX versions compatibility)
           dd 14000h ; dwFlags <= DSBCAPS_STICKYFOCUS OR DSBCAPS_GETCURRENTPOSITION2
           dd uFMOD_BUFFER_SIZE ; dwBufferBytes
           dd 0
           dd pcm    ; lpwfxFormat

; Error message
szError    db "DirectX error",0

MsgCaption db "FASM",0
MsgBoxText db "uFMOD ruleZ!",0

lpDS       dd 0 ; -> IDirectSound
lpDSBuf    dd 0 ; -> IDirectSoundBuffer

section '.text' code readable executable

PUBLIC _start
_start:
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
	push 2        ; DSSCL_PRIORITY
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

	; Release DirectSound instance and free all buffers.
cleanup:
	mov eax,[esi]
	test eax,eax
	jz exit
	mov ecx,[eax]
	push eax
	call DWORD [ecx + 8]  ; IDirectSound::Release

exit:
	push ebx
	call [ExitProcess]

	; Signal an error condition.
ds_error:
	push 10h    ; MB_ICONSTOP
	push ebx    ; "Error"
	push szError
	push ebx
	call [MessageBox]
	jmp cleanup