@echo off

rem TODO: Switch between x86 and x64

set depsDir=P:\Breakout\dependencies

set compilerFlags=/nologo /MT /GR- /Gm- /EHa- /Od /Oi /FC /Zi /WX /W4 /wd4100 /I%depsDir%\sdl2\include\
set linkerFlags=/opt:ref /SUBSYSTEM:console /LIBPATH:%depsDir%\sdl2\lib\ sdl2.lib sdl2main.lib sdl2_image.lib sdl2_ttf.lib sdl2_mixer.lib

set files=..\src\breakout.cpp ..\src\entity.cpp

WHERE cl.exe >nul 2>nul
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

IF NOT EXIST build\ mkdir build
pushd build
cl %compilerFlags% %files% /link %linkerFlags%
popd
