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

string(REGEX REPLACE "[a-z]+" ".." DOXYREST_INSTALL_ROOT_SUBDIR ${DOXYREST_INSTALL_CMAKE_SUBDIR})

configure_file(
	doxyrest_config_build.cmake.in
	doxyrest_config.cmake
	@ONLY
	)

configure_file(
	doxyrest_config_install.cmake.in
	doxyrest_config_install.cmake
	@ONLY
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

install(
	FILES import_doxyrest.cmake
	DESTINATION ${DOXYREST_INSTALL_CMAKE_SUBDIR}
	)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/doxyrest_config_install.cmake
	RENAME doxyrest_config.cmake
	DESTINATION ${DOXYREST_INSTALL_CMAKE_SUBDIR}
	)

install(
	FILES ${DOXYREST_ROOT_DIR}/version.cmake
	RENAME doxyrest_version.cmake
	DESTINATION ${DOXYREST_INSTALL_CMAKE_SUBDIR}
	)

#...............................................................................
