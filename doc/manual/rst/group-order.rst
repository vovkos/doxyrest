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

Ordering Groups
===============

Most times, you do not want to show Doxygen groups in random order or in alphabetic order -- there's always some desired logical sequence. With Doxygen, the only way to circumvent this issue is to place all group documentation *into the same* file and within it, sort groups manually.

Doxyrest provides a much more elegant approach by (optionally) assigning each group a *sorting order value*. To use this facility, add the following custom command to you ``Doxyfile``:

.. code-block:: none

	ALIASES += "grouporder{1}=\internal :grouporder(\1) \endinternal"

In your C/C++ code, you can now add this extra command to the documentation of your groups:

.. code-block:: cpp

	/*!
		\defgroup group_a Group A
		\grouporder 1

		Documentation for group A goes here...
	*/

	...

	/*!
		\defgroup group_b Group B
		\grouporder 2

		Documentation for group B goes here...
	*/

	...

	/*!
		\defgroup group_c Group C
		\grouporder 3

		Documentation for group C goes here...
	*/

This will ensure Group A is placed 1st, then Group B, then Group C -- no matter the group titles, placement in files or declaration order.

By default, groups are sorted using their title names. You can also sort groups using their ``ID``-s. To do so, use add this to your ``doxyrest-config.lua``:

.. ref-code-block:: lua

	`SORT_GROUPS_BY` = "id"
