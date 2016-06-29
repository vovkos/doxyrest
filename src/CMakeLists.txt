# This file is part of DoxyReST (R) Project
# Tibbo Technology Inc (C) 2016-2016. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# app folder
#

set (
	APP_H_LIST
	CmdLine.h
	Module.h
	Generator.h
	DoxyXmlEnum.h
	DoxyXmlType.h
	DoxyXmlParser.h
	version.h.in
	)

set (
	APP_CPP_LIST
	main.cpp
	CmdLine.cpp
	Module.cpp
	Generator.cpp
	DoxyXmlEnum.cpp
	DoxyXmlType.cpp
	DoxyXmlParser.cpp
	)

source_group (
	app
	FILES
	${APP_H_LIST}
	${APP_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# frame folder
#

file (	
	GLOB FRAME_LIST 
	${DOXYREST_ROOT_DIR}/frame/*.*
	)

message ("FRAME_LIST = ${FRAME_LIST}")

source_group (
	frame
	FILES
	${FRAME_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# res folder
#

set (
	RES_RC_LIST
	res.rc
	)

source_group (
	res
	FILES
	${RES_RC_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set (GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file (MAKE_DIRECTORY ${GEN_DIR})

axl_push_and_set (CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

configure_file (
	version.h.in
	${GEN_DIR}/version.h
	)

set (
	GEN_H_LIST
	${GEN_DIR}/version.h
	)

axl_pop (CMAKE_CURRENT_BINARY_DIR)

source_group (
	gen
	FILES
	${GEN_H_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_group (
	pch
	FILES
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# doxyrest doxygen-to-restructured-text conversion tool
#

include_directories (
	${EXPAT_INC_DIR}
	${LUA_INC_DIR}
	${AXL_INC_DIR}
	${GEN_DIR}
	)

link_directories (
	${EXPAT_LIB_DIR}
	${LUA_LIB_DIR}
	${AXL_LIB_DIR}
	)

add_executable (
	doxyrest
	${PCH_H}
	${PCH_CPP}
	${APP_H_LIST}
	${APP_CPP_LIST}
	${FRAME_LIST}
	${RES_RC_LIST}
	${GEN_H_LIST}
	)

axl_set_pch (
	doxyrest
	${PCH_H}
	${PCH_CPP}
	)

target_link_libraries (
	doxyrest
	axl_st
	axl_lua
	axl_lex
	axl_xml
	axl_io
	axl_core
	expat
	lua
	)

if (UNIX)
	target_link_libraries (
		doxyrest
		pthread
		dl
		)

	if (NOT APPLE)
		target_link_libraries (
			doxyrest
			rt
			)
	endif ()
endif ()

#..............................................................................
