; Some useful DirectSound constants and structures.
; (prefixed with UF_ to avoid naming conflicts)

#UF_DSBUFFERDESC1               = 20
#UF_DSSCL_PRIORITY              = 2
#UF_DSBCAPS_STICKYFOCUS         = $4000
#UF_DSBCAPS_GLOBALFOCUS         = $8000
#UF_DSBCAPS_GETCURRENTPOSITION2 = $10000
#XM_RESOURCE                    = 0
#XM_MEMORY                      = 1
#XM_FILE                        = 2
#XM_NOLOOP                      = 8
#XM_SUSPENDED                   = 16
#uFMOD_BUFFER_SIZE              = $10000
#uFMOD_MIN_VOL                  = 0
#uFMOD_MAX_VOL                  = 25
#uFMOD_DEFAULT_VOL              = 25

Structure UF_WAVEFORMATEX
	wFormatTag_nChannels.l
	nSamplesPerSec.l
	nAvgBytesPerSec.l
	nBlockAlign_wBitsPerSample.l
	cbSize.l
EndStructure

Structure UF_DSBUFFERDESC
	dwSize.l
	dwFlags.l
	dwBufferBytes.l
	dwReserved.l
	lpwfxFormat.l
EndStructure
; IDE Options = PureBasic v4.00 (Windows - x86)
; CursorPosition = 5
; Folding = -