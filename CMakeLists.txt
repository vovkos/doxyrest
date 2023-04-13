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

cmake_minimum_required(VERSION 3.16)

project(doxyrest)

include(cmake/axl_cmake.cmake NO_POLICY_SCOPE)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set(DOXYREST_ROOT_DIR       ${CMAKE_CURRENT_LIST_DIR})
set(DOXYREST_BIN_BASE_DIR   ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(DOXYREST_DOC_DIR        ${CMAKE_CURRENT_BINARY_DIR}/doc)
set(DOXYREST_PACKAGE_DIR    ${CMAKE_CURRENT_BINARY_DIR}/package)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DOXYREST_BIN_BASE_DIR}/${CONFIGURATION_SCG})

set(DOXYREST_INSTALL_BIN_SUBDIR bin)

if(WIN32)
	set(DOXYREST_INSTALL_CMAKE_SUBDIR   cmake)
	set(DOXYREST_INSTALL_FRAME_SUBDIR   frame)
	set(DOXYREST_INSTALL_SPHINX_SUBDIR  sphinx)
	set(DOXYREST_INSTALL_DOC_SUBDIR     doc)
	set(DOXYREST_INSTALL_LICENSE_SUBDIR license)
else()
	set(DOXYREST_INSTALL_CMAKE_SUBDIR   lib/cmake/${PROJECT_NAME})
	set(DOXYREST_INSTALL_FRAME_SUBDIR   share/${PROJECT_NAME}/frame)
	set(DOXYREST_INSTALL_SPHINX_SUBDIR  share/${PROJECT_NAME}/sphinx)
	set(DOXYREST_INSTALL_DOC_SUBDIR     share/${PROJECT_NAME}/doc)
	set(DOXYREST_INSTALL_LICENSE_SUBDIR share/${PROJECT_NAME}/license)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_subdirectory(cmake)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/doxyrest_config.cmake)

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(samples)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

install(
	DIRECTORY sphinx/
	DESTINATION ${DOXYREST_INSTALL_SPHINX_SUBDIR}
	PATTERN "*.pyc" EXCLUDE
	PATTERN "__pycache__" EXCLUDE
)

install(
	DIRECTORY frame/
	DESTINATION ${DOXYREST_INSTALL_FRAME_SUBDIR}
)

install(
	DIRECTORY license/
	DESTINATION ${DOXYREST_INSTALL_LICENSE_SUBDIR}
)

#...............................................................................

set(CPACK_PACKAGE_VERSION_MAJOR ${DOXYREST_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${DOXYREST_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${DOXYREST_VERSION_REVISION})

if(WIN32)
	set(CPACK_SOURCE_GENERATOR 7Z)
else()
	set(CPACK_SOURCE_GENERATOR TXZ)
endif()

set(CPACK_SOURCE_IGNORE_FILES "/[.];[.]user$;/build/;/ci/;/paths.cmake$")

set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-${DOXYREST_VERSION_FILE_SUFFIX}-src)

if(APPLE)
	set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${DOXYREST_VERSION_FILE_SUFFIX}-mac)
else()
	string(TOLOWER ${CMAKE_SYSTEM_NAME} OS_NAME)
	set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${DOXYREST_VERSION_FILE_SUFFIX}-${OS_NAME}-${TARGET_CPU})
endif()

include(CPack)

configure_file(
	${CMAKE_CURRENT_LIST_DIR}/ci/github-actions/print-package-file-name.cmake
	${CMAKE_CURRENT_BINARY_DIR}/print-package-file-name.cmake
	COPYONLY
)

#...............................................................................
