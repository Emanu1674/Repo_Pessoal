; Copy the Ufmod library to [PureBasic]\PureLibraries\UserLibraries
; before compiling this example for the first time.

; Supported OS: Win32, Linux

; uFMOD constants
#XM_RESOURCE       = 0 ; Win32 only
#XM_MEMORY         = 1
#XM_FILE           = 2
#XM_NOLOOP         = 8
#XM_SUSPENDED      = 16
#uFMOD_MIN_VOL     = 0
#uFMOD_MAX_VOL     = 25
#uFMOD_DEFAULT_VOL = 25

If uFMOD_PlaySong(?xm, ?endxm-?xm, #XM_MEMORY) <> 0
	; Pop-up a message box to let uFMOD play the XM till user input.
	; You can stop it anytime calling uFMOD_PlaySong(0,0,0).
	; PureBasic will stop it automatically before exiting.
	MessageRequester("PureBasic", "uFMOD ruleZ!")
EndIf

End

DataSection

xm: 
	IncludeBinary "mini.xm"
endxm:
  
EndDataSection
; IDE Options = PureBasic 4.10 (Windows - x86)
; Folding = -
; Executable = test.exe
; DisableDebugger