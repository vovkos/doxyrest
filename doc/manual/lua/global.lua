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
--! \defgroup global
--! \grouporder 99
--! \title Global Scope of Lua Frames
--!
--! This is a list of everything what's exported by Doxyrest into the scope of
--! Lua frames.
--!

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

--!
--! \defgroup api
--! \grouporder 1
--! \title Lua Frame API
--!
--! This section provides a reference to the API exposed by Doxyrest to the
--! code running inside Lua frames.
--!

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

--!
--! \defgroup api-global-vars
--! \ingroup api
--! \grouporder 5
--! \title Doxyrest Global Variables
--!
--! These global variables represent the root of the Doxygen tree. Lua frames
--! use them to walk around the tree in a specific order and generate
--! documentation for everything.
--!
--! @{
--!

--------------------------------------------------------------------------------

--! A table of type `GlobalNamespace` containing the global namespace of the
--! source tree.

g_globalNamespace = {}

--! Array of group `Compound`-s

g_groupArray = {}

--! Array of `Page`-s

g_pageArray = {}

--! Array of example `Compounds`-s

g_pageArray = {}

--! Holds the directory of the frame file which is being currently processed.

g_frameDir = ""

--! Holds the directory of the target file which is being currently written to.

g_targetDir = ""

--! Holds the file name of the target file which is being currently written to.

g_targetFileName = ""

--------------------------------------------------------------------------------

--! @}
