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

.. _cid-reftext:

RefText
=======

Table of this type describes a block (either plain-text or hyperlink) of `LinkedText`.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_refKind <cid-reftext.m_refkind>`
	:ref:`m_text <cid-reftext.m_text>`
	:ref:`m_id <cid-reftext.m_id>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-reftext.m_refkind:
.. code-block:: lua
	:class: title-code-block

	m_refKind

Holds a string describing hyperlink target. Must be one of:

	| ``<undefined>``
	| ``compound``
	| ``member``

If this block is not a hyperlink, ``m_refKind`` contains ``<undefined>``.

.. _cid-reftext.m_text:
.. code-block:: lua
	:class: title-code-block

	m_text

Holds a string with plain-text representation of the block.

.. _cid-reftext.m_id:
.. code-block:: lua
	:class: title-code-block

	m_id

Holds a Doxygen identifier of the hyperlink target. If this block is not a hyperlink ``m_id`` contains an empty string.
