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

.. _cid-member:

Member
======

Table of this type describes a Doxygen *member* such as *variable*, *function*, *typedef* etc.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_parent <cid-member.m_parent>`
	:ref:`m_memberKind <cid-member.m_memberkind>`
	:ref:`m_protectionKind <cid-member.m_protectionkind>`
	:ref:`m_virtualKind <cid-member.m_virtualkind>`
	:ref:`m_flags <cid-member.m_flags>`
	:ref:`m_id <cid-member.m_id>`
	:ref:`m_name <cid-member.m_name>`
	:ref:`m_path <cid-member.m_path>`
	:ref:`m_modifiers <cid-member.m_modifiers>`
	:ref:`m_type <cid-member.m_type>`
	:ref:`m_returnType <cid-member.m_returntype>`
	:ref:`m_argString <cid-member.m_argstring>`
	:ref:`m_paramArray <cid-member.m_paramarray>`
	:ref:`m_templateParamArray <cid-member.m_templateparamarray>`
	:ref:`m_templateSpecParamArray <cid-member.m_templatespecparamarray>`
	:ref:`m_exceptions <cid-member.m_exceptions>`
	:ref:`m_enumValueArray <cid-member.m_enumvaluearray>`
	:ref:`m_bitField <cid-member.m_bitfield>`
	:ref:`m_initializer <cid-member.m_initializer>`
	:ref:`m_briefDescription <cid-member.m_briefdescription>`
	:ref:`m_detailedDescription <cid-member.m_detaileddescription>`
	:ref:`m_inBodyDescription <cid-member.m_inbodydescription>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-member.m_parent:
.. code-block:: lua
	:class: title-code-block

	m_parent

Holds a back-pointer to parent ``Compound`` table.

.. _cid-member.m_memberkind:
.. code-block:: lua
	:class: title-code-block

	m_memberKind

Holds a string describing member type. Must be one of:

	| ``<undefined>``
	| ``define``
	| ``property``
	| ``event``
	| ``variable``
	| ``typedef``
	| ``enum``
	| ``enumvalue``
	| ``function``
	| ``signal``
	| ``prototype``
	| ``friend``
	| ``dcop``
	| ``slot``
	| ``interface``
	| ``service``
	| ``alias``
	| ``footnote``

.. _cid-member.m_protectionkind:
.. code-block:: lua
	:class: title-code-block

	m_protectionkind

Holds a string describing protection of the member. Must be one of:

	| ``<undefined>``
	| ``public``
	| ``protected``
	| ``private``
	| ``package``

.. _cid-member.m_virtualkind:
.. code-block:: lua
	:class: title-code-block

	m_virtualKind

Holds a string describing whether this member is virtual or not. Must be one of:

	| ``<undefined>``
	| ``non-virtual``
	| ``virtual``
	| ``pure-virtual``

.. _cid-member.m_flags:
.. code-block:: lua
	:class: title-code-block

	m_flags

An empty string or a space-delimited combination of:

	| ``static``
	| ``const``
	| ``explicit``
	| ``inline``
	| ``volatile``
	| ``mutable``
	| ``readable``
	| ``writable``
	| ``initonly``
	| ``settable``
	| ``gettable``
	| ``final``
	| ``sealed``
	| ``new``
	| ``add``
	| ``remove``
	| ``raise``
	| ``optional``
	| ``required``
	| ``accessor``
	| ``attribute``
	| ``property``
	| ``readonly``
	| ``bound``
	| ``removable``
	| ``contrained``
	| ``transient``
	| ``maybevoid``
	| ``maybedefault``
	| ``maybeambiguous``

.. _cid-member.m_id:
.. code-block:: lua
	:class: title-code-block

	m_id

Holds Doxygen identifier string, e.g. ``structjnc___guid_1aec4f3165bbae5d72881b0d216e4ff15c``.

This string can be used as a unique identifier for creating labels and reference links.

.. _cid-member.m_name:
.. code-block:: lua
	:class: title-code-block

	m_name

Holds a string with the name of the member.

.. _cid-member.m_path:
.. code-block:: lua
	:class: title-code-block

	m_path

Holds a string with a forward-slash-separated path to the enum starting from the global namespace. For example, C++ enum ``io::NetworkAdapterType`` will have its path encoded as ``io/NetworkAdapterType``.

This field can be used to reconstruct a fully qualified name of the enum.

This field is only set when ``m_memberKind`` is ``enum``.

.. _cid-member.m_modifiers:
.. code-block:: lua
	:class: title-code-block

	m_modifiers

Holds a string with space-separated list of language-specific modifiers applied to the member.

.. _cid-member.m_type:
.. code-block:: lua
	:class: title-code-block

	m_type

Holds a `LinkedText` table describing the type of the member.

.. _cid-member.m_returntype:
.. code-block:: lua
	:class: title-code-block

	m_returnType

Holds a `LinkedText` table describing the return type of the member.

This field is only set when ``m_memberKind`` is one of:

	| ``function``
	| ``property``

.. _cid-member.m_argstring:
.. code-block:: lua
	:class: title-code-block

	m_argString

Holds a string describing type suffix (function parameters, array dimensions, etc)

This field is only set when ``m_memberKind`` is one of:

	| ``typedef``
	| ``variable``
	| ``function``
	| ``property``
	| ``event``

.. _cid-member.m_paramarray:
.. code-block:: lua
	:class: title-code-block

	m_paramArray

Holds an array table with detailed description of all parameters of the member.

Type of each element of the array is `Param`.

This field is only set when ``m_memberKind`` is one of:

	| ``typedef``
	| ``variable``
	| ``function``
	| ``property``
	| ``event``
	| ``define``

.. _cid-member.m_templateparamarray:
.. code-block:: lua
	:class: title-code-block

	m_templateParamArray

Holds a table containing an array of `Param` elements describing template parameters of the function.

This field is only set when ``m_memberKind`` is ``function``.

.. _cid-member.m_templatespecparamarray:
.. code-block:: lua
	:class: title-code-block

	m_templateSpecParamArray

Holds a table containing an array of `Param` elements describing template specialization parameters of the function.

This field is only set when ``m_memberKind`` is ``function``.

.. _cid-member.m_exceptions:
.. code-block:: lua
	:class: title-code-block

	m_exceptions

Holds a `LinkedText` table describing which exception could be thrown by the function.

This field is only set when ``m_memberKind`` is ``function``.

.. _cid-member.m_enumvaluearray:
.. code-block:: lua
	:class: title-code-block

	m_enumValueArray

Holds an array table with detailed description of all enum values of the enum.

Type of each element of the array is `EnumValue`.

This field is only set when ``m_memberKind`` is ``enum``.

.. _cid-member.m_bitfield:
.. code-block:: lua
	:class: title-code-block

	m_bitField

Holds a string with bit field suffix of the struct or union field.

This field is only set when ``m_memberKind`` is ``variable``.

.. _cid-member.m_initializer:
.. code-block:: lua
	:class: title-code-block

	m_initializer

Holds a `LinkedText` table with the member initializer.

This field is only set when ``m_memberKind`` is one of:

	| ``variable``
	| ``alias``
	| ``define``

.. _cid-member.m_briefdescription:
.. code-block:: lua
	:class: title-code-block

	m_briefDescription

Holds a `Description` table with the brief description of the member.

.. _cid-member.m_detaileddescription:
.. code-block:: lua
	:class: title-code-block

	m_detailedDescription

Holds a `Description` table with the detailed description of the member.

.. _cid-member.m_inbodydescription:
.. code-block:: lua
	:class: title-code-block

	m_inBodyDescription

Holds a `Description` table with the in-body description of the member.
