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

.. _cid-docblock:

DocBlock
========

Table of this type describes a block which `Description` table consists of.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_blockKind <cid-docblock.m_blockkind>`
	:ref:`m_title <cid-docblock.m_title>`
	:ref:`m_text <cid-docblock.m_text>`
	:ref:`m_childBlockList <cid-docblock.m_childblocklist>`
	:ref:`m_refKind <cid-docblock.m_refkind>`
	:ref:`m_id <cid-docblock.m_id>`
	:ref:`m_simpleSectionKind <cid-docblock.m_simplesectionkind>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-docblock.m_blockkind:
.. code-block:: lua
	:class: title-code-block

	m_blockKind

Holds a string describing the type of the block. Must be one of:

	| ``<undefined>``,
	| ``paragraph``,
	| ``section``,
	| ``internal``,
	| ``simplesect``,
	| ``ref``,
	| ``computeroutput``,

.. _cid-docblock.m_title:
.. code-block:: lua
	:class: title-code-block

	m_title

Holds a string with the title of the block (empty string for untitled blocks).

.. _cid-docblock.m_text:
.. code-block:: lua
	:class: title-code-block

	m_text

Holds a string with the plain-text contents of the block.

.. _cid-docblock.m_childblocklist:
.. code-block:: lua
	:class: title-code-block

	m_childBlockList

Holds an array table with all the child blocks of the current block.

Type of each element of the array is ``DocBlock``.

.. _cid-docblock.m_refkind:
.. code-block:: lua
	:class: title-code-block

	m_refKind

Holds a string describing hyperlink target. Must be one of:

	| ``compound``
	| ``member``

This field is only set if ``m_blockKind`` is ``ref``.

.. _cid-docblock.m_id:
.. code-block:: lua
	:class: title-code-block

	m_id

Holds a Doxygen identifier.

This field is only set if ``m_blockKind`` is one of:

	| ``ref``
	| ``section``
	| ``simplesec``

If ``m_blockKind`` is  ``ref``, then ``m_id`` describes the identifier of the hyperlink target.

For section blocks, ``m_id`` describes the identifier of the section itself.

.. _cid-docblock.m_simplesectionkind:
.. code-block:: lua
	:class: title-code-block

	m_simpleSectionKind

Holds the type of the simple section block. Must be one of:

	| ``<undefined>``
	| ``see``

This field is only set if ``m_blockKind`` is ``simplesect``.

If ``m_simpleSectionKind`` is ``see``, then this is a **see-also** block.