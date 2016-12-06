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

Command Line Reference
======================

This chapter provides a reference to Doxyrest command line options.

.. rubric:: General form of command-line tool usage:

.. code-block:: bash

	doxyrest <doxygen-index.xml> <options>...

When specifying options *values*, you can use either of the following ways:

* Separate with *space*:

	.. code-block:: bash

		-o index.rst
		--output index.rst

* Separate with ``=``:

	.. code-block:: bash

		-o=index.rst
		--output=index.rst

* Concatenate (only for single-char options):

	.. code-block:: bash

		-oindex.rst

Options
-------

.. option:: -h, --help

Prints help screen.

.. option:: -v, --version

Prints version of Doxyrest.

.. option:: -o, --output

Specifies master (index) output file, for example:

.. code-block:: bash

	-o index.rst
	--output index.rst

.. option:: -f, --frame

Specifies Lua master (index) frame file, for example:

.. code-block:: bash

	-f index.rst.in
	--frame index.rst.in

.. option:: -F, --frame-dir

Adds Lua frame directory, for example:

.. code-block:: bash

	-F ./my-doxyrest-frame
	--frame-dir ./my-doxyrest-frame

It is allowed to use ``--frame-dir`` switch multiple times to add multiple directories.

.. option:: -p, --protection

Sets the filter for non-public items, for example:

.. code-block:: bash

	-p protected
	--protection protected

Available protection levels:

* ``public``
* ``protected``
* ``private``

Doxyrest will ignore items with protection level *higher* than specified. For example, if you specify ``-p=protected``, then only ``public`` and ``protected`` members will be documented; ``private`` members will be excluded from documentation.

By default, only ``public`` members are documented.

.. option:: -D, --define

Defines a Lua variable, for example:

.. code-block:: bash

	-D myvar=value
	--define myvar=value

If ``value`` if omitted, then it is assumed to be ``true``, for example.:

.. code-block:: bash

	-D myvar

is equivalent to:

.. code-block:: bash

	-D myvar=true

This option allows you to write highly customizable Lua frames which produce different output depending on controlling variables.
