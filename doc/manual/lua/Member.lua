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
--! \defgroup api-members
--! \ingroup api
--! \grouporder 3
--! \title Doxygen Members
--!
--! This section describes types (enumerations and tables) used to represent
--! Doxygen members (variables, functions, properties, etc).
--!
--! @{
--!

--------------------------------------------------------------------------------

--!
--! \luaenum
--!

MemberKind =
{
	"define",
	"property",
	"event",
	"variable",
	"typedef",
	"enum",
	"enumvalue",
	"function",
	"signal",
	"prototype",
	"friend",
	"dcop",
	"slot",
	"interface",
	"service",
	"alias",
	"footnote",
}

--!
--! \luaenum
--!

MemberFlag =
{
	"static",
	"const",
	"explicit",
	"inline",
	"volatile",
	"mutable",
	"readable",
	"writable",
	"initonly",
	"settable",
	"gettable",
	"final",
	"sealed",
	"new",
	"add",
	"remove",
	"raise",
	"optional",
	"required",
	"accessor",
	"attribute",
	"property",
	"readonly",
	"bound",
	"removable",
	"contrained",
	"transient",
	"maybevoid",
	"maybedefault",
	"maybeambiguous",
}

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

--!
--! \luastruct
--!
--! Table of this type describes a Doxygen *member* such as *variable*,
--! *function*, *typedef* etc.
--!

Member =
{
	--! Holds a string describing member type. Must be one of `MemberKind`.

	memberKind,

	--! Holds a string describing protection of the member. Must be one of
	--! `ProtectionKind`.

	protectionKind,

	--! Holds a string describing whether this member is virtual or not.
	--! Must be one of `VirtualKind`.

	virtualKind,

	--! An empty string or a space-delimited combination of `MemberFlag`
	--! values.

	flags,

	--!
	--! Holds Doxygen identifier string, e.g.
	--! ``structjnc___guid_1aec4f3165bbae5d72881b0d216e4ff15c``.
	--!
	--! This string can be used as a unique identifier for creating labels and
	--! reference links.
	--!

	id,

	--! Holds a string with the name of the member.

	name,

	--! Holds a string with space-separated list of language-specific modifiers
	--! applied to the member.

	modifiers,
	group,
	importArray,

	--!
	--! Holds a string with a forward-slash-separated path to the enum starting
	--! from the global namespace. For example, C++ enum
	--! ``io::NetworkAdapterType`` will have its path encoded as
	--! ``io/NetworkAdapterType``.
	--!
	--! This field can be used to reconstruct a fully qualified name of the
	--! enum.
	--!

	path,

	--! Holds a `Description` table with the brief description of the member.

	briefDescription,

	--! Holds a `Description` table with the detailed description of the member.

	detailedDescription,

	--! Holds a `Description` table with the in-body description of the member.

	inBodyDescription,

	--! Holds a `Location` table describing the place of declaration.

	location,
}

--!
--! \luastruct
--! \luabasetype Member
--!

Typedef =
{
	--! Holds a `LinkedText` table describing the target type of the typedef.

	type,

	--! Holds a string describing type suffix (function parameters, array
	--! dimensions, etc)

	argString,

	--!
	--! Holds an array table with detailed description of all parameters of the
	--! member.
	--!
	--! Type of each element of the array is `Param`.
	--!

	paramArray,
}

--!
--! \luastruct
--! \luabasetype Member
--!

Enum =
{
	--!
	--! Holds an array table with detailed description of all enum values of the
	--! enum.
	--!
	--! Type of each element of the array is `EnumValue`.
	--!
	--! This field is only set when ``memberKind`` is ``enum``.
	--!

	enumValueArray,
}


--!
--! \luastruct
--!
--! Table of this type describes Doxygen enum value.
--!

EnumValue =
{
	--!
	--! Holds a string describing protection of the enum value. Must be one of
	--! `ProtectionKind`.
	--!
	--! Usually it's just ``public``.
	--!

	protectionKind,

	--!
	--! Holds Doxygen identifier string, e.g.
	--! ``group__operators_1ggae611bfb523eb332069f2b7030accb66``.
	--!
	--! This string can be used as a unique identifier for creating labels and
	--! reference links.
	--!

	id,

	--! Holds a string with the name of the enum value.

	name,

	--! Holds a string containing member kind; for ``EnumValue always set to
	--! ``"enummember"``

	memberKind,

	path,

	--[[!
		Holds a `LinkedText` table with the manually defined value.

		.. rubric:: Sample:

		.. code-block:: cpp

			enum MyEnum
			{
				MyEnum_Value1,
				MyEnum_Value2 = 100,
			}

		``initializer`` for ``MyEnum_Value1`` will be empty
		(`LinkedText.isEmpty` will be set to ``true``).

		``initializer`` for ``MyEnum_Value2`` will contain ``= 100``.
	]]

	initializer,

	--! Holds a `Description` table with the brief description of the enum
	--! value.

	briefDescription,

	--! Holds a `Description` table with the detailed description of the enum
	--! value.

	detailedDescription,
}

--!
--! \luastruct
--! \luabasetype Member
--!

Variable =
{
	--! Holds a `LinkedText` table describing the type of the variable.

	type,

	--! Holds a string describing type suffix (function parameters, array
	--! dimensions, etc)

	argString,

	--! Holds a string with bit field suffix of the struct or union field.

	bitField,

	--!
	--! Holds an array table with detailed description of all parameters of the
	--! member.
	--!
	--! Type of each element of the array is `Param`.
	--!

	paramArray,

	--! Holds a `LinkedText` table with the member initializer.

	initializer,
}

--!
--! \luastruct
--! \luabasetype Member
--!

Function =
{
	--! Holds a `LinkedText` table describing the return type of the function.

	returnType,

	--! Holds a string describing type suffix (function parameters, array
	--! dimensions, etc)

	argString,

	--! Holds a `LinkedText` table describing which exception could be thrown
	--! by the function.

	exceptions,

	--! Holds a table containing an array of `Param` elements describing
	--! template parameters of the function.

	templateParamArray,

	--! Holds a table containing an array of `Param` elements describing
	--! template specialization parameters of the function.

	templateSpecParamArray,

	--!
	--! Holds an array table with detailed description of all parameters of the
	--! member.
	--!
	--! Type of each element of the array is `Param`.
	--!

	paramArray,
}

--!
--! \luastruct
--! \luabasetype Member
--!

Property =
{
	--! Holds a `LinkedText` table describing the return type of the property.

	returnType,

	--! Holds a string describing type suffix (function parameters, array
	--! dimensions, etc)

	argString,

	--!
	--! Holds an array table with detailed description of all parameters of the
	--! member.
	--!
	--! Type of each element of the array is `Param`.
	--!

	paramArray,
}

--!
--! \luastruct
--! \luabasetype Member
--!

Event =
{
	--! Holds a `LinkedText` table describing the type of the event.

	type,

	--! Holds a string describing type suffix (function parameters, array
	--! dimensions, etc)

	argString,

	--!
	--! Holds an array table with detailed description of all parameters of the
	--! member.
	--!
	--! Type of each element of the array is `Param`.
	--!

	paramArray,
}

--!
--! \luastruct
--! \luabasetype Member
--!

Alias =
{
	--! Holds a `LinkedText` table with the member initializer.

	initializer,
}

--!
--! \luastruct
--! \luabasetype Member
--!en

Define =
{
	--!
	--! Holds an array table with detailed description of all parameters of the
	--! member.
	--!
	--! Type of each element of the array is `Param`.
	--!

	paramArray,

	--! Holds a `LinkedText` table with the member initializer.

	initializer,
}

--------------------------------------------------------------------------------

--! @}
