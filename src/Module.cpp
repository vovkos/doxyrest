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
#include "Module.h"
#include "CmdLine.h"

//..............................................................................

sl::String
createPath(
	const sl::StringRef& name,
	Namespace* parentNamespace
	)
{
	if (!parentNamespace)
		return name;

	parentNamespace->m_compound->preparePath();
	sl::String path = parentNamespace->m_compound->m_path;
	if (!path.isEmpty())
		path += '/';

	path += name;
	return path;
}

template <typename T>
void
removeDuplicates(sl::List<T>* list)
{
	sl::DuckTypePtrHashTable<T, bool> map;

	sl::Iterator<T> it = list->getHead();
	while (it)
	{
		sl::Iterator<T> next = it.getNext();
		bool result = map.addIfNotExists(*it, true) != NULL;
		if (!result)
			list->erase(it);

		it = next;
	}
}

template <>
void
removeDuplicates<EnumValue> (sl::List<EnumValue>* list)
{
	sl::Iterator<EnumValue> it = list->getHead();
	while (it)
	{
		sl::Iterator<EnumValue> next = it.getNext();
		if (it->m_isDuplicate)
			list->erase(it);

		it = next;
	}
}

void
removeSubPages(sl::Array<Compound*>* pageArray)
{
	Compound** p = *pageArray;
	size_t count = pageArray->getCount();

	size_t i = 0;
	while (i < count && !p[i]->m_isSubPage)
		i++;

	size_t j = i;
	for (i++; i < count; i++)
	{
		Compound* compound = p[i];
		if (!compound->m_isSubPage)
		{
			p[j] = p[i];
			j++;
		}
	}

	pageArray->setCount(j);
}

//..............................................................................

void
RefText::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberString("m_refKind", getRefKindString(m_refKind));
	luaState->setMemberString("m_text", m_text);
	luaState->setMemberString("m_id", m_id);
	luaState->setMemberString("m_external", m_external);
	luaState->setMemberString("m_tooltip", m_tooltip);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
LinkedText::luaExport(lua::LuaState* luaState)
{
	normalize();

	luaState->createTable();

	luaState->setMemberBoolean("m_isEmpty", m_plainText.isEmpty ());
	luaState->setMemberString("m_plainText", m_plainText);

	luaExportList(luaState, m_refTextList);
	luaState->setMember("m_refTextArray");
}

void
LinkedText::normalize()
{
	m_plainText.clear();

	sl::Iterator<RefText> it = m_refTextList.getHead();
	while (it)
	{
		if (!it->m_text.isEmpty())
		{
			m_plainText += it->m_text;
			it++;
		}
		else
		{
			sl::Iterator<RefText> next = it.getNext();
			m_refTextList.erase(it);
			it = next;
		}
	}
}

//..............................................................................

void
DocBlock::luaExportMembers(lua::LuaState* luaState)
{
	luaState->setMemberString("m_blockKind", m_blockKind);
	luaState->setMemberString("m_title", m_title);
	luaState->setMemberString("m_text", m_text);

	luaExportList(luaState, m_childBlockList);
	luaState->setMember("m_childBlockList");
}

void
DocBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);
}

//.............................................................................

void
DocRefBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	if (m_refKind == RefKind_Compound)
	{
		Compound* compound = m_module->m_compoundMap.findValue(m_id, NULL);
		if (compound && compound->m_compoundKind == CompoundKind_File) // we don't export files, so remove reference
		{
			m_blockKind = "computeroutput";
			DocBlock::luaExportMembers(luaState);
			return;
		}
	}

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("m_refKind", getRefKindString(m_refKind));
	luaState->setMemberString("m_id", m_id);
	luaState->setMemberString("m_external", m_external);
}

//.............................................................................

void
DocAnchorBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("m_id", m_id);
}

//.............................................................................

void
DocImageBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("m_imageKind", getImageKindString(m_imageKind));
	luaState->setMemberString("m_name", m_name);
	luaState->setMemberInteger("m_width", m_width);
	luaState->setMemberInteger("m_height", m_height);
}

//.............................................................................

void
DocUlinkBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("m_url", m_url);
}

//.............................................................................

void
DocSectionBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("m_id", m_id);
}

//.............................................................................

void
DocSimpleSectionBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("m_simpleSectionKind", m_simpleSectionKind);
}

//.............................................................................

void
Description::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberBoolean("m_isEmpty", isEmpty ());

	luaExportList(luaState, m_docBlockList);
	luaState->setMember("m_docBlockList");
}

