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
--! \defgroup api-1-compounds
--! \ingroup api
--! \title Doxygen Compounds
--!
--! This section describes types (enumerations and tables) used to describe
--! Doxygen compounds (classes, namespaces, groups, etc).
--!
--! @{
--!

--------------------------------------------------------------------------------

--!
--! \luaenum
--!

CompoundKind =
{
	"class",
	"struct",
	"union",
	"interface",
	"protocol",
	"category",
	"exception",
	"service",
	"singleton",
	"file",
	"namespace",
	"group",
	"page",
	"example",
	"dir",
}

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

--!
--! \luastruct
--! \luabasetype NamespaceContents
--!
--! Table of this type describes a Doxygen *compound* such as *class*,
--! *namespace*, *group* etc.
--!

Compound =
{
	--! Holds a string describing compound type. Must be one of `CompoundKind`.

	compoundKind,

	--!
	--! Holds Doxygen identifier string, e.g.
	--! ``group__type_1gada12e3c9caedb0e18d384bfdc7e7def3``.
	--!
	--! This string can be used as a unique identifier for creating labels and
	--! reference links.
	--!

	id,

	--! Holds a string with the name of the compound.

	name,

	--!
	--! Holds a string with the title (if specified) or an empty string.
	--!
	--! Usually compound titles are not used unless this is a ``group``
	--! compound.
	--!

	title,
	importId,
	group,

	--!
	--! Holds a string with a forward-slash-separated path to the compound
	--! starting from the global namespace. For example, C++ class
	--! ``sys::win::WaitableTimer`` will have its path encoded as
	--! ``sys/win/WaitableTimer``.
	--!
	--! This field can be used to reconstruct a fully qualified name of the
	--! compound.
	--!

	path,
	importArray,

	--! Holds a `Description` table with the brief description of the compound.

	briefDescription,

	--! Holds a `Description` table with the detailed description of the
	--! compound.

	detailedDescription,

	--! Holds a `Location` table describing the place of declaration.

	location,
}

--!
--! \luastruct
--! \luabasetype Compound
--!

Page =
{
	subPageArray,
}

--!
--! \luastruct
--! \luabasetype Compound
--!

Class =
{
	--[[!
		Holds a table containing an array of all template parameters of the
		compound.

		Type of each element of the array is `Param`

		This field is only set when ``compoundKind`` is one of:

			| ``struct``
			| ``union``
			| ``class``
			| ``interface``
	]]

	templateParamArray,

	--[[!
		Holds a table containing an array of all template specialization
		parameters of the compound.

		Type of each element of the array is `Param`

		This field is only set when ``compoundKind`` is one of:

			| ``struct``
			| ``union``
			| ``class``
			| ``interface``
	]]

	templateSpecParamArray,
	baseTypeArray,
	baseTypeProtectionArray,
	derivedTypeArray,
}

--!
--! \luastruct
--!

NamespaceContents =
{
	--!
	--! Holds a table containing an array of all the groups withing the
	--! compound.
	--!
	--! Type of each element of the array is ``Compound``.
	--! `Compound.compoundKind` should be ``group``.
	--!

	groupArray,

	--!
	--! Holds a table containing an array of all the nested namespaces.
	--!
	--! Type of each element of the array is ``Compound``.
	--! `Compound.compoundKind` should be ``namespace``.
	--!

	namespaceArray,

	--!
	--! Holds a table containing an array of all the enums withing the compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``enum``.
	--!

	enumArray,

	--!
	--! Holds a table containing an array of all the nested structs within the
	--! compound.
	--!
	--! Type of each element of the array is ``Compound``.
	--! `Compound.compoundKind` should be ``struct``.
	--!

	structArray,

	--!
	--! Holds a table containing an array of all the nested unions within the
	--! compound.
	--!
	--! Type of each element of the array is ``Compound``.
	--! `Compound.compoundKind` should be ``union``.
	--!

	unionArray,

	--!
	--! Holds a table containing an array of all the nested classes within the
	--! compound.
	--!
	--! Type of each element of the array is ``Compound``.
	--! `Compound.compoundKind` should be ``class``.
	--!

	classArray,

	interfaceArray,
	protocolArray,
	exceptionArray,
	serviceArray,
	singletonArray,

	--!
	--! Holds a table containing an array of all the typedefs within the
	--! compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``typedef``.
	--!

	typedefArray,

	--![[!
	--! Holds a table containing an array of all the variables within the
	--! compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``variable``.
	--!

	variableArray,

	--!
	--! Holds a table containing an array of all the constructors of the type
	--! compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``function``.
	--!

	constructorArray,

	--!
	--! Holds a `Member` table describing the destructor of the type compound
	--! (if there is any) or ``nil``.
	--!
	--! `Member.memberKind` should be ``function``.
	--!

	destructor,

	--!
	--! Holds a table containing an array of all the functions/methods within
	--! the compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``function``.
	--!

	functionArray,

	--!
	--! Holds a table containing an array of all the properties within the
	--! compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``property``.
	--!

	propertyArray,

	--!
	--! Holds a table containing an array of all the events within the compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``event``.
	--!

	eventArray,

	--!
	--! Holds a table containing an array of all the aliases within the
	--! compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``alias``.
	--!

	aliasArray,

	--!
	--! Holds a table containing an array of all the macro definitions within
	--! the compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``define``.
	--!

	defineArray,

	--!
	--! Holds a table containing an array of all the footnotes defined within
	--! the compound.
	--!
	--! Type of each element of the array is `Member`. `Member.memberKind`
	--! should be ``footnote``.
	--!

	footnoteArray,
}

--!
--! \luastruct
--! \luabasetype NamespaceContents
--!

GlobalNamespace =
{
	path,
	id, --! always set to "global"
	compoundKind, --! always set to "namespace"
	briefDescription,
	detailedDescription,
}

--------------------------------------------------------------------------------

--! @}
