@echo off
rem Set FPC compiler location (requires FPC v2.2 or later):
SET FPC=C:\Tools\FPC\2.2.0

if not exist "%FPC%\bin\i386-win32\fpc.exe" goto Err1
"%FPC%\bin\i386-win32\fpc" -Xs test_al.pp
goto TheEnd
:Err1
echo Couldn't find fpc.exe in %FPC%\bin\i386-win32
:TheEnd
pause
@echo on
cls
