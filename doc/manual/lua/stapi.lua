--------------------------------------------------------------------------------
--
--  This file is part of the Doxyrest toolkit.
--
--  Doxyrest is distributed under the MIT license.
--  For details see accompanying license.txt file,
--  the public copy of which is also available at:
--  http://tibbo.com/downloads/archive/doxyrest/license.txt
--
--------------------------------------------------------------------------------

--!
--! \defgroup api-0-stapi
--! \ingroup api
--! \title String templating API functions
--!
--! This section describes functions exported to the namespace of Lua frames
--! by the main Doxyrest string templating engine.
--!
--! @{
--!

--------------------------------------------------------------------------------

--! Returns current line number in the emitted output.

function getLine() end

--! Returns current column number in the emitted output.

function getCol() end

--! Returns text of the current line in the emitted output.

function getLineText() end

--! Emits ``s``, i.e. appends string ``s`` to the output.

function emit(s) end

--!
--! Emits the *original* text from the current string template frame. The
--! arguments ``offset`` and ``length`` define the region of the frame to emit.
--! Normally, you never have to manually call this function, as the string
--! template engine inserts calls to ``passthrough(...)`` in necessary
--! locations automatically.
--!

function passthrough(offset, length) end

--! Removes all trailing spaces from the current line in the emitted output.

function trimOutput() end

--!
--! \subgroup
--! Processes a frame located in the file ``fileName`` and appends the results
--! is appended to the current output.
--!
--! All the variables and functions declared up to this point remain accessible
--! from Lua code in the frame. It's also possible to pass extra arguments to
--! the frame by passing more values after the ``fileName`` argument [#f1]_.
--!
--! ``includeFileWithIndent`` function also injects extra indent before each
--! and every line generated as a result of processing the frame ``fileName``.
--!
--!

function includeFile(fileName, ...) end
function includeFileWithIndent(indent, fileName, ...) end

--!
--! Processes a frame located in the file ``frameFileName`` and writes the
--! output to the file ``targetFileName``.
--!
--! All the variables and functions declared up to this point remain accessible
--! from Lua code in the frame. It's also possible to pass extra arguments to
--! the frame by passing more values after the ``frameFileName`` argument [#f1]_.
--!

function generateFile(targetFileName, frameFileName, ...) end

--[[!
	\footnote f1

	As always in Lua, you can pass extra arguments to frame processing
	functions in addition to mandatory ones. This technique provides a
	convenient way of passing certain **context** to the frame (and is
	relied upon in standard ``*.rst.in`` files).

	Later you access these extra arguments from your frame like this:

	.. code-block:: lua

		local argArray = table.pack (...)
		local extraArg = argArray [1]
]]

--------------------------------------------------------------------------------

--! @}
