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

Doxyrest CMake Frames
=====================

Doxyrest CMake frames produce reStructuredText documentation for the CMake language.

CMake frames reside in the ``$DOXYREST_FRAME_DIR/cmake`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

There are many configuration parameters for tweaking the output of the CMake frames. As usual, all the parameters which require non-default values, may be either passed via the command-line ``-D`` switch (e.g. ``-D PRE_PARAM_LIST_SPACE``) or placed in the ``doxyrest-config.lua`` file and then referenced via the command-line ``-c`` switch (e.g. ``-c doxyrest-config.lua``). If ``-c`` switch is omitted, ``doxyrest-config.lua`` will be searched in the *current directory*.

For your reference, a default ``doxyrest-config.lua`` file for CMake frames can be found at ``$DOXYREST_FRAME_DIR/cmake/doxyrest-config.lua``. Copy it to your project directory and then adjust all the necessary parameters.
