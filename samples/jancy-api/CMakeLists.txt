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

include (jancy_version.cmake)

set (DOC_NAME "Jancy C API Reference")

set (STATIC_RST_DIR ${CMAKE_CURRENT_LIST_DIR}/rst)

set (XML_DIR  ${CMAKE_CURRENT_LIST_DIR}/xml)
set (RST_DIR  ${CMAKE_CURRENT_BINARY_DIR}/rst)
set (TMP_DIR  ${CMAKE_CURRENT_BINARY_DIR}/tmp)
set (HTML_DIR ${DOXYREST_DOC_DIR}/html/samples/jancy-api)

set (SPHINX_HTML_THEME ${AXL_SPHINX_HTML_THEME})

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set (
	DOXYREST_CMD_LINE
	${XML_DIR}/index.xml
	-D g_hasNewLineAfterReturnType
	-D g_nameDelimiter=::
	-D g_introFile=intro.rst
	-o ${RST_DIR}/index.rst
	-F ${DOXYREST_FRAME_DIR}
	-f c_index.rst.in
	)

set (
	SPHINX_HTML_CMD_LINE
	-b html
	-d ${TMP_DIR}/sphinx-doctrees
	-c ${CMAKE_CURRENT_BINARY_DIR}
	${RST_DIR}
	${HTML_DIR}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set (
	CMD_LINE_VAR_LIST

	DOXYREST_CMD_LINE
	SPHINX_HTML_CMD_LINE
	)

foreach (_VAR ${CMD_LINE_VAR_LIST})
	string (
		REPLACE
		";" " "
		${_VAR}
		"${${_VAR}}"
		)
endforeach ()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

configure_file (
	conf.py.in
	conf.py
	)

if (WIN32)
	string (REPLACE "$(Configuration)" "%1" DOXYREST_EXE "${DOXYREST_EXE}")

	configure_file (
		${AXL_DOC_SHARE_DIR}/sh/build-rst.bat.in
		build-rst.bat
		)

	configure_file (
		${AXL_DOC_SHARE_DIR}/sh/build-html.bat.in
		build-html.bat
		)

	add_test (
		NAME sample-jancy-api
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND build-rst.bat $<CONFIGURATION>
		)
else ()
	axl_configure_file_w_permissions (
		${AXL_DOC_SHARE_DIR}/sh/build-rst.sh.in
		build-rst.sh
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_WRITE GROUP_EXECUTE
		WORLD_READ WORLD_WRITE WORLD_EXECUTE
		)

	axl_configure_file_w_permissions (
		${AXL_DOC_SHARE_DIR}/sh/build-html.sh.in
		build-html.sh
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_WRITE GROUP_EXECUTE
		WORLD_READ WORLD_WRITE WORLD_EXECUTE
		)

	add_test (
		NAME sample-jancy-api
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ./build-rst.sh
		)
endif ()

#...............................................................................
