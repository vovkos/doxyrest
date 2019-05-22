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

# now to official APT packages

if [ "$TARGET_CPU" != "x86" ]; then
	sudo apt-get -qq update

	sudo apt-get install -y liblua5.2-dev
else
	sudo dpkg --add-architecture i386
	sudo apt-get -qq update

	sudo apt-get install -y liblua5.2-dev:i386

	# Expat is already installed, but 64-bit only

	sudo apt-get install -y libexpat-dev:i386

	# install g++-multilib -- in the end, after i386 packages!

	sudo apt-get install -y g++-multilib

	# CMake fails to properly switch between 32-bit and 64-bit libraries on Ubuntu

	echo "set (EXPAT_LIB_DIR /usr/lib/i386-linux-gnu)" >> paths.cmake
	echo "set (LUA_LIB_DIR /usr/lib/i386-linux-gnu)" >> paths.cmake
	echo "set (OPENSSL_INC_DIR DISABLED)" >> paths.cmake
fi

if [ "$GET_COVERAGE" != "" ]; then
	sudo apt-get install -y lcov
	echo "axl_override_setting (GCC_FLAG_COVERAGE -coverage)" >> settings.cmake
fi

if [ "$BUILD_DOC" != "" ]; then
	sudo pip install sphinx==1.7.9 sphinx_rtd_theme
fi

if [ "$BUILD_PACKAGE" != "" ]; then
	sudo apt-get install -y fakeroot
fi

exit 0 # ignore any errors
