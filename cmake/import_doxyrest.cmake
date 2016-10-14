#..............................................................................

axl_find_file (
	_CONFIG_CMAKE
	doxyrest_config.cmake
	${DOXYREST_CMAKE_DIR}
	)

if (_CONFIG_CMAKE)
	include (${_CONFIG_CMAKE})

	axl_message ("Doxyrest cmake files: ${DOXYREST_CMAKE_DIR}")
	axl_message ("Doxyrest frames:      ${DOXYREST_FRAME_DIR}")
	axl_message ("Doxyrest Sphinx exts: ${DOXYREST_SPHINX_DIR}")
	axl_message ("Doxyrest executable:  ${DOXYREST_EXE}")

	set (DOXYREST_FOUND TRUE)
else ()
	set (DOXYREST_FOUND FALSE)
endif ()

#..............................................................................
