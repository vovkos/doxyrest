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

Tools
=====

Doxyrest build system relies on its own path configuration so you do not actually need to add any of these tools to ``PATH``. However, it is recommended to make CMake easily available (e.g. via ``PATH`` or a shortcut to ``cmake-gui``) -- because it is used to initiate the whole build process.

Required Tools
--------------

These tools are **required** for building the Doxyrest:

* CMake 3.3 or above

	Doxyrest uses CMake-based build system. CMake is a cross-platform C/C++-centric build system which uses build scripts to generate platform-specific makefiles. CMake is quickly becoming a de-facto standard when it comes to building cross-platform C/C++ applications.

	CMake is available in official repositories of most Linux distributions. Source and pre-compiled binaries can also be directly downloaded from the official website: http://cmake.org

	Any version of CMake above 3.3 should work. Versions prior to CMake 3.3 improperly expand ``CMAKE_PARENT_LIST_FILE`` variable which Doxyrest build system relies on.

.. _optional-tools:

Optional Tools
--------------

These tools are **optional** and only needed if you plan to build Doxyrest documentation:

* Python

	Doxyrest documentation uses Sphinx as a back-end, therefore, Python is also needed.

	Python is available out-of-the-box in most Linux distributions and Mac OS X. On Window you can install the official binaries available at: https://www.python.org
	or use ActivePython: http://www.activestate.com/activepython

	Both 3.5.x and 2.7.x branches should be fine (Sphinx documentation states that the required version is at least 2.6).

* Sphinx

	Doxyrest documentation uses Sphinx for generating HTML or PDF output.

	Sphinx is available via ``pip`` Python package management infrastructure. It should also be available in many Linux distributions as a stand-alone binary package.

	The official website of Sphinx: http://www.sphinx-doc.org
