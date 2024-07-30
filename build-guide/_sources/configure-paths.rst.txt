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

.. expand-macro:: paths-cmake Doxyrest

.. code-block:: bash

	LUA_INC_DIR       # path to Lua C include directory
	LUA_LIB_DIR       # path to Lua library directory
	LUA_LIB_NAME      # (optional) name of Lua library (lua/lua51/lua52/lua53)
	EXPAT_INC_DIR     # path to Expat include directory
	EXPAT_LIB_DIR     # path to Expat library directory
	SPHINX_BUILD_EXE  # (optional) path to Sphinx compiler executable sphinx-build
	PDFLATEX_EXE      # (optional) path to Latex-to-PDF compiler

.. expand-macro:: dependencies-cmake Doxyrest

On Windows you will have to specify paths to the required Lua and Expat libraries -- they are unlikely to be found automatically.

.. rubric:: Sample paths.cmake on Linux:

.. code-block:: cmake

	set (LUA_VERSION   5.2.1)
	set (EXPAT_VERSION 2.2.0)

	set (LUA_INC_DIR   /home/vladimir/Develop/lua/lua-${LUA_VERSION}/include)
	set (EXPAT_INC_DIR /home/vladimir/Develop/expat/expat-${EXPAT_VERSION}/lib)

	if ("${TARGET_CPU}" STREQUAL "amd64")
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

	if ("${TARGET_CPU}" STREQUAL "amd64")
		set (LUA_LIB_DIR   c:/Develop/lua/lua-${LUA_VERSION}/lib/amd64/${CONFIGURATION})
		set (EXPAT_LIB_DIR c:/Develop/expat/expat-${EXPAT_VERSION}/build/msvc10-amd64/${CONFIGURATION})
	else ()
		set (LUA_LIB_DIR   c:/Develop/lua/lua-${LUA_VERSION}/lib/x86/${CONFIGURATION})
		set (EXPAT_LIB_DIR c:/Develop/expat/expat-${EXPAT_VERSION}/build/msvc10/${CONFIGURATION})
	endif()

	set (SPHINX_BUILD_EXE c:/Develop/ActivePython/Scripts/sphinx-build.exe)
	set (PDFLATEX_EXE     "c:/Program Files (x86)/MiKTeX 2.9/miktex/bin/pdflatex.exe")
