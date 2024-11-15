' Copy the mod\ufmod.mod\ufmod.mod module to [BlitzMax]\mod\ufmod.mod
' before compiling this example for the first time.

Framework ufmod.ufmod

Import brl.system

AppTitle = "BlitzMax"

Incbin "mini.xm"

If uFMOD_PlayMem(IncbinPtr "mini.xm",IncbinLen "mini.xm",0)
	Notify "uFMOD ruleZ!"
	uFMOD_Stop
EndIf

End
