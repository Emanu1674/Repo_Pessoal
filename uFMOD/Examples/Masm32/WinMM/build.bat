@echo off
rem Set compiler location:
SET MASM32=\masm32

if not exist "%MASM32%\bin\ml.exe" goto Err1
"%MASM32%\bin\ml" /c /coff /Cp minimal.asm
"%MASM32%\bin\link" /SUBSYSTEM:WINDOWS /OPT:nowin98 /LIBPATH:"%MASM32%\lib" /STUB:stub.bin /MERGE:.rdata=.text -ignore:4078 minimal.obj ufmod.lib
del minimal.obj
goto TheEnd
:Err1
echo Couldn't find ml.exe in %MASM32%\bin
:TheEnd
pause
cls
