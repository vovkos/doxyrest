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

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
	# lcov doesn't work with clang on ubuntu out-of-the-box
	# also, coverage should be collected without optimizations

	if [ "$CC" != "clang" ] && [ "$BUILD_CONFIGURATION" == "Debug" ]; then
		export GET_COVERAGE=ON
	fi

	if [ "$TARGET_CPU" == "amd64" ] && \
	   [ "$CC" == "gcc" ] && \
	   [ "$BUILD_CONFIGURATION" == "Release" ]; then
		export BUILD_DOC=ON
	fi
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then

	# it's not as easy to install lua-5.2 side-by-side with lua-5.3 on OSX
	# we better use static linking to lua as to avoid versioning issues

	LUA_VERSION=5.3.4
	LUA_LIB_NAME=lua53
	LUA_OS=MacOS1011

	export LUA_TAR=lua-${LUA_VERSION}_${LUA_OS}_lib.tar.gz
	export LUA_URL=https://sourceforge.net/projects/luabinaries/files/$LUA_VERSION/Other%20Libraries/${LUA_TAR}/download
	export LUA_DIR=$TRAVIS_BUILD_DIR/lua-$LUA_VERSION
fi

if [ "$TRAVIS_TAG" != "" ] && [ "$BUILD_CONFIGURATION" == "Release" ] && \
	([ "$TRAVIS_OS_NAME" != "linux" ] || [ "$CC" == "gcc" ]); then
	export BUILD_PACKAGE=ON
fi
