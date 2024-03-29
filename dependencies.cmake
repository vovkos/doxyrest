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

set(
	AXL_PATH_LIST

	LUA_INC_DIR
	LUA_LIB_DIR
	LUA_LIB_NAME
	EXPAT_INC_DIR
	EXPAT_LIB_DIR
	AXL_CMAKE_DIR
	LUADOXYXML_EXE
	SPHINX_BUILD_EXE
	PDFLATEX_EXE
)

set(
	AXL_IMPORT_LIST

	REQUIRED
		axl
		expat
		lua
	OPTIONAL
		luadoxyxml
		sphinx
		latex
)

set(
	AXL_IMPORT_DIR_LIST
	${CMAKE_CURRENT_LIST_DIR}/cmake
)

#...............................................................................