//.............................................................................

void
Location::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberString("m_file", m_file);
	luaState->setMemberInteger("m_line", m_line);
	luaState->setMemberInteger("m_column", m_column);
	luaState->setMemberString("m_bodyFile", m_bodyFile);
	luaState->setMemberInteger("m_bodyStartLine", m_bodyStartLine);
	luaState->setMemberInteger("m_bodyEndLine", m_bodyEndLine);
}

//..............................................................................

void
Param::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberString("m_declarationName", m_declarationName);
	luaState->setMemberString("m_definitionName", m_definitionName);
	luaState->setMemberString("m_array", m_array);

	m_type.luaExport(luaState);
	luaState->setMember("m_type");

	m_defaultValue.luaExport(luaState);
	luaState->setMember("m_defaultValue");

	m_typeConstraint.luaExport(luaState);
	luaState->setMember("m_typeConstraint");

	m_briefDescription.luaExport(luaState);
	luaState->setMember("m_briefDescription");
}

//..............................................................................

void
EnumValue::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberString("m_protectionKind", getProtectionKindString(m_protectionKind));
	luaState->setMemberString("m_id", m_id);
	luaState->setMemberString("m_name", m_name);

	m_parentEnum->preparePath();
	sl::String path = m_parentEnum->m_path;
	if (!path.isEmpty())
		path += '/';

	path += m_name;
	luaState->setMemberString("m_path", path);

	m_initializer.luaExport(luaState);
	luaState->setMember("m_initializer");

	m_briefDescription.luaExport(luaState);
	luaState->setMember("m_briefDescription");

	m_detailedDescription.luaExport(luaState);
	luaState->setMember("m_detailedDescription");
}

//..............................................................................

const char*
getMemberFlagString(MemberFlag flag)
{
	const char* stringTable[] =
	{
		"static",          // MemberFlag_Static        = 0x00000001,
		"const",           // MemberFlag_Const         = 0x00000002,
		"explicit",        // MemberFlag_Explicit      = 0x00000004,
		"inline",          // MemberFlag_Inline        = 0x00000008,
		"volatile",        // MemberFlag_Volatile      = 0x00000010,
		"mutable",         // MemberFlag_Mutable       = 0x00000020,
		"readable",        // MemberFlag_Readable      = 0x00000040,
		"writable",        // MemberFlag_Writeable     = 0x00000080,
		"initonly",        // MemberFlag_InitOnly      = 0x00000100,
		"settable",        // MemberFlag_Settable      = 0x00000200,
		"gettable",        // MemberFlag_Gettable      = 0x00000400,
		"final",           // MemberFlag_Final         = 0x00000800,
		"sealed",          // MemberFlag_Sealed        = 0x00001000,
		"new",             // MemberFlag_New           = 0x00002000,
		"add",             // MemberFlag_Add           = 0x00004000,
		"remove",          // MemberFlag_Remove        = 0x00008000,
		"raise",           // MemberFlag_Raise         = 0x00010000,
		"optional",        // MemberFlag_Optional      = 0x00020000,
		"required",        // MemberFlag_Required      = 0x00040000,
		"accessor",        // MemberFlag_Accessor      = 0x00080000,
		"attribute",       // MemberFlag_Attribute     = 0x00100000,
		"property",        // MemberFlag_Property      = 0x00200000,
		"readonly",        // MemberFlag_ReadOnly      = 0x00400000,
		"bound",           // MemberFlag_Bound         = 0x00800000,
		"removable",       // MemberFlag_Removable     = 0x01000000,
		"contrained",      // MemberFlag_Contrained    = 0x02000000,
		"transient",       // MemberFlag_Transient     = 0x04000000,
		"maybevoid",       // MemberFlag_MaybeVoid     = 0x08000000,
		"maybedefault",    // MemberFlag_MaybeDefault  = 0x10000000,
		"maybeambiguous",  // MemberFlag_MaybeAmbiguos = 0x20000000,
		"duplicate",       // MemberFlag_Duplicate     = 0x40000000,
	};

	size_t i = sl::getLoBitIdx32(flag);
	return i < countof(stringTable) ?
		stringTable[i] :
		"<undefined>";
}

inline
MemberFlag
getFirstMemberFlag(uint_t flags)
{
	return (MemberFlag)(1 << sl::getLoBitIdx(flags));
}

