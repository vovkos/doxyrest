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

#pragma once

#include "Module.h"
#include "DoxyXmlEnum.h"

class DoxyXmlParser;

//..............................................................................

class DoxyXmlType
{
	friend class DoxyXmlParser;

protected:
	DoxyXmlParser* m_parser;

public:
	DoxyXmlType()
	{
		m_parser = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		const char* name,
		const char** attributes
		)
	{
		m_parser = parser;
		return true;
	}

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		)
	{
		return true;
	}

	virtual
	bool
	onEndElement(const char* name)
	{
		return true;
	}

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		return true;
	}

	virtual
	void
	onPopType()
	{
	}
};

//..............................................................................

class StringType: public DoxyXmlType
{
protected:
	sl::String* m_string;

public:
	StringType()
	{
		m_string = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::String* string,
		const char* name,
		const char** attributes
		)
	{
		m_string = string;
		return true;
	}

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_string->append(string, length);
		return true;
	}
};

//..............................................................................

class DoxygenIndexType: public DoxyXmlType
{
protected:
	enum IndexElemKind
	{
		IndexElemKind_Undefined = 0,
		IndexElemKind_Compound,
	};

	enum IndexAttrKind
	{
		IndexAttrKind_Undefined = 0,
		IndexAttrKind_Version,
	};

	enum CompoundAttrKind
	{
		CompoundAttrKind_Undefined = 0,
		CompoundAttrKind_RefId,
		CompoundAttrKind_Kind,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(IndexElemKindMap, IndexElemKind)
		AXL_SL_HASH_TABLE_ENTRY("compound", IndexElemKind_Compound)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(IndexAttrKindMap, IndexAttrKind)
		AXL_SL_HASH_TABLE_ENTRY("version", IndexAttrKind_Version)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(CompoundAttrKindMap, CompoundAttrKind)
		AXL_SL_HASH_TABLE_ENTRY("refid", CompoundAttrKind_RefId)
		AXL_SL_HASH_TABLE_ENTRY("kind",  CompoundAttrKind_Kind)
	AXL_SL_END_HASH_TABLE()

public:
	bool
	create(
		DoxyXmlParser* parser,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);

protected:
	bool
	onCompound(
		const char* name,
		const char** attributes
		);

