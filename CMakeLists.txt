# This file is part of Doxyrest project
# Tibbo Technology Inc (C) 2004-2016. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (doxyrest)

cmake_minimum_required (VERSION 2.8)

include (cmake/axl_cmake.cmake NO_POLICY_SCOPE)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set (DOXYREST_ROOT_DIR     ${CMAKE_CURRENT_LIST_DIR})
set (DOXYREST_BIN_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (DOXYREST_DOC_DIR      ${CMAKE_CURRENT_BINARY_DIR}/doc)
set (DOXYREST_PACKAGE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/package)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_subdirectory (cmake)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/doxyrest_config.cmake)

add_subdirectory (src)

#..............................................................................
