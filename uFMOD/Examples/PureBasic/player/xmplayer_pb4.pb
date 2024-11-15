; -----------------------------
; VuMeter version 0.7 by Flaith
; Compiles with PureBasic v4
; -----------------------------

;-uFMOD constants
#XM_RESOURCE       = 0
#XM_MEMORY         = 1
#XM_FILE           = 2
#XM_NOLOOP         = 8
#XM_SUSPENDED      = 16
#uFMOD_MIN_VOL     = 0
#uFMOD_MAX_VOL     = 25
#uFMOD_DEFAULT_VOL = 25

Enumeration
 #MainWindow
 #Vu_Meter_0
 #Vu_Meter_1
 #Gadget_VM_0
 #Gadget_VM_1
 #Toolbar
 #TrackBar
 #Lib_Time
 #But_Open
 #But_Stop
 #But_Pause
 #But_Play
EndEnumeration

;-Valeurs fixes
;-Fixed values
#_Width     = 20
#_Height    = 32
#_NBColor   = 32
#_Sep       = 1
#TITLE      = "Tiny XM Player"
#width      = #_Width - 1
#height     = #_Height - 1
#NBColor    = #_NBColor - 1
#_Step      = #_Height / #_NBColor
#_LogVAL    = #height / 4
#Intervalle = 2

;-Global
Global Font1 = LoadFont(0, "Courier", 10)
Global buf.s, playing.s
Global woc.WAVEOUTCAPS
Global uaux.l, *hWave.l, oldseconds.l, vol_oldL.l, vol_oldR.l, timer.l

