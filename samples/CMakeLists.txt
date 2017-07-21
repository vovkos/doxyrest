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

option (
	BUILD_DOXYREST_SAMPLES
	"Build example documentation for a bunch of opensource projects (used for testing as well)"
	ON
	)

if (BUILD_DOXYREST_SAMPLES)
	add_subdirectory (jancy-api)
	add_subdirectory (jancy-stdlib)
	add_subdirectory (libusb)
	add_subdirectory (libssh)
	add_subdirectory (alsa)
	add_subdirectory (apr)
	add_subdirectory (opencv)
	add_subdirectory (poco)
endif ()

#...............................................................................
