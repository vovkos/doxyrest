:: .............................................................................
::
::  This file is part of the AXL library.
::
::  AXL is distributed under the MIT license.
::  For details see accompanying license.txt file,
::  the public copy of which is also available at:
::  http://tibbo.com/downloads/archive/axl/license.txt
::
:: .............................................................................

@echo off

:loop

if "%1" == "" goto :finalize
if /i "%1" == "msvc15" goto :msvc15
if /i "%1" == "msvc16" goto :msvc16
if /i "%1" == "msvc17" goto :msvc17
if /i "%1" == "x86" goto :x86
if /i "%1" == "i386" goto :x86
if /i "%1" == "amd64" goto :amd64
if /i "%1" == "x86_64" goto :amd64
if /i "%1" == "x64" goto :amd64

echo Invalid argument: '%1'
exit -1

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Toolchain

:msvc15
set TOOLCHAIN=msvc15
set CMAKE_GENERATOR=Visual Studio 15 2017
set LUA_TOOLCHAIN=dll15
shift
goto :loop

:msvc16
set TOOLCHAIN=msvc16
set CMAKE_GENERATOR=Visual Studio 16 2019
set CMAKE_USE_ARCH_OPTIONS=true
set LUA_TOOLCHAIN=dll16
shift
goto :loop

:msvc17
set TOOLCHAIN=msvc17
set CMAKE_GENERATOR=Visual Studio 17 2022
set CMAKE_USE_ARCH_OPTIONS=true
set LUA_TOOLCHAIN=dll16
shift
goto :loop

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Platform

:x86
set TARGET_CPU=x86
set CMAKE_ARCH_SUFFIX=
set CMAKE_ARCH_OPTIONS=-A Win32
set LUA_PLATFORM=Win32
set CHOCO_PLATFORM=--x86
set PROGRAM_FILES_DIR_SUFFIX= (x86)
shift
goto :loop

:amd64
set TARGET_CPU=amd64
set CMAKE_ARCH_SUFFIX= Win64
set CMAKE_ARCH_OPTIONS=-A x64
set LUA_PLATFORM=Win64
set CHOCO_PLATFORM=
set PROGRAM_FILES_DIR_SUFFIX=
shift
goto :loop

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:finalize

if "%TOOLCHAIN%" == "" goto :msvc14
if "%TARGET_CPU%" == "" goto :amd64
if "%CONFIGURATION%" == "" (set CONFIGURATION=Release)
if "%CMAKE_USE_ARCH_OPTIONS%" == "" (set CMAKE_GENERATOR=%CMAKE_GENERATOR%%CMAKE_ARCH_SUFFIX%)
if not "%CMAKE_USE_ARCH_OPTIONS%" == "" (set CMAKE_OPTIONS=%CMAKE_OPTIONS%%CMAKE_ARCH_OPTIONS%)

set LUA_VERSION=5.3.5
set LUA_LIB_NAME=lua53
set LUA_DOWNLOAD_URL=https://sourceforge.net/projects/luabinaries/files/%LUA_VERSION%/Windows%%20Libraries/Dynamic/lua-%LUA_VERSION%_%LUA_PLATFORM%_%LUA_TOOLCHAIN%_lib.zip/download

set EXPAT_VERSION=2.1.0
set EXPAT_VERSION_TAG=R_2_1_0
set EXPAT_DOWNLOAD_FILE=expat-%EXPAT_VERSION%.tar.gz
set EXPAT_DOWNLOAD_URL=https://github.com/libexpat/libexpat/releases/download/%EXPAT_VERSION_TAG%/%EXPAT_DOWNLOAD_FILE%

set EXPAT_CMAKE_FLAGS= ^
	-DBUILD_shared=OFF ^
	-DBUILD_examples=OFF ^
	-DBUILD_tests=OFF ^
	-DBUILD_tools=OFF

set RAGEL_DOWNLOAD_URL=https://github.com/eloraiby/ragel-windows/raw/master/ragel.exe

set CMAKE_CONFIGURE_FLAGS=-G "%CMAKE_GENERATOR%" %CMAKE_OPTIONS%

set CMAKE_BUILD_FLAGS= ^
	--config %CONFIGURATION% ^
	-- ^
	/nologo ^
	/verbosity:minimal ^
	/consoleloggerparameters:Summary

echo ---------------------------------------------------------------------------
echo LUA_LIB_NAME:       %LUA_LIB_NAME%
echo LUA_DOWNLOAD_URL:   %LUA_DOWNLOAD_URL%
echo EXPAT_DOWNLOAD_URL: %EXPAT_DOWNLOAD_URL%
echo RAGEL_DOWNLOAD_URL: %RAGEL_DOWNLOAD_URL%
echo ---------------------------------------------------------------------------
