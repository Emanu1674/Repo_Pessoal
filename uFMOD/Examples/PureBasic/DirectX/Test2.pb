; Copy the DSUfmod library to [PureBasic]\PureLibraries\UserLibraries
; before compiling this example for the first time.

; Supported OS: Win32

; This is an alternative method for obtaining a DirectSound instance
; using PB native sound library and inline asm. Contributed by chris_b.

; Set to actual sampling rate in Hz. (48KHz is the default value.)
#uFMOD_MixRate = 48000

IncludeFile "dsufmod.pbi"

Global DirectSound.IDirectSound

; Request an instance of IDirectSound.
InitSound()
!extrn _PB_DirectSound 
!mov eax, [_PB_DirectSound] 
!mov [v_DirectSound],eax

; Wave format descriptor used to configure the DirectSound buffer
wpcm.UF_WAVEFORMATEX\wFormatTag_nChannels       = $20001 ; PCM stereo
wpcm.UF_WAVEFORMATEX\nSamplesPerSec             = #uFMOD_MixRate
wpcm.UF_WAVEFORMATEX\nAvgBytesPerSec            = #uFMOD_MixRate*4
wpcm.UF_WAVEFORMATEX\nBlockAlign_wBitsPerSample = $100004
wpcm.UF_WAVEFORMATEX\cbSize                     = 0 ; (no extra info)

; DirectSound buffer descriptor
bufDesc.UF_DSBUFFERDESC\dwSize        = #UF_DSBUFFERDESC1 ; (for older DirectX versions compatibility)
bufDesc.UF_DSBUFFERDESC\dwFlags       = #UF_DSBCAPS_GLOBALFOCUS | #UF_DSBCAPS_GETCURRENTPOSITION2
bufDesc.UF_DSBUFFERDESC\dwBufferBytes = #uFMOD_BUFFER_SIZE
bufDesc.UF_DSBUFFERDESC\dwReserved    = 0
bufDesc.UF_DSBUFFERDESC\lpwfxFormat   = wpcm

; Create a secondary sound buffer.
If DirectSound\CreateSoundBuffer(bufDesc, @DirectSoundBuffer.IDirectSoundBuffer, 0) < 0
	MessageRequester("Error", "Couldn't create the sound buffer")
Else

	; Start playback.
	If uFMOD_DSPlaySong(?xm, ?endxm-?xm, #XM_MEMORY, DirectSoundBuffer) = -1
		MessageRequester("Error", "Unable to start playback")
	Else

		; Pop-up a message box To let uFMOD play the XM till user input.
		; You can stop it anytime calling uFMOD_DSPlaySong(0,0,0,0).
		; PureBasic will stop it automatically before exiting.
		MessageRequester("PureBasic", "uFMOD ruleZ!")
	EndIf
EndIf

End

DataSection

xm: 
	IncludeBinary "mini.xm"
endxm:
  
EndDataSection
; IDE Options = PureBasic v3.94 (Windows - x86)
; Folding = -
; Executable = test2.exe
; DisableDebugger