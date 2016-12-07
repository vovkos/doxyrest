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

.. _cid-description:

Description
===========

Table of this type describes a Doxygen *description* extracted from in-source documentation comments.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_isEmpty <cid-description.m_isempty>`
	:ref:`m_docBlockList <cid-description.m_docblocklist>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-description.m_isempty:
.. code-block:: lua
	:class: title-code-block

	m_isEmpty

Holds ``true`` if description is empty or ``false`` otherwise.

.. _cid-description.m_docblocklist:
.. code-block:: lua
	:class: title-code-block

	m_docBlockList

Holds an array table of blocks constituting this description.

Type of each element of the array is `DocBlock`.
