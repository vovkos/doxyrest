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

.. _disposable:

Using Disposable Pattern
========================

A lot of classes in Jancy standard library (e.g. `io.File`, `io.Serial` etc) provide ``close`` methods  to terminate current session and release associated resources.

If an instance of such class is being abandoned without calling ``close``, it will be closed automatically in class *destructor* -- which will happen whenever the Jancy Garbage Collector decides to sweep unused objects.

However, leaving this at Garbage Collector' discretion is undesirable, as it leads to undeterministic resource release -- file handle or other resources will be held for unknown period of time.

An easy way for overcoming this problem is to work with objects that do require timely invokation of ``close`` or similar resource-releasing method using Jancy ``disposable`` storage specifier.

Example:

.. ref-code-block:: jnc

	foo ()
	{
	    // ...

	    disposable `io.File` file;
	    file.open ();

	    // work with file...
	} // <-- file.close () is guaranteed to be called upon exiting the scope

When program runs out of scope where a variable of disposable class was declared in, Jancy compiler will insert a call to ``dispose`` method (which is usually *aliased* to an actual release methods such as ``close``). This method will be called no matter how the program runs out of scope, be it a normal control flow, ``return``,  ``break``, ``continue`` or an exception.

Note that you can easily write disposable classes yourself. All you have to do is to provide a ``dispose`` method:

.. ref-code-block:: jnc

	class MyDisposableClass
	{
	    dispose ()
	    {
	        printf ("Releasing resources...");
	    }
	}

	foo ()
	{
	    // ...

	    disposable MyDisposableClass c;

	    // work with c...

	    throw; // <-- c.dispose () will get called
	}
