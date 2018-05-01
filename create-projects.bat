@echo off

rem Generate solutions
tools\premake5 vs2017 --vcpkg-dir=%VCPKG_DIR%
pause
