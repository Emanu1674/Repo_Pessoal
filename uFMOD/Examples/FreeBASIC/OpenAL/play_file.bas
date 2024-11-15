' A simple console file playback example.
' Using uFMOD with OpenAL.

#define WIN_INCLUDEALL
#include "windows.bi"
#include "al/al.bi"
#include "al/alc.bi"
#include "OALuFMOD.bi" ' uFMOD (OpenAL)

Dim Shared OAL_Source As Integer
Dim Shared As Integer Ptr OAL_Device, OAL_Context

Function InitOAL() As Integer
	OAL_Device = alcOpenDevice(0)
	If OAL_Device = 0 Then Return 0
	OAL_Context = alcCreateContext(OAL_Device, 0)
	alcMakeContextCurrent(OAL_Context)
	alGenSources(1, @OAL_Source)
	If alGetError = 0 Then Return 1
	Return 0
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

If InitOAL() <> 0 Then

	If uFMOD_OALPlaySong(@"mini.xm", 0, XM_FILE, OAL_Source) <> 0 Then

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
		print "-ERR: Unable to initialize OpenAL";
EndIf

End