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

set (DOC_NAME "Doxyrest Documentation Index")

set (RST_DIR  ${CMAKE_CURRENT_LIST_DIR})
set (HTML_DIR ${CMAKE_CURRENT_BINARY_DIR}/html)
set (TMP_DIR  ${CMAKE_CURRENT_BINARY_DIR}/tmp)

set (SPHINX_HTML_THEME ${AXL_SPHINX_HTML_THEME})

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set (
	SPHINX_HTML_CMD_LINE
	-b html
	-d ${TMP_DIR}/sphinx-doctrees
	-c ${CMAKE_CURRENT_BINARY_DIR}
	${RST_DIR}
	${HTML_DIR}
	)

string (
	REPLACE
	";" " "
	SPHINX_HTML_CMD_LINE
	"${SPHINX_HTML_CMD_LINE}"
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

configure_file (
	conf.py.in
	conf.py
	)

if (WIN32)
	configure_file (
		${AXL_DOC_SHARE_DIR}/sh/build-html.bat.in
		build-html.bat
		)
else ()
	axl_configure_file_w_permissions (
		${AXL_DOC_SHARE_DIR}/sh/build-html.sh.in
		build-html.sh
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_WRITE GROUP_EXECUTE
		WORLD_READ WORLD_WRITE WORLD_EXECUTE
		)
endif ()

#...............................................................................
