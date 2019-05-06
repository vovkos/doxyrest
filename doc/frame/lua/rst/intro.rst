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

Doxyrest Lua Frames
========================

Doxyrest Lua frames produce reStructuredText documentation for the Lua language.

Lua frames reside in the ``$DOXYREST_FRAME_DIR/lua`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

There are many configuration parameters for tweaking the output of the Lua frames. As usual, all the parameters which require non-default values, may be either passed via the command-line ``-D`` switch (e.g. ``-D PRE_PARAM_LIST_SPACE``) or placed in the ``doxyrest-config.lua`` file and then referenced via the command-line ``-c`` switch (e.g. ``-c doxyrest-config.lua``). If ``-c`` switch is omitted, ``doxyrest-config.lua`` will be searched in the *current directory*.

For your reference, a default ``doxyrest-config.lua`` file for Lua frames can be found at ``$DOXYREST_FRAME_DIR/lua/doxyrest-config.lua``. Copy it to your project directory and then adjust all the necessary parameters.
