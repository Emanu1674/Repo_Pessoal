@echo off
rem Build a mixed standalone CLI/C++ uFMOD example

rem *** CONFIG START
SET VC_ENV=C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat
SET CSC_HOME=C:\Windows\Microsoft.NET\Framework\v2.0.50727
SET LNK_HOME=C:\Program Files\Microsoft Visual Studio 9.0\VC\bin
rem *** CONFIG END

if not exist "%VC_ENV%" goto Err1
if not exist "%CSC_HOME%\csc.exe" goto Err2
if not exist "%LNK_HOME%\link.exe" goto Err3
call "%VC_ENV%"
"%CSC_HOME%\csc" /target:module /addmodule:cliwrapper\i_ufmod.netmodule test.cs
"%LNK_HOME%\link" /LTCG /CLRIMAGETYPE:IJW /ENTRY:Program.Main /SUBSYSTEM:WINDOWS /ASSEMBLYMODULE:cliwrapper\i_ufmod.netmodule cliwrapper\i_ufmod.obj cliwrapper\ufmod.obj test.netmodule mini.res kernel32.lib winmm.lib
goto TheEnd

:Err1
echo Couldn't find the Visual Studio environment batch file:
echo %VC_ENV
goto TheEnd

:Err2
echo csc.exe  not found in %CSC_HOME%
goto TheEnd

:Err3
echo link.exe not found in %LNK_HOME%

:TheEnd
pause
@echo on
cls
