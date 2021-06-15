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

Subgroups
=========

Doxygen groups (``\groupdef``, ``addtogroup``, etc) produce separate pages by design -- as such, they cannot be used to group members together. However, often times a bunch of members relate so closely to one another that they simply do *not deserve* a dedicated documentation block for each and every one of them. We would like to have *one block* for a *subgroup* of closely related members -- and display these members as such, i.e. together.

Doxyrest provides a facility just for that -- *subgroups*.

In your ``Doxyfile``, add this alias:

.. code-block:: none

	ALIASES += "subgroup=\internal :subgroup: \endinternal"

In your C/C++ code, add this special comment to the very first member in the subgroup:

.. code-block:: cpp

	/*!
		\subgroup

		Functions ``foo()``, ``bar()``, and ``baz()`` do similar things
		with a few minor differences, as outlined here: ...

	*/

	void foo();
	void bar();
	void baz();

In your final documentation, you will have all three functions grouped together as such:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void foo()
	void bar()
	void baz()

Functions ``foo()``, ``bar()``, and ``baz()`` do similar things with a few minor differences, as outlined as outlined here: ...

.. note::

	Why not have something like ``\groupdef``, but for members on the same page?

	Well, in my opinion, that would be a *wrong thing* to do. We already have two levels of member grouping:

	* By protection level (``public``, ``protected``, etc);
	* By member kind (variables, functions, etc).

	Adding yet another level of grouping -- *to the same page* -- would destroy the nice compound overview we have right now and would be unintuitive to comprehend.

	If there really is a need to subdivide members (of a class, for instance), the proper approach would be to adjust the API itself (i.e. subdivide related members between subclasses).
