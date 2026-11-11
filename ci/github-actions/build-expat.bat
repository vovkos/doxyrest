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

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set THIS_DIR=%CD%
set THIS_DIR_CMAKE=%THIS_DIR:\=/%

mkdir expat\.build
cd expat\.build

cmake .. ^
	-DCMAKE_INSTALL_PREFIX=%THIS_DIR_CMAKE%/expat/install ^
	-DEXPAT_SHARED_LIBS=OFF ^
	-DEXPAT_BUILD_EXAMPLES=OFF ^
	-DEXPAT_BUILD_TESTS=OFF ^
	-DEXPAT_BUILD_TOOLS=OFF ^
	-DEXPAT_BUILD_DOCS=OFF ^
	-DEXPAT_BUILD_PKGCONFIG=OFF ^
	-DEXPAT_DEBUG_POSTFIX= ^
	-DEXPAT_RELEASE_POSTFIX= ^
	%CMAKE_CONFIGURE_FLAGS% ^
	|| exit

cmake ^
	--build . ^
	--target install %CMAKE_BUILD_FLAGS% ^
	|| exit

cd %THIS_DIR%

echo set(EXPAT_INC_DIR %THIS_DIR_CMAKE%/expat/install/include) >> paths.cmake
echo set(EXPAT_LIB_DIR %THIS_DIR_CMAKE%/expat/install/lib) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
