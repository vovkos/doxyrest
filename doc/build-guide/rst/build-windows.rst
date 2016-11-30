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

Building on Windows
===================

To build Doxyrest on Windows you can start Visual Studio by double-clicking the newly generated solution file ``doxyrest_b.sln`` and build it from the IDE.

If you prefer building from the command line, run in ``./build`` folder::

	cmake --build .

You can pass extra command-line arguments to ``msbuild.exe`` using ``--`` switch. For example, if you want to have a multi-core build of ``Release`` configuration, run::

	cmake --build . -- /maxcpucount /property:configuration=Release

Please refer to ``msbuild`` documentation for more details on command-line parameters: https://msdn.microsoft.com/en-us/library/ms164311.aspx

After Doxyrest build is complete ``doxyrest`` command line tool can be found in ``./build/doxyrest/bin/$(Configuration)``.
