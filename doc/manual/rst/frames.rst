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

Doxyrest C-family Frames
------------------------

Doxyrest C-family frames produce reStructuredText documentation for languages with *C-family syntax* (e.g. C, C++, D, Java, etc).

C-family frames reside in the ``$DOXYREST_FRAME_DIR/cfamily`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

.. note::

	Here in Tibbo, C-family frames are used to produce documentation for C, C++ and Jancy. Other C-family languages, while likely to work alright out-of-the-box, may require some adjustments in the frames regarding the declaration syntax.

There are many configuration parameters for tweaking the output of the C-family frames. As usual, all the parameters which require non-default values, may be either passed via the command-line ``-D`` switch (e.g. ``-D EXCLUDE_DEFAULT_CONSTRUCTORS``) or placed in the ``doxyrest-config.lua`` file and then referenced via the command-line ``-c`` switch (e.g. ``-c doxyrest-config.lua``). If ``-c`` switch is omitted, ``doxyrest-config.lua`` will be searched in the *current directory*.

For your reference, a default ``doxyrest-config.lua`` file for C-family frames can be found at ``$DOXYREST_FRAME_DIR/cfamily/doxyrest-config.lua``. Copy it to your project directory and then adjust all the necessary parameters.

Doxyrest Lua Frames
-------------------

Doxyrest Lua frames produce reStructuredText documentation for the Lua language.

Lua frames reside in the ``$DOXYREST_FRAME_DIR/lua`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

There are many configuration parameters for tweaking the output of the Lua frames. As usual, all the parameters which require non-default values, may be either passed via the command-line ``-D`` switch (e.g. ``-D PRE_PARAM_LIST_SPACE``) or placed in the ``doxyrest-config.lua`` file and then referenced via the command-line ``-c`` switch (e.g. ``-c doxyrest-config.lua``). If ``-c`` switch is omitted, ``doxyrest-config.lua`` will be searched in the *current directory*.

For your reference, a default ``doxyrest-config.lua`` file for Lua frames can be found at ``$DOXYREST_FRAME_DIR/lua/doxyrest-config.lua``. Copy it to your project directory and then adjust all the necessary parameters.

Doxyrest CMake Frames
---------------------

Doxyrest CMake frames produce reStructuredText documentation for the CMake language.

CMake frames reside in the ``$DOXYREST_FRAME_DIR/cmake`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

There are many configuration parameters for tweaking the output of the CMake frames. As usual, all the parameters which require non-default values, may be either passed via the command-line ``-D`` switch (e.g. ``-D PRE_PARAM_LIST_SPACE``) or placed in the ``doxyrest-config.lua`` file and then referenced via the command-line ``-c`` switch (e.g. ``-c doxyrest-config.lua``). If ``-c`` switch is omitted, ``doxyrest-config.lua`` will be searched in the *current directory*.

For your reference, a default ``doxyrest-config.lua`` file for CMake frames can be found at ``$DOXYREST_FRAME_DIR/cmake/doxyrest-config.lua``. Copy it to your project directory and then adjust all the necessary parameters.

Further Reading
---------------

.. toctree::
	:titlesonly:

	group_api.rst
	group_frame-config.rst
	global.rst
