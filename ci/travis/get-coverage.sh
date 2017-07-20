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

# codecov seems to fail at properly combining lcov reports from multiple jobs
# so let's only collect coverage under linux gcc amd64 for now

if [ "$TRAVIS_OS_NAME" != "linux" ] || [ "$TARGET_CPU" != "amd64" ]; then
	return
fi

if [ "$TRAVIS_OS_NAME" == "linux" ] && [ "$CC" == "clang" ]; then
	return # lcov doesn't work with clang on ubuntu out-of-the-box
fi

if [ "$BUILD_CONFIGURATION" != "Debug" ]; then
	return # coverage should be collected without optimizations
fi

lcov --capture --directory . --no-external --output-file coverage.info
lcov --remove coverage.info '*/axl/*' --output-file coverage.info
lcov --list coverage.info

curl -s https://codecov.io/bash | bash
