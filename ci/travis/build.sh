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

mkdir axl/build
pushd axl/build
cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
make
popd

echo "set(AXL_CMAKE_DIR $THIS_DIR/axl/cmake $THIS_DIR/axl/build/cmake)" >> paths.cmake

if [ "$BUILD_DOC" != "" ]; then
	mkdir graco/build
	pushd graco/build
	cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
	make
	popd

	echo "set(GRACO_CMAKE_DIR $THIS_DIR/graco/cmake $THIS_DIR/graco/build/cmake)" >> paths.cmake

	mkdir luadoxyxml/build
	pushd luadoxyxml/build
	cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
	make
	popd

	echo "set(LUADOXYXML_EXE $THIS_DIR/luadoxyxml/build/bin/$BUILD_CONFIGURATION/luadoxyxml)" >> paths.cmake
fi

mkdir build
pushd build
cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
make
ctest --output-on-failure

if [ "$BUILD_PACKAGE" != "" ]; then
	fakeroot cpack -G TXZ --config CPackConfig.cmake
fi

popd

if [ "$GET_COVERAGE" != "" ]; then
	lcov --capture --directory . --no-external --output-file coverage.info
	lcov --remove coverage.info '*/axl/*' --output-file coverage.info
	lcov --list coverage.info

	curl -s https://codecov.io/bash | bash
fi

if [ "$BUILD_DOC" != "" ]; then
	pushd build

	source doc/index/build-html.sh
	source doc/build-guide/build-html.sh
	source doc/manual/build-xml.sh
	source doc/manual/build-rst.sh
	source doc/manual/build-html.sh

	source samples/libusb/build-rst.sh
	source samples/libusb/build-html.sh -D html_theme=sphinxdoc
	mv doc/html/samples/{libusb,libusb-sphinxdoc}
	source samples/libusb/build-html.sh

	source samples/libssh/build-rst.sh
	source samples/libssh/build-html.sh -D html_theme=sphinxdoc
	mv doc/html/samples/{libssh,libssh-sphinxdoc}
	source samples/libssh/build-html.sh

	source samples/alsa/build-rst.sh
	source samples/alsa/build-html.sh -D html_theme=sphinxdoc
	mv doc/html/samples/{alsa,alsa-sphinxdoc}
	source samples/alsa/build-html.sh

	source samples/apr/build-rst.sh
	source samples/apr/build-html.sh -D html_theme=sphinxdoc
	mv doc/html/samples/{apr,apr-sphinxdoc}
	source samples/apr/build-html.sh

	touch doc/html/.nojekyll

	popd
fi
