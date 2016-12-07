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

Lua API Reference
=================

.. toctree::
	:hidden:

	lua_Compound.rst
	lua_Member.rst
	lua_EnumValue.rst
	lua_Param.rst
	lua_LinkedText.rst
	lua_RefText.rst
	lua_Description.rst
	lua_DocBlock.rst

Doxyrest exports all the Doxygen XML database in form of inter-connected Lua variables. It also exports a couple of handy functions useful for ``.rst`` file emission.

Overview
~~~~~~~~

Globals
-------

.. ref-code-block:: lua
	:class: overview-code-block

	function `includeFile` (fileName)
	function `generateFile` (fileName)

	`g_globalNamespace`

Lua Table Types
---------------

.. ref-code-block:: lua
	:class: overview-code-block

	`Compound`
	`Member`
	`EnumValue`
	`Param`
	`Description`
	`DocBlock`
	`LinkedText`
	`RefText`

In addition to the globals above, all the variables passed via command line using :option:`--define` will also be added to the Lua global namespace.

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

.. _cid-includeFile:
.. code-block:: lua
	:class: title-code-block

	function includeFile (frameFileName)

Process the frame in ``frameFileName`` and includes its output at the current position of the current frame [#f1]_.

.. _cid-generateFile:
.. code-block:: lua
	:class: title-code-block

	function generateFile (targetFileName, frameFileName)

Generates a new file named ``targetFileName`` by processing the frame ``frameFileName`` and writing its output to ``targetFileName`` [#f1]_.

.. _cid-g_globalNamespace:
.. code-block:: lua
	:class: title-code-block

	g_globalNamespace

Holds a `Compound` table containing global namespace of the source tree.

.. rubric:: Footnotes:

.. [#f1] As always in Lua, you can pass extra arguments to frame processing functions in the addition to mandatory ones. This technique provides a convenient way of passing certain **context** to the frame (and is relied upon in standard ``c_*.rst.in`` files). Later you access these extra arguments from your frame like this:

	.. code-block:: lua

		local argArray = table.pack (...)
		local extraArg = argArray [1]

