@echo off

rem Make the simple console file playback example
fbc -s console -a dsufmod.obj play_file.bas

rem Make the more complex GUI example featuring a visualization
fbc -s gui -a dsufmod.obj torus.bas mini.rc

pause
@echo on
cls
