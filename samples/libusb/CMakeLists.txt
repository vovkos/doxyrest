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

set(DOC_NAME "LibUSB Documentation")

set(LIBUSB_VERSION_MAJOR 1)
set(LIBUSB_VERSION_MINOR 0)
set(LIBUSB_VERSION_REVISION 21)

set(LIBUSB_VERSION_COMPANY "LibUSB Maintainers")
set(LIBUSB_VERSION_YEARS   "2007-2017")

set(XML_TAR_FILE ${CMAKE_CURRENT_LIST_DIR}/xml.tar.xz)
set(STATIC_RST_DIR)

set(XML_DIR  ${CMAKE_CURRENT_BINARY_DIR}/xml)
set(RST_DIR  ${CMAKE_CURRENT_BINARY_DIR}/rst)
set(TMP_DIR  ${CMAKE_CURRENT_BINARY_DIR}/tmp)
set(HTML_DIR ${DOXYREST_DOC_DIR}/html/samples/libusb)

set(SPHINX_HTML_THEME ${AXL_SPHINX_HTML_THEME})

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set(
	DOXYREST_CMD_LINE
	${XML_DIR}/index.xml
	-c ${CMAKE_CURRENT_LIST_DIR}/doxyrest-config.lua
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
		NAME sample-libusb
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
		NAME sample-libusb
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ./test.sh
		)
endif()

#...............................................................................
