@echo off
setlocal enabledelayedexpansion

IF NOT EXIST build mkdir build
IF NOT EXIST build\debug mkdir build\debug

pushd .\build\debug
set FILES=
for /R ..\..\src %%f in (*.c) do set FILES=!FILES! "%%f"

cl ^
  /Zi /Od /MDd ^
  !FILES! ^
  /I. ^
  /Fo:.\ ^
  /Fe:.\wr.exe ^
  /link ^
  /LIBPATH:..\..\lib\debug\ ^
  raylib.lib ^
  user32.lib gdi32.lib winmm.lib shell32.lib

popd
endlocal
