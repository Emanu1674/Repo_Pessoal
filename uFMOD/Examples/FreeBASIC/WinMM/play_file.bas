' A simple console file playback example.
' Using uFMOD with WINMM.

#define WIN_INCLUDEALL
#include "windows.bi"
#include "uFMOD.bi" ' uFMOD (WINMM)

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

If uFMOD_PlaySong(@"mini.xm", 0, XM_FILE) <> 0 Then

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

End