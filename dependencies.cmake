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

set (
	AXL_PATH_LIST

	LUA_INC_DIR
	LUA_LIB_DIR
	EXPAT_INC_DIR
	EXPAT_LIB_DIR
	AXL_CMAKE_DIR
	7Z_EXE
	RAGEL_EXE
	)

set (
	AXL_IMPORT_LIST

	REQUIRED
		axl
		expat
		lua
	OPTIONAL
		7z
		ragel
	)

#...............................................................................
