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

.. _cid-enumvalue:

EnumValue
=========

Table of this type describes Doxygen enum value.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_parent <cid-enumvalue.m_parent>`
	:ref:`m_protectionKind <cid-enumvalue.m_protectionkind>`
	:ref:`m_id <cid-enumvalue.m_id>`
	:ref:`m_name <cid-enumvalue.m_name>`
	:ref:`m_initializer <cid-enumvalue.m_initializer>`
	:ref:`m_briefDescription <cid-enumvalue.m_briefdescription>`
	:ref:`m_detailedDescription <cid-enumvalue.m_detaileddescription>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-enumvalue.m_parent:
.. code-block:: lua
	:class: title-code-block

	m_parent

Holds a back-pointer to parent enum `Member`. `Member.m_memberKind` should be ``enum``.

.. _cid-enumvalue.m_protectionkind:
.. code-block:: lua
	:class: title-code-block

	m_protectionKind

Holds a string describing protection of the enum value. Must be one of:

	| ``<undefined>``
	| ``public``
	| ``protected``
	| ``private``
	| ``package``

Usually it's just ``public``.

.. _cid-enumvalue.m_id:
.. code-block:: lua
	:class: title-code-block

	m_id

Holds Doxygen identifier string, e.g. ``group__operators_1ggae611bfb523eb332069f2b7030accb66``.

This string can be used as a unique identifier for creating labels and reference links.

.. _cid-enumvalue.m_name:
.. code-block:: lua
	:class: title-code-block

	m_name

Holds a string with the name of the enum value.

.. _cid-enumvalue.m_initializer:
.. code-block:: lua
	:class: title-code-block

	m_initializer

Holds a `LinkedText` table with the manually defined value.

.. rubric:: Sample:

.. code-block:: cpp

	enum MyEnum
	{
		MyEnum_Value1,
		MyEnum_Value2 = 100,
	}


``m_initializer`` for ``MyEnum_Value1`` will be empty (`LinkedText.m_isEmpty` will be set to ``false``).

``m_initializer`` for ``MyEnum_Value2`` will contain ``= 100``.

.. _cid-enumvalue.m_briefdescription:
.. code-block:: lua
	:class: title-code-block

	m_briefDescription

Holds a `Description` table with the brief description of the enum value.

.. _cid-enumvalue.m_detaileddescription:
.. code-block:: lua
	:class: title-code-block

	m_detailedDescription

Holds a `Description` table with the detailed description of the enum value.
