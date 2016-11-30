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

Configuring
===========

Alright, all the necessary tools and libraries are available on your machine. What's next? As with every CMake-based project, we now need to *configure* our build and create appropriate makefiles.

However, there is an important difference with most CMake-based projects. Doxyrest build system does not rely on CMake ``find_package`` facility to find dependencies. Why not?

First of all - ``find_package`` simply does not work reliably on Windows. Here you would do need to set up an extra layer of path configuration variables and then provide these paths as *hints* to ``find_package``. This effectively **voids** the whole idea behind ``find_package`` -- it's you, not ``find_package``, who is doing the actual "finding"!

Another important reason -- ``find_package`` is not convenient to use in situations when you need to **switch** between different **versions** of the same library, between **Debug/Release** builds, between **static/dynamic** libraries, between different **processor architectures** during cross-compilation and so on.

In other words, ``find_package`` is really convenient on **Unix** systems when all you do is a **native** build with the **default** versions of installed tools and libraries. Need more flexibility? Then ``find_package`` alone is not the answer.

.. toctree::
	:titlesonly:

	configure-paths.rst
	configure-cmake.rst