	bool
	parseCompound(const char* refId);
};

//..............................................................................

class DoxygenCompoundType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined = 0,
		ElemKind_CompoundDef,
	};

	enum AttrKind
	{
		AttrKind_Undefined = 0,
		AttrKind_Version,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("compounddef", ElemKind_CompoundDef)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("version", AttrKind_Version)
	AXL_SL_END_HASH_TABLE()

public:
	bool
	create(
		DoxyXmlParser* parser,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class CompoundDefType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined = 0,
		ElemKind_CompoundName,
		ElemKind_Title,
		ElemKind_BaseCompoundRef,
		ElemKind_DerivedCompoundRef,
		ElemKind_Includes,
		ElemKind_IncludedBy,
		ElemKind_IncDepGraph,
		ElemKind_InvIncDepGraph,
		ElemKind_InnerDir,
		ElemKind_InnerFile,
		ElemKind_InnerClass,
		ElemKind_InnerNamespace,
		ElemKind_InnerPage,
		ElemKind_InnerGroup,
		ElemKind_TemplateParamList,
		ElemKind_SectionDef,
		ElemKind_BriefDescription,
		ElemKind_DetailedDescription,
		ElemKind_InheritanceGraph,
		ElemKind_CollaborationGraph,
		ElemKind_ProgramListing,
		ElemKind_Location,
		ElemKind_ListOfAllMembers,
	};

	enum AttrKind
	{
		AttrKind_Undefined = 0,
		AttrKind_Id,
		AttrKind_Kind,
		AttrKind_Language,
		AttrKind_Prot,
		AttrKind_Final,
		AttrKind_Sealed,
		AttrKind_Abstract,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("compoundname",        ElemKind_CompoundName)
		AXL_SL_HASH_TABLE_ENTRY("title",               ElemKind_Title)
		AXL_SL_HASH_TABLE_ENTRY("basecompoundref",     ElemKind_BaseCompoundRef)
		AXL_SL_HASH_TABLE_ENTRY("derivedcompoundref",  ElemKind_DerivedCompoundRef)
		AXL_SL_HASH_TABLE_ENTRY("includes",            ElemKind_Includes)
		AXL_SL_HASH_TABLE_ENTRY("includedby",          ElemKind_IncludedBy)
		AXL_SL_HASH_TABLE_ENTRY("incdepgraph",         ElemKind_IncDepGraph)
		AXL_SL_HASH_TABLE_ENTRY("invincdepgraph",      ElemKind_InvIncDepGraph)
		AXL_SL_HASH_TABLE_ENTRY("innerdir",            ElemKind_InnerDir)
		AXL_SL_HASH_TABLE_ENTRY("innerdile",           ElemKind_InnerFile)
		AXL_SL_HASH_TABLE_ENTRY("innerclass",          ElemKind_InnerClass)
		AXL_SL_HASH_TABLE_ENTRY("innernamespace",      ElemKind_InnerNamespace)
		AXL_SL_HASH_TABLE_ENTRY("innerpage",           ElemKind_InnerPage)
		AXL_SL_HASH_TABLE_ENTRY("innergroup",          ElemKind_InnerGroup)
		AXL_SL_HASH_TABLE_ENTRY("templateparamlist",   ElemKind_TemplateParamList)
		AXL_SL_HASH_TABLE_ENTRY("sectiondef",          ElemKind_SectionDef)
		AXL_SL_HASH_TABLE_ENTRY("briefdescription",    ElemKind_BriefDescription)
		AXL_SL_HASH_TABLE_ENTRY("detaileddescription", ElemKind_DetailedDescription)
		AXL_SL_HASH_TABLE_ENTRY("inheritancegraph",    ElemKind_InheritanceGraph)
		AXL_SL_HASH_TABLE_ENTRY("collaborationgraph",  ElemKind_CollaborationGraph)
		AXL_SL_HASH_TABLE_ENTRY("programlisting",      ElemKind_ProgramListing)
		AXL_SL_HASH_TABLE_ENTRY("location",            ElemKind_Location)
		AXL_SL_HASH_TABLE_ENTRY("listofallmembers",    ElemKind_ListOfAllMembers)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("id",       AttrKind_Id)
		AXL_SL_HASH_TABLE_ENTRY("kind",     AttrKind_Kind)
		AXL_SL_HASH_TABLE_ENTRY("language", AttrKind_Language)
		AXL_SL_HASH_TABLE_ENTRY("prot",     AttrKind_Prot)
		AXL_SL_HASH_TABLE_ENTRY("final",    AttrKind_Final)
		AXL_SL_HASH_TABLE_ENTRY("sealed",   AttrKind_Sealed)
		AXL_SL_HASH_TABLE_ENTRY("abstract", AttrKind_Abstract)
	AXL_SL_END_HASH_TABLE()

protected:
	Compound* m_compound;

public:
	CompoundDefType()
	{
		m_compound = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);

	virtual
	void
	onPopType();
};

//..............................................................................

class ListOfAllMembersType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined = 0,
		ElemKind_Member,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("member", ElemKind_Member)
	AXL_SL_END_HASH_TABLE()

public:
};

//..............................................................................

