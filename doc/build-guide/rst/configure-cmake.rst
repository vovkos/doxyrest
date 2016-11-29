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

CMake Configuration Step
========================

Once ``paths.cmake`` is created, it's time for CMake configuration step.

Create a build folder. I usually create two-level build paths: ``./build/<build-specific-folder>``, e.g. ``./build/msvc10-amd64``, ``./build/make-x86``, ``./build/netbeans`` etc. But to keep things shorter, I will use ``./build`` in the snippets below::

	cd doxyrest_b
	mkdir build
	cd build
	cmake ..

If all the required paths have been configured properly, you should see something like this.

.. rubric:: Sample Linux output:

::

	AXL CMake:
	    Invoked from:        /home/vladimir/Projects/doxyrest_b/CMakeLists.txt
	    dependencies.cmake:  /home/vladimir/Projects/doxyrest_b/dependencies.cmake
	    settings.cmake:      /home/vladimir/Projects/settings.cmake
	    paths.cmake:         /home/vladimir/Projects/paths.cmake
	    Target CPU:          amd64
	    Build configuration: Debug
	C/C++:
	    C Compiler:          /usr/bin/cc
	    C flags (Debug):     -m64 -mcx16 -fPIC -fvisibility=hidden -Wno-multichar -g
	    C flags (Release):   -m64 -mcx16 -fPIC -fvisibility=hidden -Wno-multichar -O3 -DNDEBUG
	    C++ Compiler:        /usr/bin/c++
	    C++ flags (Debug):   -m64 -mcx16 -fPIC -fvisibility=hidden -Wno-multichar -g
	    C++ flags (Release): -m64 -mcx16 -fPIC -fvisibility=hidden -Wno-multichar -O3 -DNDEBUG
	Expat paths:
	    Includes:            /usr/include
	    Libraries:           /usr/lib64
	Lua paths:
	    Includes:            /usr/include
	    Library dir:         /usr/lib64
	    Library name:        lua
	Sphinx found at:         /usr/bin/sphinx-build
	Pdflatex found at:       /usr/bin/pdflatex
	AXL paths:
	    CMake files:         /home/vladimir/Projects/doxyrest_b/axl/cmake;/home/vladimir/Projects/doxyrest_b/build/axl/cmake
	    Includes:            /home/vladimir/Projects/doxyrest_b/axl/include
	    Libraries:           /home/vladimir/Projects/doxyrest_b/build/axl/lib/Debug
	Configuring done

.. rubric:: Sample Windows output:

::

	AXL CMake:
	    Invoked from:        C:/Projects/repos/doxyrest_b/CMakeLists.txt
	    dependencies.cmake:  C:/Projects/repos/doxyrest_b/dependencies.cmake
	    paths.cmake:         C:/Projects/repos/paths.cmake
	    Target CPU:          x86
	C/C++:
	    C Compiler:          C:/Develop/msvs/msvs2010/VC/bin/cl.exe
	    C flags (Debug):     /DWIN32 /D_WINDOWS /W3 /EHsc /GR /D_DEBUG /Ob0 /Od /RTC1 /MTd /Zi
	    C flags (Release):   /DWIN32 /D_WINDOWS /W3 /EHsc /GR /O2 /Ob2 /D NDEBUG /MT /Zi
	    C++ Compiler:        C:/Develop/msvs/msvs2010/VC/bin/cl.exe
	    C++ flags (Debug):   /DWIN32 /D_WINDOWS /W3 /EHsc /GR /D_DEBUG /Ob0 /Od /RTC1 /MTd /Zi
	    C++ flags (Release): /DWIN32 /D_WINDOWS /W3 /EHsc /GR /O2 /Ob2 /D NDEBUG /MT /Zi
	    C/C++ definitions:   UNICODE;_UNICODE
	Dependency path definitions:
	    7Z_EXE:              c:/Program Files/7-Zip/7z.exe
	    EXPAT_INC_DIR:       c:/Develop/expat/expat-2.1.0/lib
	    EXPAT_LIB_DIR:       c:/Develop/expat/expat-2.1.0/build/msvc10/$(Configuration)
	    LUA_INC_DIR:         c:/Develop/lua/lua-5.2.1/include
	    LUA_LIB_DIR:         c:/Develop/lua/lua-5.2.1/lib/x86/$(Configuration)
	    LUA_LIB_NAME:        lua52
	    RAGEL_EXE:           c:/Develop/ragel/ragel-6.7/ragel.exe
	Expat paths:
	    Includes:            c:/Develop/expat/expat-2.1.0/lib
	    Libraries:           c:/Develop/expat/expat-2.1.0/build/msvc10/$(Configuration)
	Lua paths:
	    Includes:            c:/Develop/lua/lua-5.2.1/include
	    Library dir:         c:/Develop/lua/lua-5.2.1/lib/x86/$(Configuration)
	    Library name:        lua52
	AXL paths:
	    CMake files:         C:/Projects/repos/doxyrest_b/axl/cmake;C:/Projects/repos/doxyrest_b/build/axl/cmake
	    Includes:            C:/Projects/repos/doxyrest_b/axl/include
	    Libraries:           C:/Projects/repos/doxyrest_b/build/axl/lib/$(Configuration)
	Configuring done

After that you can optionally run::

	cmake-gui .

This will launch a GUI client for CMake and allow you to **fine-tune** CMake configuration variables. For example, you may want to turn **precompiled headers** ON or OFF, change C++ **RTTI** settings, for *make*-based builds change **configuration** from ``Debug`` to ``Release`` or vice versa and so on.

You can also use ``cmake-gui`` all along for the whole CMake configuration process, without doing command line ``cmake`` step.
