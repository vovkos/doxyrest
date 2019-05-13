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

Documenting Unnamed Structs/Unions
==================================

Doxygen has a major flaw in its XML model for unnamed C/C++ structs or unions. The problem is that it's impossible to reconstruct the original declaration *just* from the generated XML database.

For example, the following code snippet:

.. code-block:: cpp

	struct Parent
	{
		int m_field1;

		struct
		{
			int m_field2;
		};

		int m_field2;
	};

\... yields the following XML:

.. code-block:: xml

	struct Parent
	{
		int m_field1;

		struct
		{
			int m_field2;
		};

		int m_field2;
	};
