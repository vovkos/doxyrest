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

Samples
=======

A picture is worth a thousand words. Take a look at the results of Doxyrest usage and see for yourself.

C libraries
~~~~~~~~~~~

.. list-table::

	*	- LibUSB
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest/samples/libusb>`__
		- `sphinxdoc <https://vovkos.github.io/doxyrest/samples/libusb-sphinxdoc>`__
		- vs
		- `original <http://libusb.sourceforge.net/api-1.0>`__

	*	- LibSSH
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest/samples/libssh>`__
		- `sphinxdoc <https://vovkos.github.io/doxyrest/samples/libssh-sphinxdoc>`__
		- vs
		- `original <http://api.libssh.org/stable>`__

	*	- ALSA Library
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest/samples/alsa>`__
		- `sphinxdoc <https://vovkos.github.io/doxyrest/samples/alsa-sphinxdoc>`__
		- vs
		- `original <http://www.alsa-project.org/alsa-doc/alsa-lib>`__

	*	- Apache Portable Runtime
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest/samples/apr>`__
		- `sphinxdoc <https://vovkos.github.io/doxyrest/samples/apr-sphinxdoc>`__
		- vs
		- `original <https://apr.apache.org/docs/apr/1.5>`_

C++ libraries
~~~~~~~~~~~~~

.. list-table::

	*	- OpenCV
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest-showcase/opencv/sphinx_rtd_theme>`__
		- `sphinxdoc <https://vovkos.github.io/doxyrest-showcase/opencv/sphinxdoc>`__
		- vs
		- `original <http://docs.opencv.org/trunk>`__

	*	- POCO Libraries
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest-showcase/poco/sphinx_rtd_theme>`__
		- `sphinxdoc <https://vovkos.github.io/doxyrest-showcase/poco/sphinxdoc>`__
		- vs
		- `original <https://pocoproject.org/docs>`__

Doxyrest generates a decent overview even if a project has no Doxygen documentation comments at all:

.. list-table::

	* 	- AXL
		- `sphinx_rtd_theme <https://vovkos.github.io/axl/manual/global.html>`__

Replace Doxygen with your own generator of Doxygen-compatible XML database and apply the same pipeline for documenting APIs in other languages:

Lua libraries
~~~~~~~~~~~~~

.. list-table::

	*	- Doxyrest Lua API
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest/manual/group_api.html>`__

	*	- Doxyrest Frame Settings
		- `sphinx_rtd_theme <https://vovkos.github.io/doxyrest/frame/group_frame-config.html>`__

The above manuals were generated from Doxy-comments inside ``.lua`` files by using `LuaDoxyXML <https://github.com/vovkos/luadoxyxml>`__ instead of Doxygen at the first stage of the pipeline.

Jancy libraries
~~~~~~~~~~~~~~~

.. list-table::

	*	- Jancy Standard Library
		- `sphinx_rtd_theme <https://vovkos.github.io/jancy/stdlib>`__

	*	- IO Ninja Jancy API
		- `sphinx_rtd_theme <http://ioninja.com/doc/api>`__

The above manuals were generated from Doxy-comments inside ``.jnc`` files by using the official Jancy compiler instead of Doxygen at the first stage of the pipeline.
