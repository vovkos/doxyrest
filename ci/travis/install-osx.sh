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

brew update
brew install expat
brew install ragel

wget $LUA_URL -O $LUA_TAR
mkdir -p $LUA_DIR
tar -xzf $LUA_TAR -C $LUA_DIR --exclude=*.dylib

ls -R $LUA_DIR

echo "set (LUA_INC_DIR $LUA_DIR/include)" >> paths.cmake
echo "set (LUA_LIB_DIR $LUA_DIR)" >> paths.cmake
echo "set (LUA_LIB_NAME $LUA_LIB_NAME)" >> paths.cmake

if [ "$BUILD_PACKAGE" != "" ]; then
	brew install fakeroot
fi

exit 0 # ignore any errors