sl::String
getMemberFlagString(uint_t flags)
{
	if (!flags)
		return sl::String();

	sl::String string;

	while (flags)
	{
		MemberFlag flag = getFirstMemberFlag(flags);
		flags &= ~flag;

		string += getMemberFlagString(flag);
		string += ' ';
	}

	string.chop(1);
	return string;
}

//..............................................................................

Member::Member()
{
	m_parentNamespace = NULL;
	m_parentCompound = NULL;
	m_groupCompound = NULL;
	m_memberKind = MemberKind_Undefined;
	m_protectionKind = ProtectionKind_Public;
	m_virtualKind = VirtualKind_NonVirtual;
	m_flags = 0;
	m_cacheIdx = -1;
}

void
Member::luaExport(lua::LuaState* luaState)
{
	if (m_cacheIdx != -1)
	{
		luaState->getGlobalArrayElement("g_exportCache", m_cacheIdx);
		return;
	}

	// add to the global cache first

	m_cacheIdx = luaState->getGlobalArrayLen("g_exportCache") + 1;

	luaState->createTable();
	luaState->pushValue(); // duplicate
	luaState->setGlobalArrayElement("g_exportCache", m_cacheIdx);

	// now fill the members

	luaState->setMemberString("m_memberKind", getMemberKindString(m_memberKind));
	luaState->setMemberString("m_protectionKind", getProtectionKindString(m_protectionKind));
	luaState->setMemberString("m_virtualKind", getVirtualKindString(m_virtualKind));
	luaState->setMemberString("m_flags", getMemberFlagString(m_flags));
	luaState->setMemberString("m_id", m_id);
	luaState->setMemberString("m_name", m_name);
	luaState->setMemberString("m_modifiers", m_modifiers);

	AXL_TODO("use g_exportCache to export group tables instead of IDs (more natural to use from the Lua frames)")

	if (m_groupCompound)
		luaState->setMemberString("m_groupId", m_groupCompound->m_id);

	luaExportStringList(luaState, m_importList);
	luaState->setMember("m_importArray");

	preparePath();
	luaState->setMemberString("m_path",  m_path);

	switch (m_memberKind)
	{
	case MemberKind_Typedef:
		m_type.luaExport(luaState);
		luaState->setMember("m_type");
		luaState->setMemberString("m_argString", m_argString);

		luaExportList(luaState, m_paramList);
		luaState->setMember("m_paramArray");
		break;

	case MemberKind_Enum:
		removeDuplicates(&m_enumValueList);
		luaExportList(luaState, m_enumValueList);
		luaState->setMember("m_enumValueArray");
		break;

	case MemberKind_Variable:
		m_type.luaExport(luaState);
		luaState->setMember("m_type");

		luaState->setMemberString("m_argString", m_argString);
		luaState->setMemberString("m_bitField", m_bitField);

		luaExportList(luaState, m_paramList);
		luaState->setMember("m_paramArray");

		m_initializer.luaExport(luaState);
		luaState->setMember("m_initializer");
		break;

	case MemberKind_Function:
		m_type.luaExport(luaState);
		luaState->setMember("m_returnType");

		luaState->setMemberString("m_argString", m_argString);

		m_exceptions.luaExport(luaState);
		luaState->setMember("m_exceptions");

		luaExportList(luaState, m_templateParamList);
		luaState->setMember("m_templateParamArray");

		luaExportList(luaState, m_templateSpecParamList);
		luaState->setMember("m_templateSpecParamArray");

		luaExportList(luaState, m_paramList);
		luaState->setMember("m_paramArray");
		break;

	case MemberKind_Property:
		m_type.luaExport(luaState);
		luaState->setMember("m_returnType");

		luaState->setMemberString("m_argString", m_argString);

		luaExportList(luaState, m_paramList);
		luaState->setMember("m_paramArray");
		break;

	case MemberKind_Event:
		m_type.luaExport(luaState);
		luaState->setMember("m_type");

		luaState->setMemberString("m_argString", m_argString);

		luaExportList(luaState, m_paramList);
		luaState->setMember("m_paramArray");
		break;

	case MemberKind_Alias:
		m_initializer.luaExport(luaState);
		luaState->setMember("m_initializer");
		break;

	case MemberKind_Define:
		luaExportList(luaState, m_paramList);
		luaState->setMember("m_paramArray");

		m_initializer.luaExport(luaState);
		luaState->setMember("m_initializer");
		break;
	}

	m_briefDescription.luaExport(luaState);
	luaState->setMember("m_briefDescription");

	m_detailedDescription.luaExport(luaState);
	luaState->setMember("m_detailedDescription");

	m_inBodyDescription.luaExport(luaState);
	luaState->setMember("m_inBodyDescription");

	m_location.luaExport(luaState);
	luaState->setMember("m_location");
}

