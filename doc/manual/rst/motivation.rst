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

Motivation
==========

Sadly enough, up until now fellow C/C++ programmers don't have much choice when it comes to documenting their C/C++ APIs. It basically boils down to these three options:

1. **Doxygen**;
2. **QDoc**;
3. Writing everything *manually*.

I will dismiss the option #3 right away. If the API is evolving -- which is almost always the case as long as the product is alive -- it is simply *impossible to keep coherence* between the documentation and the C/C++ sources. To rephrase, a manually written API documentation is *always out-of-sync*.

Next, Doxygen. Don't get me wrong. Doxygen was revolutional when if first emerged, and it quickly became a de-facto standard of documenting C/C++ code. But unfortunately, it has two major problems:

1.1. The default Doxygen output is... not exactly good-looking.

	Some people may not be as picky as I am, but I believe no professional web designer would ever approve the HTML output of Doxygen. However, it would be alright -- should Doxygen provide a way to tweak its output. Which leads us to a larger problem...

1.2. Doxygen never really developed *sufficient customizability*.

	Yeah, you can change CSS and replace headers and footers, but that's about it. The thing is, too much is *hardcoded into the monolithic C++* core of Doxygen. That applies to both the back-end (generation of HTML and PDF) and to the front end (source code parser).

QDoc generates much, much nicer default HTML output. But it shares the biggest problem of Doxygen:

2.1. Both the parser and the HTML emitter are hardcoded into the monolithic C++ core!

	You can't customize much in the output other than changing a few QDoc configuration variables in ``.qdocconf`` files and then twiddling CSS. Moreover, unlike Doxygen (which is a multi-language documentation generator) QDoc is strictly *C++ only* [#f1]_!

Besides, and this was also crucial for us at Tibbo,

2.2. There is no PDF output!

I was contemplating the idea of creating a better alternative for documenting C/C++ for many years already. But the actual decision to finally get down to it came with the need to document `Jancy <https://github.com/vovkos/jancy>`_ API for `IO Ninja <https://ioninja.com>`_ (a universal all-in-one low-level IO debugger).

Jancy is a C-family scripting language, but its keyword set and new syntax constructs make it impossible to be passed to the out-of-the-box Doxygen (and obviously, not to QDoc). And neither provide front-end modularity -- to add a new parser you need to merge it into the monolithic С++ core and re-compile everything.

So, I finally decided to fix the problem once and for all -- and **Doxyrest** was born!

----------------------

.. rubric:: Footnotes:

.. [#f1] And not even just C++, but a dialect of C++ -- with all the ``Q_OBJECT``, ``Q_PROPERTY``, ``foreach`` etc macro language extensions.
