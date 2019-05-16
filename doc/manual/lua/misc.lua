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
--! \defgroup api-aux-types
--! \ingroup api
--! \grouporder 4
--! \title Doxygen Auxillary Types
--!
--! This section describes auxillary types (enumerations and tables) used in
--! various places.
--!
--! @{
--!

--------------------------------------------------------------------------------

--!
--! \luaenum
--!

RefKind =
{
	"compound",
	"member",
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

--!
--! \luaenum
--!

ProtectionKind =
{
	"public",
	"protected",
	"private",
	"package",
}

--!
--! \luaenum
--!

VirtualKind =
{
	"non-virtual",
	"virtual",
	"pure-virtual",
	"abstract",
	"override",
}

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

--!
--! \luastruct
--!
--! Table of this type describes a block (either plain-text or hyperlink) of
--! `LinkedText`.
--!

RefText =
{
	--!
	--! Holds a string describing hyperlink target. Must be one of `RefKind`.
	--!
	--! If this block is not a hyperlink, ``refKind`` contains ``<undefined>``.
	--!

	refKind,

	--! Holds a string with plain-text representation of the block.

	text,

	--! Holds a Doxygen identifier of the hyperlink target. If this block is not
	--! a hyperlink ``id`` contains an empty string.

	id,
	external,
	tooltip,
}

--!
--! \luastruct
--!
--! Table of this type describes a text (possibly) with injected hyperlinks.
--!

LinkedText =
{
	--! Holds ``true`` if the text is empty or ``false`` otherwise.

	isEmpty,

	--! Holds a string with plain-text representation of the text, i.e. as if
	--! there were no any hyperlinks.

	plainText,

	--!
	--! Holds an array table of plain-text and hyperlink blocks constituting the full text.
	--!
	--! Type of each element of the array is `RefText`.
	--!

	refTextArray,
}

--!
--! \luastruct
--!
--! Table of this type describes a Doxygen *description* extracted from in-source documentation comments.
--!

Description =
{
	--! Holds ``true`` if description is empty or ``false`` otherwise.

	isEmpty,

	--!
	--! Holds an array table of blocks constituting this description.
	--!
	--! Type of each element of the array is `DocBlock`.
	--!

	docBlockList,
}

--!
--! \luastruct
--!

Location =
{
	file,
	line,
	column,
	bodyFile,
	bodyStartLine,
	bodyEndLine,
}

--!
--! \luastruct
--!
--! Table of this type describes a Doxygen *param* such as *function argument* or *template parameter*.
--!

Param =
{
	--! Holds a string describing a name given to the parameter during *declaration*.

	declarationName,

	--! Holds a string describing a name given to the parameter during *definition*.

	definitionName,

	--! Holds a string describing array dimension suffix of the parameter (if the parameter is of array type).

	array,

	--! Holds a `LinkedText` table describing the type of the parameter.

	type,

	--[[!
		Holds a `LinkedText` table with the default value assigned to the parameter.

		.. rubric:: Sample:

		.. code-block:: cpp

			int foo (int a, int b = 100);

		``defaultValue`` for ``a`` will be empty (`LinkedText.isEmpty` will be set to ``true``).

		``defaultValue`` for ``b`` will contain ``= 100``.
	]]

	defaultValue,

	typeConstraint,

	--! Holds a `Description` table with the brief description of the parameter.

	briefDescription,
}

--------------------------------------------------------------------------------

--! @}