//..............................................................................

Compound::Compound()
{
	m_selfNamespace = NULL;
	m_parentNamespace = NULL;
	m_groupCompound = NULL;
	m_compoundKind = CompoundKind_Undefined;
	m_languageKind = LanguageKind_Undefined;
	m_protectionKind = ProtectionKind_Public;
	m_isFinal = false;
	m_isSealed = false;
	m_isAbstract = false;
	m_isDuplicate = false;
	m_isSubPage = false;
	m_cacheIdx = -1;
}

void
Compound::luaExport(lua::LuaState* luaState)
{
	if (m_cacheIdx != -1)
	{
		luaState->getGlobalArrayElement("g_exportCache", m_cacheIdx);
		return;
	}

	// add to the global cache first

	m_cacheIdx = luaState->getGlobalArrayLen("g_exportCache") + 1;

	luaState->createTable();
	luaState->pushValue(); // duplicate
	luaState->setGlobalArrayElement("g_exportCache", m_cacheIdx);

	// now fill the members

	luaState->setMemberString("m_compoundKind", getCompoundKindString(m_compoundKind));
	luaState->setMemberString("m_id", m_id);
	luaState->setMemberString("m_name", m_name);
	luaState->setMemberString("m_title", m_title);

	if (!m_importId.isEmpty())
		luaState->setMemberString("m_importId", m_importId);

	if (m_groupCompound)
		luaState->setMemberString("m_groupId", m_groupCompound->m_id);

	preparePath();
	luaState->setMemberString("m_path", m_path);

	size_t count;

	switch (m_compoundKind)
	{
	case CompoundKind_Group:
	case CompoundKind_Namespace:
		break;

	case CompoundKind_Page:
		luaExportArray(luaState, m_subPageArray);
		luaState->setMember("m_subPageArray");
		break;

	case CompoundKind_Struct:
	case CompoundKind_Union:
	case CompoundKind_Class:
	case CompoundKind_Interface:
	case CompoundKind_Protocol:
	case CompoundKind_Exception:
	case CompoundKind_Service:
	case CompoundKind_Singleton:
		luaExportList(luaState, m_templateParamList);
		luaState->setMember("m_templateParamArray");

		luaExportList(luaState, m_templateSpecParamList);
		luaState->setMember("m_templateSpecParamArray");

		luaExportArray(luaState, m_baseTypeArray);
		luaState->setMember("m_baseTypeArray");

		count = m_baseTypeProtectionArray.getCount();
		luaState->createTable(count);
		for (size_t i = 0; i < count; i++)
			luaState->setArrayElementString(i + 1, getProtectionKindString(m_baseTypeProtectionArray[i]));

		luaState->setMember("m_baseTypeProtectionArray");

		// prefer explicitly specified derived type (fallback to auto-generated if absent)

		luaExportArray(
			luaState,
			!m_derivedTypeArray_doxy.isEmpty() ?
				m_derivedTypeArray_doxy :
				m_derivedTypeArray_auto
			);

		luaState->setMember("m_derivedTypeArray");
		break;
	}

	luaExportStringList(luaState, m_importList);
	luaState->setMember("m_importArray");

	m_briefDescription.luaExport(luaState);
	luaState->setMember("m_briefDescription");

	m_detailedDescription.luaExport(luaState);
	luaState->setMember("m_detailedDescription");

	m_location.luaExport(luaState);
	luaState->setMember("m_location");

	if (m_selfNamespace) // pages don't have namespaces
		m_selfNamespace->luaExportMembers(luaState);
}

void
Compound::unqualifyName()
{
	const char* unqualifiedName = m_name;
	const char* p = m_name;
	const char* end = p + m_name.getLength();

	for (; p < end; p++)
	{
		switch (*p)
		{
		case '<':
			p = end;
			break;

		case ':':
		case '.':
			unqualifiedName = p + 1;
			break;
		}
	}

	if (unqualifiedName != m_name)
		m_name.offset(unqualifiedName - m_name);
}

