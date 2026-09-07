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

.. _footnotes:

Footnotes
=========

Footnotes are essential for a good documentation system. Alas, Doxygen doesn't provide any support for that at all.

Doxyrest offers a workaround by getting documentation blocks for specially named pseudo-members and converting those into footnotes. To avoid inteference with the actual members, place all the footnote declarations into a section which normally is excluded from compilation.

.. code-block:: cpp

	class C
	{
	public:
		int m_publicField;

		//! Documentation for foo [#f1]_
		void foo();

		//! Documentation for bar [#f2]_
		void bar();

	#ifdef _DOXYGEN
		//! Documentation for footnote f1
		int _footnote_f1;

		//! Documentation for footnote f2
		int _footnote_f2;
	#endif
	}

In your ``Doxyfile``:

.. code-block:: none

	PREDEFINED = _DOXYGEN

.. warning

Make sure ``_DOXYGEN`` is only defined when you are building documentation (or else you may end up with a broken data layout)!

In your ``doxyrest-config``:

.. ref-code-block:: lua

	`FOOTNOTE_MEMBER_PREFIX` = "_footnote_"

If `FOOTNOTE_MEMBER_PREFIX` is ``nil`` or an empty string, Doxyrest will not attempt to convert any members to footnotes.

After running the Doxyrest pipeline for the above example, you should see something like that:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void foo()

Documentation for foo [#f1]_

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void bar()

Documentation for bar [#f2]_

----------------------

.. rubric:: Footnotes:

.. [#f1] Documentation for footnote f1

.. [#f2] Documentation for footnote f2
