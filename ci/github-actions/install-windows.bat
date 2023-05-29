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

:: Lua (download from SourceForge, need PowerShell user agent to handle redirect)

echo Downloading Lua...

mkdir %DOWNLOAD_DIR%\lua
powershell "Invoke-WebRequest -UserAgent [Microsoft.PowerShell.Commands.PSUserAgent]::FireFox -Uri %LUA_DOWNLOAD_URL% -OutFile %DOWNLOAD_DIR%\lua\lua.zip"
7z x -y %DOWNLOAD_DIR%\lua\lua.zip -o%DOWNLOAD_DIR%\lua

echo set (LUA_INC_DIR %DOWNLOAD_DIR_CMAKE%/lua/include) >> paths.cmake
echo set (LUA_LIB_DIR %DOWNLOAD_DIR_CMAKE%/lua) >> paths.cmake
echo set (LUA_DLL_DIR %DOWNLOAD_DIR_CMAKE%/lua) >> paths.cmake
echo set (LUA_LIB_NAME %LUA_LIB_NAME%) >> paths.cmake
echo set (OPENSSL_INC_DIR DISABLED) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Expat

powershell "Invoke-WebRequest -Uri %EXPAT_DOWNLOAD_URL% -OutFile %DOWNLOAD_DIR%\%EXPAT_DOWNLOAD_FILE%"
7z x -y %DOWNLOAD_DIR%\%EXPAT_DOWNLOAD_FILE% -o%DOWNLOAD_DIR%
7z x -y %DOWNLOAD_DIR%\expat-%EXPAT_VERSION%.tar -o%DOWNLOAD_DIR%
ren %DOWNLOAD_DIR%\expat-%EXPAT_VERSION% expat

::..............................................................................
