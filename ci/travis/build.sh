#...............................................................................
#
#  This file is part of the Doxyrest toolkit.
#
#  Doxyrest is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/doxyrest/license.txt
#
#...............................................................................

THIS_DIR=`pwd`

if [ "$BUILD_DOC" != "" ]; then
	mkdir axl/build
	cd axl/build
	cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION

	cd $THIS_DIR
	echo "set (AXL_CMAKE_DIR $THIS_DIR/axl/cmake $THIS_DIR/axl/build/cmake)" >> paths.cmake

	mkdir build
	cd build
	cmake ..

	source doc/index/build-html.sh
	source doc/manual/build-html.sh
	source doc/build-guide/build-html.sh

	touch doc/html/.nojekyll
else
	mkdir axl/build
	cd axl/build
	cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
	make

	cd $THIS_DIR
	echo "set (AXL_CMAKE_DIR $THIS_DIR/axl/cmake $THIS_DIR/axl/build/cmake)" >> paths.cmake

	mkdir build
	cd build
	cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
	make
fi
