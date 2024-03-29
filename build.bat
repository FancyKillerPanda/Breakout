@echo off

rem TODO: Switch between x86 and x64

set depsDir=P:\Breakout\dependencies

set compilerFlags=/nologo /Fe:breakout.exe /Fo:breakout.obj /MT /GR- /Gm- /EHsc /Od /Oi /FC /Zi /WX /W4 /wd4100 /IP:\Breakout\src /I%depsDir%\sdl2\include\
set linkerFlags=/opt:ref /SUBSYSTEM:console /LIBPATH:%depsDir%\sdl2\lib\ sdl2.lib sdl2main.lib sdl2_image.lib sdl2_ttf.lib sdl2_mixer.lib resources.res

set files=..\src\main.cpp

WHERE cl.exe >nul 2>nul
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

IF NOT EXIST build\ mkdir build
pushd build

rem NOTE(fkp): Windows only
rc /nologo /fo resources.res ../resources.rc

cl %compilerFlags% %files% /link %linkerFlags%
IF %ERRORLEVEL% EQU 0 breakout.exe

popd
