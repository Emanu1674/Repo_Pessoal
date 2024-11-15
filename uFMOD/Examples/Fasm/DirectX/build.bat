@echo off

rem Set linker: MS_LINK or POLINK
SET UF_LNKNAME=POLINK

rem Pathes:
SET UF_FASM=\fasm
SET UF_LINK=\masm32\bin
SET UF_LIBS=\masm32\lib

SET UF_LNKPATH="%UF_LINK%\link.exe"
if %UF_LNKNAME%==POLINK SET UF_LNKPATH="%UF_LINK%\polink.exe"
if not exist "%UF_FASM%\fasm.exe" goto Err1
if not exist %UF_LNKPATH% goto Err2
if not exist "%UF_LIBS%\kernel32.lib" goto Err3
if %UF_LNKNAME%==MS_LINK SET UF_LNKPATH=%UF_LNKPATH% -ignore:4078
"%UF_FASM%\fasm" mini_ds.asm
%UF_LNKPATH% /SUBSYSTEM:WINDOWS /ENTRY:start /RELEASE /OPT:nowin98 /LIBPATH:"%UF_LIBS%" /STUB:stub.bin /MERGE:.rdata=.text mini_ds.obj dsufmod.obj kernel32.lib user32.lib dsound.lib
del mini_ds.obj
goto TheEnd

:Err1
echo Couldn't find fasm.exe      in %UF_FASM%\
goto TheEnd
:Err2
if %UF_LNKNAME%==POLINK goto Err4
echo Couldn't find link.exe      in %UF_LINK%\
goto TheEnd
:Err3
echo Couldn't find library files in %UF_LIBS%\
goto TheEnd
:Err4
echo Couldn't find polink.exe    in %UF_LINK%\

:TheEnd
pause
@echo on
cls
