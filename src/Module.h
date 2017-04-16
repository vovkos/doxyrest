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

#include "DoxyXmlEnum.h"

struct Namespace;
struct Member;
struct Compound;

//..............................................................................

sl::String
createPath (
	const sl::StringRef& name,
	Namespace* parentNamespace
	);

//..............................................................................

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct LinkedText
{
	sl::String m_plainText;
	sl::StdList <RefText> m_refTextList;

	void
	luaExport (lua::LuaState* luaState);

	void
	normalize ();
};

//..............................................................................

struct DocBlock: sl::ListLink
{
	sl::String m_blockKind;
	sl::String m_title;
	sl::String m_text;
	sl::StdList <DocBlock> m_childBlockList;

	virtual ~DocBlock ()
	{
	}

	virtual
	void
	luaExport (lua::LuaState* luaState);

	void
	luaExportMembers (lua::LuaState* luaState);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DocRefBlock: DocBlock
{
	RefKind m_refKind;
	sl::String m_id;
	sl::String m_external;

	DocRefBlock ()
	{
		m_refKind = RefKind_Undefined;
	}

	virtual
	void
	luaExport (lua::LuaState* luaState);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DocSectionBlock: DocBlock
{
	sl::String m_id;

	virtual
	void
	luaExport (lua::LuaState* luaState);
};

//.............................................................................

struct DocSimpleSectionBlock: DocBlock
{
	sl::String m_simpleSectionKind;

	virtual
	void
	luaExport (lua::LuaState* luaState);
};

//.............................................................................

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

//..............................................................................

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

//..............................................................................

struct EnumValue: sl::ListLink
{
	ProtectionKind m_protectionKind;

	sl::String m_id;
	sl::String m_name;
	Member* m_parentEnum;
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


//..............................................................................

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
	MemberFlag_Duplicate     = 0x40000000,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getMemberFlagString (MemberFlag flag);

sl::String
getMemberFlagString (uint_t flags);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Member: sl::ListLink
{
	Namespace* m_parentNamespace;
	Compound* m_parentCompound;
	Compound* m_groupCompound;

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

	sl::BoxList <sl::String> m_importList;
	sl::StdList <Param> m_paramList;
	sl::StdList <Param> m_templateParamList;
	sl::StdList <Param> m_templateSpecParamList;
	sl::StdList <EnumValue> m_enumValueList;

	sl::String m_path;

	Description m_briefDescription;
	Description m_detailedDescription;
	Description m_inBodyDescription;

	Member ();

	void
	luaExport (lua::LuaState* luaState);

	void
	preparePath ()
	{
		if (m_path.isEmpty ())
			m_path = createPath (m_name, m_parentNamespace);
	}
};

//..............................................................................

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

//..............................................................................

struct Compound: sl::ListLink
{
	Namespace* m_selfNamespace;
	Namespace* m_parentNamespace;
	Compound* m_groupCompound;

	CompoundKind m_compoundKind;
	LanguageKind m_languageKind;
	ProtectionKind m_protectionKind;

	sl::String m_id;
	sl::String m_name;
	sl::String m_title;
	sl::BoxList <sl::String> m_importList;
	sl::StdList <Param> m_templateParamList;
	sl::StdList <Param> m_templateSpecParamList;
	sl::StdList <Member> m_memberList;
	sl::Array <Member*> m_groupFootnoteArray;

	sl::StdList <Ref> m_baseRefList;
	sl::StdList <Ref> m_derivedRefList;
	sl::StdList <Ref> m_innerRefList;

	sl::String m_path;

	bool m_isFinal     : 1;
	bool m_isSealed    : 1;
	bool m_isAbstract  : 1;

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
	preparePath ()
	{
		if (m_path.isEmpty ())
			m_path = createPath (m_name, m_parentNamespace);
	}
};

//..............................................................................

struct Module
{
	sl::String m_version;
	sl::StdList <Compound> m_compoundList;
	sl::Array <Compound*> m_namespaceArray;
	sl::Array <Compound*> m_groupArray;
	sl::Array <Compound*> m_pageArray;
	sl::StringHashTable <Compound*> m_compoundMap;
	sl::StringHashTable <Member*> m_memberMap;

	Compound*
	findCompound (const sl::StringRef& id)
	{
		return m_compoundMap.findValue (id, NULL);
	}

	Member*
	findMember (const sl::StringRef& id)
	{
		return m_memberMap.findValue (id, NULL);
	}
};

//..............................................................................

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
	sl::Array <Member*> m_aliasArray;
	sl::Array <Member*> m_defineArray;
	sl::Array <Member*> m_footnoteArray;
	sl::Array <Member*> m_constructorArray;
	Member* m_destructor;

	NamespaceContents ()
	{
		m_destructor = NULL;
	}

	bool
	add (Compound* compound);

	bool
	add (
		Member* member,
		Compound* thisCompound
		);

	void
	luaExportMembers (lua::LuaState* luaState);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class GlobalNamespace: public NamespaceContents
{
protected:
	sl::StdList <Namespace> m_namespaceList;

public:
	void
	clear ();

	bool
	build (Module* module);

	void
	luaExport (lua::LuaState* luaState);

protected:
	Namespace*
	getGroupNamespace (
		Module* module,
		Compound* groupCompound
		);
};

//..............................................................................

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
	luaExportArray (luaState, array.cp (), array.getCount ());
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

inline
void
luaExportStringList (
	lua::LuaState* luaState,
	const sl::BoxList <sl::String>& list
	)
{
	luaState->createTable (list.getCount ());

	sl::BoxIterator <sl::String> it = list.getHead ();
	for (size_t i = 1; it; it++, i++) // lua arrays are 1-based
	{
		luaState->pushString (*it);
		luaState->setArrayElement (i);
	}
}

//..............................................................................
