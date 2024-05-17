::..............................................................................
::
::  This file is part of the AXL library.
::
::  AXL is distributed under the MIT license.
::  For details see accompanying license.txt file,
::  the public copy of which is also available at:
::  http://tibbo.com/downloads/archive/axl/license.txt
::
::..............................................................................

@echo off

set DOWNLOAD_DIR=%CD:~0,2%\downloads
set DOWNLOAD_DIR_CMAKE=%DOWNLOAD_DIR:\=/%

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Ragel

echo Downloading Ragel...

mkdir %DOWNLOAD_DIR%
powershell "Invoke-WebRequest -Uri %RAGEL_DOWNLOAD_URL% -OutFile %DOWNLOAD_DIR%\ragel.exe"

echo set (RAGEL_EXE %DOWNLOAD_DIR_CMAKE%/ragel.exe) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Lua (CMake-based)

echo Downloading Lua...

mkdir %DOWNLOAD_DIR%\lua
powershell "Invoke-WebRequest -Uri %LUA_DOWNLOAD_URL% -OutFile %DOWNLOAD_DIR%\lua\lua.zip"
7z x -y %DOWNLOAD_DIR%\lua\lua.zip
ren Lua-%LUA_VERSION% lua

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Expat

powershell "Invoke-WebRequest -Uri %EXPAT_DOWNLOAD_URL% -OutFile %DOWNLOAD_DIR%\%EXPAT_DOWNLOAD_FILE%"
7z x -y %DOWNLOAD_DIR%\%EXPAT_DOWNLOAD_FILE% -o%DOWNLOAD_DIR%
7z x -y %DOWNLOAD_DIR%\expat-%EXPAT_VERSION%.tar -o%DOWNLOAD_DIR%
ren %DOWNLOAD_DIR%\expat-%EXPAT_VERSION% expat

::..............................................................................
