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

Doxyrest C-family Frames
========================

Doxyrest C-family frames produce reStructuredText documentation for languages with *C-family syntax* (e.g. C, C++, D, Java, etc).

C-family frames reside in the ``$DOXYREST_FRAME_DIR/cfamily`` directory; they also reference common frames in ``$DOXYREST_FRAME_DIR/common`` directory.

.. note::

	Here in Tibbo, C-family frames are used to produce documentation for C, C++ and Jancy. Other C-family languages, while likely to work alright out-of-the-box, may require some adjustments in the frames regarding the declaration syntax.

There are many configuration parameters for tweaking the output of the C-family frames. As usual, all the parameters which require non-default values, may be either passed via the command-line ``-D`` switch (e.g. ``-D EXCLUDE_DEFAULT_CONSTRUCTORS``) or placed in the ``doxyrest-config.lua`` file and then referenced via the command-line ``-c`` switch (e.g. ``-c doxyrest-config.lua``). If ``-c`` switch is omitted, ``doxyrest-config.lua`` will be searched in the *current directory*.

For your reference, a default ``doxyrest-config.lua`` file for C-family frames can be found at ``$DOXYREST_FRAME_DIR/cfamily/doxyrest-config.lua``. Copy it to your project directory and then adjust all the necessary parameters.