class MemberRefType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined = 0,
		ElemKind_Scope,
		ElemKind_Name,
	};

	enum AttrKind
	{
		AttrKind_Undefined = 0,
		AttrKind_RefId,
		AttrKind_Prot,
		AttrKind_Virt,
		AttrKind_AmbiguityScope,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("scope", ElemKind_Scope)
		AXL_SL_HASH_TABLE_ENTRY("name",  ElemKind_Name)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("refid", AttrKind_RefId)
		AXL_SL_HASH_TABLE_ENTRY("prot",  AttrKind_Prot)
		AXL_SL_HASH_TABLE_ENTRY("virt",  AttrKind_Virt)
		AXL_SL_HASH_TABLE_ENTRY("ambiguityscope", AttrKind_AmbiguityScope)
	AXL_SL_END_HASH_TABLE()

public:
};

//..............................................................................

class CompoundRefType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined = 0,
		AttrKind_RefId,
		AttrKind_Prot,
		AttrKind_Virt,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("refid", AttrKind_RefId)
		AXL_SL_HASH_TABLE_ENTRY("prot",  AttrKind_Prot)
		AXL_SL_HASH_TABLE_ENTRY("virt",  AttrKind_Virt)
	AXL_SL_END_HASH_TABLE()

public:
};

//..............................................................................

// ReimplementType

enum ReimplementAttr
{
	ReimplementAttrKind_RefId,
};

// IncType

enum IncAttr
{
	IncAttrKind_RefId,
	IncAttrKind_Local,
};

//..............................................................................

class RefType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_RefId,
		AttrKind_ImportId,
		AttrKind_Prot,
		AttrKind_Virt,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("refid",    AttrKind_RefId)
		AXL_SL_HASH_TABLE_ENTRY("importid", AttrKind_ImportId)
		AXL_SL_HASH_TABLE_ENTRY("prot",     AttrKind_Prot)
		AXL_SL_HASH_TABLE_ENTRY("virt",     AttrKind_Virt)
	AXL_SL_END_HASH_TABLE()

protected:
	Ref* m_ref;

public:
	RefType()
	{
		m_ref = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<Ref>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_ref->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class SectionDefType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Header,
		ElemKind_Description,
		ElemKind_MemberDef,
	};

	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Kind,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("header",      ElemKind_Header)
		AXL_SL_HASH_TABLE_ENTRY("description", ElemKind_Description)
		AXL_SL_HASH_TABLE_ENTRY("memberdef",   ElemKind_MemberDef)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("kind", AttrKind_Kind)
	AXL_SL_END_HASH_TABLE()

protected:
	Compound* m_parent;

