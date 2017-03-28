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

project (doxyrest)

cmake_minimum_required (VERSION 3.3)

include (cmake/axl_cmake.cmake NO_POLICY_SCOPE)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set (DOXYREST_ROOT_DIR     ${CMAKE_CURRENT_LIST_DIR})
set (DOXYREST_BIN_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (DOXYREST_DOC_DIR      ${CMAKE_CURRENT_BINARY_DIR}/doc)
set (DOXYREST_PACKAGE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/package)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DOXYREST_BIN_BASE_DIR}/${CONFIGURATION_SUFFIX_0})

set (DOXYREST_INSTALL_INC_SUBDIR bin)

if (WIN32)
	set (DOXYREST_INSTALL_CMAKE_SUBDIR   cmake)
	set (DOXYREST_INSTALL_FRAME_SUBDIR   frame)
	set (DOXYREST_INSTALL_SPHINX_SUBDIR  sphinx)
	set (DOXYREST_INSTALL_DOC_SUBDIR     doc)
	set (DOXYREST_INSTALL_LICENSE_SUBDIR license)
else ()
	set (DOXYREST_INSTALL_CMAKE_SUBDIR   lib/cmake/${CMAKE_PROJECT_NAME})
	set (DOXYREST_INSTALL_FRAME_SUBDIR   share/${CMAKE_PROJECT_NAME}/frame)
	set (DOXYREST_INSTALL_SPHINX_SUBDIR  share/${CMAKE_PROJECT_NAME}/sphinx)
	set (DOXYREST_INSTALL_DOC_SUBDIR     share/${CMAKE_PROJECT_NAME}/doc)
	set (DOXYREST_INSTALL_LICENSE_SUBDIR share/${CMAKE_PROJECT_NAME}/license)
endif ()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_subdirectory (cmake)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/doxyrest_config.cmake)

add_subdirectory (src)
add_subdirectory (doc)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

install (
	DIRECTORY sphinx/
	DESTINATION ${DOXYREST_INSTALL_SPHINX_SUBDIR}
	PATTERN "*.pyc" EXCLUDE
	PATTERN "__pycache__" EXCLUDE
	)

install (
	DIRECTORY frame/
	DESTINATION ${DOXYREST_INSTALL_FRAME_SUBDIR}
	)

install (
	DIRECTORY license/
	DESTINATION ${DOXYREST_INSTALL_LICENSE_SUBDIR}
	)

#...............................................................................

set (CPACK_PACKAGE_VERSION_MAJOR ${DOXYREST_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${DOXYREST_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${DOXYREST_VERSION_REVISION})

set (CPACK_SOURCE_GENERATOR TXZ)
set (CPACK_SOURCE_IGNORE_FILES "/[.];[.]user$;/build/;/travis/;/paths.cmake$")

string (TOLOWER ${CMAKE_SYSTEM_NAME} OS_NAME)

set (CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${OS_NAME}-${DOXYREST_VERSION_FILE_SUFFIX}-${TARGET_CPU})
set (CPACK_SOURCE_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-src-${DOXYREST_VERSION_FILE_SUFFIX})

include (CPack)

#...............................................................................