;-Tables des couleurs
;-Colors table
Global Dim vuOn(#NBColor)
Global Dim vuOff(#NBColor)

;-Initialisation
Restore ValVM_ON
For i = 0 To #NBColor
 Read vuOn(i)
Next i

Restore ValVM_OFF
For i = 0 To #NBColor
 Read vuOff(i)
Next i

;-Creation de l'image VuMeter
;-Create image of the VuMeter
If CreateImage(#Vu_Meter_0,#width,#height)
  StartDrawing(ImageOutput(#Vu_Meter_0))

  Box (0,0,#width,#height,RGB(0, 0, 0))
  For i = 0 To #height
    vv = Round((i/#_Step),0)
    LineXY(0,i,#width,i,RGB(vuOff(vv) >> 8, vuOff(vv) & $00FF, 0))
  Next i

  For i = #height To 0 Step -#Intervalle
    Box(0, i, #width, #_Sep, RGB(0, 0, 0))
  Next i

  StopDrawing()
EndIf

;-Creation d'une autre image
;-Copy another one
CopyImage(#Vu_Meter_0, #Vu_Meter_1)

;-[Procedure]
Procedure VM_Show(Canal.l, value.l)
Protected vv.l, vg.l, vm.l

  Select Canal
  Case 0 :
    vg = #Gadget_VM_0
    vm = #Vu_Meter_0
  Case 1 :
    vg = #Gadget_VM_1
    vm = #Vu_Meter_1
  EndSelect

  StartDrawing(ImageOutput(vm))
  ;VuMetre a OFF
  For i = 0 To #height
    vv = Round((i/#_Step),0)
    LineXY(0, i, #width, i, RGB(vuOff(vv) >> 8, vuOff(vv) & $00FF, 0))
  Next i

  ;Affichage des niveaux ssi la valeur est > a 0
  If value > 0
    For i = #height-value To #height
      vv = Round((i/#_Step),0)
      LineXY(0, i, #width, i, RGB(vuOn(vv) >> 8, vuOn(vv) & $00FF, 0))
    Next i
  EndIf

  For i = #height To 0 Step -#Intervalle
    Box(0, i, #width, #_Sep, RGB(0, 0, 0))
  Next i

  StopDrawing()
  SetGadgetState(vg, ImageID(vm))
EndProcedure

Procedure TimerProc(hwnd.l, uMsg.l, idEvent.l, dwTime.l)

 useconds = uFMOD_GetTime() / 1000
 If useconds <> oldseconds
   oldseconds = useconds
   uminutes = useconds / 60
   uhours = uminutes / 60
   hh$ = RSet(Str(uhours % 24), 2, "0")
   mm$ = RSet(Str(uminutes % 60), 2, "0")
   ss$ = RSet(Str(useconds % 60), 2, "0")
   SetGadgetText(#Lib_Time, hh$+":"+mm$+":"+ss$)
 EndIf

 uaux = uFMOD_GetStats() ; coded as $XXXXXXXX - DWORD

 vol_newL = uaux >> 16
 vol_newR = uaux & $FFFF

 vol_newL = #_LogVAL * Log10(vol_newL)
 vol_newR = #_LogVAL * Log10(vol_newR)

 If vol_newL <> vol_oldL
   vol_oldL = vol_newL
   VM_Show(0, vol_newL)
 EndIf

 If vol_newR <> vol_oldR
   vol_oldR = vol_newR
   VM_Show(1, vol_newR)
 EndIf

 If idEvent = timer ; Timer n° 2
     KillTimer_(0, timer)
     If *hWave
       SetWindowTitle(#MainWindow, playing)
     EndIf
 EndIf

EndProcedure

Procedure.l PlayXM()
 theFile.s = OpenFileRequester("Open XM file", "", "XM Files (*.xm)|*.xm", 0)

 If theFile = "" : ProcedureReturn 0 : EndIf

 *hWave = uFMOD_PlaySong(theFile, 0, #XM_FILE)
 If *hWave = 0
   MessageRequester("*** ERROR ***","Unable to play "+Chr(34)+theFile+Chr(34))
 Else
   DisableToolBarButton(#ToolBar, #But_Open, 1)
   DisableToolBarButton(#ToolBar, #But_Stop, 0)
   DisableToolBarButton(#ToolBar, #But_Pause, 0)
   DisableGadget(#TrackBar, 0)

   XM_Title.s = Trim(uFMOD_GetTitle())
   If Len(XM_Title) = 0
     XM_Title = "{anonymous track}"
   EndIf
   playing = "Playing "+XM_Title
   SetWindowTitle(#MainWindow, playing)
 EndIf
EndProcedure

Procedure StopXM()
 uFMOD_PlaySong(0, 0, 0)
 *hWave = 0
 DisableToolBarButton(#ToolBar, #But_Open, 0)
 DisableToolBarButton(#ToolBar, #But_Stop, 1)
 DisableToolBarButton(#ToolBar, #But_Pause, 1)
 DisableToolBarButton(#ToolBar, #But_Play, 1)
 DisableGadget(#TrackBar, 1)
 SetWindowTitle(#MainWindow, #TITLE)
 VM_Show(0,0)
 VM_Show(1,0)
EndProcedure

Procedure PauseXM()
 If *hWave
   uFMOD_Pause()
   DisableToolBarButton(#ToolBar, #But_Pause, 1)
   DisableToolBarButton(#ToolBar, #But_Play, 0)
 EndIf
EndProcedure

Procedure ResumeXM()
 If *hWave
   uFMOD_Resume()
   DisableToolBarButton(#ToolBar, #But_Pause, 0)
   DisableToolBarButton(#ToolBar, #But_Play, 1)
 EndIf
EndProcedure

;-[Main]
If OpenWindow(#MainWindow, 0, 0, 328, 40, #TITLE,  #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
 If CreateToolBar(#ToolBar,WindowID(#MainWindow))
   CatchImage(#But_Open, ?tbar_open)             : ToolBarImageButton(#But_Open, ImageID(#But_Open))
   ToolBarToolTip(#ToolBar, #But_Open, "Open")

   CatchImage(#But_Stop, ?tbar_stop)             : ToolBarImageButton(#But_Stop, ImageID(#But_Stop))
   ToolBarToolTip(#ToolBar, #But_Stop, "Stop")   : DisableToolBarButton(#ToolBar, #But_Stop, 1)

   CatchImage(#But_Pause, ?tbar_pause)           : ToolBarImageButton(#But_Pause, ImageID(#But_Pause))
   ToolBarToolTip(#ToolBar, #But_Pause, "Pause") : DisableToolBarButton(#ToolBar, #But_Pause, 1)

   CatchImage(#But_Play, ?tbar_play)             : ToolBarImageButton(#But_Play, ImageID(#But_Play))
   ToolBarToolTip(#ToolBar, #But_Play, "Play")   : DisableToolBarButton(#ToolBar, #But_Play, 1)

   hToolBar.l = GetWindow_(WindowID(#MainWindow), #GW_CHILD)
   SetWindowLong_(hToolBar, #GWL_STYLE, $5000094D)
   MoveWindow_(hToolBar, 5, 10, 95, 50, 0)
 EndIf

 If CreateGadgetList(WindowID(#MainWindow))
   ImageGadget(#Gadget_VM_0, 280,           4, 0, 0, ImageID(#Vu_Meter_0))
   ImageGadget(#Gadget_VM_1, 280+#_Width+4, 4, 0, 0, ImageID(#Vu_Meter_1))

   TrackBarGadget(#TrackBar, 100, 10, 100, 22, 0, 100)
   SetGadgetState(#TrackBar, 100)
   TextGadget(#Lib_Time, 205, 14, 70, 22, "00:00:00")
   SetGadgetFont(#Lib_Time, Font1)
 EndIf

 ; Set a timer to update both playback progress and VU meters
 oldseconds = -1
 SetTimer_(0, 0, 64, @TimerProc())

 quitMain_Form=0
 Repeat
   EventID  =WaitWindowEvent()
   MenuID   =EventMenu()
   GadgetID =EventGadget()
   WindowID =EventWindow()

   Select EventID
     Case #PB_Event_CloseWindow
       If WindowID = 0
         quitMain_Form=1
       EndIf

     Case #PB_Event_Gadget
       Select GadgetID
         Case #TrackBar
           If *hWave
             uaux = GetGadgetState(#TrackBar)
             buf = "VOL: "+StrU(uaux,#Long)+"%"
             SetWindowTitle(#MainWindow, buf)
             uFMOD_SetVolume(uaux * #uFMOD_MAX_VOL / 100)
             timer = SetTimer_(0, 0, 1024, @TimerProc())
           EndIf
       EndSelect

     Case #PB_Event_Menu
       Select MenuID
         Case #But_Open  : PlayXM()
         Case #But_Stop  : StopXM()
         Case #But_Pause : PauseXM()
         Case #But_Play  : ResumeXM()
       EndSelect

   EndSelect
 Until quitMain_Form

EndIf

End

;-Datasection
DataSection

 ValVM_ON:
   Data.l $FF10
   Data.l $FF20
   Data.l $FF30
   Data.l $FF40
   Data.l $FF50
   Data.l $FF60
   Data.l $FF70
   Data.l $FF80
   Data.l $FF90
   Data.l $FFA0
   Data.l $FFB0
   Data.l $FFC0
   Data.l $FFD0
   Data.l $FFE0
   Data.l $FFF0
   Data.l $FFFF

   Data.l $EFFF
   Data.l $DEFF
   Data.l $CFFF
   Data.l $BFFF
   Data.l $AFFF
   Data.l $9FFF
   Data.l $8FFF
   Data.l $7FFF
   Data.l $6FFF
   Data.l $5FFF
   Data.l $4FFF
   Data.l $3FFF
   Data.l $2FFF
   Data.l $1FFF
   Data.l $0FFF
   Data.l $00FF

 ValVM_OFF:
   Data.l $6606
   Data.l $660C
   Data.l $6613
   Data.l $6619
   Data.l $6620
   Data.l $6626
   Data.l $662C
   Data.l $6633
   Data.l $6639
   Data.l $6640
   Data.l $6646
   Data.l $664C
   Data.l $6653
   Data.l $6659
   Data.l $6660
   Data.l $6666

   Data.l $5F66
   Data.l $5966
   Data.l $5266
   Data.l $4C66
   Data.l $4666
   Data.l $3F66
   Data.l $3966
   Data.l $3266
   Data.l $2C66
   Data.l $2666
   Data.l $1F66
   Data.l $1966
   Data.l $1266
   Data.l $0C66
   Data.l $0666
   Data.l $0066

 tbar_open:  IncludeBinary "load.ico"
 tbar_stop:  IncludeBinary "stop.ico"
 tbar_pause: IncludeBinary "pause.ico"
 tbar_play:  IncludeBinary "resume.ico"

EndDataSection
; IDE Options = PureBasic v4.00 (Windows - x86)
; Folding = --
; UseIcon = player.ico
; Executable = XMPlayer.exe
; DisableDebugger