void
Compound::unspecializeName()
{
	size_t i = m_name.find('<');
	if (i == -1)
		return;

	size_t unspecializedNameLength = i;

	const char* p = m_name.sz() + unspecializedNameLength + 1;
	const char* end = m_name.sz() + m_name.getLength();

	const char* p0 = p;
	size_t level = 0;

	for (; p < end; p++)
	{
		switch (*p)
		{
		case '<':
			level++;
			break;

		case '>':
			level--;
			if (level == -1)
			{
				createTemplateSpecParam(sl::StringRef(p0, p - p0));
				p = end; // outta here
			}

			break;

		case ',':
			if (!level)
			{
				createTemplateSpecParam(sl::StringRef(p0, p - p0));
				p0 = p + 1;
			}

			break;
		}

	}

	m_name.setReducedLength(unspecializedNameLength);
	m_name.trimRight();

	const char* x = strchr(m_name, '>');
	ASSERT(!x);
}

Param*
Compound::createTemplateSpecParam(const sl::StringRef& name)
{
	Param* param = AXL_MEM_NEW(Param);
	param->m_declarationName = name;
	param->m_declarationName.trim();

	m_templateSpecParamList.insertTail(param);
	return param;
}

//..............................................................................

bool
NamespaceContents::add(Compound* compound)
{
	if (compound->m_isDuplicate)
		return false;

	switch (compound->m_compoundKind)
	{
	case CompoundKind_Namespace:
		ASSERT(compound->m_selfNamespace);
		m_namespaceArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Struct:
		ASSERT(compound->m_selfNamespace);
		m_structArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Union:
		ASSERT(compound->m_selfNamespace);
		m_unionArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Class:
		ASSERT(compound->m_selfNamespace);
		m_classArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Interface:
		ASSERT(compound->m_selfNamespace);
		m_interfaceArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Protocol:
		ASSERT(compound->m_selfNamespace);
		m_protocolArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Exception:
		ASSERT(compound->m_selfNamespace);
		m_exceptionArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Service:
		ASSERT(compound->m_selfNamespace);
		m_serviceArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Singleton:
		ASSERT(compound->m_selfNamespace);
		m_singletonArray.append(compound->m_selfNamespace);
		break;

	case CompoundKind_Page:
		// pages are handled separately

	case CompoundKind_Group:
		// groups are added implicitly, via group members

	case CompoundKind_Category:
	case CompoundKind_File:
	case CompoundKind_Example:
	case CompoundKind_Dir:
		// not used in doxyrest

	default:
		return false;
	}

	return true;
}

inline
bool
isCppDestructorName(
	const sl::StringRef& memberName,
	const sl::StringRef& compoundName
	)
{
	return
		!memberName.isEmpty() &&
		memberName[0] == '~' &&
		memberName.getSubString(1) == compoundName;
}

bool
NamespaceContents::add(
	Member* member,
	Compound* thisCompound
	)
{
	if (member->m_flags & MemberFlag_Duplicate)
		return false;

	enum FunctionKind
	{
		FunctionKind_Normal = 0,
		FunctionKind_Constructor,
		FunctionKind_Destructor,
	};

	FunctionKind functionKind;

	switch (member->m_memberKind)
	{
	case MemberKind_Property:
		m_propertyArray.append(member);
		break;

	case MemberKind_Event:
		m_eventArray.append(member);
		break;

	case MemberKind_Variable:
		m_variableArray.append(member);
		break;

	case MemberKind_Typedef:
		m_typedefArray.append(member);
		break;

	case MemberKind_Enum:
		m_enumArray.append(member);
		break;

	case MemberKind_Function:
		functionKind = FunctionKind_Normal;

		if (thisCompound)
			switch (thisCompound->m_languageKind)
			{
			case LanguageKind_Cpp:
			case LanguageKind_Java:
				if (member->m_name == thisCompound->m_name)
					functionKind = FunctionKind_Constructor;
				else if (isCppDestructorName(member->m_name, thisCompound->m_name))
					functionKind = FunctionKind_Destructor;
				break;

			case LanguageKind_Jancy:
				if (member->m_name == "construct")
					functionKind = FunctionKind_Constructor;
				else if (member->m_name == "destruct")
					functionKind = FunctionKind_Destructor;
				break;
			}

		if (functionKind == FunctionKind_Constructor)
			m_constructorArray.append(member);
		else if (functionKind == FunctionKind_Destructor)
			m_destructor = member;
		else
			m_functionArray.append(member);

		break;

	case MemberKind_Alias:
		m_aliasArray.append(member);
		break;

	case MemberKind_Define:
		m_defineArray.append(member);
		break;

	case MemberKind_Footnote:
		m_footnoteArray.append(member);
		break;

	case MemberKind_Interface:
	case MemberKind_Service:
		ASSERT(false); // should have been handled earlier

	case MemberKind_Signal:
	case MemberKind_Prototype:
	case MemberKind_Friend:
	case MemberKind_Dcop:
	case MemberKind_Slot:
	case MemberKind_EnumValue:

	default:
		return false;
	}

	return true;
}

