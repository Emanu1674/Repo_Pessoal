$IFNDEF __uFMOD_INCLUDED__
$DEFINE __uFMOD_INCLUDED__

$USE "uFMOD.lib"

SETID "XM_RESOURCE", 0
SETID "XM_MEMORY", 1
SETID "XM_FILE", 2
SETID "XM_NOLOOP", 8
SETID "XM_SUSPENDED", 16

SETID "uFMOD_MIN_VOL", 0
SETID "uFMOD_MAX_VOL", 25
SETID "uFMOD_DEFAULT_VOL", 25

Class CuFMOD

	Public
		Declare PlaySong(lpXM:POINTER, param:INT, fdwSong:INT),POINTER
		Declare GetStats(),UINT
		Declare GetRowOrder(),UINT
		Declare GetTime(),UINT
		Declare GetTitle(),STRING
		Declare Jump2Pattern(pat:UINT),UINT
		Declare Pause(),UINT
		Declare Resume(),UINT
		Declare SetVolume(vol:UINT),UINT

EndClass

$ENDIF