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

.. _cid-linkedtext:

LinkedText
==========

Table of this type describes a text (possibly) with injected hyperlinks.

Overview of Members
~~~~~~~~~~~~~~~~~~~

.. ref-code-block:: lua
	:class: overview-code-block

	:ref:`m_isEmpty <cid-linkedtext.m_isempty>`
	:ref:`m_plainText <cid-linkedtext.m_plaintext>`
	:ref:`m_refTextArray <cid-linkedtext.m_reftextarray>`

Detailed Description of Members
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _cid-linkedtext.m_isempty:
.. code-block:: lua
	:class: title-code-block

	m_isEmpty

Holds ``true`` if the text is empty or ``false`` otherwise.

.. _cid-linkedtext.m_plaintext:
.. code-block:: lua
	:class: title-code-block

	m_plainText

Holds a string with plain-text representation of the text, i.e. as if there were no any hyperlinks.

.. _cid-linkedtext.m_reftextarray:
.. code-block:: lua
	:class: title-code-block

	m_refTextArray

Holds an array table of plain-text and hyperlink blocks constituting the full text.

Type of each element of the array is `RefText`.