void
NamespaceContents::luaExportMembers(lua::LuaState* luaState)
{
	luaExportArray(luaState, m_groupArray);
	luaState->setMember("m_groupArray");

	luaExportArray(luaState, m_namespaceArray);
	luaState->setMember("m_namespaceArray");

	luaExportArray(luaState, m_enumArray);
	luaState->setMember("m_enumArray");

	luaExportArray(luaState, m_structArray);
	luaState->setMember("m_structArray");

	luaExportArray(luaState, m_unionArray);
	luaState->setMember("m_unionArray");

	luaExportArray(luaState, m_classArray);
	luaState->setMember("m_classArray");

	luaExportArray(luaState, m_interfaceArray);
	luaState->setMember("m_interfaceArray");

	luaExportArray(luaState, m_protocolArray);
	luaState->setMember("m_protocolArray");

	luaExportArray(luaState, m_exceptionArray);
	luaState->setMember("m_exceptionArray");

	luaExportArray(luaState, m_serviceArray);
	luaState->setMember("m_serviceArray");

	luaExportArray(luaState, m_singletonArray);
	luaState->setMember("m_singletonArray");

	luaExportArray(luaState, m_typedefArray);
	luaState->setMember("m_typedefArray");

	luaExportArray(luaState, m_variableArray);
	luaState->setMember("m_variableArray");

	luaExportArray(luaState, m_constructorArray);
	luaState->setMember("m_constructorArray");

	if (m_destructor)
	{
		m_destructor->luaExport(luaState);
		luaState->setMember("m_destructor");
	}

	luaExportArray(luaState, m_functionArray);
	luaState->setMember("m_functionArray");

	luaExportArray(luaState, m_propertyArray);
	luaState->setMember("m_propertyArray");

	luaExportArray(luaState, m_eventArray);
	luaState->setMember("m_eventArray");

	luaExportArray(luaState, m_aliasArray);
	luaState->setMember("m_aliasArray");

	luaExportArray(luaState, m_defineArray);
	luaState->setMember("m_defineArray");

	luaExportArray(luaState, m_footnoteArray);
	luaState->setMember("m_footnoteArray");
}

//..............................................................................

void
GlobalNamespace::clear()
{
	m_groupArray.clear();
	m_namespaceArray.clear();
	m_enumArray.clear();
	m_structArray.clear();
	m_unionArray.clear();
	m_classArray.clear();
	m_typedefArray.clear();
	m_variableArray.clear();
	m_functionArray.clear();
	m_propertyArray.clear();
	m_eventArray.clear();
	m_aliasArray.clear();
	m_namespaceList.clear();
}

