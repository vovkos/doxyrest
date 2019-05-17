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

Basic Usage
===========

1 Prepare Configuration Files
-----------------------------

1.1 Preparing ``Doxyfile``
~~~~~~~~~~~~~~~~~~~~~~~~~~

When applying Doxyrest to legacy Doxygen-based projects, a few key adjustments must be applied to the Doxygen configuration file ``Doxyfile``.

First and foremost, we do need XML:

.. code-block:: bash

	GENERATE_XML = YES

You may also want to turn the generation of HTML off now that you will be using Sphinx to build the final HTML documentation.

.. code-block:: bash

	GENERATE_HTML = NO

Next, choose the location of the resulting XML database:

.. code-block:: bash

	XML_OUTPUT = xml-dir

Program listing vastly increases the size of XML so it's recommended to turn it OFF:

.. code-block:: bash

	XML_PROGRAMLISTING = NO

The next one is not obvious, but it's *essential*! Sphinx uses lowercase reference IDs, so Doxygen can't use mixed-case IDs. This must be set even for case-sensitive languages such as C.

.. note::

	No need to worry, Doxygen will produce unique IDs for all identifiers.

.. code-block:: bash

	CASE_SENSE_NAMES = NO

The next one is also not so obvious -- and quite frankly, I don't have a clear explanation for why it produces the desired effect. However, it's highly recommended to set it for C++ projects -- otherwise Doxygen generates lots of bogus links to template arguments:

.. code-block:: bash

	HIDE_UNDOC_RELATIONS = YES

The last one is not essential, but recommended if your project sets ``AUTOLINK_SUPPORT`` to ``ON`` (like most projects do). Itherwise auto-generated links may point to discarded items:

.. code-block:: bash

	EXTRACT_ALL = YES

.. _doxyrest-config:

1.2 Preparing ``doxyrest-config.lua``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``doxyrest-config.lua`` is a Lua file with settings for frames.

A default ``doxyrest-config.lua`` file for standard frames can be found at
``$DOXYREST_FRAME_DIR/doxyrest-config.lua``. Copy it to your project
directory and then adjust all the necessary parameters.

Refer to :ref:`doxid-group_frame_config` for more details.

1.3 Preparing ``conf.py``
~~~~~~~~~~~~~~~~~~~~~~~~~

While preparing a Sphinx configuration file ``conf.py``, only a few adjustments have to be made:

Most importantly, specify the path to Doxyrest extensions for Sphinx and add these extensions:

.. code-block:: python

	sys.path.insert(1, os.path.abspath('doxyrest-sphinx-dir'))
	extensions += ['doxyrest', 'cpplexer']

Another one is closely related to the `INTRO_FILE` setting.

If one of the Doxyen pages was serving as the main page for your manual (was declared using ``\mainpage`` directive) and it's now force-included into ``index.rst`` (with ``INTRO_FILE`` inside ``doxyrest-config.lua``), it's better to exclude it from the Sphinx input:

.. code-block:: python

	exclude_patterns += ['page_index.rst']

Otherwise, there will be build warnings such as:

.. code-block:: none

	path-to-rst-dir/page_main.rst: WARNING: document isn't included in any toctree

Other adjustments to ``conf.py`` are optional and are mostly related to :ref:`Styling <styling>`

2. Build
--------

After the configuration files are ready, it's time to run all three stages of the Doxyrest pipeline:

.. code-block:: bash

		$ doxygen Doxyfile
		$ doxyrest -c doxyrest-config.lua
		$ sphinx-build -b html rst-dir html-dir

The resulting files are now in ``html-dir``
