' Copy mod\ufmod.mod\dsufmod.mod and mod\ufmod.mod\dsound.mod modules to [BlitzMax]\mod\ufmod.mod
' and lib\libdsound.a to [BlizMax]\lib before compiling this example for the first time.

Framework ufmod.dsufmod
Import pub.win32
Import brl.system

' Latest BlitzMax releases have a built-in DirectSound module: pub.directx.
' When in Framework mode, you can choose between uFMOD's ufmod.dsound module
' and the standart BlitzMax module according to your BlitzMax version. If not
' in Framework mode, pub.directx will be used by default in BlitzMax v1.24
' and later.

Import ufmod.dsound ' uFMOD's DirectX DirectSound module
' Import pub.directx ' Default DirectX DirectSound module for BlitzMax v1.22 and later

AppTitle = "BlitzMax"

' Set To actual sampling rate in Hz. (48KHz For Default build.)
Const uFMOD_MixRate = 48000

' Embed an XM file for playback from memory. (It is also possible to play from file and resource.)
Incbin "mini.xm"

Global DirectSound:IDirectSound
Global DirectSoundBuffer:IDirectSoundBuffer

' Request an instance of IDirectSound.
If DirectSoundCreate(Null, DirectSound, Null) < 0
	Notify "Couldn't initialize DirectSound", True
Else

	' It is important To set the cooperative level to at least
	' DSSCL_PRIORITY prior to creating 16-bit stereo buffers.
	If DirectSound.SetCooperativeLevel(GetForegroundWindow(), DSSCL_PRIORITY) < 0
		Notify "Couldn't set the cooperative level", True
	Else

		' Wave format descriptor used to configure the DirectSound buffer
		wpcm:WAVEFORMATEX = New WAVEFORMATEX
		wpcm.wFormatTag = 1 ' PCM
		wpcm.nChannels = 2 ' stereo
		wpcm.nSamplesPerSec = uFMOD_MixRate
		wpcm.nAvgBytesPerSec = uFMOD_MixRate*4
		wpcm.nBlockAlign = 4
		wpcm.wBitsPerSample = 16
		wpcm.cbSize = 0 ' (no extra info)

		' DirectSound buffer descriptor
		bufDesc:DSBUFFERDESC = New DSBUFFERDESC
		bufDesc.dwSize = 20 ' (For older DirectX versions compatibility)
		bufDesc.dwFlags = DSBCAPS_STICKYFOCUS | DSBCAPS_GETCURRENTPOSITION2
		bufDesc.dwBufferBytes = uFMOD_BUFFER_SIZE
		bufDesc.dwReserved = 0
		bufDesc.lpwfxFormat = Byte Ptr wpcm

		' Create a secondary sound buffer.
		If DirectSound.CreateSoundBuffer(Byte Ptr bufDesc, DirectSoundBuffer, Null) < 0
			Notify "Couldn't create the sound buffer", True
		Else

			' Start playback.
			If uFMOD_DSPlayMem(IncbinPtr "mini.xm", IncbinLen "mini.xm", 0, DirectSoundBuffer) = -1
				Notify "Unable to start playback", True
			Else

				' Wait for user input.
				Notify "uFMOD ruleZ!"
				' Stop playback.
				uFMOD_DSStop
			EndIf
		EndIf
	EndIf
EndIf

End