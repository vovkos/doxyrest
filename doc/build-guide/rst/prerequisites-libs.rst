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

Libraries
=========

These libraries are **required** for building the Doxyrest.

* Lua

	Doxyrest uses Lua string templates for generating Re-Structured Text from Doxygen XML database. Therefore, Lua headers and libraries (not executables!) are required for building Doxyrest.

	Lua executables, header files and both static and dynamic libraries are available in official repositories of most Linux distributions.

	Lua sources as well as the links to pre-compiled binaries (of both executables and libraries) could be found at the official Lua website: http://www.lua.org

	Both 5.2.x and 5.3.x branches should be fine.

* Expat

	Doxyrest uses Expat library for parsing XML.

	Expat header files and libraries are available in official repositories of most Linux distributions.

	On Windows and Linux you would need to build Expat from sources which could be downloaded from the official Expat website: http://expat.sourceforge.net
