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

set(DOXYREST_VERSION_MAJOR     2)
set(DOXYREST_VERSION_MINOR     1)
set(DOXYREST_VERSION_REVISION  0)
set(DOXYREST_VERSION_TAG       )

set(DOXYREST_VERSION_FULL "${DOXYREST_VERSION_MAJOR}.${DOXYREST_VERSION_MINOR}.${DOXYREST_VERSION_REVISION}")

if(NOT "${DOXYREST_VERSION_TAG}" STREQUAL "")
	set(DOXYREST_VERSION_TAG_SUFFIX  " ${DOXYREST_VERSION_TAG}")
	set(DOXYREST_VERSION_FILE_SUFFIX "${DOXYREST_VERSION_FULL}-${DOXYREST_VERSION_TAG}")
else()
	set(DOXYREST_VERSION_TAG_SUFFIX)
	set(DOXYREST_VERSION_FILE_SUFFIX "${DOXYREST_VERSION_FULL}")
endif()

string(TIMESTAMP DOXYREST_VERSION_YEAR  "%Y")
string(TIMESTAMP DOXYREST_VERSION_MONTH "%m")
string(TIMESTAMP DOXYREST_VERSION_DAY   "%d")

set(DOXYREST_VERSION_COMPANY "Tibbo Technology Inc")
set(DOXYREST_VERSION_YEARS   "2016-${DOXYREST_VERSION_YEAR}")

#...............................................................................
