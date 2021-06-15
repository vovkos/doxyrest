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

These libraries are **required** for building Doxyrest.

* Lua

	Doxyrest uses Lua string templates for generating reStructured Text from Doxygen XML databases. Therefore, Lua headers and libraries (not executables!) are required for building Doxyrest.

	.. expand-macro:: lua-common-info

* Expat

	Doxyrest uses Expat library for parsing XML.

	.. expand-macro:: expat-common-info

* AXL

	Doxyrest uses AXL as a general purpose C++ support library.

	.. expand-macro:: axl-bundle-info Doxyrest doxyrest doxyrest_b