bool
GlobalNamespace::build(
	Module* module,
	uint_t cmdLineFlags
	)
{
	clear();

	// loop #1 assign groups

	bool isMemberGroupAllowed = (cmdLineFlags & CmdLineFlag_AllowMemberGroups) != 0;

	size_t count = module->m_groupArray.getCount();
	for (size_t i = 0; i < count; i++)
	{
		Compound* compound = module->m_groupArray[i];
		sl::Iterator<Member> memberIt = compound->m_memberList.getHead();
		for (; memberIt; memberIt++)
		{
			if (memberIt->m_memberKind == MemberKind_Footnote)
			{
				compound->m_groupFootnoteArray.append(*memberIt);
			}
			else
			{
				Member* member = module->m_memberMap.findValue(memberIt->m_id, NULL);
				if (member && (isMemberGroupAllowed || member->m_parentCompound->isMemberGroupAllowed()))
					member->m_groupCompound = compound;
			}
		}

		sl::Iterator<Ref> refIt = compound->m_innerRefList.getHead();
		for (; refIt; refIt++)
		{
			Compound* innerCompound = module->m_compoundMap.findValue(refIt->m_id, NULL);
			if (innerCompound)
				innerCompound->m_groupCompound = compound;
		}
	}

	module->m_groupArray.clear(); // will contain non-empty root groups only

	// loop #2 initializes namespaces (including classes, structs, unions)

	count = module->m_namespaceArray.getCount();
	for (size_t i = 0; i < count; i++)
	{
		Compound* compound = module->m_namespaceArray[i];

		Namespace* nspace = AXL_MEM_NEW(Namespace);
		m_namespaceList.insertTail(nspace);
		nspace->m_compound = compound;
		compound->m_selfNamespace = nspace;
	}

	// loop #3 resolves inner, base and derived references and add members

	for (size_t i = 0; i < count; i++)
	{
		Compound* compound = module->m_namespaceArray[i];
		Namespace* nspace = compound->m_selfNamespace;
		ASSERT(nspace);

		// clean-up compound name

		compound->unqualifyName();
		compound->unspecializeName();

		sl::Iterator<Member> memberIt = compound->m_memberList.getHead();
		for (; memberIt; memberIt++)
		{
			Compound* memberCompound;

			switch (memberIt->m_memberKind)
			{
			case MemberKind_Interface:
			case MemberKind_Service:
				memberCompound = createMemberCompound(module, *memberIt);

				nspace->add(memberCompound);
				memberCompound->m_parentNamespace = nspace; // namespace, not group!

				if (memberCompound->m_groupCompound)
				{
					Namespace* groupNspace = getGroupNamespace(module, memberCompound->m_groupCompound);
					groupNspace->add(memberCompound);
				}

				break;

			default:
				nspace->add(*memberIt, compound);
				memberIt->m_parentNamespace = nspace; // namespace, not group!

				if (memberIt->m_groupCompound)
				{
					Namespace* groupNspace = getGroupNamespace(module, memberIt->m_groupCompound);
					groupNspace->add(*memberIt, compound);
				}
			}
		}

		sl::Iterator<Ref> refIt = compound->m_innerRefList.getHead();
		for (; refIt; refIt++)
		{
			Compound* innerCompound = module->m_compoundMap.findValue(refIt->m_id, NULL);
			if (!innerCompound)
			{
				printf("warning: can't find inner compound refid: %s\n", refIt->m_id.sz());
				continue;
			}

			nspace->add(innerCompound);
			innerCompound->m_parentNamespace = nspace; // namespace, not group!

			if (innerCompound->m_groupCompound)
			{
				if (isMemberGroupAllowed || compound->isMemberGroupAllowed())
				{
					Namespace* groupNspace = getGroupNamespace(module, innerCompound->m_groupCompound);
					groupNspace->add(innerCompound);
				}
				else
				{
					innerCompound->m_groupCompound = NULL;
				}
			}
		}

		removeDuplicates(&compound->m_baseRefList);
		removeDuplicates(&compound->m_derivedRefList);

		refIt = compound->m_baseRefList.getHead();
		for (; refIt; refIt++)
		{
			Compound* baseCompound;

			if (refIt->m_id.isEmpty() || !refIt->m_importId.isEmpty()) // template or imported base
			{
				baseCompound = AXL_MEM_NEW(Compound);
				baseCompound->m_id = refIt->m_id;
				baseCompound->m_importId = refIt->m_importId;
				baseCompound->m_name = refIt->m_text;
				module->m_compoundList.insertTail(baseCompound);
			}
			else
			{
				baseCompound = module->m_compoundMap.findValue(refIt->m_id, NULL);
				if (!baseCompound)
				{
					err::setFormatStringError("can't find base compound refid: %s\n", refIt->m_id.sz());
					return false;
				}

				baseCompound->m_derivedTypeArray_auto.append(compound);
			}

			compound->m_baseTypeArray.append(baseCompound);
			compound->m_baseTypeProtectionArray.append(refIt->m_protectionKind);
		}

		sl::StringHashTable<bool> idMap; // derivedcompoundref may specify the same class multiple types -- we don't want that

		refIt = compound->m_derivedRefList.getHead();
		for (; refIt; refIt++)
		{
			if (!idMap.addIfNotExists(refIt->m_id, true))
				continue;

			Compound* derivedCompound = module->m_compoundMap.findValue(refIt->m_id, NULL);
			if (!derivedCompound)
			{
				printf("warning: can't find derived compound refid: %s\n", refIt->m_id.sz());
				continue;
			}

			compound->m_derivedTypeArray_doxy.append(derivedCompound);
		}
	}

	// loop #4 resolves sub pages

	count = module->m_pageArray.getCount();
	for (size_t i = 0; i < count; i++)
	{
		Compound* compound = module->m_pageArray[i];

		sl::Iterator<Ref> refIt = compound->m_innerRefList.getHead();
		for (; refIt; refIt++)
		{
			Compound* subPage = module->m_compoundMap.findValue(refIt->m_id, NULL);
			if (!subPage)
			{
				printf("warning: can't find subpage refid: %s\n", refIt->m_id.sz());
				continue;
			}

			compound->m_subPageArray.append(subPage);
			subPage->m_isSubPage = true;
		}
	}

	removeSubPages(&module->m_pageArray);

	// loop #5 adds leftovers to the global namespace

	sl::Iterator<Compound> compoundIt = module->m_compoundList.getHead();
	for (; compoundIt; compoundIt++)
	{
		sl::Iterator<Member> memberIt;

		switch (compoundIt->m_compoundKind)
		{
		case CompoundKind_Undefined: // template base type or incomplete compound
		case CompoundKind_Group:     // groups are added implicitly, via group members
			break;

		case CompoundKind_File:
			memberIt = compoundIt->m_memberList.getHead();

			for (; memberIt; memberIt++)
			{
				Compound* memberCompound;

				if (memberIt->m_flags & MemberFlag_Duplicate)
					continue;

				switch (memberIt->m_memberKind)
				{
				case MemberKind_Interface:
				case MemberKind_Service:
					memberCompound = createMemberCompound(module, *memberIt);
					add(memberCompound);

					if (memberCompound->m_groupCompound)
					{
						Namespace* groupNspace = getGroupNamespace(module, memberCompound->m_groupCompound);
						groupNspace->add(memberCompound);
					}

					break;

				default:
					add(*memberIt, NULL);
					if (memberIt->m_groupCompound)
					{
						Namespace* groupNspace = getGroupNamespace(module, memberIt->m_groupCompound);
						groupNspace->add(*memberIt, NULL);
					}
				}
			}
			break;

		default:
			if (!compoundIt->m_parentNamespace)
			{
				add(*compoundIt);
				if (compoundIt->m_groupCompound)
				{
					Namespace* groupNspace = getGroupNamespace(module, compoundIt->m_groupCompound);
					groupNspace->add(*compoundIt);
				}
			}
		}
	}

	return true;
}

