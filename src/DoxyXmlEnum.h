#pragma once

//.............................................................................

enum BoolKind
{
	BoolKind_Undefined = 0,
	BoolKind_Yes,
	BoolKind_No,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (BoolKindMap, BoolKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("yes", BoolKind_Yes)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("no",  BoolKind_No)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getBoolKindString (BoolKind boolKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum LanguageKind
{
	LanguageKind_Undefined = 0,
	LanguageKind_Unknown,
	LanguageKind_Idl,
	LanguageKind_Java,
	LanguageKind_CSharp,
	LanguageKind_D,
	LanguageKind_Php,
	LanguageKind_ObjectiveC,
	LanguageKind_Cpp,
	LanguageKind_JavaScript,
	LanguageKind_Python,
	LanguageKind_Fortran,
	LanguageKind_Vhdl,
	LanguageKind_Xml,
	LanguageKind_Tcl,
	LanguageKind_Markdown,
	LanguageKind_Jancy,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (LanguageKindMap, LanguageKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Unknown",     LanguageKind_Unknown)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("IDL",         LanguageKind_Idl)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Java",        LanguageKind_Java)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("C#",          LanguageKind_CSharp)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("D",           LanguageKind_D)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("PHP",         LanguageKind_Php)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Objective-C", LanguageKind_ObjectiveC)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("C++",         LanguageKind_Cpp)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Javascript",  LanguageKind_JavaScript)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Python",      LanguageKind_Python)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Fortran",     LanguageKind_Fortran)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("VHDL",        LanguageKind_Vhdl)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("XML",         LanguageKind_Xml)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Tcl",         LanguageKind_Tcl)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Markdown",    LanguageKind_Markdown)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("Jancy",       LanguageKind_Jancy)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getLanguageKindString (LanguageKind languageKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum MemberKind
{
	MemberKind_Undefined = 0,
	MemberKind_Define,
	MemberKind_Property,
	MemberKind_Event,
	MemberKind_Variable,
	MemberKind_Typedef,
	MemberKind_Enum,
	MemberKind_EnumValue,
	MemberKind_Function,
	MemberKind_Signal,
	MemberKind_Prototype,
	MemberKind_Friend,
	MemberKind_Dcop,
	MemberKind_Slot,
	MemberKind_Interface,
	MemberKind_Service,
	MemberKind_Alias,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (MemberKindMap, MemberKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("define",    MemberKind_Define)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("property",  MemberKind_Property)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("event",     MemberKind_Event)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("variable",  MemberKind_Variable)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("typedef",   MemberKind_Typedef)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("enum",      MemberKind_Enum)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("enumvalue", MemberKind_EnumValue)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("function",  MemberKind_Function)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("signal",    MemberKind_Signal)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("prototype", MemberKind_Prototype)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("friend",    MemberKind_Friend)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("dcop",      MemberKind_Dcop)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("slot",      MemberKind_Slot)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("interface", MemberKind_Interface)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("service",   MemberKind_Service)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("alias",     MemberKind_Alias)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getMemberKindString (MemberKind memberKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ProtectionKind
{
	ProtectionKind_Undefined = 0,
	ProtectionKind_Public,
	ProtectionKind_Protected,
	ProtectionKind_Private,
	ProtectionKind_Package,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (ProtectionKindMap, ProtectionKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public",    ProtectionKind_Public)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected", ProtectionKind_Protected)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private",   ProtectionKind_Private)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("package",   ProtectionKind_Package)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getProtectionKindString (ProtectionKind protectionKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum VirtualKind
{
	VirtualKind_Undefined = 0,
	VirtualKind_NonVirtual,
	VirtualKind_Virtual,
	VirtualKind_PureVirtual,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (VirtualKindMap, VirtualKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("non-virtual",  VirtualKind_NonVirtual)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("virtual",      VirtualKind_Virtual)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("pure-virtual", VirtualKind_PureVirtual)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getVirtualKindString (VirtualKind virtualKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum RefKind
{
	RefKind_Undefined = 0,
	RefKind_Compound,
	RefKind_Member,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (RefKindMap, RefKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("compound", RefKind_Compound)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("member",   RefKind_Member)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getRefKindString (RefKind refKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum GraphRelationKind
{
	GraphRelationKind_Undefined = 0,
	GraphRelationKind_Include,
	GraphRelationKind_Usage,
	GraphRelationKind_TemplateInstance,
	GraphRelationKind_PublicInheritance,
	GraphRelationKind_ProtectedInheritance,
	GraphRelationKind_PrivateInheritance,
	GraphRelationKind_TypeConstraint,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (GraphRelationKindMap, GraphRelationKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("include",               GraphRelationKind_Include)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("usage",                 GraphRelationKind_Usage)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("template-instance",     GraphRelationKind_TemplateInstance)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public-inheritance",    GraphRelationKind_PublicInheritance)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected-inheritance", GraphRelationKind_ProtectedInheritance)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private-inheritance",   GraphRelationKind_PrivateInheritance)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("type-constraint",       GraphRelationKind_TypeConstraint)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getGraphRelationKindString (GraphRelationKind graphRelationKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum CompoundKind
{
	CompoundKind_Undefined = 0,
	CompoundKind_Class,
	CompoundKind_Struct,
	CompoundKind_Union,
	CompoundKind_Interface,
	CompoundKind_Protocol,
	CompoundKind_Category,
	CompoundKind_Exception,
	CompoundKind_File,
	CompoundKind_Namespace,
	CompoundKind_DoxyGroup, // this is compound as described in doxy xml
	CompoundKind_Group, // this is actual group -- intersection of a doxy group and a namespace
	CompoundKind_Page,
	CompoundKind_Example,
	CompoundKind_Dir,	
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (CompoundKindMap, CompoundKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("class",     CompoundKind_Class)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("struct",    CompoundKind_Struct)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("union",     CompoundKind_Union)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("interface", CompoundKind_Interface)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protocol",  CompoundKind_Protocol)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("category",  CompoundKind_Category)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("exception", CompoundKind_Exception)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("file",      CompoundKind_File)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("namespace", CompoundKind_Namespace)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("group",     CompoundKind_DoxyGroup)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("page",      CompoundKind_Page)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("example",   CompoundKind_Example)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("dir",       CompoundKind_Dir)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getCompoundKindString (CompoundKind compoundKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SectionKind
{
	SectionKind_Undefined = 0,
	SectionKind_UserDefined,
	SectionKind_PublicType,
	SectionKind_PublicFunc,
	SectionKind_PublicAttrib,
	SectionKind_PublicSlot,
	SectionKind_Signal,
	SectionKind_DcopFunc,
	SectionKind_Property,
	SectionKind_Event,
	SectionKind_PublicStaticFunc,
	SectionKind_PublicStaticAttrib,
	SectionKind_ProtectedType,
	SectionKind_ProtectedFunc,
	SectionKind_ProtectedAttrib,
	SectionKind_ProtectedSlot,
	SectionKind_ProtectedStaticFunc,
	SectionKind_ProtectedStaticAttrib,
	SectionKind_PackageType,
	SectionKind_PackageFunc,
	SectionKind_PackageAttrib,
	SectionKind_PackageStaticFunc,
	SectionKind_PackageStaticAttrib,
	SectionKind_PrivateType,
	SectionKind_PrivateFunc,
	SectionKind_PrivateAttrib,
	SectionKind_PrivateSlot,
	SectionKind_PrivateStaticFunc,
	SectionKind_PrivateStaticAttrib,
	SectionKind_Friend,
	SectionKind_Related,
	SectionKind_Define,
	SectionKind_Prototype,
	SectionKind_Typedef,
	SectionKind_Enum,
	SectionKind_Func,
	SectionKind_Var,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (SectionKindMap, SectionKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("user-defined",            SectionKind_UserDefined)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public-type",             SectionKind_PublicType)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public-func",             SectionKind_PublicFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public-attrib",           SectionKind_PublicAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public-slot",             SectionKind_PublicSlot)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("signal",                  SectionKind_Signal)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("dcop-func",               SectionKind_DcopFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("property",                SectionKind_Property)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("event",                   SectionKind_Event)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public-static-func",      SectionKind_PublicStaticFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("public-static-attrib",    SectionKind_PublicStaticAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected-type",          SectionKind_ProtectedType)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected-func",          SectionKind_ProtectedFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected-attrib",        SectionKind_ProtectedAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected-slot",          SectionKind_ProtectedSlot)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected-static-func",   SectionKind_ProtectedStaticFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("protected-static-attrib", SectionKind_ProtectedStaticAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("package-type",            SectionKind_PackageType)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("package-func",            SectionKind_PackageFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("package-attrib",          SectionKind_PackageAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("package-static-func",     SectionKind_PackageStaticFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("package-static-attrib",   SectionKind_PackageStaticAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private-type",            SectionKind_PrivateType)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private-func",            SectionKind_PrivateFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private-attrib",          SectionKind_PrivateAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private-slot",            SectionKind_PrivateSlot)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private-static-func",     SectionKind_PrivateStaticFunc)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("private-static-attrib",   SectionKind_PrivateStaticAttrib)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("friend",                  SectionKind_Friend)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("related",                 SectionKind_Related)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("define",                  SectionKind_Define)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("prototype",               SectionKind_Prototype)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("typedef",                 SectionKind_Typedef)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("enum",                    SectionKind_Enum)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("func",                    SectionKind_Func)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("var",                     SectionKind_Var)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getSectionKindString (SectionKind sectionKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ImageKind
{
	ImageKind_Undefined = 0,
	ImageKind_Html,
	ImageKind_Latex,
	ImageKind_Rtf,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (ImageKindMap, ImageKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("html",  ImageKind_Html)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("latex", ImageKind_Latex)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("rtf",   ImageKind_Rtf)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getImageKindString (ImageKind imageKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ParamListKind
{
	ParamListKind_Undefined = 0,
	ParamListKind_Param,
	ParamListKind_RetVal,
	ParamListKind_Exception,
	ParamListKind_TemplateParam,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (ParamListKindMap, ParamListKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("param",         ParamListKind_Param)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("retval",        ParamListKind_RetVal)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("exception",     ParamListKind_Exception)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("templateparam", ParamListKind_TemplateParam)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getParamListKindString (ParamListKind paramListKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ParamDirKind
{
	ParamDirKind_Undefined = 0,
	ParamDirKind_In,
	ParamDirKind_Out,
	ParamDirKind_InOut,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (ParamDirKindMap, ParamDirKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("in",    ParamDirKind_In)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("out",   ParamDirKind_Out)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("inout", ParamDirKind_InOut)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getParamDirKindString (ParamDirKind paramDirKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum AccessorKind
{
	AccessorKind_Undefined = 0,
	AccessorKind_Retain,
	AccessorKind_Copy,
	AccessorKind_Assign,
	AccessorKind_Weak,
	AccessorKind_Strong,
	AccessorKind_Unretained,
};

AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (AccessorKindMap, AccessorKind)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("retain",     AccessorKind_Retain)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("copy",       AccessorKind_Copy)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("assign",     AccessorKind_Assign)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("weak",       AccessorKind_Weak)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("strong",     AccessorKind_Strong)
	AXL_SL_HASH_TABLE_MAP_ENTRY ("unretained", AccessorKind_Unretained)
AXL_SL_END_HASH_TABLE_MAP ()

const char*
getAccessorKindString (AccessorKind accessorKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
