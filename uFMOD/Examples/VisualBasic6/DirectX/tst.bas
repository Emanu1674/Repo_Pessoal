Attribute VB_Name = "Tst"
Private Declare Function GetForegroundWindow Lib "user32" () As Long
Private Declare Sub CopyMem Lib "kernel32" Alias "RtlMoveMemory" (pDst As Any, pSrc As Any, ByVal ByteLen As Long)

' Set to actual sampling rate in Hz. (48KHz is the default value.)
Public Const uFMOD_MixRate As Long = 48000

Dim ds As DirectSound
Dim dsb As DirectSoundBuffer

' Returns the track's title
Function Title() As String
        Title = Space$(20)
        CopyMem ByVal Title, ByVal uFMOD_GetTitle, 20
        If Len(Trim$(Title)) = 0 Then Title = "{anonymous track}"
End Function

Sub Main()
Dim hresult As Long
On Error GoTo MainCatch

        ' It is mandatory to specify the buffer format!
        Dim waveFormat As WAVEFORMATEX
        With waveFormat
                .wFormatTag = 1 ' PCM wave
                .nChannels = 2  ' stereo
                .nSamplesPerSec = uFMOD_MixRate
                .nAvgBytesPerSec = uFMOD_MixRate * 4
                .nBlockAlign = 4
                .wBitsPerSample = 16
        End With

        Dim bufferDesc As DSBUFFERDESC
        With bufferDesc
                .dwSize = Len(bufferDesc)
                .dwFlags = DSBCAPS_GETCURRENTPOSITION2 Or DSBCAPS_STICKYFOCUS
                .dwBufferBytes = uFMOD_BUFFER_SIZE ' don't change this value
                .lpwfxFormat = VarPtr(waveFormat)
        End With

        ' Get an instance of DirectSound
        DirectSoundCreate ByVal 0&, ds, Nothing

        ' Normally, you whould specify a handle to your own form,
        ' but since we don't have one in this project, we'll grab
        ' the foreground window handle instead
        ds.SetCooperativeLevel GetForegroundWindow(), DSSCL_PRIORITY

        ' Create a secondary sound buffer for playback
        ds.CreateSoundBuffer bufferDesc, dsb, Nothing

        ' Start playback
        '    1 is the ID of the XM resource
        hresult = uFMOD_DSPlaySong(1, 0, XM_RESOURCE, dsb)
        If hresult < 0 Then Err.Rise hresult

        ' Pop-up a message box to let uFMOD play the XM till user input
        MsgBox "uFMOD ruleZ!", vbOKOnly, "Visual Basic"

        ' Stop playback
        uFMOD_DSPlaySong 0, 0, 0, Nothing

MainExit:
        ' Free DirectSound interfaces
        Set dsb = Nothing
        Set ds = Nothing
        Exit Sub

MainCatch:
        ' Trap all DirectSound exceptions here
        MsgBox Err.Description, vbCritical, "Error"
        GoTo MainExit

End Sub
