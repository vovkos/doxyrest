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

.. _cid-param:

Param
=====

Table of this type describes a Doxygen *param* such as *function argument* or *template parameter*.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_declarationName <cid-param.m_declarationname>`
	:ref:`m_definitionName <cid-param.m_definitionname>`
	:ref:`m_array <cid-param.m_array>`
	:ref:`m_type <cid-param.m_type>`
	:ref:`m_defaultValue <cid-param.m_defaultvalue>`
	:ref:`m_briefDescription <cid-param.m_briefdescription>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-param.m_declarationname:
.. code-block:: lua
	:class: title-code-block

	m_declarationName

Holds a string describing a name given to the parameter during *declaration*.

.. _cid-param.m_definitionname:
.. code-block:: lua
	:class: title-code-block

	m_definitionName

Holds a string describing a name given to the parameter during *definition*.

.. _cid-param.m_array:
.. code-block:: lua
	:class: title-code-block

	m_array

Holds a string describing array dimension suffix of the parameter (if the parameter is of array type).

.. _cid-param.m_type:
.. code-block:: lua
	:class: title-code-block

	m_type

Holds a `LinkedText` table describing the type of the parameter.

.. _cid-param.m_defaultvalue:
.. code-block:: lua
	:class: title-code-block

	m_defaultValue

Holds a `LinkedText` table with the default value assigned to the parameter.

.. rubric:: Sample:

.. code-block:: cpp

	int foo (int a, int b = 100);

``m_defaultValue`` for ``a`` will be empty (`LinkedText.m_isEmpty` will be set to ``false``).

``m_defaultValue`` for ``b`` will contain ``= 100``.

.. _cid-param.m_briefdescription:
.. code-block:: lua
	:class: title-code-block

	m_briefDescription

Holds a `Description` table with the brief description of the parameter.