public:
	SectionDefType()
	{
		m_parent = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		Compound* parent,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class MemberDefType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_TemplateParamList,
		ElemKind_Type,
		ElemKind_Definition,
		ElemKind_ArgString,
		ElemKind_Name,
		ElemKind_Read,
		ElemKind_Write,
		ElemKind_BitField,
		ElemKind_Reimplements,
		ElemKind_ReimplementedBy,
		ElemKind_Param,
		ElemKind_EnumValue,
		ElemKind_Initializer,
		ElemKind_Exceptions,
		ElemKind_BriefDescription,
		ElemKind_DetailedDescription,
		ElemKind_InBodyDescription,
		ElemKind_Location,
		ElemKind_References,
		ElemKind_ReferencedBy,
		ElemKind_Modifiers,
		ElemKind_Includes,
	};

	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Kind,
		AttrKind_Id,
		AttrKind_Prot,
		AttrKind_Static,
		AttrKind_Const,
		AttrKind_Explicit,
		AttrKind_Inline,
		AttrKind_Virtual,
		AttrKind_Volatile,
		AttrKind_Mutable,
		AttrKind_Readable,
		AttrKind_Writeable,
		AttrKind_InitOnly,
		AttrKind_Settable,
		AttrKind_Gettable,
		AttrKind_Final,
		AttrKind_Sealed,
		AttrKind_New,
		AttrKind_Add,
		AttrKind_Remove,
		AttrKind_Raise,
		AttrKind_Optional,
		AttrKind_Required,
		AttrKind_Accessor,
		AttrKind_Attribute,
		AttrKind_Property,
		AttrKind_ReadOnly,
		AttrKind_Bound,
		AttrKind_Removable,
		AttrKind_Contrained,
		AttrKind_Transient,
		AttrKind_MaybeVoid,
		AttrKind_MaybeDefault,
		AttrKind_MaybeAmbiguos,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("templateparamlist",    ElemKind_TemplateParamList)
		AXL_SL_HASH_TABLE_ENTRY("type",                 ElemKind_Type)
		AXL_SL_HASH_TABLE_ENTRY("definition",           ElemKind_Definition)
		AXL_SL_HASH_TABLE_ENTRY("argsstring",           ElemKind_ArgString)
		AXL_SL_HASH_TABLE_ENTRY("name",                 ElemKind_Name)
		AXL_SL_HASH_TABLE_ENTRY("read",                 ElemKind_Read)
		AXL_SL_HASH_TABLE_ENTRY("write",                ElemKind_Write)
		AXL_SL_HASH_TABLE_ENTRY("bitfield",             ElemKind_BitField)
		AXL_SL_HASH_TABLE_ENTRY("reimplements",         ElemKind_Reimplements)
		AXL_SL_HASH_TABLE_ENTRY("reimplementedby",      ElemKind_ReimplementedBy)
		AXL_SL_HASH_TABLE_ENTRY("param",                ElemKind_Param)
		AXL_SL_HASH_TABLE_ENTRY("enumvalue",            ElemKind_EnumValue)
		AXL_SL_HASH_TABLE_ENTRY("initializer",          ElemKind_Initializer)
		AXL_SL_HASH_TABLE_ENTRY("exceptions",           ElemKind_Exceptions)
		AXL_SL_HASH_TABLE_ENTRY("briefdescription",     ElemKind_BriefDescription)
		AXL_SL_HASH_TABLE_ENTRY("detaileddescription",  ElemKind_DetailedDescription)
		AXL_SL_HASH_TABLE_ENTRY("inbodydescription",    ElemKind_InBodyDescription)
		AXL_SL_HASH_TABLE_ENTRY("location",             ElemKind_Location)
		AXL_SL_HASH_TABLE_ENTRY("references",           ElemKind_References)
		AXL_SL_HASH_TABLE_ENTRY("referencedby",         ElemKind_ReferencedBy)
		AXL_SL_HASH_TABLE_ENTRY("modifiers",            ElemKind_Modifiers)
		AXL_SL_HASH_TABLE_ENTRY("includes",             ElemKind_Includes)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("kind",           AttrKind_Kind)
		AXL_SL_HASH_TABLE_ENTRY("id",             AttrKind_Id)
		AXL_SL_HASH_TABLE_ENTRY("prot",           AttrKind_Prot)
		AXL_SL_HASH_TABLE_ENTRY("static",         AttrKind_Static)
		AXL_SL_HASH_TABLE_ENTRY("const",          AttrKind_Const)
		AXL_SL_HASH_TABLE_ENTRY("explicit",       AttrKind_Explicit)
		AXL_SL_HASH_TABLE_ENTRY("inline",         AttrKind_Inline)
		AXL_SL_HASH_TABLE_ENTRY("virt",           AttrKind_Virtual)
		AXL_SL_HASH_TABLE_ENTRY("volatile",       AttrKind_Volatile)
		AXL_SL_HASH_TABLE_ENTRY("mutable",        AttrKind_Mutable)
		AXL_SL_HASH_TABLE_ENTRY("readable",       AttrKind_Readable)
		AXL_SL_HASH_TABLE_ENTRY("writable",       AttrKind_Writeable)
		AXL_SL_HASH_TABLE_ENTRY("initonly",       AttrKind_InitOnly)
		AXL_SL_HASH_TABLE_ENTRY("settable",       AttrKind_Settable)
		AXL_SL_HASH_TABLE_ENTRY("gettable",       AttrKind_Gettable)
		AXL_SL_HASH_TABLE_ENTRY("final",          AttrKind_Final)
		AXL_SL_HASH_TABLE_ENTRY("sealed",         AttrKind_Sealed)
		AXL_SL_HASH_TABLE_ENTRY("new",            AttrKind_New)
		AXL_SL_HASH_TABLE_ENTRY("add",            AttrKind_Add)
		AXL_SL_HASH_TABLE_ENTRY("remove",         AttrKind_Remove)
		AXL_SL_HASH_TABLE_ENTRY("raise",          AttrKind_Raise)
		AXL_SL_HASH_TABLE_ENTRY("optional",       AttrKind_Optional)
		AXL_SL_HASH_TABLE_ENTRY("required",       AttrKind_Required)
		AXL_SL_HASH_TABLE_ENTRY("accessor",       AttrKind_Accessor)
		AXL_SL_HASH_TABLE_ENTRY("attribute",      AttrKind_Attribute)
		AXL_SL_HASH_TABLE_ENTRY("property",       AttrKind_Property)
		AXL_SL_HASH_TABLE_ENTRY("readonly",       AttrKind_ReadOnly)
		AXL_SL_HASH_TABLE_ENTRY("bound",          AttrKind_Bound)
		AXL_SL_HASH_TABLE_ENTRY("removable",      AttrKind_Removable)
		AXL_SL_HASH_TABLE_ENTRY("contrained",     AttrKind_Contrained)
		AXL_SL_HASH_TABLE_ENTRY("transient",      AttrKind_Transient)
		AXL_SL_HASH_TABLE_ENTRY("maybevoid",      AttrKind_MaybeVoid)
		AXL_SL_HASH_TABLE_ENTRY("maybedefault",   AttrKind_MaybeDefault)
		AXL_SL_HASH_TABLE_ENTRY("maybeambiguous", AttrKind_MaybeAmbiguos)
	AXL_SL_END_HASH_TABLE()

