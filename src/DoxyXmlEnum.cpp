//..............................................................................
//
//  This file is part of the Doxyrest toolkit.
//
//  Doxyrest is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/doxyrest/license.txt
//
//..............................................................................

#include "pch.h"
#include "DoxyXmlEnum.h"

//..............................................................................

const char*
getBoolKindString(BoolKind boolKind) {
	const char* stringTable[] = {
		"<undefined>", // BoolKind_Undefined
		"yes",         // BoolKind_Yes
		"no",          // BoolKind_No
	};

	return (size_t)boolKind < countof(stringTable) ?
		stringTable[boolKind] :
		stringTable[0];
}

const char*
getLanguageKindString(LanguageKind languageKind) {
	const char* stringTable[] = {
		"<undefined>", // LanguageKind_Undefined
		"Unknown",     // LanguageKind_Unknown
		"IDL",         // LanguageKind_Idl
		"Java",        // LanguageKind_Java
		"C#",          // LanguageKind_CSharp
		"D",           // LanguageKind_D
		"PHP",         // LanguageKind_Php
		"Objective-C", // LanguageKind_ObjectiveC
		"C++",         // LanguageKind_Cpp
		"Javascript",  // LanguageKind_JavaScript
		"Python",      // LanguageKind_Python
		"Fortran",     // LanguageKind_Fortran
		"VHDL",        // LanguageKind_Vhdl
		"XML",         // LanguageKind_Xml
		"Tcl",         // LanguageKind_Tcl
		"Markdown",    // LanguageKind_Markdown
		"Lua",         // LanguageKind_Lua
		"Perl",        // LanguageKind_Perl
		"Jancy",       // LanguageKind_Jancy
	};

	return (size_t)languageKind < countof(stringTable) ?
		stringTable[languageKind] :
		stringTable[0];
}

const char*
getMemberKindString(MemberKind memberKind) {
	const char* stringTable[] = {
		"<undefined>", // MemberKind_Undefined
		"define",      // MemberKind_Define
		"property",    // MemberKind_Property
		"event",       // MemberKind_Event
		"variable",    // MemberKind_Variable
		"typedef",     // MemberKind_Typedef
		"enum",        // MemberKind_Enum
		"enumvalue",   // MemberKind_EnumValue
		"function",    // MemberKind_Function
		"signal",      // MemberKind_Signal
		"prototype",   // MemberKind_Prototype
		"friend",      // MemberKind_Friend
		"dcop",        // MemberKind_Dcop
		"slot",        // MemberKind_Slot
		"interface",   // MemberKind_Interface
		"service",     // MemberKind_Service
		"alias",       // MemberKind_Alias
		"footnote",    // MemberKind_Footnote
	};

	return (size_t)memberKind < countof(stringTable) ?
		stringTable[memberKind] :
		stringTable[0];
}

const char*
getProtectionKindString(ProtectionKind protectionKind) {
	const char* stringTable[] = {
		"<undefined>", // ProtectionKind_Undefined
		"public",      // ProtectionKind_Public
		"protected",   // ProtectionKind_Protected
		"private",     // ProtectionKind_Private
		"package",     // ProtectionKind_Package
	};

	return (size_t)protectionKind < countof(stringTable) ?
		stringTable[protectionKind] :
		stringTable[0];
}

const char*
getVirtualKindString(VirtualKind virtualKind) {
	const char* stringTable[] = {
		"<undefined>",  // VirtualKind_Undefined
		"non-virtual",  // VirtualKind_NonVirtual
		"virtual",      // VirtualKind_Virtual
		"pure-virtual", // VirtualKind_PureVirtual
		"abstract",     // VirtualKind_Abstract,
		"override",     // VirtualKind_Override,
	};

	return (size_t)virtualKind < countof(stringTable) ?
		stringTable[virtualKind] :
		stringTable[0];
}

const char*
getRefKindString(RefKind refKind) {
	const char* stringTable[] = {
		"<undefined>", // RefKind_Undefined
		"compound",    // RefKind_Compound
		"member",      // RefKind_Member
	};

	return (size_t)refKind < countof(stringTable) ?
		stringTable[refKind] :
		stringTable[0];
}

const char*
getGraphRelationKindString(GraphRelationKind graphRelationKind) {
	const char* stringTable[] = {
		"<undefined>",           // GraphRelationKind_Undefined
		"include",               // GraphRelationKind_Include
		"usage",                 // GraphRelationKind_Usage
		"template-instance",     // GraphRelationKind_TemplateInstance
		"public-inheritance",    // GraphRelationKind_PublicInheritance
		"protected-inheritance", // GraphRelationKind_ProtectedInheritance
		"private-inheritance",   // GraphRelationKind_PrivateInheritance
		"type-constraint",       // GraphRelationKind_TypeConstraint
	};

	return (size_t)graphRelationKind < countof(stringTable) ?
		stringTable[graphRelationKind] :
		stringTable[0];
}

