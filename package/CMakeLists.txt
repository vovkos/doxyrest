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

if (WIN32)
	add_subdirectory (windows)
elseif (UNIX)
	add_subdirectory (unix)
endif ()

#...............................................................................