protected:
	Member* m_member;

public:
	MemberDefType()
	{
		m_member = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		Compound* parent,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class LocationType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_File,
		AttrKind_Line,
		AttrKind_Column,
		AttrKind_BodyFile,
		AttrKind_BodyStart,
		AttrKind_BodyEnd,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("file",      AttrKind_File)
		AXL_SL_HASH_TABLE_ENTRY("line",      AttrKind_Line)
		AXL_SL_HASH_TABLE_ENTRY("column",    AttrKind_Column)
		AXL_SL_HASH_TABLE_ENTRY("bodyfile",  AttrKind_BodyFile)
		AXL_SL_HASH_TABLE_ENTRY("bodystart", AttrKind_BodyStart)
		AXL_SL_HASH_TABLE_ENTRY("bodyend",   AttrKind_BodyEnd)
	AXL_SL_END_HASH_TABLE()

public:
	bool
	create(
		DoxyXmlParser* parser,
		Location* location,
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class DescriptionType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Title,
		ElemKind_Para,
		ElemKind_Sect1,
		ElemKind_Internal,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("title",    ElemKind_Title)
		AXL_SL_HASH_TABLE_ENTRY("para",     ElemKind_Para)
		AXL_SL_HASH_TABLE_ENTRY("sect1",    ElemKind_Sect1)
		AXL_SL_HASH_TABLE_ENTRY("internal", ElemKind_Internal)
	AXL_SL_END_HASH_TABLE()

protected:
	Description* m_description;

public:
	bool
	create(
		DoxyXmlParser* parser,
		Description* description,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class DocSectionBlockType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Title,
		ElemKind_Para,
		ElemKind_Sect1,
		ElemKind_Sect2,
		ElemKind_Sect3,
		ElemKind_Sect4,
		ElemKind_Internal,
	};

	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Id,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("title",    ElemKind_Title)
		AXL_SL_HASH_TABLE_ENTRY("para",     ElemKind_Para)
		AXL_SL_HASH_TABLE_ENTRY("sect1",    ElemKind_Sect1)
		AXL_SL_HASH_TABLE_ENTRY("sect2",    ElemKind_Sect2)
		AXL_SL_HASH_TABLE_ENTRY("sect3",    ElemKind_Sect3)
		AXL_SL_HASH_TABLE_ENTRY("sect4",    ElemKind_Sect4)
		AXL_SL_HASH_TABLE_ENTRY("internal", ElemKind_Internal)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("id", AttrKind_Id)
	AXL_SL_END_HASH_TABLE()

protected:
	DocSectionBlock* m_sectionBlock;

public:
	bool
	create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class EnumValueType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Name,
		ElemKind_Initializer,
		ElemKind_BriefDescription,
		ElemKind_DetailedDescription,
	};

	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Id,
		AttrKind_Prot,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("name",                ElemKind_Name)
		AXL_SL_HASH_TABLE_ENTRY("initializer",         ElemKind_Initializer)
		AXL_SL_HASH_TABLE_ENTRY("briefdescription",    ElemKind_BriefDescription)
		AXL_SL_HASH_TABLE_ENTRY("detaileddescription", ElemKind_DetailedDescription)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("id",   AttrKind_Id)
		AXL_SL_HASH_TABLE_ENTRY("prot", AttrKind_Prot)
	AXL_SL_END_HASH_TABLE()

protected:
	EnumValue* m_enumValue;

public:
	EnumValueType()
	{
		m_enumValue = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		Member* member,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class TemplateParamListType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Param,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("param", ElemKind_Param)
	AXL_SL_END_HASH_TABLE()

protected:
	sl::List<Param>* m_list;

public:
	TemplateParamListType()
	{
		m_list = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<Param>* list,
		const char* name,
		const char** attributes
		);

	bool
	create(
		DoxyXmlParser* parser,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class ParamType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Type,
		ElemKind_DeclName,
		ElemKind_DefName,
		ElemKind_Array,
		ElemKind_DefVal,
		ElemKind_TypeConstraint,
		ElemKind_BriefDescription,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("type",             ElemKind_Type)
		AXL_SL_HASH_TABLE_ENTRY("declname",         ElemKind_DeclName)
		AXL_SL_HASH_TABLE_ENTRY("defname",          ElemKind_DefName)
		AXL_SL_HASH_TABLE_ENTRY("array",            ElemKind_Array)
		AXL_SL_HASH_TABLE_ENTRY("defval",           ElemKind_DefVal)
		AXL_SL_HASH_TABLE_ENTRY("typeconstraint",   ElemKind_TypeConstraint)
		AXL_SL_HASH_TABLE_ENTRY("briefdescription", ElemKind_BriefDescription)
	AXL_SL_END_HASH_TABLE()

protected:
	Param* m_param;

public:
	ParamType()
	{
		m_param = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<Param>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

class LinkedTextType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Ref,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("ref", ElemKind_Ref)
	AXL_SL_END_HASH_TABLE()

protected:
	LinkedText* m_linkedText;
	RefText* m_refText;

public:
	LinkedTextType()
	{
		m_linkedText = NULL;
		m_refText = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		LinkedText* linkedText,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_refText->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class RefTextType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_RefId,
		AttrKind_KindRef,
		AttrKind_External,
		AttrKind_Tooltip,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("refid",    AttrKind_RefId)
		AXL_SL_HASH_TABLE_ENTRY("kindref",  AttrKind_KindRef)
		AXL_SL_HASH_TABLE_ENTRY("external", AttrKind_External)
		AXL_SL_HASH_TABLE_ENTRY("tooltip",  AttrKind_Tooltip)
	AXL_SL_END_HASH_TABLE()

protected:
	RefText* m_refText;

public:
	RefTextType()
	{
		m_refText = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		LinkedText* linkedText,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_refText->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class DocParaType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Ref,
		ElemKind_Anchor,
		ElemKind_Image,
		ElemKind_SimpleSect,
		ElemKind_Ulink,

		// ...add as needed
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("ref",        ElemKind_Ref)
		AXL_SL_HASH_TABLE_ENTRY("anchor",     ElemKind_Anchor)
		AXL_SL_HASH_TABLE_ENTRY("image",      ElemKind_Image)
		AXL_SL_HASH_TABLE_ENTRY("simplesect", ElemKind_SimpleSect)
		AXL_SL_HASH_TABLE_ENTRY("ulink",      ElemKind_Ulink)
	AXL_SL_END_HASH_TABLE()

protected:
	DocBlock* m_paragraphBlock;
	DocBlock* m_textBlock;

public:
	DocParaType()
	{
		m_paragraphBlock = NULL;
		m_textBlock = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* blockList,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_textBlock->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class DocRefTextType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_RefId,
		AttrKind_KindRef,
		AttrKind_External,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("refid",    AttrKind_RefId)
		AXL_SL_HASH_TABLE_ENTRY("kindref",  AttrKind_KindRef)
		AXL_SL_HASH_TABLE_ENTRY("external", AttrKind_External)
	AXL_SL_END_HASH_TABLE()

protected:
	DocRefBlock* m_refBlock;

public:
	DocRefTextType()
	{
		m_refBlock = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_refBlock->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class DocAnchorType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Id,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("id",    AttrKind_Id)
	AXL_SL_END_HASH_TABLE()

protected:
	DocAnchorBlock* m_anchorBlock;

public:
	DocAnchorType()
	{
		m_anchorBlock = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_anchorBlock->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class DocImageType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Type,
		AttrKind_Name,
		AttrKind_Width,
		AttrKind_Height,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("type",    AttrKind_Type)
		AXL_SL_HASH_TABLE_ENTRY("name",    AttrKind_Name)
		AXL_SL_HASH_TABLE_ENTRY("width",   AttrKind_Width)
		AXL_SL_HASH_TABLE_ENTRY("height",  AttrKind_Height)
	AXL_SL_END_HASH_TABLE()

protected:
	DocImageBlock* m_imageBlock;

public:
	DocImageType()
	{
		m_imageBlock = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_imageBlock->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class DocUlinkType: public DoxyXmlType
{
protected:
	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Url,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("url", AttrKind_Url)
	AXL_SL_END_HASH_TABLE()

protected:
	DocUlinkBlock* m_ulinkBlock;

public:
	DocUlinkType()
	{
		m_ulinkBlock = NULL;
	}

	bool
	create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		m_ulinkBlock->m_text.append(string, length);
		return true;
	}
};

//..............................................................................

class DocSimpleSectionType: public DoxyXmlType
{
protected:
	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_Para,
	};

	enum AttrKind
	{
		AttrKind_Undefined,
		AttrKind_Kind,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("para", ElemKind_Para)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(AttrKindMap, AttrKind)
		AXL_SL_HASH_TABLE_ENTRY("kind", AttrKind_Kind)
	AXL_SL_END_HASH_TABLE()

protected:
	DocSimpleSectionBlock* m_sectionBlock;

public:
	bool
	create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* list,
		const char* name,
		const char** attributes
		);

	virtual
	bool
	onStartElement(
		const char* name,
		const char** attributes
		);
};

//..............................................................................

// GraphType

enum GraphElem
{
	GraphElemKind_Node,
};

// NodeType

enum NodeElem
{
	NodeElemKind_Label,
	NodeElemKind_Link,
	NodeElemKind_ChildNode,
};

enum NodeAttr
{
	NodeAttrKind_Id,
};

// NodeType

enum ChildNodeElem
{
	ChildNodeElemKind_EdgeLabel,
};

enum ChildNodeAttr
{
	ChildNodeAttrKind_RefId,
	ChildNodeAttrKind_Relation
};

// LinkType

enum LinkTypeAttr
{
	LinkTypeAttrKind_RefId,
	LinkTypeAttrKind_External,
};

//..............................................................................