const char*
getCompoundKindString(CompoundKind compoundKind) {
	const char* stringTable[] = {
		"<undefined>", // CompoundKind_Undefined
		"class",       // CompoundKind_Class
		"struct",      // CompoundKind_Struct
		"union",       // CompoundKind_Union
		"interface",   // CompoundKind_Interface
		"protocol",    // CompoundKind_Protocol
		"category",    // CompoundKind_Category
		"exception",   // CompoundKind_Exception
		"service",     // CompoundKind_Service
		"singleton",   // CompoundKind_Singleton
		"file",        // CompoundKind_File
		"namespace",   // CompoundKind_Namespace
		"group",       // CompoundKind_Group
		"page",        // CompoundKind_Page
		"example",     // CompoundKind_Example
		"dir",         // CompoundKind_Dir
	};

	return (size_t)compoundKind < countof(stringTable) ?
		stringTable[compoundKind] :
		stringTable[0];
}

const char*
getSectionKindString(SectionKind sectionKind) {
	const char* stringTable[] = {
		"<undefined>",             // SectionKind_Undefined
		"user-defined",            // SectionKind_UserDefined
		"public-type",             // SectionKind_PublicType
		"public-func",             // SectionKind_PublicFunc
		"public-attrib",           // SectionKind_PublicAttrib
		"public-slot",             // SectionKind_PublicSlot
		"signal",                  // SectionKind_Signal
		"dcop-func",               // SectionKind_DcopFunc
		"property",                // SectionKind_Property
		"event",                   // SectionKind_Event
		"public-static-func",      // SectionKind_PublicStaticFunc
		"public-static-attrib",    // SectionKind_PublicStaticAttrib
		"protected-type",          // SectionKind_ProtectedType
		"protected-func",          // SectionKind_ProtectedFunc
		"protected-attrib",        // SectionKind_ProtectedAttrib
		"protected-slot",          // SectionKind_ProtectedSlot
		"protected-static-func",   // SectionKind_ProtectedStaticFunc
		"protected-static-attrib", // SectionKind_ProtectedStaticAttrib
		"package-type",            // SectionKind_PackageType
		"package-func",            // SectionKind_PackageFunc
		"package-attrib",          // SectionKind_PackageAttrib
		"package-static-func",     // SectionKind_PackageStaticFunc
		"package-static-attrib",   // SectionKind_PackageStaticAttrib
		"private-type",            // SectionKind_PrivateType
		"private-func",            // SectionKind_PrivateFunc
		"private-attrib",          // SectionKind_PrivateAttrib
		"private-slot",            // SectionKind_PrivateSlot
		"private-static-func",     // SectionKind_PrivateStaticFunc
		"private-static-attrib",   // SectionKind_PrivateStaticAttrib
		"friend",                  // SectionKind_Friend
		"related",                 // SectionKind_Related
		"define",                  // SectionKind_Define
		"prototype",               // SectionKind_Prototype
		"typedef",                 // SectionKind_Typedef
		"enum",                    // SectionKind_Enum
		"func",                    // SectionKind_Func
		"var",                     // SectionKind_Var
	};

	return (size_t)sectionKind < countof(stringTable) ?
		stringTable[sectionKind] :
		stringTable[0];
}

const char*
getImageKindString(ImageKind imageKind) {
	const char* stringTable[] = {
		"<undefined>", // ImageKind_Undefined
		"html",        // ImageKind_Html
		"latex",       // ImageKind_Latex
		"rtf",         // ImageKind_Rtf
	};

	return (size_t)imageKind < countof(stringTable) ?
		stringTable[imageKind] :
		stringTable[0];
}

const char*
getParamListKindString(ParamListKind paramListKind) {
	const char* stringTable[] = {
		"<undefined>",   // ParamListKind_Undefined
		"param",         // ParamListKind_Param
		"retval",        // ParamListKind_RetVal
		"exception",     // ParamListKind_Exception
		"templateparam", // ParamListKind_TemplateParam
	};

	return (size_t)paramListKind < countof(stringTable) ?
		stringTable[paramListKind] :
		stringTable[0];
}

const char*
getParamDirKindString(ParamDirKind paramDirKind) {
	const char* stringTable[] = {
		"<undefined>", // ParamDirKind_Undefined
		"in",          // ParamDirKind_In
		"out",         // ParamDirKind_Out
		"inout",       // ParamDirKind_InOut
	};

	return (size_t)paramDirKind < countof(stringTable) ?
		stringTable[paramDirKind] :
		stringTable[0];
}

const char*
getAccessorKindString(AccessorKind accessorKind) {
	const char* stringTable[] = {
		"<undefined>", // AccessorKind_Undefined
		"retain",      // AccessorKind_Retain
		"copy",        // AccessorKind_Copy
		"assign",      // AccessorKind_Assign
		"weak",        // AccessorKind_Weak
		"strong",      // AccessorKind_Strong
		"unretained",  // AccessorKind_Unretained
	};

	return (size_t)accessorKind < countof(stringTable) ?
		stringTable[accessorKind] :
		stringTable[0];
}

//..............................................................................
