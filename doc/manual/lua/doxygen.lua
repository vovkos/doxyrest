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
};

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
};

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
};

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
--!

RefText =
{
	refKind, --! one of `RefKind`
	text,
	id,
	external,
	tooltip,
}

--!
--! \luastruct
--!

LinkedText =
{
	isEmpty,
	plainText,
	refTextArray, --! list of `RefText`
}

--!
--! \luastruct
--!

DocBlock =
{
	blockKind,
	title,
	text,
	childBlockList, --! list of `DocBlock`
}

--!
--! \luastruct
--! \internal :basetype:DocBlock
--!

DocRefBlock =
{
	refKind, --! one of `RefKind`
	id,
	external,
}

--!
--! \luastruct
--! \internal :basetype:DocBlock
--!

DocAnchorBlock =
{
	id,
}

--!
--! \luastruct
--! \internal :basetype:DocBlock
--!

DocImageBlock =
{
	imageKind, --! one of `ImageKind`
	name,
	width,
	height,
}

--!
--! \luastruct
--! \internal :basetype:DocBlock
--!

DocUlinkBlock =
{
	url,
}

--!
--! \luastruct
--! \internal :basetype:DocBlock
--!

DocSectionBlock =
{
	id,
}

--!
--! \luastruct
--! \internal :basetype:DocBlock
--!

DocSimpleSectionBlock =
{
	simpleSectionKind,
}

--!
--! \luastruct
--!

Description =
{
	isEmpty,
	docBlockList, --! list of `DocBlock`
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

Param =
{
	declarationName,
	definitionName,
	array,
	type,
	defaultValue,
	typeConstraint,
	briefDescription,
}

--!
--! \luastruct
--!

EnumValue =
{
	protectionKind, --! one of `ProtectionKind`
	id,
	name,
	memberKind, --! always set to ``"enummember"``
	path,
	initializer,
	briefDescription,
	detailedDescription,
}

--!
--! \luastruct
--!

Member =
{
	memberKind, --! one of `MemberKind`
	protectionKind, --! one of `ProtectionKind`
	virtualKind, --! one of `VirtualKind`
	flags,
	id,
	name,
	modifiers,
	group,
	importArray,
	path,
	briefDescription,
	detailedDescription,
	inBodyDescription,
	location,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Typedef =
{
	type,
	argString,
	paramArray,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Enum =
{
	enumValueArray,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Variable =
{
	type,
	argString,
	bitField,
	paramArray,
	initializer,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Function =
{
	returnType,
	argString,
	exceptions,
	templateParamArray,
	templateSpecParamArray,
	paramArray,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Property =
{
	returnType,
	argString,
	paramArray,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Event =
{
	type,
	argString,
	paramArray,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Alias =
{
	initializer,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Define =
{
	paramArray,
	initializer,
}

--!
--! \luastruct
--! \internal :basetype:NamespaceContents
--!

Compound =
{
	compoundKind, --! one of `CompoundKind`
	id,
	name,
	title,
	importId,
	groupId,
	path,
	importArray,
	briefDescription,
	detailedDescription,
	location,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Page =
{
	subPageArray,
}

--!
--! \luastruct
--! \internal :basetype:Member
--!

Class =
{
	templateParamArray,
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
	groupArray,
	namespaceArray,
	enumArray,
	structArray,
	unionArray,
	classArray,
	interfaceArray,
	protocolArray,
	exceptionArray,
	serviceArray,
	singletonArray,
	typedefArray,
	variableArray,
	constructorArray,
	destructor,
	functionArray,
	propertyArray,
	eventArray,
	aliasArray,
	defineArray,
	footnoteArray,
}

--!
--! \luastruct
--! \internal :basetype:NamespaceContents
--!

GlobalNamespace =
{
	path,
	id, --! always set to "global"
	compoundKind, --! always set to "namespace"
	briefDescription,
	detailedDescription,
}

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

--!
--! Global namespace table of type `GlobalNamespace`
--!

g_globalNamespace = {}

--!
--! Array of group `Compound`-s
--!

g_groupArray = {}

--!
--! Array of `Page`-s
--!

g_pageArray = {}

--!
--! Array of example `Compounds`-s
--!

g_pageArray = {}

--!
--! Holds the directory of the frame file which is being currently processed.
--!

g_frameDir = ""

--!
--! Holds the directory of the target file which is being currently written to.
--!

g_targetDir = ""

--!
--! Holds the file name of the target file which is being currently written to.
--!

g_targetFileName = ""

--------------------------------------------------------------------------------
