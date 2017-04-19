#...............................................................................
#
#  This file is part of the Jancy toolkit.
#
#  Jancy is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/jancy/license.txt
#
#...............................................................................

set (JANCY_VERSION_MAJOR     1)
set (JANCY_VERSION_MINOR     8)
set (JANCY_VERSION_REVISION  0)
set (JANCY_VERSION_TAG)

set (JANCY_VERSION_FULL "${JANCY_VERSION_MAJOR}.${JANCY_VERSION_MINOR}.${JANCY_VERSION_REVISION}")

if (NOT "${JANCY_VERSION_TAG}" STREQUAL "")
	set (JANCY_VERSION_TAG_SUFFIX  " ${JANCY_VERSION_TAG}")
	set (JANCY_VERSION_FILE_SUFFIX "${JANCY_VERSION_FULL}-${JANCY_VERSION_TAG}")
else ()
	set (JANCY_VERSION_TAG_SUFFIX)
	set (JANCY_VERSION_FILE_SUFFIX "${JANCY_VERSION_FULL}")
endif ()

string (TIMESTAMP JANCY_VERSION_YEAR  "%Y")
string (TIMESTAMP JANCY_VERSION_MONTH "%m")
string (TIMESTAMP JANCY_VERSION_DAY   "%d")

set (JANCY_VERSION_COMPANY   "Tibbo Technology Inc")
set (JANCY_VERSION_YEARS     "2012-${JANCY_VERSION_YEAR}")

#...............................................................................
