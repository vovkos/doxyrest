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
removeDuplicates<EnumValue>(sl::List<EnumValue>* list)
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

	luaState->setMemberString("refKind", getRefKindString(m_refKind));
	luaState->setMemberString("text", m_text);
	luaState->setMemberString("id", m_id);
	luaState->setMemberString("external", m_external);
	luaState->setMemberString("tooltip", m_tooltip);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
LinkedText::luaExport(lua::LuaState* luaState)
{
	normalize();

	luaState->createTable();

	luaState->setMemberBoolean("isEmpty", m_plainText.isEmpty ());
	luaState->setMemberString("plainText", m_plainText);

	luaExportList(luaState, m_refTextList);
	luaState->setMember("refTextArray");
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
	luaState->setMemberString("blockKind", m_blockKind);
	luaState->setMemberString("title", m_title);
	luaState->setMemberString("text", m_text);

	luaExportList(luaState, m_childBlockList);
	luaState->setMember("childBlockList");
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

	luaState->setMemberString("refKind", getRefKindString(m_refKind));
	luaState->setMemberString("id", m_id);
	luaState->setMemberString("external", m_external);
}

//.............................................................................

void
DocAnchorBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("id", m_id);
}

//.............................................................................

DocImageBlock::DocImageBlock()
{
	m_imageKind = ImageKind_Undefined;
	m_width = 0;
	m_height = 0;
}

void
DocImageBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("imageKind", getImageKindString(m_imageKind));
	luaState->setMemberString("name", m_name);
	luaState->setMemberInteger("width", m_width);
	luaState->setMemberInteger("height", m_height);
}

//.............................................................................

void
DocUlinkBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("url", m_url);
}

//.............................................................................

void
DocHeadingBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberInteger("level", m_level);
}

//.............................................................................

void
DocSectionBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("id", m_id);
}

//.............................................................................

void
DocSimpleSectionBlock::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	DocBlock::luaExportMembers(luaState);

	luaState->setMemberString("simpleSectionKind", m_simpleSectionKind);
}

//.............................................................................

void
Description::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberBoolean("isEmpty", isEmpty ());

	luaExportList(luaState, m_docBlockList);
	luaState->setMember("docBlockList");
}

//.............................................................................

Location::Location()
{
	m_line = 0;
	m_column = 0;
	m_bodyStartLine = 0;
	m_bodyEndLine = 0;
}

void
Location::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberString("file", m_file);
	luaState->setMemberInteger("line", m_line);
	luaState->setMemberInteger("column", m_column);
	luaState->setMemberString("bodyFile", m_bodyFile);
	luaState->setMemberInteger("bodyStartLine", m_bodyStartLine);
	luaState->setMemberInteger("bodyEndLine", m_bodyEndLine);
}

//..............................................................................

void
Param::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberString("declarationName", m_declarationName);
	luaState->setMemberString("definitionName", m_definitionName);
	luaState->setMemberString("array", m_array);

	m_type.luaExport(luaState);
	luaState->setMember("type");

	m_defaultValue.luaExport(luaState);
	luaState->setMember("defaultValue");

	m_typeConstraint.luaExport(luaState);
	luaState->setMember("typeConstraint");

	m_briefDescription.luaExport(luaState);
	luaState->setMember("briefDescription");
}

//..............................................................................

EnumValue::EnumValue()
{
	m_protectionKind = ProtectionKind_Undefined;
	m_parentEnum = NULL;
	m_isDuplicate = false;
}

