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

:: Dependency versions

set LUA_VERSION=5.4.7
set LUA_DOWNLOAD_URL=https://github.com/walterschell/Lua/archive/refs/tags/v%LUA_VERSION%.zip

set EXPAT_VERSION=2.7.5
set EXPAT_VERSION_TAG=R_2_7_5
set EXPAT_DOWNLOAD_FILE=expat-%EXPAT_VERSION%.tar.gz
set EXPAT_DOWNLOAD_URL=https://github.com/libexpat/libexpat/releases/download/%EXPAT_VERSION_TAG%/%EXPAT_DOWNLOAD_FILE%

set RAGEL_DOWNLOAD_URL=https://github.com/eloraiby/ragel-windows/raw/master/ragel.exe

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
7z x -y %DOWNLOAD_DIR%\expat-%EXPAT_VERSION%.tar
ren expat-%EXPAT_VERSION% expat

::..............................................................................
