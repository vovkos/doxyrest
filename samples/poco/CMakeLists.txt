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

set(DOC_NAME "POCO C++ Libraries Documentation")

set(POCO_VERSION_MAJOR 1)
set(POCO_VERSION_MINOR 7)
set(POCO_VERSION_REVISION 8)

set(POCO_VERSION_COMPANY "POCO Maintainers")
set(POCO_VERSION_YEARS   "2005-2017")

set(XML_TAR_FILE ${CMAKE_CURRENT_LIST_DIR}/xml.tar.xz)
set(STATIC_RST_DIR)

set(XML_DIR  ${CMAKE_CURRENT_BINARY_DIR}/xml)
set(RST_DIR  ${CMAKE_CURRENT_BINARY_DIR}/rst)
set(TMP_DIR  ${CMAKE_CURRENT_BINARY_DIR}/tmp)
set(HTML_DIR ${DOXYREST_DOC_DIR}/html/samples/poco)

set(SPHINX_HTML_THEME ${AXL_SPHINX_HTML_THEME})

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set(
	DOXYREST_CMD_LINE
	${XML_DIR}/index.xml
	-D ML_SPECIFIER_MODIFIER_LIST
	-D VERBATIM_TO_CODE_BLOCK=cpp
	-D \"g_indexTitle=${DOC_NAME}\"
	-D ESCAPE_ASTERISKS
	-D PROTECTION_FILTER=protected
	-o ${RST_DIR}/index.rst
	-F ${DOXYREST_FRAME_DIR}/cfamily
	-F ${DOXYREST_FRAME_DIR}/common
	-f index.rst.in
	)

set(
	SPHINX_HTML_CMD_LINE
	-b html
	-d ${TMP_DIR}/sphinx-doctrees
	-c ${CMAKE_CURRENT_BINARY_DIR}
	${RST_DIR}
	${HTML_DIR}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set(
	CMD_LINE_VAR_LIST

	DOXYREST_CMD_LINE
	SPHINX_HTML_CMD_LINE
	)

foreach(_VAR ${CMD_LINE_VAR_LIST})
	string(
		REPLACE
		";" " "
		${_VAR}
		"${${_VAR}}"
		)
endforeach()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

configure_file(
	conf.py.in
	conf.py
	)

if(WIN32)
	string(REPLACE "$(Configuration)" "%1" DOXYREST_EXE "${DOXYREST_EXE}")

	configure_file(
		${DOXYREST_SAMPLE_SH_DIR}/unpack-xml.bat.in
		unpack-xml.bat
		)

	configure_file(
		${DOXYREST_SAMPLE_SH_DIR}/test.bat.in
		test.bat
		)

	configure_file(
		${AXL_DOC_SHARE_DIR}/sh/build-rst.bat.in
		build-rst.bat
		)

	configure_file(
		${AXL_DOC_SHARE_DIR}/sh/build-html.bat.in
		build-html.bat
		)

	add_test(
		NAME sample-poco
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND test.bat $<CONFIGURATION>
		)
else()
	axl_configure_file_w_permissions(
		${DOXYREST_SAMPLE_SH_DIR}/unpack-xml.sh.in
		unpack-xml.sh
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_WRITE GROUP_EXECUTE
		WORLD_READ WORLD_WRITE WORLD_EXECUTE
		)

	axl_configure_file_w_permissions(
		${DOXYREST_SAMPLE_SH_DIR}/test.sh.in
		test.sh
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_WRITE GROUP_EXECUTE
		WORLD_READ WORLD_WRITE WORLD_EXECUTE
		)

	axl_configure_file_w_permissions(
		${AXL_DOC_SHARE_DIR}/sh/build-rst.sh.in
		build-rst.sh
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_WRITE GROUP_EXECUTE
		WORLD_READ WORLD_WRITE WORLD_EXECUTE
		)

	axl_configure_file_w_permissions(
		${AXL_DOC_SHARE_DIR}/sh/build-html.sh.in
		build-html.sh
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_WRITE GROUP_EXECUTE
		WORLD_READ WORLD_WRITE WORLD_EXECUTE
		)

	add_test(
		NAME sample-poco
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ./test.sh
		)
endif()

#...............................................................................
