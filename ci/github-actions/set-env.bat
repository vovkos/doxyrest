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
if /i "%1" == "arm64" goto :arm64
if /i "%1" == "aarch64" goto :arm64

echo Invalid argument: '%1'
exit -1

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Toolchain

:msvc15
set TOOLCHAIN=msvc15
set CMAKE_GENERATOR=Visual Studio 15 2017
shift
goto :loop

:msvc16
set TOOLCHAIN=msvc16
set CMAKE_GENERATOR=Visual Studio 16 2019
set CMAKE_USE_ARCH_OPTIONS=true
shift
goto :loop

:msvc17
set TOOLCHAIN=msvc17
set CMAKE_GENERATOR=Visual Studio 17 2022
set CMAKE_USE_ARCH_OPTIONS=true
shift
goto :loop

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Platform

:x86
set TARGET_CPU=x86
set CMAKE_ARCH_SUFFIX=
set CMAKE_ARCH_OPTIONS=-A Win32
shift
goto :loop

:amd64
set TARGET_CPU=amd64
set CMAKE_ARCH_SUFFIX= Win64
set CMAKE_ARCH_OPTIONS=-A x64
shift
goto :loop

:arm64
set TARGET_CPU=arm64
set CMAKE_ARCH_SUFFIX=
set CMAKE_ARCH_OPTIONS=-A ARM64
shift
goto :loop

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:finalize

if "%TOOLCHAIN%" == "" goto :msvc17
if "%TARGET_CPU%" == "" goto :amd64
if "%CONFIGURATION%" == "" (set CONFIGURATION=Release)
if "%CMAKE_USE_ARCH_OPTIONS%" == "" (set CMAKE_GENERATOR=%CMAKE_GENERATOR%%CMAKE_ARCH_SUFFIX%)
if not "%CMAKE_USE_ARCH_OPTIONS%" == "" (set CMAKE_OPTIONS=%CMAKE_OPTIONS%%CMAKE_ARCH_OPTIONS%)

set LUA_VERSION=5.4.7
set LUA_DOWNLOAD_URL=https://github.com/walterschell/Lua/archive/refs/tags/v%LUA_VERSION%.zip

set EXPAT_VERSION=2.7.5
set EXPAT_VERSION_TAG=R_2_7_5
set EXPAT_DOWNLOAD_FILE=expat-%EXPAT_VERSION%.tar.gz
set EXPAT_DOWNLOAD_URL=https://github.com/libexpat/libexpat/releases/download/%EXPAT_VERSION_TAG%/%EXPAT_DOWNLOAD_FILE%

set EXPAT_CMAKE_FLAGS= ^
	-DEXPAT_SHARED_LIBS=OFF ^
	-DEXPAT_BUILD_EXAMPLES=OFF ^
	-DEXPAT_BUILD_TESTS=OFF ^
	-DEXPAT_BUILD_TOOLS=OFF ^
	-DEXPAT_BUILD_DOCS=OFF ^
	-DEXPAT_BUILD_PKGCONFIG=OFF ^
	-DEXPAT_DEBUG_POSTFIX= ^
	-DEXPAT_RELEASE_POSTFIX=

set RAGEL_DOWNLOAD_URL=https://github.com/eloraiby/ragel-windows/raw/master/ragel.exe

set CMAKE_CONFIGURE_FLAGS=-G "%CMAKE_GENERATOR%" %CMAKE_OPTIONS%

set CMAKE_BUILD_FLAGS= ^
	--config %CONFIGURATION% ^
	-- ^
	/nologo ^
	/verbosity:minimal ^
	/consoleloggerparameters:Summary

echo ---------------------------------------------------------------------------
echo LUA_DOWNLOAD_URL:   %LUA_DOWNLOAD_URL%
echo EXPAT_DOWNLOAD_URL: %EXPAT_DOWNLOAD_URL%
echo RAGEL_DOWNLOAD_URL: %RAGEL_DOWNLOAD_URL%
echo ---------------------------------------------------------------------------
