' Original Torus example by rel (see examples/gfx/rel-torus.bas)
' Modified version to work as an audio visualization.
' Using uFMOD with DirectX DirectSound.

#define WIN_INCLUDEALL
#define uFMOD_MixRate 48000 ' Set to actual sampling rate in Hz. (48KHz for default uFMOD build.)
'#define PTC_WIN ' Uncomment to run windowed.

#include "windows.bi"
#include "win/dsound.bi"
#include "tinyptc.bi"
#include "DSuFMOD.bi" ' uFMOD (DirectX)

Const SCR_TITLE = "FreeBASIC"
Const SCR_WIDTH = 320
Const SCR_HEIGHT = 240
Const PI_180 = 0.017453292519943295769236907684886 ' Pi/180
Const XMID = SCR_WIDTH\2
Const YMID = SCR_HEIGHT\2
Const LENS = 128

Dim Shared As Integer volume, ang, angx, angy, angz, dist, ttx, tty
Dim Shared As LPDIRECTSOUND lpDS
Dim Shared As LPDIRECTSOUNDBUFFER lpDSBuffer, lpDSBufferPrimary
Dim Shared SCR_BUFFER(0 To SCR_WIDTH * SCR_HEIGHT - 1) As Integer
Dim Shared As Single ssx, ssy, ssz, ccx, ccy, ccz, xxx, xxy, xxz, yyx, yyy, yyz, zzx, zzy, zzz
Dim Shared As Single rrx, rry, aax, aay, aaz, a, p, q, r, rad, x, y, z
Dim Shared As Ushort lvol, rvol, ll, lr
Dim Shared hW As HWND

Function InitDS(hWnd As HANDLE) As Integer
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
	If IDirectSound_SetCooperativeLevel(lpDS, hWnd, DSSCL_PRIORITY) < 0 Then Return 0
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

Sub smooth(buffer() As Integer)
Dim As Integer maxpixel, offset, pixel, r, g, b
	maxpixel = Ubound(buffer)
	For offset = SCR_WIDTH To maxpixel-SCR_WIDTH
		pixel = buffer(offset-1) Shr 2
		r = pixel Shr 16
		g = pixel Shr 8 And 63
		b = pixel And 63
		pixel = buffer(offset+1) Shr 2
		r = r + (pixel Shr 16)
		g = g + (pixel Shr 8 And 63)
		b = b + (pixel And 63)
		pixel = buffer(offset+SCR_WIDTH) Shr 2
		r = r + (pixel Shr 16)
		g = g + (pixel Shr 8 And 63)
		b = b + (pixel And 63)
		pixel = buffer(offset-SCR_WIDTH) Shr 2
		r = r + (pixel Shr 16)
		g = g + (pixel Shr 8 And 63)
		b = b + (pixel And 63)
		buffer(offset) = r Shl 16 Or g Shl 8 Or b
	Next offset
End Sub

coInitialize(0)
If ptc_open(SCR_TITLE, SCR_WIDTH, SCR_HEIGHT) <> 0 Then
	hW = GetForegroundWindow()
	If InitDS(hW) <> 0 Then

		angx = 90
		angy = 180
		angz = 270
		p    = -11
		q    = 12
		rad  = 60
		ptc_update @SCR_BUFFER(0) ' Update the screen buffer to prevent lags.

		If uFMOD_DSPlaySong(Cast(Any Ptr, 1), 0, XM_RESOURCE, lpDSBuffer) <> -1 Then
			Dim d as Integer = -1

			While Len(Inkey) = 0 And IsWindow(hW) <> 0
				volume = uFMOD_GetStats()
				lvol = Loword(volume) Shr 9
				rvol = Hiword(volume) Shr 9
				p = p + ((lvol)*.001)
				q = q - ((rvol)*.001)
				If rad <= 30 or (lvol > ll and rvol > lr) Then
					d = 5
				Elseif rad >= 60 or (lvol < ll and rvol < lr) Then
					d = -3
				EndIf
				rad += ((lvol+rvol)/ 20 ) * 0.15 * d
				angx = (angx + 1 + (1/lvol)) Mod 360
				angy = (angx + 1 + (1/(lvol+rvol))) Mod 360
				angz = (angx + 1 + (1/rvol)) Mod 360
				aax = angx * PI_180
				aay = angy * PI_180
				aaz = angz * PI_180
				ccx = Cos(aax)
				ssx = Sin(aax)
				ccy = Cos(aay)
				ssy = Sin(aay)
				ccz = Cos(aaz)
				ssz = Sin(aaz)
				xxx = ccy * ccz
				xxy = ssx * ssy * ccz - ccx * ssz
				xxz = ccx * ssy * ccz + ssx * ssz
				yyx = ccy * ssz
				yyy = ccx * ccz + ssx * ssy * ssz
				yyz = -ssx * ccz + ccx * ssy * ssz
				zzx = -ssy
				zzy = ssx * ccy
				zzz = ccx * ccy
				For ang = 0 To 359
					a = ang * PI_180
					r = .5 * (2 + Sin(q * a)) * rad
					x = Cos(p * a) * r
					y = Cos(q * a) * r
					z = Sin(p * a) * r
					dist = LENS - (x * zzx + y * zzy + z * zzz)
					If dist > 0 Then
						rrx = (x * xxx + y * xxy + z * xxz)
						rry = (x * yyx + y * yyy + z * yyz)
						ttx = XMID + (rrx * LENS / dist)
						tty = YMID - (rry * LENS / dist)
						If(tty > 0 And tty < SCR_HEIGHT-1 And ttx > 0 And ttx < SCR_WIDTH-1) Then SCR_Buffer(tty * SCR_WIDTH + ttx) = &h80FFFF
					EndIf
				Next ang
				smooth SCR_BUFFER()
				ptc_update @SCR_BUFFER(0)
				ll = lvol
				lr = rvol
				SleepEx(1, 0)
			Wend

			uFMOD_StopSong()
		EndIf
	EndIf

	PTC_CLOSE()
EndIf

CoUninitialize
End