#...............................................................................
#
#  This file is part of the AXL library.
#
#  AXL is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/axl/license.txt
#
#...............................................................................

# To make use of the AXL cmake build infrastructure, copy this dropper file into
# your project directory and include it in master CMakeLists.txt like this:

# include (axl_cmake.cmake NO_POLICY_SCOPE)

# The purpose of this dropper is to remember the location from where it was
# included and then find and include axl_init.cmake which will do the rest.

#...............................................................................

# subtree include guard (still possible to have axl-cmake siblings)

if (AXL_CMAKE_ONCE)
	return ()
endif ()

set (AXL_CMAKE_ONCE TRUE)

#...............................................................................

# necessary excerpt from axl_utils.cmake

macro (
axl_find_file
	_RESULT
	_FILE_NAME
	# ...
	)

	set (_DIR_LIST ${ARGN})

	set (_FILE_PATH ${_FILE_NAME}-NOTFOUND)

	foreach (_DIR ${_DIR_LIST})
		if (EXISTS ${_DIR}/${_FILE_NAME})
			set (_FILE_PATH ${_DIR}/${_FILE_NAME})
			break ()
		endif ()
	endforeach ()

	set (${_RESULT} ${_FILE_PATH})
endmacro ()

macro (
axl_find_file_recurse_parent_dirs
	_RESULT
	_FILE_NAME
	_START_DIR
	)

	set (_DIR ${_START_DIR})

	while (TRUE)
		if (EXISTS ${_DIR}/${_FILE_NAME})
			set (_FILE_PATH ${_DIR}/${_FILE_NAME})
			break ()
		endif ()

		get_filename_component (_PARENT_DIR "${_DIR}/.." ABSOLUTE)

		if (${_DIR} STREQUAL ${_PARENT_DIR})
			set (_FILE_PATH ${_FILE_NAME}-NOTFOUND)
			break ()
		endif ()

		set (_DIR ${_PARENT_DIR})
	endwhile ()

	set (${_RESULT} ${_FILE_PATH})
endmacro ()

#...............................................................................

# remember the location from where this file was included (it's not necessarily
# the same as CMAKE_SOURCE_DIR, which indicates the root of CMake source tree)

get_filename_component (AXL_CMAKE_ORIGIN_DIR ${CMAKE_PARENT_LIST_FILE} DIRECTORY)
set (AXL_CMAKE_ORIGIN_FILE ${CMAKE_PARENT_LIST_FILE})

# make sure dependencies.cmake is where it's supposed to be

if (NOT EXISTS ${AXL_CMAKE_ORIGIN_DIR}/dependencies.cmake)
	message (FATAL_ERROR "dependencies.cmake not found at: ${AXL_CMAKE_ORIGIN_DIR}")
endif ()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/axl_init.cmake")
	# we are building AXL -- axl_init.cmake is right next to the dropper

	set (_AXL_INIT_CMAKE "${CMAKE_CURRENT_LIST_DIR}/axl_init.cmake")
else ()
	if (NOT AXL_CMAKE_DIR)
		# AXL is not anywhere in source tree, try to find it via paths.cmake

		axl_find_file_recurse_parent_dirs (_PATHS_CMAKE paths.cmake ${AXL_CMAKE_ORIGIN_DIR})

		if (NOT _PATHS_CMAKE)
			include (${AXL_CMAKE_ORIGIN_DIR}/dependencies.cmake)
			string (REPLACE ";" " " _PATH_LIST "${AXL_PATH_LIST}")
			message (FATAL_ERROR "AXL_CMAKE_DIR not found, create paths.cmake and fill it with definitions for: ${_PATH_LIST}")
		endif ()

		include (${_PATHS_CMAKE})

		if (NOT AXL_CMAKE_DIR)
			message (FATAL_ERROR "AXL_CMAKE_DIR not found, check your path definitions at: ${_PATHS_CMAKE}")
		endif ()
	endif ()

	# make sure axl_init.cmake is where it's supposed to be

	axl_find_file (_AXL_INIT_CMAKE axl_init.cmake ${AXL_CMAKE_DIR})

	if (NOT _AXL_INIT_CMAKE)
		message (FATAL_ERROR "axl_init.cmake not found at AXL_CMAKE_DIR: ${AXL_CMAKE_DIR}")
	endif ()
endif ()

# include axl_init.cmake -- it will do the rest

include (${_AXL_INIT_CMAKE} NO_POLICY_SCOPE)

#...............................................................................
