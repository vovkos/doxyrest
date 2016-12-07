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

.. _cid-compound:

Compound
========

Table of this type describes a Doxygen *compound* such as *class*, *namespace*, *group* etc.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_parent <cid-compound.m_parent>`
	:ref:`m_compoundKind <cid-compound.m_compoundkind>`
	:ref:`m_id <cid-compound.m_id>`
	:ref:`m_name <cid-compound.m_name>`
	:ref:`m_title <cid-compound.m_title>`
	:ref:`m_path <cid-compound.m_path>`
	:ref:`m_briefDescription <cid-compound.m_briefdescription>`
	:ref:`m_detailedDescription <cid-compound.m_detaileddescription>`
	:ref:`m_templateParamArray <cid-compound.m_templateparamarray>`
	:ref:`m_templateSpecParamArray <cid-compound.m_templatespecparamarray>`
	:ref:`m_groupArray <cid-compound.m_grouparray>`
	:ref:`m_namespaceArray <cid-compound.m_namespacearray>`
	:ref:`m_enumArray <cid-compound.m_enumarray>`
	:ref:`m_structArray <cid-compound.m_structarray>`
	:ref:`m_unionArray <cid-compound.m_unionarray>`
	:ref:`m_classArray <cid-compound.m_classarray>`
	:ref:`m_typedefArray <cid-compound.m_typedefarray>`
	:ref:`m_variableArray <cid-compound.m_variablearray>`
	:ref:`m_constructorArray <cid-compound.m_constructorarray>`
	:ref:`m_destructor <cid-compound.m_destructor>`
	:ref:`m_functionArray <cid-compound.m_functionarray>`
	:ref:`m_propertyArray <cid-compound.m_propertyarray>`
	:ref:`m_eventArray <cid-compound.m_eventarray>`
	:ref:`m_aliasArray <cid-compound.m_aliasarray>`
	:ref:`m_defineArray <cid-compound.m_definearray>`
	:ref:`m_footnoteArray <cid-compound.m_footnotearray>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-compound.m_parent:
.. code-block:: lua
	:class: title-code-block

	m_parent

Holds a back-pointer to parent ``Compound`` table.

.. _cid-compound.m_compoundkind:
.. code-block:: lua
	:class: title-code-block

	m_compoundKind

Holds a string describing compound type. Must be one of:

	| ``<undefined>``
	| ``class``
	| ``struct``
	| ``union``
	| ``interface``
	| ``protocol``
	| ``category``
	| ``exception``
	| ``file``
	| ``namespace``
	| ``doxygroup``
	| ``group``
	| ``page``
	| ``example``
	| ``dir``

.. _cid-compound.m_id:
.. code-block:: lua
	:class: title-code-block

	m_id

Holds Doxygen identifier string, e.g. ``group__type_1gada12e3c9caedb0e18d384bfdc7e7def3``.

This string can be used as a unique identifier for creating labels and reference links.

.. _cid-compound.m_name:
.. code-block:: lua
	:class: title-code-block

	m_name

Holds a string with the name of the compound.

.. _cid-compound.m_title:
.. code-block:: lua
	:class: title-code-block

	m_title

Holds a string with the title (if specified) or an empty string.

Usually compound titles are not used unless this is a ``group`` compound.

.. _cid-compound.m_path:
.. code-block:: lua
	:class: title-code-block

	m_path

Holds a string with a forward-slash-separated path to the compound starting from the global namespace. For example, C++ class ``sys::win::WaitableTimer`` will have its path encoded as ``sys/win/WaitableTimer``.

This field can be used to reconstruct a fully qualified name of the compound.

.. _cid-compound.m_briefdescription:
.. code-block:: lua
	:class: title-code-block

	m_briefDescription

Holds a `Description` table with the brief description of the compound.

.. _cid-compound.m_detaileddescription:
.. code-block:: lua
	:class: title-code-block

	m_detailedDescription

Holds a `Description` table with the detailed description of the compound.

.. _cid-compound.m_templateparamarray:
.. code-block:: lua
	:class: title-code-block

	m_templateParamArray

