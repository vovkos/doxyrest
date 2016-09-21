#pragma once

#include "DoxyXmlEnum.h"

struct Namespace;
struct Compound;

//.............................................................................

struct RefText: sl::ListLink
{
	RefKind m_refKind;
	sl::String m_text;
	sl::String m_id;
	sl::String m_external;
	sl::String m_tooltip;

	RefText ()
	{
		m_refKind = RefKind_Undefined;
	}

	void
	luaExport (lua::LuaState* luaState);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct LinkedText
{
	sl::String m_plainText;
	sl::StdList <RefText> m_refTextList;

	void
	luaExport (lua::LuaState* luaState);

	void
	normalize ();
};

//.............................................................................

enum DocBlockKind
{
	DocBlockKind_Undefined,
	DocBlockKind_Paragraph,	
	DocBlockKind_Section,
	DocBlockKind_Internal,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DocBlock: sl::ListLink
{
	DocBlockKind m_docBlockKind;
	sl::String m_title;

	DocBlock ()
	{
		m_docBlockKind = DocBlockKind_Undefined;
	}

	virtual ~DocBlock ()
	{
	}

	virtual 
	void
	luaExport (lua::LuaState* luaState) = 0;

	void
	luaExportMembers (lua::LuaState* luaState);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DocParagraphBlock: DocBlock
{
	LinkedText m_contents;

	virtual 
	void
	luaExport (lua::LuaState* luaState);
};

struct DocSectionBlock: DocBlock
{
	sl::String m_id;
	sl::StdList <DocBlock> m_childBlockList;

	virtual 
	void
	luaExport (lua::LuaState* luaState);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Description
{
	sl::String m_title;	
	sl::StdList <DocBlock> m_docBlockList;

	bool isEmpty ()
	{
		return m_title.isEmpty () && m_docBlockList.isEmpty ();
	}

	void
	luaExport (lua::LuaState* luaState);
};

//.............................................................................

struct Param: sl::ListLink
{
	LinkedText m_type;
	sl::String m_declarationName;
	sl::String m_definitionName;
	sl::String m_array;
	LinkedText m_defaultValue;
	LinkedText m_typeConstraint;

	Description m_briefDescription;

	void
	luaExport (lua::LuaState* luaState);
};

//.............................................................................

struct EnumValue: sl::ListLink
{
	ProtectionKind m_protectionKind;

	sl::String m_id;
	sl::String m_name;
	LinkedText m_initializer;

	Description m_briefDescription;
	Description m_detailedDescription;

	EnumValue ()
	{
		m_protectionKind = ProtectionKind_Undefined;
	}

	void
	luaExport (lua::LuaState* luaState);
};


//.............................................................................

enum MemberFlag
{
	MemberFlag_Static        = 0x00000001,
	MemberFlag_Const         = 0x00000002,
	MemberFlag_Explicit      = 0x00000004,
	MemberFlag_Inline        = 0x00000008,
	MemberFlag_Volatile      = 0x00000010,
	MemberFlag_Mutable       = 0x00000020,
	MemberFlag_Readable      = 0x00000040,
	MemberFlag_Writeable     = 0x00000080,
	MemberFlag_InitOnly      = 0x00000100,
	MemberFlag_Settable      = 0x00000200,
	MemberFlag_Gettable      = 0x00000400,
	MemberFlag_Final         = 0x00000800,
	MemberFlag_Sealed        = 0x00001000,
	MemberFlag_New           = 0x00002000,
	MemberFlag_Add           = 0x00004000,
	MemberFlag_Remove        = 0x00008000,
	MemberFlag_Raise         = 0x00010000,
	MemberFlag_Optional      = 0x00020000,
	MemberFlag_Required      = 0x00040000,
	MemberFlag_Accessor      = 0x00080000,
	MemberFlag_Attribute     = 0x00100000,
	MemberFlag_Property      = 0x00200000,
	MemberFlag_ReadOnly      = 0x00400000,
	MemberFlag_Bound         = 0x00800000,
	MemberFlag_Removable     = 0x01000000,
	MemberFlag_Contrained    = 0x02000000,
	MemberFlag_Transient     = 0x04000000,
	MemberFlag_MaybeVoid     = 0x08000000,
	MemberFlag_MaybeDefault  = 0x10000000,
	MemberFlag_MaybeAmbiguos = 0x20000000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getMemberFlagString (MemberFlag flag);

sl::String
getMemberFlagString (uint_t flags);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Member: sl::ListLink
{
	Compound* m_parentCompound;
	Compound* m_doxyGroupCompound;

	MemberKind m_memberKind;
	ProtectionKind m_protectionKind;
	VirtualKind m_virtualKind;
	uint_t m_flags;

	sl::String m_id;
	LinkedText m_type;
	sl::String m_name;
	sl::String m_definition;
	sl::String m_argString;
	sl::String m_bitField;
	LinkedText m_initializer;
	LinkedText m_exceptions;
	sl::String m_modifiers;

	sl::StdList <Param> m_paramList;
	sl::StdList <Param> m_templateParamList;
	sl::StdList <Param> m_templateSpecParamList;
	sl::StdList <EnumValue> m_enumValueList;

	Description m_briefDescription;
	Description m_detailedDescription;
	Description m_inBodyDescription;

	Member ();

	void
	luaExport (lua::LuaState* luaState);
};

//.............................................................................

struct Ref: sl::ListLink
{
	ProtectionKind m_protectionKind;
	VirtualKind m_virtualKind;

	sl::String m_id;
	sl::String m_text;

	Ref ()
	{
		m_protectionKind = ProtectionKind_Undefined;
		m_virtualKind = VirtualKind_Undefined;
	}
};

//.............................................................................

struct Compound: sl::ListLink
{
	Namespace* m_selfNamespace;
	Namespace* m_parentNamespace;
	Compound* m_doxyGroupCompound;

	CompoundKind m_compoundKind;
	LanguageKind m_languageKind;
	ProtectionKind m_protectionKind;
		
	sl::String m_id;
	sl::String m_name;
	sl::String m_title;
	sl::StdList <Param> m_templateParamList;
	sl::StdList <Param> m_templateSpecParamList;
	sl::StdList <Member> m_memberList;

	sl::StdList <Ref> m_baseRefList;
	sl::StdList <Ref> m_derivedRefList;
	sl::StdList <Ref> m_innerRefList;

	sl::String m_path;

	bool m_isFinal    : 1;
	bool m_isSealed   : 1;
	bool m_isAbstract : 1;

	Description m_briefDescription;
	Description m_detailedDescription;

	Compound ();

	void
	luaExport (lua::LuaState* luaState);
	
	void
	unqualifyName ();

	void
	unspecializeName ();

	Param*
	createTemplateSpecParam (const sl::StringRef& name);

	void
	preparePath ();
};

//.............................................................................

struct Module
{
	sl::String m_version;
	sl::StdList <Compound> m_compoundList;
	sl::Array <Compound*> m_namespaceArray;
	sl::Array <Compound*> m_doxyGroupArray;
	sl::StringHashTableMap <Compound*> m_compoundMap;
	sl::StringHashTableMap <Member*> m_memberMap;
	sl::StringHashTableMap <size_t> m_groupMap;

	Compound* 
	findCompound (const char* id)
	{
		sl::StringHashTableMapIterator <Compound*> mapIt = m_compoundMap.find (id);
		return mapIt ? mapIt->m_value : NULL;
	}
	
	Member* 
	findMember (const char* id)
	{
		sl::StringHashTableMapIterator <Member*> mapIt = m_memberMap.find (id);
		return mapIt ? mapIt->m_value : NULL;
	}
};

//.............................................................................

struct NamespaceContents
{
	sl::Array <Namespace*> m_groupArray;
	sl::Array <Namespace*> m_namespaceArray;
	sl::Array <Member*> m_enumArray;
	sl::Array <Namespace*> m_structArray;
	sl::Array <Namespace*> m_unionArray;
	sl::Array <Namespace*> m_classArray;
	sl::Array <Member*> m_typedefArray;
	sl::Array <Member*> m_variableArray;
	sl::Array <Member*> m_functionArray;
	sl::Array <Member*> m_propertyArray;
	sl::Array <Member*> m_eventArray;

	sl::StringHashTableMap <Namespace*> m_groupMap;

	bool
	add (Compound* compound);

	bool
	add (Member* member);

	void
	luaExportMembers (lua::LuaState* luaState);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Namespace: 
	sl::ListLink,
	NamespaceContents 
{
	Compound* m_compound;

	Namespace ()
	{
		m_compound = NULL;
	}

	void
	luaExport (lua::LuaState* luaState)
	{
		ASSERT (m_compound);
		m_compound->luaExport (luaState);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class GlobalNamespace: public NamespaceContents
{
protected:
	sl::StdList <Namespace> m_namespaceList;

public:
	void
	clear ();

	bool
	build (
		Module* module,
		ProtectionKind protectionFilter
		);

	void
	luaExport (lua::LuaState* luaState);

protected:
	Namespace* 
	getSubGroupNamespace (
		Module* module,
		NamespaceContents* parent,
		Namespace* parentNamespace,
		Compound* doxyGroupCompound
		);
};

//.............................................................................

template <typename T>
void
luaExportArray (
	lua::LuaState* luaState,
	T* const* a,
	size_t count
	)
{
	luaState->createTable (count);

	for (size_t i = 0; i < count; i++)
	{
		a [i]->luaExport (luaState);
		luaState->setArrayElement (i + 1); // lua arrays are 1-based
	}
}

template <typename T>
void
luaExportArray (
	lua::LuaState* luaState,
	const sl::Array <T*>& array
	)
{
	luaExportArray (luaState, array.ca (), array.getCount ());
}

template <typename T>
void
luaExportArraySetParent (
	lua::LuaState* luaState,
	T* const* a,
	size_t count,
	const char* parentFieldName,
	int parentIndex = -1
	)
{
	luaState->createTable (count);

	for (size_t i = 0; i < count; i++)
	{
		a [i]->luaExport (luaState);
		luaState->pushValue (parentIndex - 2); // [-1] element, [-2] table
		luaState->setMember (parentFieldName);
		luaState->setArrayElement (i + 1); // lua arrays are 1-based
	}
}

template <typename T>
void
luaExportArraySetParent (
	lua::LuaState* luaState,
	const sl::Array <T*>& array,
	const char* parentFieldName,
	int parentIndex = -1
	)
{
	luaExportArraySetParent (luaState, array.ca (), array.getCount (), parentFieldName, parentIndex);
}

template <typename T>
void
luaExportList (
	lua::LuaState* luaState,
	const sl::StdList <T>& list
	)
{
	luaState->createTable (list.getCount ());

	sl::Iterator <T> it = list.getHead ();
	for (size_t i = 1; it; it++, i++) // lua arrays are 1-based
	{
		it->luaExport (luaState);
		luaState->setArrayElement (i);
	}
}

//.............................................................................

