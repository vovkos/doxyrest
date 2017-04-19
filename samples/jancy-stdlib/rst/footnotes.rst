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

.. |footnote-errorcode| replace::

	You can use exception semantics to handle the erroneous return values of ``errorcode`` functions. For more details refer to :ref:`errorcode`.

.. |footnote-disposable| replace::

	You can ensure timely release of resources by using *disposable* pattern. For more details refer to :ref:`disposable`.

.. |footnote-set-property-fail| replace::

	If setting this property fails, the actual IO error reported by the operating system is recorded; then exception is thrown.

.. |footnote-sync-id| replace::

	When assigining scheduled event handlers, you need to make sure the particular event is still relevant. For more details refer to :ref:`sync-id`.