void
EnumValue::luaExport(lua::LuaState* luaState)
{
	luaState->createTable();

	luaState->setMemberString("protectionKind", getProtectionKindString(m_protectionKind));
	luaState->setMemberString("id", m_id);
	luaState->setMemberString("name", m_name);
	luaState->setMemberString("memberKind", getMemberKindString(MemberKind_EnumValue));

	m_parentEnum->preparePath();
	sl::String path = m_parentEnum->m_path;
	if (!path.isEmpty())
		path += '/';

	path += m_name;
	luaState->setMemberString("path", path);

	m_initializer.luaExport(luaState);
	luaState->setMember("initializer");

	m_briefDescription.luaExport(luaState);
	luaState->setMember("briefDescription");

	m_detailedDescription.luaExport(luaState);
	luaState->setMember("detailedDescription");
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

	luaState->setMemberString("memberKind", getMemberKindString(m_memberKind));
	luaState->setMemberString("protectionKind", getProtectionKindString(m_protectionKind));
	luaState->setMemberString("virtualKind", getVirtualKindString(m_virtualKind));
	luaState->setMemberString("flags", getMemberFlagString(m_flags));
	luaState->setMemberString("id", m_id);
	luaState->setMemberString("name", m_name);
	luaState->setMemberString("modifiers", m_modifiers);

	if (m_groupCompound)
	{
		m_groupCompound->luaExport(luaState);
		luaState->setMember("group");
	}

	luaExportStringList(luaState, m_importList);
	luaState->setMember("importArray");

	preparePath();
	luaState->setMemberString("path",  m_path);

	switch (m_memberKind)
	{
	case MemberKind_Typedef:
		m_type.luaExport(luaState);
		luaState->setMember("type");
		luaState->setMemberString("argString", m_argString);

		luaExportList(luaState, m_paramList);
		luaState->setMember("paramArray");
		break;

	case MemberKind_Enum:
		removeDuplicates(&m_enumValueList);
		luaExportList(luaState, m_enumValueList);
		luaState->setMember("enumValueArray");
		break;

	case MemberKind_Variable:
		m_type.luaExport(luaState);
		luaState->setMember("type");

		luaState->setMemberString("argString", m_argString);
		luaState->setMemberString("bitField", m_bitField);

		luaExportList(luaState, m_paramList);
		luaState->setMember("paramArray");

		m_initializer.luaExport(luaState);
		luaState->setMember("initializer");
		break;

	case MemberKind_Function:
		m_type.luaExport(luaState);
		luaState->setMember("returnType");

		luaState->setMemberString("argString", m_argString);

		m_exceptions.luaExport(luaState);
		luaState->setMember("exceptions");

		luaExportList(luaState, m_templateParamList);
		luaState->setMember("templateParamArray");

		luaExportList(luaState, m_templateSpecParamList);
		luaState->setMember("templateSpecParamArray");

		luaExportList(luaState, m_paramList);
		luaState->setMember("paramArray");
		break;

	case MemberKind_Property:
		m_type.luaExport(luaState);
		luaState->setMember("returnType");

		luaState->setMemberString("argString", m_argString);

		luaExportList(luaState, m_paramList);
		luaState->setMember("paramArray");
		break;

	case MemberKind_Event:
		m_type.luaExport(luaState);
		luaState->setMember("type");

		luaState->setMemberString("argString", m_argString);

		luaExportList(luaState, m_paramList);
		luaState->setMember("paramArray");
		break;

	case MemberKind_Alias:
		m_initializer.luaExport(luaState);
		luaState->setMember("initializer");
		break;

	case MemberKind_Define:
		luaExportList(luaState, m_paramList);
		luaState->setMember("paramArray");

		m_initializer.luaExport(luaState);
		luaState->setMember("initializer");
		break;
	}

	m_briefDescription.luaExport(luaState);
	luaState->setMember("briefDescription");

	m_detailedDescription.luaExport(luaState);
	luaState->setMember("detailedDescription");

	m_inBodyDescription.luaExport(luaState);
	luaState->setMember("inBodyDescription");

	m_location.luaExport(luaState);
	luaState->setMember("location");
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
	m_hasGlobalNamespace = false;
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

	luaState->setMemberString("compoundKind", getCompoundKindString(m_compoundKind));
	luaState->setMemberString("id", m_id);
	luaState->setMemberString("name", m_name);
	luaState->setMemberString("title", m_title);

	if (!m_importId.isEmpty())
		luaState->setMemberString("importId", m_importId);

	if (m_groupCompound)
	{
		m_groupCompound->luaExport(luaState);
		luaState->setMember("group");
	}

	preparePath();
	luaState->setMemberString("path", m_path);

	size_t count;

	switch (m_compoundKind)
	{
	case CompoundKind_Group:
		luaState->setMemberBoolean("hasGlobalNamespace", m_hasGlobalNamespace);
		break;

	case CompoundKind_Namespace:
		break;

	case CompoundKind_Page:
		luaExportArray(luaState, m_subPageArray);
		luaState->setMember("subPageArray");
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
		luaState->setMember("templateParamArray");

		luaExportList(luaState, m_templateSpecParamList);
		luaState->setMember("templateSpecParamArray");

		luaExportArray(luaState, m_baseTypeArray);
		luaState->setMember("baseTypeArray");

		count = m_baseTypeProtectionArray.getCount();
		luaState->createTable(count);
		for (size_t i = 0; i < count; i++)
			luaState->setArrayElementString(i + 1, getProtectionKindString(m_baseTypeProtectionArray[i]));

		luaState->setMember("baseTypeProtectionArray");

		// prefer explicitly specified derived type (fallback to auto-generated if absent)

		luaExportArray(
			luaState,
			!m_derivedTypeArray_doxy.isEmpty() ?
				m_derivedTypeArray_doxy :
				m_derivedTypeArray_auto
			);

		luaState->setMember("derivedTypeArray");
		break;
	}

	luaExportStringList(luaState, m_importList);
	luaState->setMember("importArray");

	m_briefDescription.luaExport(luaState);
	luaState->setMember("briefDescription");

	m_detailedDescription.luaExport(luaState);
	luaState->setMember("detailedDescription");

	m_location.luaExport(luaState);
	luaState->setMember("location");

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
	luaState->setMember("groupArray");

	luaExportArray(luaState, m_namespaceArray);
	luaState->setMember("namespaceArray");

	luaExportArray(luaState, m_enumArray);
	luaState->setMember("enumArray");

	luaExportArray(luaState, m_structArray);
	luaState->setMember("structArray");

	luaExportArray(luaState, m_unionArray);
	luaState->setMember("unionArray");

	luaExportArray(luaState, m_classArray);
	luaState->setMember("classArray");

	luaExportArray(luaState, m_interfaceArray);
	luaState->setMember("interfaceArray");

	luaExportArray(luaState, m_protocolArray);
	luaState->setMember("protocolArray");

	luaExportArray(luaState, m_exceptionArray);
	luaState->setMember("exceptionArray");

	luaExportArray(luaState, m_serviceArray);
	luaState->setMember("serviceArray");

	luaExportArray(luaState, m_singletonArray);
	luaState->setMember("singletonArray");

	luaExportArray(luaState, m_typedefArray);
	luaState->setMember("typedefArray");

	luaExportArray(luaState, m_variableArray);
	luaState->setMember("variableArray");

	luaExportArray(luaState, m_constructorArray);
	luaState->setMember("constructorArray");

	if (m_destructor)
	{
		m_destructor->luaExport(luaState);
		luaState->setMember("destructor");
	}

	luaExportArray(luaState, m_functionArray);
	luaState->setMember("functionArray");

	luaExportArray(luaState, m_propertyArray);
	luaState->setMember("propertyArray");

	luaExportArray(luaState, m_eventArray);
	luaState->setMember("eventArray");

	luaExportArray(luaState, m_aliasArray);
	luaState->setMember("aliasArray");

	luaExportArray(luaState, m_defineArray);
	luaState->setMember("defineArray");

	luaExportArray(luaState, m_footnoteArray);
	luaState->setMember("footnoteArray");
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
	const sl::StringRef& auxCompoundId
	)
{
	clear();

	// loop #1 assign groups

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
				if (member && member->m_parentCompound->isMemberGroupAllowed())
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

	if (!auxCompoundId.isEmpty())
	{
		m_auxCompound = module->m_compoundMap.findValue(auxCompoundId, NULL);
		if (m_auxCompound && m_auxCompound->m_groupCompound)
			m_auxCompound->m_groupCompound->m_hasGlobalNamespace = true;
	}

	module->m_groupArray.clear(); // will contain non-empty root groups only

	// loop #2 initialize namespaces (including classes, structs, unions)

	count = module->m_namespaceArray.getCount();
	for (size_t i = 0; i < count; i++)
	{
		Compound* compound = module->m_namespaceArray[i];

		Namespace* nspace = AXL_MEM_NEW(Namespace);
		m_namespaceList.insertTail(nspace);
		nspace->m_compound = compound;
		compound->m_selfNamespace = nspace;
	}

	// loop #3 resolve inner, base and derived references and add members

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
				fprintf(stderr, "warning: can't find inner compound refid: %s\n", refIt->m_id.sz());
				continue;
			}

			nspace->add(innerCompound);
			innerCompound->m_parentNamespace = nspace; // namespace, not group!

			if (innerCompound->m_groupCompound)
			{
				if (compound->isMemberGroupAllowed())
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
				fprintf(stderr, "warning: can't find derived compound refid: %s\n", refIt->m_id.sz());
				continue;
			}

			compound->m_derivedTypeArray_doxy.append(derivedCompound);
		}
	}

	// loop #4 resolve sub pages

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
				fprintf(stderr, "warning: can't find subpage refid: %s\n", refIt->m_id.sz());
				continue;
			}

			compound->m_subPageArray.append(subPage);
			subPage->m_isSubPage = true;
		}
	}

	removeSubPages(&module->m_pageArray);

	// loop #5 add leftovers to the global namespace

	sl::Iterator<Compound> compoundIt = module->m_compoundList.getHead();
	for (; compoundIt; compoundIt++)
	{
		sl::Iterator<Member> memberIt;

		switch (compoundIt->m_compoundKind)
		{
		case CompoundKind_Undefined: // template base type or incomplete compound
			break;

		case CompoundKind_Group:
			if (!compoundIt->m_briefDescription.isEmpty() ||
				!compoundIt->m_detailedDescription.isEmpty())
				getGroupNamespace(module, *compoundIt); // ensure group is added to the tree

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

	luaState->setMemberString("path", "");
	luaState->setMemberString("id", "global");
	luaState->setMemberString("compoundKind", "namespace");

	if (m_auxCompound)
	{
		if (!m_auxCompound->m_title.isEmpty())
			luaState->setMemberString("title", m_auxCompound->m_title);

		m_auxCompound->m_briefDescription.luaExport(luaState);
		luaState->setMember("briefDescription");

		m_auxCompound->m_detailedDescription.luaExport(luaState);
		luaState->setMember("detailedDescription");
	}
	else
	{
		// we still want valid m_briefDescription/m_detailedDescription

		Description emptyDescription;

		emptyDescription.luaExport(luaState);
		luaState->setMember("briefDescription");

		emptyDescription.luaExport(luaState);
		luaState->setMember("detailedDescription");
	}
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
