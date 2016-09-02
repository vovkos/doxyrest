#..............................................................................

axl_find_file (
	_CONFIG_CMAKE
	doxyrest_config.cmake
	${DOXYREST_CMAKE_DIR}
	)

if (_CONFIG_CMAKE)
	include (${_CONFIG_CMAKE})

	message (STATUS "Doxyrest paths:")
	message (STATUS "    Doxyrest cmake files: ${DOXYREST_CMAKE_DIR}")
	message (STATUS "    Doxyrest frames:      ${DOXYREST_FRAME_DIR}")
	message (STATUS "    Doxyrest Sphinx exts: ${DOXYREST_SPHINX_DIR}")	
	message (STATUS "    Doxyrest executable:  ${DOXYREST_EXE}")

	set (DOXYREST_FOUND TRUE)
else ()
	set (DOXYREST_FOUND FALSE)
endif ()

#..............................................................................
