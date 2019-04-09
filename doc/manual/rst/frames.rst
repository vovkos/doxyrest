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

Lua Frames
==========

Lua frames are used as templates for reStructuredText generation.

Default set of frames is located at: ``$DOXYREST_DIR/frame``. Currently this folder only contains frames for C-family languages (``c_*.rst.in`` and ``c_utils.lua``). These frames have been tested with C, C++ and Jancy. Minor tweaks may be necessary for other C-family languages like Java or D.

Frame Structure
---------------

Each frame consists of **reStructuredText** with **injections of Lua** code. Lua code should be placed inside braces: ``%{`` and ``}``. Control flow graph of the injected code can be used to **include**, **exclude** or **loop** pieces of template reStructuredText.

.. rubric:: Example:

Frame:

.. code-block:: none

	Header
	======

	%{
	if false then
	}
	This text will be excluded.
	%{
	end -- if

	for i = 1, 3 do
	}
	* List item
	%{
	end -- for
	}

Output:

.. code-block:: none

	Header
	======

	* List item
	* List item
	* List item

Variables or expressions may be injected directly into the template reStructuredText using ``$variable`` or ``$(expression)`` operators.

.. rubric:: Example:

Frame:

.. code-block:: none

	%{
	a = 10
	b = 20
	}
	$a + $b = $(a + b)

Output:

.. code-block:: none

	10 + 20 = 30

