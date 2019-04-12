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
--! File with project-specific reStructuredText definitions. When non-nil, this
--! file will be included into every generated ``.rst`` file.
--!

GLOBAL_DEF_FILE = nil

--!
--! File with project introduction (reStructuredText). When non-nil, this file
--! will be included into ``index.rst`` file and NOT added to the list of other
--! documentation pages.
--!

INTRO_FILE = nil

--!
--! The title of the main (index) page.
--!

INDEX_TITLE = "My Project Documentation"

--!
--! Convert ``\verbatim`` sections in doxy-comments to ``.. code-block::``
--! sections in the output reStructuredText. The string value of
--! ``VERBATIM_TO_CODE_BLOCK`` will be used as the language of
--! ``.. code-block::`` section. By default, it's ``none`` which results in
--! no syntax highlighting. To disable conversion at all, use ``nil``.
--!

VERBATIM_TO_CODE_BLOCK = "none"

--!
--! Exclude items declared in specific locations. Use a regular expression to
--! define a mask of directories/source files to completely exclude from the
--! final documentation. For example, ``.*/impl/.*lua$`` will exclude all
--! ``.lua`` files located in ``impl/`` directory.
--!

EXCLUDE_LOCATION_PATTERN = nil
