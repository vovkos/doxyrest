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

axl_find_file(
	_CONFIG_CMAKE
	doxyrest_config.cmake
	${DOXYREST_CMAKE_DIR}
	)

if(_CONFIG_CMAKE)
	include(${_CONFIG_CMAKE})

	message(STATUS "Doxyrest ${DOXYREST_VERSION_FULL} paths:")
	axl_message("    CMake files:" "${DOXYREST_CMAKE_DIR}")
	axl_message("    Frames:"      "${DOXYREST_FRAME_DIR}")
	axl_message("    Sphinx exts:" "${DOXYREST_SPHINX_DIR}")
	axl_message("    Executable:"  "${DOXYREST_EXE}")

	set(DOXYREST_FOUND TRUE)
else()
	set(DOXYREST_FOUND FALSE)
endif()

#...............................................................................
