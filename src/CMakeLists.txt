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

#
# app folder
#

set(
	APP_H_LIST
	CmdLine.h
	Module.h
	Generator.h
	DoxyXmlEnum.h
	DoxyXmlType.h
	DoxyXmlParser.h
	version.h.in
)

set(
	APP_CPP_LIST
	main.cpp
	CmdLine.cpp
	Module.cpp
	Generator.cpp
	DoxyXmlEnum.cpp
	DoxyXmlType.cpp
	DoxyXmlParser.cpp
)

source_group(
	app
	FILES
	${APP_H_LIST}
	${APP_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# frame folder
#

file(
	GLOB FRAME_LIST
	${DOXYREST_ROOT_DIR}/frame/*.*
)

source_group(
	frame
	FILES
	${FRAME_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# res folder
#

set(
	RES_RC_LIST
	res.rc
)

source_group(
	res
	FILES
	${RES_RC_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file(MAKE_DIRECTORY ${GEN_DIR})

axl_push_and_set(CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

configure_file(
	version.h.in
	${GEN_DIR}/version.h
)

set(
	GEN_H_LIST
	${GEN_DIR}/version.h
)

axl_pop(CMAKE_CURRENT_BINARY_DIR)

source_group(
	gen
	FILES
	${GEN_H_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

source_group(
	pch
	FILES
	${PCH_H}
)

#...............................................................................
#
# doxyrest doxygen-to-restructured-text conversion tool
#

include_directories(
	${EXPAT_INC_DIR}
	${LUA_INC_DIR}
	${AXL_INC_DIR}
	${GEN_DIR}
)

link_directories(
	${EXPAT_LIB_DIR}
	${LUA_LIB_DIR}
	${AXL_LIB_DIR}
)

add_executable(
	doxyrest
	${PCH_H}
	${APP_H_LIST}
	${APP_CPP_LIST}
	${FRAME_LIST}
	${RES_RC_LIST}
	${GEN_H_LIST}
)

target_precompile_headers(
	doxyrest
	PRIVATE
	${PCH_H}
)

target_link_libraries(
	doxyrest
	axl_st
	axl_lua
	axl_lex
	axl_xml
	axl_io
	axl_core
	expat
	${LUA_LIB_NAME}
)

if(UNIX)
	target_link_libraries(
		doxyrest
		pthread
		dl
	)

	if(NOT APPLE)
		target_link_libraries(
			doxyrest
			rt
		)
	endif()
endif()

if(WIN32)
	set(_DLL_LIST)

	if(EXPAT_DLL_DIR)
		list(APPEND _DLL_LIST ${EXPAT_DLL_DIR}/expat.dll)
	endif()

	if(LUA_DLL_DIR)
		list(APPEND _DLL_LIST ${LUA_DLL_DIR}/${LUA_LIB_NAME}.dll)
	endif()

	if(_DLL_LIST)
		add_custom_command(
			TARGET doxyrest
			POST_BUILD
			COMMAND
				echo Copying DLL files for doxyrest...
			COMMAND
				${CMAKE_COMMAND} -E copy
				${_DLL_LIST}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}
			)

		axl_replace_configuration_for_install(_INSTALL_DLL_LIST ${_DLL_LIST})

		install(
			FILES ${_INSTALL_DLL_LIST}
			DESTINATION ${DOXYREST_INSTALL_BIN_SUBDIR}
		)
	endif()
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

install(
	TARGETS doxyrest
	DESTINATION ${DOXYREST_INSTALL_BIN_SUBDIR}
)

#...............................................................................
