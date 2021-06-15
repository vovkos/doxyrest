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

Unnamed Structs/Unions
======================

Doxygen has a major flaw in its XML model for unnamed C/C++ structs or unions. The problem is that it's impossible to reconstruct the original declaration *just* from the generated XML database.

For example, the following snippet:

.. code-block:: cpp

	struct Parent
	{
		int m_field1;

		struct
		{
			int m_field2;
			int m_field3;
		};

		int m_field4;
	};

\... *AND* the following snippet:

.. code-block:: cpp

	struct Parent
	{
		struct
		{
			int m_field1;
			int m_field2;

			struct
			{
				int m_field3;
			};
		};

		int m_field4;
	};

\... both yield the same XML:

.. code-block:: mxml

  	<compounddef>
  		<!-- struct Parent -->

    	<memberdef>
    		<!-- int m_field1 -->
	  	</memberdef>

    	<memberdef>
    		<!-- int m_field2 -->
	  	</memberdef>

    	<memberdef>
    		<!-- int m_field3 -->
	  	</memberdef>

    	<memberdef>
    		<!-- unnamed member of type Parent::@0 -->
	  	</memberdef>

    	<memberdef>
    		<!-- int m_field4 -->
	  	</memberdef>
  	</compounddef>

I have omitted the gory details of the Doxygen XML, but it should be enough to demonstrate the point -- Doxygen XML alone is simply ambiguous!

We have to somehow add extra information, but we don't want a reader of our documentation to see that. Solution -- ``\internal`` sections!

In your ``Doxyfile``, add this alias:

.. code-block:: none

	ALIASES += "unnamed{1}=\internal :unnamed(\1) \endinternal"

In your C/C++ code, add this special comment to the very first field of the unnamed struct:

.. code-block:: cpp

	struct Parent
	{
		union
		{
			//! \unnamed{union}
			int m_field1;
			int m_field2;
		};

		int m_field3;
	};

In case of nested unnamed structs, you have to also provide:

1. path to the field in the unnamed struct;
2. the number of fields in this unnamed struct.

.. code-block:: cpp

	struct Parent
	{
		union
		{
			struct
			{
				//! \unnamed{union/struct:2}
				int m_field1;
				int m_field2;
			};

			struct
			{
				//! \unnamed{union/struct:3}
				int m_field2;
				int m_field3;
				int m_field4;
			};
		};

		int m_field5;
	};

.. note::

	It's OK to inject ``\unnamed`` anywhere into a documentation block for the first member, for example:

	.. code-block:: cpp

		//! This is documentation \unnamed{union} for m_field1
