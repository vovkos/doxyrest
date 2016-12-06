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

Overview
~~~~~~~~

Table of this type describes a Doxygen *member*.

.. ref-code-block:: lua
	:class: overview-code-block

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
	:ref:`m_templateParamArray <cid-member.m_templateparamarray>`
	:ref:`m_templateSpecParamArray <cid-member.m_templatespecparamarray>`
	:ref:`m_paramArray <cid-member.m_paramarray>`
	:ref:`m_exceptions <cid-member.m_exceptions>`
	:ref:`m_enumValueArray <cid-member.m_enumvaluearray>`
	:ref:`m_bitField <cid-member.m_bitfield>`
	:ref:`m_initializer <cid-member.m_initializer>`
	:ref:`m_briefDescription <cid-member.m_briefdescription>`
	:ref:`m_detailedDescription <cid-member.m_detaileddescription>`
	:ref:`m_inBodyDescription <cid-member.m_inbodydescription>`

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

.. _cid-member.m_memberkind:
.. code-block:: lua
	:class: title-code-block

	m_memberKind

Holds a string describing member type. Must be one of:

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

	| ``public``
	| ``protected``
	| ``private``
	| ``package``

.. _cid-member.m_virtualkind:
.. code-block:: lua
	:class: title-code-block

	m_virtualKind

Holds a string describing whether this member is virtual or not. Must be one of:

	``non-virtual``
	``virtual``
	``pure-virtual``

.. _cid-member.m_flags:
.. code-block:: lua
	:class: title-code-block

	m_flags

An empty string or a space-delimited list of:

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

Detailed documentation for ``m_name``.

.. _cid-member.m_path:
.. code-block:: lua
	:class: title-code-block

	m_path

Detailed documentation for ``m_path``.

.. _cid-member.m_modifiers:
.. code-block:: lua
	:class: title-code-block

	m_modifiers

Detailed documentation for ``m_modifiers``.

.. _cid-member.m_type:
.. code-block:: lua
	:class: title-code-block

	m_type

Detailed documentation for ``m_type``.

.. _cid-member.m_returntype:
.. code-block:: lua
	:class: title-code-block

	m_returnType

Detailed documentation for ``m_returnType``.

.. _cid-member.m_argstring:
.. code-block:: lua
	:class: title-code-block

	m_argString

Detailed documentation for ``m_argString``.

.. _cid-member.m_templateparamarray:
.. code-block:: lua
	:class: title-code-block

	m_templateParamArray

Detailed documentation for ``m_templateParamArray``.

.. _cid-member.m_templatespecparamarray:
.. code-block:: lua
	:class: title-code-block

	m_templateSpecParamArray

Detailed documentation for ``m_templateSpecParamArray``.

.. _cid-member.m_paramarray:
.. code-block:: lua
	:class: title-code-block

	m_paramArray

Detailed documentation for ``m_paramArray``.

.. _cid-member.m_exceptions:
.. code-block:: lua
	:class: title-code-block

	m_exceptions

Detailed documentation for ``m_exceptions``.

.. _cid-member.m_enumvaluearray:
.. code-block:: lua
	:class: title-code-block

	m_enumValueArray

Detailed documentation for ``m_enumValueArray``.

.. _cid-member.m_bitfield:
.. code-block:: lua
	:class: title-code-block

	m_bitField

Detailed documentation for ``m_bitField``.

.. _cid-member.m_initializer:
.. code-block:: lua
	:class: title-code-block

	m_initializer

Detailed documentation for ``m_initializer``.

.. _cid-member.m_briefdescription:
.. code-block:: lua
	:class: title-code-block

	m_briefDescription

Detailed documentation for ``m_briefDescription``.

.. _cid-member.m_detaileddescription:
.. code-block:: lua
	:class: title-code-block

	m_detailedDescription

Detailed documentation for ``m_detailedDescription``.

.. _cid-member.m_inbodydescription:
.. code-block:: lua
	:class: title-code-block

	m_inBodyDescription

Detailed documentation for ``m_inBodyDescription``.
