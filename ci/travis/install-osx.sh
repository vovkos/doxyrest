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
brew install lua
brew install expat
brew install ragel

# coverage should be collected without optimizations

if [ "$BUILD_CONFIGURATION" == "Debug" ]; then
	brew install lcov
	echo "axl_override_setting (GCC_FLAG_COVERAGE -coverage)" >> settings.cmake
else
 	BUILD_DOC=ON

	pip install sphinx sphinx_rtd_theme
	rvm get stable
fi
