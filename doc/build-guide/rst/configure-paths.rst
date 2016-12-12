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

paths.cmake
===========

Doxyrest build system uses the ``paths.cmake`` file as the main reference when it needs to find a certain tool or library. When a path is not specified, a fallback attempt to find it using ``find_package`` will be made.

This allows for out-of-the-box *default* build and at the same time provides a *fine-grained* control over locations of dependencies. Here at Tibbo we have multiple versions of tools and libraries installed on a single build machine and at the same time we are always in full control over which tool or library is going to be used when building a particular project.

``paths.cmake`` files are **cascading**. It means, you can place one *anywhere* above the current directory and it will be found and used. From there you can chain-include the next ``paths.cmake`` and so on. This way you can specify some default locations for *all* your projects but still be able to override the paths for sub-projects.

Being machine-specific, ``paths.cmake`` files are added to ``.gitignore`` and are never tracked in Git. Therefore, creatomg  the ``paths.cmake`` file should be the very first step of configuration process. So, what should be inside?

To answer this question, you need to check the ``dependencies.cmake`` file. Inside this file, a variable called ``AXL_PATH_LIST`` contains all the paths that will be used during the build. For ``doxyrest_b`` package this list looks like this:

.. code-block:: bash

	LUA_INC_DIR         # path to Lua C include directory
	LUA_LIB_DIR         # path to Lua library directory
	LUA_LIB_NAME        # name of Lua library (lua/lua51/lua52/lua53)
	EXPAT_INC_DIR       # path to Expat include directory
	EXPAT_LIB_DIR       # path to Expat library directory
	SPHINX_BUILD_EXE    # (optional) path to Sphinx compiler executable sphinx-build
	PDFLATEX_EXE        # (optional) path to Latex-to-PDF compiler

Note that you don't necessarily have to specify each and every variable above.

First of all, it's OK to completely omit *optional* dependencies -- if you don't need those.

Secondly, required dependencies may be auto-detected -- on Unix systems installed libraries and tools will likely be found automatically. On Windows Doxyrest build system will automatically find executables if they are added to ``PATH`` (via ``where`` command).

On Windows, however, you will need to specify paths to Lua and Expat libraries -- they are unlikely to be found automatically.

And of course, you can always use ``paths.cmake`` to *fine-tune* the location of a specific tool/library.

I personally prefer to always specify all the paths explicitly.

.. rubric:: Sample paths.cmake on Linux:

.. code-block:: cmake

	set (LUA_VERSION   5.2.1)
	set (EXPAT_VERSION 2.2.0)

	set (LUA_INC_DIR   /home/vladimir/Develop/lua/lua-${LUA_VERSION}/include)
	set (EXPAT_INC_DIR /home/vladimir/Develop/expat/expat-${EXPAT_VERSION}/lib)

	if ("${AXL_CPU}" STREQUAL "amd64")
		set (LUA_LIB_DIR   /home/vladimir/Develop/lua/lua-${LUA_VERSION}/lib-amd64)
		set (EXPAT_LIB_DIR /home/vladimir/Develop/expat/expat-${EXPAT_VERSION}/build/make-amd64)
	else ()
		set (LUA_LIB_DIR   /home/vladimir/Develop/lua/lua-${LUA_VERSION}/lib-x86)
		set (EXPAT_LIB_DIR /home/vladimir/Develop/expat/expat-${EXPAT_VERSION}/build/make-x86)
	endif ()

.. rubric:: Sample paths.cmake on Windows:

.. code-block:: cmake

	set (LUA_VERSION   5.2.1)
	set (EXPAT_VERSION 2.1.0)

	set (LUA_INC_DIR   c:/Develop/lua/lua-${LUA_VERSION}/include)
	set (EXPAT_INC_DIR c:/Develop/expat/expat-${EXPAT_VERSION}/lib)

	if ("${AXL_CPU}" STREQUAL "amd64")
		set (LUA_LIB_DIR   c:/Develop/lua/lua-${LUA_VERSION}/lib/amd64/${CONFIGURATION_SUFFIX})
		set (EXPAT_LIB_DIR c:/Develop/expat/expat-${EXPAT_VERSION}/build/msvc10-amd64/${CONFIGURATION_SUFFIX})
	else ()
		set (LUA_LIB_DIR   c:/Develop/lua/lua-${LUA_VERSION}/lib/x86/${CONFIGURATION_SUFFIX})
		set (EXPAT_LIB_DIR c:/Develop/expat/expat-${EXPAT_VERSION}/build/msvc10/${CONFIGURATION_SUFFIX})
	endif()

	set (SPHINX_BUILD_EXE c:/Develop/ActivePython/Scripts/sphinx-build.exe)
	set (PDFLATEX_EXE     "c:/Program Files (x86)/MiKTeX 2.9/miktex/bin/pdflatex.exe")

#..............................................................................
