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
--! \defgroup api-3-docblocks
--! \ingroup api
--! \title Doxygen Documentation Blocks
--!
--! This section describes types (enumerations and tables) used to describe
--! Doxygen documentation blocks -- atoms of Doxygen documentation comments.
--!
--! @{
--!

--------------------------------------------------------------------------------

--!
--! \luaenum
--!

DocBlockKind =
{
	"computeroutput",
	"programlisting",
	"preformatted",
	"formula",
	"verbatim",
	"itemizedlist",
	"orderedlist",
	"variablelist",
	"linebreak",
	"ref",
	"anchor",
	"image",
	"bold",
	"emphasis",
	"sp",
	"varlistentry",
	"listitem",
	"para",
	"parametername",
	"parameterdescription",
	"sect1",
	"sect2",
	"sect3",
	"sect4",
	"simplesect",
	"ulink",
	"table",
}

--!
--! \luaenum
--!

ImageKind =
{
	"html",
	"latex",
	"rtf",
}

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

--!
--! \luastruct
--!
--! Table of this type describes a block which `Description` table consists of.
--!

DocBlock =
{
	--! Holds a string describing the type of the block. Must be one of
	--! `DocBlockKind`.

	blockKind,

	--! Holds a string with the title of the block (empty string for untitled
	--! blocks).

	title,

	--! Holds a string with the plain-text contents of the block.

	text,

	--!
	--! Holds an array table with all the child blocks of the current block.
	--!
	--! Type of each element of the array is `DocBlock`.
	--!

	childBlockList,
}

--!
--! \luastruct
--! \luabasetype DocBlock
--!

DocRefBlock =
{
	--! Holds a string describing hyperlink target. Must be one of `RefKind`.

	refKind,

	--! Holds a Doxygen identifier of the hyperlink target.

	id,
	external,
}

--!
--! \luastruct
--! \luabasetype DocBlock
--!

DocAnchorBlock =
{
	--! Holds a Doxygen identifier of the anchor.

	id,
}

--!
--! \luastruct
--! \luabasetype DocBlock
--!

DocImageBlock =
{
	imageKind, --!< one of `ImageKind`
	name,
	width,
	height,
}

--!
--! \luastruct
--! \luabasetype DocBlock
--!

DocUlinkBlock =
{
	url,
}

--!
--! \luastruct
--! \luabasetype DocBlock
--!

DocSectionBlock =
{
	--[[!
		Holds a Doxygen identifier.

		This field is only set if ``blockKind`` is one of:

			| ``ref``
			| ``section``
			| ``simplesec``

		If ``blockKind`` is  ``ref``, then ``id`` describes the identifier of
		the hyperlink target.

		For section blocks, ``id`` describes the identifier of the section
		itself.
	]]

	id,
}

--!
--! \luastruct
--! \luabasetype DocBlock
--!

DocSimpleSectionBlock =
{
	--[[!
		Holds the type of the simple section block. Must be one of:

			| ``<undefined>``
			| ``see``

		If ``simpleSectionKind`` is ``see``, then this is a **see-also** block.
	]]

	simpleSectionKind,
}

--------------------------------------------------------------------------------

--! @}
