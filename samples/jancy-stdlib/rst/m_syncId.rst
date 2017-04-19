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

.. _sync-id:

m_syncId Model Explanation
==========================

When working with asynchronous IO interfaces, you need to take care of one subtle issue.

Let's say, you have an IO stream such as `io.FileStream` or `io.Socket` and you have assigned your IO event handler. This event handler is originally called from within the internal IO thread, but may be postponed should you assign some *scheduler*).

The problem is, how to synchronize your main thread (from which you call ``open``, ``write`` etc) and the event handler? Who knows, maybe the *event* happened moments ago, before the latest ``close`` on the IO stream, but your *event handler* was invoked just now?

To deal with this issue, all asynchronous  IO classes provide ``m_syncId`` field and sequentially increments it upon every ``close`` and ``open``. In your event handler you should compare ``m_syncId`` of the IO stream and ``m_syncId`` of event params. If there is a mismatch, simply ignore this event -- it was originated from the previous IO stream session.

``m_syncId`` is a public read-write field, so you can also increment it manually, to identify certain *stages* in your session.
