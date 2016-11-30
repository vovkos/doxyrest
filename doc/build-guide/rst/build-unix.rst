.. .............................................................................
..
..  This file is part of the Doxyrest toolkit.
..
..  Doxyrest is distributed under the MIT license.
..  For details see accompanying license.txt file,
..  the public copy of which is also available at:
..  http://tibbo.com/downloads/archive/doxyrest/license.txt
..
.. .............................................................................

Building on Unix
================

Run in ``./build`` folder::

	make

You might also want to add ``-j <n>`` to make use of multiple CPU cores and speed up build process, like::

	make -j 4

After Doxyrest build is complete ``doxyrest`` command line tool can be found in ``./build/doxyrest/bin/${CMAKE_BUILD_TYPE}``.

Note that with ``make``-based build changing configuration from ``Debug`` to ``Release`` should be done at CMake configuration step (Xcode and Visual Studio are multi-configuration build systems).
