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

if [ "$BUILD_DOC" != "" ]; then
	pip install sphinx sphinx_rtd_theme
	rvm get stable
fi
