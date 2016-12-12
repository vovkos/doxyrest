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

set (ZIP_TYPE 7z)

configure_file (
	build-zip-src.bat.in
	build-zip-src.bat
	)

configure_file (
	build-zip-bin.bat.in
	build-zip-bin.bat
	)

#...............................................................................
