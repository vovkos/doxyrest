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

Lua frames are used as templates for reStructuredText generation. The default set of frames is located at: ``$DOXYREST_DIR/frame``.

Frame Structure
---------------

Each frame consists of **reStructuredText** with injections of **Lua** code. Lua code should be placed inside braces: ``%{`` and ``}``. Control flow graph of the injected code can be used to *include*, *exclude* or *loop* pieces of template reStructuredText.

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

Frames may include other frames using `includeFile` or generate extra files from other frames using `generateFile`.

Standard Frames
---------------

Doxyrest provides ready-to-use frames for the following langhuages:

* C-family

	These frames reside in the ``$DOXYREST_FRAME_DIR/cfamily`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

	.. note::

		Here in Tibbo, C-family frames are used to produce documentation for **C**, **C++** and **Jancy**. Other C-family languages, while likely to work *somewhat alright* out-of-the-box, will probably require some adjustments in the frames (mostly, regarding the declaration syntax).

* Lua

	These frames reside in the ``$DOXYREST_FRAME_DIR/lua`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

* CMake

	These frames reside in the ``$DOXYREST_FRAME_DIR/cmake`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

There are many configuration parameters for tweaking the output of the frames. All the parameters which require non-default values, may be either passed via the command-line ``-D`` switch (e.g. ``-D EXCLUDE_UNDOCUMENTED_ITEMS``) or placed in the ``doxyrest-config.lua`` file and then referenced via the command-line ``-c`` switch (e.g. ``-c doxyrest-config.lua``). If ``-c`` switch is omitted, ``doxyrest-config.lua`` will be searched in the *current directory*.

For your reference, a default ``doxyrest-config.lua`` file for standard frames can be found at ``$DOXYREST_FRAME_DIR/doxyrest-config.lua``. Copy it to your project directory and then adjust all the necessary parameters.
