import brl.blitz
XM_NOLOOP%=8
XM_SUSPENDED%=16
uFMOD_MIN_VOL%=0
uFMOD_MAX_VOL%=25
uFMOD_DEFAULT_VOL%=25
uFMOD_PlayFile%(filename$z,dwReserved%,dwFlags%)S="BB_PlayFile"
uFMOD_PlayMem%(pXM@*,length%,dwFlags%)S="BB_PlayMem"
uFMOD_PlayRes%(dwName%,hModule%,dwFlags%)S="uFMOD_PlaySong"
uFMOD_Stop%()="BB_Stop"
uFMOD_GetStats%()="uFMOD_GetStats"
uFMOD_GetRowOrder%()="uFMOD_GetRowOrder"
uFMOD_GetTime%()="uFMOD_GetTime"
uFMOD_GetTitle$z()="uFMOD_GetTitle"
uFMOD_Pause%()="uFMOD_Pause"
uFMOD_Resume%()="uFMOD_Resume"
uFMOD_SetVolume%(vol%)S="uFMOD_SetVolume"
uFMOD_Jump2Pattern%(pat%)S="uFMOD_Jump2Pattern"
