@echo off
rem Build the CLI wrapper for the unmanaged static uFMOD code
rem Compiler: VC++ 2005 or later required

rem *** CONFIG START
SET VC_ENV=C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat
SET CL_HOME=C:\Program Files\Microsoft Visual Studio 9.0\VC\bin
rem *** CONFIG END

if not exist "%VC_ENV%" goto Err1
if not exist "%CL_HOME%\cl.exe" goto Err2
call "%VC_ENV%"
"%CL_HOME%\cl" /clr /LN /Zl i_ufmod.cpp ufmod.obj /link /MANIFEST:NO kernel32.lib winmm.lib
goto TheEnd

:Err1
echo Couldn't find the Visual Studio environment batch file:
echo %VC_ENV
goto TheEnd

:Err2
echo cl.exe not found in %CL_HOME%

:TheEnd
pause
@echo on
cls
