.. .............................................................................
..
..  This file is part of the Jancy toolkit.
..
..  Jancy is distributed under the MIT license.
..  For details see accompanying license.txt file,
..  the public copy of which is also available at:
..  http://tibbo.com/downloads/archive/jancy/license.txt
..
.. .............................................................................

.. _errorcode:

errorcode Functions
===================

Jancy provides syntactic sugar over good old C-style error code check model. It looks just like throwing and catching exceptions and frees the developer from the routine of constantly checking for error codes.

This model allows the developer to choose between using error code checks or exception semantics when calling the very same function depending on what is more appropriate or convenient in each particular case!

A function marked by the ``errorcode`` modifier will have its return value interpreted as an error code. Intuitive error conditions are assumed: ``false`` for booleans, ``null`` for pointers and ``-1`` for integers. Other types will be cast to ``bool`` for error condition checking.

.. ref-code-block:: jnc

	bool errorcode bar (
	    int a,
	    int b
	    )
	{
	    printf ("bar (%d, %d)\n", a, b);

	    if (a > b)
	        return false; // fail

	    // ...

	    return true;
	}

	int foo ()
	{
	    // use 'try' operator to handle the error code manually...

	    bool result = try bar (1, -150);
	    if (!result)
	    {
	        printf ("bar failed\n");
	        // handle the error...
	    }

	    bar (2, 3);
	    bar (5, 4); // will fail

	    // ...

	    return 0;

	    // ...or use the 'catch' label to handle it in with exception semantics

	catch:
	    printf ("exception was caught\n");
	    return -1;
	}

Note that the both ``try`` and ``catch`` constructs will also properly handle other, non-``errorcode`` exceptions, such as ``null`` pointer access, division by zero and so on.