Holds a table containing an array of all template parameters of the compound.

Type of each element of the array is `Param`

This field is only set when ``m_compoundKind`` is one of:

	| ``struct``
	| ``union``
	| ``class``
	| ``interface``

.. _cid-compound.m_templatespecparamarray:
.. code-block:: lua
	:class: title-code-block

	m_templateSpecParamArray

Holds a table containing an array of all template specialization parameters of the compound.

Type of each element of the array is `Param`

This field is only set when ``m_compoundKind`` is one of:

	| ``struct``
	| ``union``
	| ``class``
	| ``interface``

.. _cid-compound.m_grouparray:
.. code-block:: lua
	:class: title-code-block

	m_groupArray

Holds a table containing an array of all the groups withing the compound.

Type of each element of the array is ``Compound``. `Compound.m_compoundKind` should be ``group``.

.. _cid-compound.m_namespacearray:
.. code-block:: lua
	:class: title-code-block

	m_namespaceArray

Holds a table containing an array of all the nested namespaces.

Type of each element of the array is ``Compound``. `Compound.m_compoundKind` should be ``namespace``.

.. _cid-compound.m_enumarray:
.. code-block:: lua
	:class: title-code-block

	m_enumArray

Holds a table containing an array of all the enums withing the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``enum``.

.. _cid-compound.m_structarray:
.. code-block:: lua
	:class: title-code-block

	m_structArray

Holds a table containing an array of all the nested structs within the compound.

Type of each element of the array is ``Compound``. `Compound.m_compoundKind` should be ``struct``.

.. _cid-compound.m_unionarray:
.. code-block:: lua
	:class: title-code-block

	m_unionArray

Holds a table containing an array of all the nested unions within the compound.

Type of each element of the array is ``Compound``. `Compound.m_compoundKind` should be ``union``.

.. _cid-compound.m_classarray:
.. code-block:: lua
	:class: title-code-block

	m_classArray

Holds a table containing an array of all the nested classes within the compound.

Type of each element of the array is ``Compound``. `Compound.m_compoundKind` should be ``class``.

.. _cid-compound.m_typedefarray:
.. code-block:: lua
	:class: title-code-block

	m_typedefArray

Holds a table containing an array of all the typedefs within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``typedef``.

.. _cid-compound.m_variablearray:
.. code-block:: lua
	:class: title-code-block

	m_variableArray

Holds a table containing an array of all the variables within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``variable``.

.. _cid-compound.m_constructorarray:
.. code-block:: lua
	:class: title-code-block

	m_constructorArray

Holds a table containing an array of all the constructors of the type compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``function``.

.. _cid-compound.m_destructor:
.. code-block:: lua
	:class: title-code-block

	m_destructor

Holds a `Member` table describing the destructor of the type compound (if there is any) or ``nil``.

`Member.m_memberKind` should be ``function``.

.. _cid-compound.m_functionarray:
.. code-block:: lua
	:class: title-code-block

	m_functionArray

Holds a table containing an array of all the functions/methods within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``function``.

.. _cid-compound.m_propertyarray:
.. code-block:: lua
	:class: title-code-block

	m_propertyArray

Holds a table containing an array of all the properties within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``property``.

.. _cid-compound.m_eventarray:
.. code-block:: lua
	:class: title-code-block

	m_eventArray

Holds a table containing an array of all the events within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``event``.

.. _cid-compound.m_aliasarray:
.. code-block:: lua
	:class: title-code-block

	m_aliasArray

Holds a table containing an array of all the aliases within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``alias``.

.. _cid-compound.m_definearray:
.. code-block:: lua
	:class: title-code-block

	m_defineArray

Holds a table containing an array of all the macro definitions within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``define``.

.. _cid-compound.m_footnotearray:
.. code-block:: lua
	:class: title-code-block

	m_footnoteArray

Holds a table containing an array of all the footnotes defined within the compound.

Type of each element of the array is `Member`. `Member.m_memberKind` should be ``footnote``.
