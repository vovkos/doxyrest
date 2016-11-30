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

Building Documentation
======================

Doxyrest contains two documentation packages:

* Build guide
* Manual

If you have required :ref:`prerequisites <optional-tools>` for building documentation, CMake should have created all the necessary shell scripts under: ``./build/doc/<doc-package>``

``sphinx-build`` is always needed; ``pdflatex`` is also needed to build PDF.

Resulting HTML pages will be placed at: ``./build/doc/<doc-package>/html``

Resulting PDF will be placed at: ``./build/doc/<doc-package>/pdf``

Build Guide
-----------

This is what you are reading right now.

Located at: ``./doc/build-guide``

Build steps:

.. code-block:: bash

	cd ./build/doc/build-guide
	./build-html
	./build-pdf

Manual
------

A manual on Doxyrest command-line tool

Located at: ``./doc/manual`` (not yet, but soon)

Build steps:

.. code-block:: bash

	cd ./build/doc/manual
	./build-html
	./build-pdf
