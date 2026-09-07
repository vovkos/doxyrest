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

.. _styling:

Styling
=======

With Sphinx, you can easily change the look-and-feel of your documentation by switching between *themes* and by tweaking CSS stylesheets.

You do all that by editing your ``conf.py``.

Choose a Sphinx theme:

.. code-block:: python

	html_theme = 'sphinx_rtd_theme'


Prepare a folder ./static/ with all the .css files you want to replace, e.g.

.. code-block:: none

    ./static/pygments.css
    ./static/css/theme.css
    ...

Then ask Sphinx to write it over the standard '_static' folder:

.. code-block:: python

	html_static_path = ['static/']

If you use a theme other than ``sphinxdoc`` or ``sphinx_rtd_theme`` (natively supported by Doxyrest), make sure your stylesheets properly define the following Doxyrest-specific ``.css`` classes:

.. code-block:: css

	pre.doxyrest-overview-code-block {
		/* ... */
	}

	pre.doxyrest-overview-inherited-code-block {
		/* ... */
	}

	pre.doxyrest-title-code-block {
		/* ... */
	}

	.doxyrest-target-highlight {
		/* ... */
	}

Use ``doxyrest-sphinx-dir/css/doxyrest-sphinxdoc.css`` and ``doxyrest-sphinx-dir/css/doxyrest-sphinx_rtd_theme.css`` as examples for how to do that.

Doxyrest extensions for Sphinx also allow you setting the tab-width to your liking with:

.. code-block:: python

	doxyrest_tab_width = 2

.. note::

	The tab-width being hardcoded to ``8`` was a longtime issue with Docutils and Sphinx. There is a setting called ``tab_width`` which should be placed into ``docutils.conf``, but it's not respected by Sphinx.

