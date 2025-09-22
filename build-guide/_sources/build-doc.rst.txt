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

.. expand-macro:: build-doc-intro ./build/doxyrest

Build Guide
-----------

.. expand-macro:: build-doc-build-guide ./build/doxyrest

Manual
------

A manual on the Doxyrest command-line tool.

Documentation source files are located at: ``./doc/manual``

Build steps:

.. code-block:: bash

	cd ./build/doxyrest/doc/manual
	./build-html
	./build-pdf
