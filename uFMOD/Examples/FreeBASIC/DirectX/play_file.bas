' A simple console file playback example.
' Using uFMOD with DirectX DirectSound.

#define WIN_INCLUDEALL
#define uFMOD_MixRate 48000 ' Set to actual sampling rate in Hz. (48KHz for default uFMOD build.)
#include "windows.bi"
#include "win/dsound.bi"
#include "DSuFMOD.bi" ' uFMOD (DirectX)

Dim Shared lpDS As LPDIRECTSOUND
Dim Shared As LPDIRECTSOUNDBUFFER lpDSBuffer, lpDSBufferPrimary

Function InitDS() As Integer
Dim As DSBUFFERDESC dsbuffdesc
Dim As WAVEFORMATEX pcm

	With dsbuffdesc
		.dwSize = 20 ' DSBUFFERDESC1
		.dwFlags = DSBCAPS_PRIMARYBUFFER
	End With
	With pcm
		.wFormatTag = WAVE_FORMAT_PCM   ' linear PCM
		.nChannels = 2                  ' stereo
		.nSamplesPerSec = uFMOD_MixRate
		.nAvgBytesPerSec = uFMOD_MixRate * 4
		.nBlockAlign = 4
		.wBitsPerSample = 16            ' 16-bit
	End With

	If FAILED(DirectSoundCreate(0, @lpDS, 0)) Then Return 0
	If IDirectSound_SetCooperativeLevel(lpDS, GetForegroundWindow(), DSSCL_PRIORITY) < 0 Then Return 0
	dsbuffdesc.guid3DAlgorithm = GUID_NULL
	If FAILED(IDirectSound_CreateSoundBuffer(lpDS, @dsbuffdesc, cast(Any Ptr, @lpDSBufferPrimary), 0)) Or FAILED(IDirectSoundBuffer_SetFormat(lpDSBufferPrimary, @pcm)) Then Return 0

	With dsbuffdesc
		.dwFlags = DSBCAPS_GETCURRENTPOSITION2 Or DSBCAPS_STICKYFOCUS
		.dwBufferBytes = uFMOD_BUFFER_SIZE
		.lpwfxFormat = @pcm
	End With

	If FAILED(IDirectSound_CreateSoundBuffer(lpDS, @dsbuffdesc, cast(Any Ptr, @lpDSBuffer), 0)) Then Return 0
	Return 1
End Function

Sub DrawVolume(vol As Integer)
Dim As Integer i, newcolor
Dim s As String * 1
	For i=0 To 60
		newcolor = 12
		If i<8 Then
			newcolor = 10
		ElseIf i<16 Then
			newcolor = 14
		EndIf
		s = "*"
		If i>=vol Then
			s = " "
		EndIf
		color newcolor : print s;

	Next i
	print " "
End Sub

coInitialize(0)
If InitDS() <> 0 Then

	If uFMOD_DSPlaySong(@"mini.xm", 0, XM_FILE, lpDSBuffer) <> -1 Then

		' Set the title bar string for the current console window.
		SetConsoleTitle(*uFMOD_GetTitle)

		' Update VUmeters and playback time until user input.
		cls
		While len(inkey)=0
			volume=uFMOD_GetStats
			locate 1,1,0
			color 15 : print "L: "; : DrawVolume(loword(volume) shr 9)
			color 15 : print "R: "; : DrawVolume(hiword(volume) shr 9)
			color 15 : print "Time (s): " & uFMOD_GetTime\1000;
			sleep 10
		Wend

		' Stop playback.
		uFMOD_StopSong()
		cls
	Else
		print "-ERR: Make sure mini.xm is still there ;)";
	EndIf
Else
		print "-ERR: Unable to initialize DirectSound";
EndIf

CoUninitialize
End