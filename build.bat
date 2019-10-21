@echo off

rem TODO: Switch between x86 and x64

set compilerFlags=/nologo /MT /GR- /Gm- /EHa- /Od /Oi /FC /Zi /WX /W4 /wd4100
set linkerFlags=/opt:ref

WHERE cl.exe >nul 2>nul
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

IF NOT EXIST build\ mkdir build
pushd build
cl %compilerFlags% ..\src\breakout.cpp /link %linkerFlags%
popd