void
GlobalNamespace::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();
	luaExportMembers(luaState);

	luaState->setMemberString("m_path", "");
	luaState->setMemberString("m_id", "global");
	luaState->setMemberString("m_compoundKind", "namespace");

	// global namespace has no description, but we still want valid m_briefDescription/m_detailedDescription

	Description emptyDescription;

	emptyDescription.luaExport(luaState);
	luaState->setMember("m_briefDescription");

	emptyDescription.luaExport(luaState);
	luaState->setMember("m_detailedDescription");
}

Namespace*
GlobalNamespace::getGroupNamespace(
	Module* module,
	Compound* groupCompound
	)
{
	if (groupCompound->m_selfNamespace)
		return groupCompound->m_selfNamespace;

	Namespace* nspace = AXL_MEM_NEW(Namespace);
	m_namespaceList.insertTail(nspace);
	nspace->m_compound = groupCompound;
	nspace->m_footnoteArray = groupCompound->m_groupFootnoteArray;
	groupCompound->m_selfNamespace = nspace;

	if (!groupCompound->m_groupCompound)
	{
		module->m_groupArray.append(groupCompound);
	}
	else
	{
		Namespace* parentGroupNspace = getGroupNamespace(module, groupCompound->m_groupCompound);
		groupCompound->m_parentNamespace = parentGroupNspace;
		parentGroupNspace->m_groupArray.append(nspace);
	}

	return nspace;
}

Compound*
GlobalNamespace::createMemberCompound(
	Module* module,
	Member* member
	)
{
	Compound* compound = AXL_MEM_NEW(Compound);
	compound->m_compoundKind = member->m_memberKind == MemberKind_Service ? CompoundKind_Service : CompoundKind_Interface;
	compound->m_name = member->m_name;
	compound->m_id = member->m_id;
	compound->m_groupCompound = member->m_groupCompound;
	sl::takeOver(&compound->m_briefDescription, &member->m_briefDescription);
	sl::takeOver(&compound->m_detailedDescription, &member->m_detailedDescription);
	compound->m_selfNamespace = AXL_MEM_NEW(Namespace);
	compound->m_selfNamespace->m_compound = compound;
	m_namespaceList.insertTail(compound->m_selfNamespace);
	module->m_compoundList.insertTail(compound);

	return compound;
}

//..............................................................................
