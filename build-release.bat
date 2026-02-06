@echo off
setlocal enabledelayedexpansion

IF NOT EXIST build mkdir build
IF NOT EXIST build\release mkdir build\release

set SOURCES=
for /R . %%f in (*.cpp) do set SOURCES=!SOURCES! "%%f"

cl ^
  /O2 /MD ^
  !SOURCES! ^
  /I. ^
  /Fo:.\build\release\ ^
  /Fe:.\build\release\wr.exe ^
  /link ^
  /LIBPATH:.\lib\release\ ^
  raylib.lib ^
  user32.lib gdi32.lib winmm.lib shell32.lib

endlocal
