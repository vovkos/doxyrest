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

Doxyrest provides a much more elegant approach by (optionally) assigning each group a sorting order priortiy. To use this facility, add the following custom command to you ``Doxyfile``:








