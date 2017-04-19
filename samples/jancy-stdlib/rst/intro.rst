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

Jancy Standard Library Reference
================================

Let me be honest with you from the very beginning. At the moment Jancy standard library is *not exactly mature*. If you try to use Jancy as a scripting engine in your own project, most likely you will soon need to write auxillary classes and functions to make up for missing functionality. Should this scare you and stop you from embedding Jancy? *Absolutely NOT*!

First of all, Jancy is the **easiest-to-extend-with-c-or-c-plus-plus** scripting language in the world. Being **ABI compatible with C** calling conventions was of paramount importance during the Jancy language design stage and now we reap the benefits of it. With Jancy you can simply write a C/C++ function, a data structure or a set of member methods in C/C++ and then **directly access** it from your script. Browse the sources of Jancy standard library and see for yourself.

Secondly, Jancy was created as a scripting language for the `IO Ninja <http://tibbo.com/ninja>`_ project, so its standard library *already* features a rich set of classes for preforming **low-level IO**: files, sockets, serial ports, SSH channels and so on. We even provide out *of-the-box* support for functionality often left for doing-it-yourself: **pcap**, windows **named pipe** servers, **mailslots**, **serial port enumeration** etc.

Finally, Jancy (including its standard library) is an **open-source** project hosted on GitHub at http://github.com/vovkos/jancy, so when you implement a class or a function missing in the standrard library, it's easy to **contribute** it and save the future developers from repeating the same work.

Now let's see what is *already* available...
