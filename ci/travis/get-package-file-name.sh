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

if [ "$BUILD_PACKAGE" == "" ]; then
	return
fi

echo 'include (build/CPackConfig.cmake)'    >  print-package-file-name.cmake
echo 'message (${CPACK_PACKAGE_FILE_NAME})' >> print-package-file-name.cmake

CPACK_PACKAGE_FILE_NAME=`cmake -P print-package-file-name.cmake 2>&1`
export DOXYREST_PACKAGE_FILE=build/$CPACK_PACKAGE_FILE_NAME.tar.xz

rm print-package-file-name.cmake
