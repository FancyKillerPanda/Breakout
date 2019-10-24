@echo off

call build.bat

pushd build
breakout.exe
popd
