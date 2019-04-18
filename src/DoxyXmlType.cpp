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
#include "DoxyXmlType.h"
#include "DoxyXmlParser.h"

//..............................................................................

bool
DoxygenIndexType::create(
	DoxyXmlParser* parser,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;

	Module* module = m_parser->getModule();

	while (*attributes)
	{
		IndexAttrKind attrKind = IndexAttrKindMap::findValue(attributes[0], IndexAttrKind_Undefined);
		switch (attrKind)
		{
		case IndexAttrKind_Version:
			module->m_version = attributes[1];
			break;
		}

		attributes += 2;
	}

	return true;
}

bool
DoxygenIndexType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	IndexElemKind elemKind = IndexElemKindMap::findValue(name, IndexElemKind_Undefined);
	switch (elemKind)
	{
	case IndexElemKind_Compound:
		onCompound(name, attributes);
		break;
	}

	return true;
}

bool
DoxygenIndexType::onCompound(
	const char* name,
	const char** attributes
	)
{
	sl::String refId;
	CompoundKind compoundKind;

	while (*attributes)
	{
		CompoundAttrKind attrKind = CompoundAttrKindMap::findValue(attributes[0], CompoundAttrKind_Undefined);
		switch (attrKind)
		{
		case CompoundAttrKind_RefId:
			refId = attributes[1];
			break;

		case CompoundAttrKind_Kind:
			compoundKind = CompoundKindMap::findValue(attributes[1], CompoundKind_Undefined);
			break;
		}

		attributes += 2;
	}

	if (refId.isEmpty())
	{
		// handle missing refid
		return true;
	}

	return parseCompound(refId);
}

bool
DoxygenIndexType::parseCompound(const char* refId)
{
	sl::String filePath = m_parser->getBaseDir() + "/" + refId + ".xml";

	DoxyXmlParser parser;
	return parser.parseFile(
		m_parser->getModule(),
		DoxyXmlFileKind_Compound,
		filePath
		);
}

//..............................................................................

bool
DoxygenCompoundType::create(
	DoxyXmlParser* parser,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;

	Module* module = m_parser->getModule();

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Version:
			if (module->m_version.isEmpty())
			{
				module->m_version = attributes[1];
			}
			else if (module->m_version != attributes[1])
			{
				// handle version mismatch
			}

			break;
		}

		attributes += 2;
	}

	return true;
}

bool
DoxygenCompoundType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_CompoundDef:
		return m_parser->pushType<CompoundDefType> (name, attributes);
	}

	return true;
}

//..............................................................................

bool
CompoundDefType::create(
	DoxyXmlParser* parser,
	const char* name,
	const char** attributes
	)
{
	Module* module = parser->getModule();

	m_parser = parser;
	m_compound = AXL_MEM_NEW(Compound);
	module->m_compoundList.insertTail(m_compound);
	parser->pushCompound(m_compound);

	sl::StringHashTableIterator<Compound*> mapIt;

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Id:
			m_compound->m_id = attributes[1];
			mapIt = module->m_compoundMap.visit(m_compound->m_id);
			if (!mapIt->m_value)
			{
				mapIt->m_value = m_compound;
			}
			else
			{
				Compound* prevCompound = mapIt->m_value;
				printf(
					"warning: duplicate compoud id: %s (%s: %s)\n",
					m_compound->m_id.sz(),
					getCompoundKindString(prevCompound->m_compoundKind),
					prevCompound->m_name.sz()
					);

				if (prevCompound->m_detailedDescription.isEmpty() && prevCompound->m_briefDescription.isEmpty())
				{
					printf("  replacing old compound as it has no documentation\n");
					mapIt->m_value = m_compound;
					prevCompound->m_isDuplicate = true;
				}
				else
				{
					m_compound->m_isDuplicate = true;
				}
			}

			break;

		case AttrKind_Kind:
			m_compound->m_compoundKind = CompoundKindMap::findValue(attributes[1], CompoundKind_Undefined);
			break;

		case AttrKind_Language:
			m_compound->m_languageKind = LanguageKindMap::findValue(attributes[1], LanguageKind_Undefined);
			break;

		case AttrKind_Prot:
			m_compound->m_protectionKind = ProtectionKindMap::findValue(attributes[1], ProtectionKind_Undefined);
			break;

		case AttrKind_Final:
			m_compound->m_isFinal = BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes;
			break;

		case AttrKind_Sealed:
			m_compound->m_isSealed = BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes;
			break;

		case AttrKind_Abstract:
			m_compound->m_isAbstract = BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes;
			break;
		}

		attributes += 2;
	}

	switch (m_compound->m_compoundKind)
	{
	case CompoundKind_Group:
		module->m_groupArray.append(m_compound);
		break;

	case CompoundKind_Page:
		module->m_pageArray.append(m_compound);
		break;

	case CompoundKind_Example:
		module->m_exampleArray.append(m_compound);
		break;

	case CompoundKind_Namespace:
	case CompoundKind_Struct:
	case CompoundKind_Union:
	case CompoundKind_Class:
	case CompoundKind_Interface:
	case CompoundKind_Protocol:
	case CompoundKind_Exception:
	case CompoundKind_Service:
	case CompoundKind_Singleton:
		module->m_namespaceArray.append(m_compound);
		break;
	}

	return true;
}

bool
CompoundDefType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	sl::BoxIterator<sl::String> stringIt;

	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_CompoundName:
		m_parser->pushType<StringType> (&m_compound->m_name, name, attributes);
		break;

	case ElemKind_Title:
		m_parser->pushType<StringType> (&m_compound->m_title, name, attributes);
		break;

	case ElemKind_BaseCompoundRef:
		m_parser->pushType<RefType> (&m_compound->m_baseRefList, name, attributes);
		break;

	case ElemKind_DerivedCompoundRef:
		m_parser->pushType<RefType> (&m_compound->m_derivedRefList, name, attributes);
		break;

	case ElemKind_Includes:
		stringIt = m_compound->m_importList.insertTail();
		m_parser->pushType<StringType> (stringIt.p(), name, attributes);
		break;

	case ElemKind_IncludedBy:
	case ElemKind_IncDepGraph:
	case ElemKind_InvIncDepGraph:
		break;

	case ElemKind_InnerDir:
	case ElemKind_InnerFile:
	case ElemKind_InnerClass:
	case ElemKind_InnerNamespace:
	case ElemKind_InnerPage:
	case ElemKind_InnerGroup:
		m_parser->pushType<RefType> (&m_compound->m_innerRefList, name, attributes);
		break;

	case ElemKind_TemplateParamList:
		m_parser->pushType<TemplateParamListType> (&m_compound->m_templateParamList, name, attributes);
		break;

	case ElemKind_SectionDef:
		m_parser->pushType<SectionDefType> (m_compound, name, attributes);
		break;

	case ElemKind_BriefDescription:
		m_parser->pushType<DescriptionType> (&m_compound->m_briefDescription, name, attributes);
		break;

	case ElemKind_DetailedDescription:
		m_parser->pushType<DescriptionType> (&m_compound->m_detailedDescription, name, attributes);
		break;

	case ElemKind_Location:
		m_parser->pushType<LocationType> (&m_compound->m_location, name, attributes);
		break;

	case ElemKind_InheritanceGraph:
	case ElemKind_CollaborationGraph:
	case ElemKind_ProgramListing:
	case ElemKind_ListOfAllMembers:
		break;
	}

	return true;
}

void
CompoundDefType::onPopType()
{
	ASSERT(m_parser);
	Compound* prevCompound = m_parser->popCompound();
	ASSERT(prevCompound == m_compound);
}

//..............................................................................

bool
RefType::create(
	DoxyXmlParser* parser,
	sl::List<Ref>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_ref = AXL_MEM_NEW(Ref);
	list->insertTail(m_ref);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_RefId:
			m_ref->m_id = attributes[1];
			break;

		case AttrKind_ImportId:
			m_ref->m_importId = attributes[1];
			break;

		case AttrKind_Prot:
			m_ref->m_protectionKind = ProtectionKindMap::findValue(attributes[1], ProtectionKind_Undefined);
			break;

		case AttrKind_Virt:
			m_ref->m_virtualKind = VirtualKindMap::findValue(attributes[1], VirtualKind_Undefined);
			break;
		}

		attributes += 2;
	}

	return true;
}

//..............................................................................

bool
SectionDefType::create(
	DoxyXmlParser* parser,
	Compound* parent,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_parent = parent;

	SectionKind sectionKind = SectionKind_Undefined;

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Kind:
			sectionKind = SectionKindMap::findValue(attributes[1], SectionKind_Undefined);
			break;
		}

		attributes += 2;
	}

	return true;
}

bool
SectionDefType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Header:
		break;

	case ElemKind_Description:
		// ignore description for SectionDefs
		break;

	case ElemKind_MemberDef:
		return m_parser->pushType<MemberDefType> (m_parent, name, attributes);
	}

	return true;
}

//..............................................................................

bool
MemberDefType::create(
	DoxyXmlParser* parser,
	Compound* parent,
	const char* name,
	const char** attributes
	)
{
	Module* module = parser->getModule();

	m_parser = parser;
	m_member = AXL_MEM_NEW(Member);
	m_member->m_parentCompound = parent;
	parent->m_memberList.insertTail(m_member);

	sl::StringHashTableIterator<Member*> mapIt;
	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Kind:
			m_member->m_memberKind = MemberKindMap::findValue(attributes[1], MemberKind_Undefined);
			break;

		case AttrKind_Id:
			m_member->m_id = attributes[1];
			if (parent->m_compoundKind == CompoundKind_Group)
				break; // doxy groups contain duplicated definitions of members

			mapIt = module->m_memberMap.visit(m_member->m_id);
			if (!mapIt->m_value)
			{
				mapIt->m_value = m_member;
			}
			else
			{
				Member* prevMember = mapIt->m_value;
				printf(
					"warning: duplicate member id %s (%s: %s)\n",
					m_member->m_id.sz(),
					getMemberKindString(prevMember->m_memberKind),
					prevMember->m_name.sz()
					);

				if (prevMember->m_detailedDescription.isEmpty() && prevMember->m_briefDescription.isEmpty())
				{
					printf("  replacing old member as it has no documentation\n");
					mapIt->m_value = m_member;
					prevMember->m_flags |= MemberFlag_Duplicate;
				}
				else
				{
					m_member->m_flags |= MemberFlag_Duplicate;
				}
			}

			break;

		case AttrKind_Prot:
			m_member->m_protectionKind = ProtectionKindMap::findValue(attributes[1], ProtectionKind_Undefined);
			break;

		case AttrKind_Static:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Static;
			break;

		case AttrKind_Const:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Const;
			break;

		case AttrKind_Explicit:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Explicit;
			break;

		case AttrKind_Inline:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Inline;
			break;

		case AttrKind_Virtual:
			m_member->m_virtualKind = VirtualKindMap::findValue(attributes[1], VirtualKind_Undefined);
			break;

		case AttrKind_Volatile:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Volatile;
			break;

		case AttrKind_Mutable:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Mutable;
			break;

		case AttrKind_Readable:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Readable;
			break;

		case AttrKind_Writeable:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Writeable;
			break;

		case AttrKind_InitOnly:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_InitOnly;
			break;

		case AttrKind_Settable:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Settable;
			break;

		case AttrKind_Gettable:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Gettable;
			break;

		case AttrKind_Final:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Final;
			break;

		case AttrKind_Sealed:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Sealed;
			break;

		case AttrKind_New:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_New;
			break;

		case AttrKind_Add:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Add;
			break;

		case AttrKind_Remove:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Remove;
			break;

		case AttrKind_Raise:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Raise;
			break;

		case AttrKind_Optional:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Optional;
			break;

		case AttrKind_Required:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Required;
			break;

		case AttrKind_Accessor:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Accessor;
			break;

		case AttrKind_Attribute:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Attribute;
			break;

		case AttrKind_Property:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Property;
			break;

		case AttrKind_ReadOnly:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_ReadOnly;
			break;

		case AttrKind_Bound:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Bound;
			break;

		case AttrKind_Removable:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Removable;
			break;

		case AttrKind_Contrained:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Contrained;
			break;

		case AttrKind_Transient:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_Transient;
			break;

		case AttrKind_MaybeVoid:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_MaybeVoid;
			break;

		case AttrKind_MaybeDefault:
			if (BoolKindMap::findValue(attributes[1], BoolKind_Undefined) == BoolKind_Yes)
				m_member->m_flags |= MemberFlag_MaybeDefault;
			break;
		}

		attributes += 2;
	}

	return true;
}

bool
MemberDefType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	sl::BoxIterator<sl::String> stringIt;

	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Includes:
		stringIt = m_member->m_importList.insertTail();
		m_parser->pushType<StringType> (stringIt.p(), name, attributes);
		break;

	case ElemKind_TemplateParamList:
		m_parser->pushType<TemplateParamListType> (&m_member->m_templateParamList, name, attributes);
		break;

	case ElemKind_Type:
		m_parser->pushType<LinkedTextType> (&m_member->m_type, name, attributes);
		break;

	case ElemKind_Definition:
		m_parser->pushType<StringType> (&m_member->m_definition, name, attributes);
		break;

	case ElemKind_ArgString:
		m_parser->pushType<StringType> (&m_member->m_argString, name, attributes);
		break;

	case ElemKind_Name:
		m_parser->pushType<StringType> (&m_member->m_name, name, attributes);
		break;

	case ElemKind_Read:
	case ElemKind_Write:
		break;

	case ElemKind_BitField:
		m_parser->pushType<StringType> (&m_member->m_bitField, name, attributes);
		break;

	case ElemKind_Initializer:
		m_parser->pushType<LinkedTextType> (&m_member->m_initializer, name, attributes);
		break;

	case ElemKind_Exceptions:
		m_parser->pushType<LinkedTextType> (&m_member->m_exceptions, name, attributes);
		break;

	case ElemKind_Reimplements:
	case ElemKind_ReimplementedBy:
		break;

	case ElemKind_Param:
		m_parser->pushType<ParamType> (&m_member->m_paramList, name, attributes);
		break;

	case ElemKind_EnumValue:
		m_parser->pushType<EnumValueType> (m_member, name, attributes);
		break;

	case ElemKind_BriefDescription:
		m_parser->pushType<DescriptionType> (&m_member->m_briefDescription, name, attributes);
		break;

	case ElemKind_DetailedDescription:
		m_parser->pushType<DescriptionType> (&m_member->m_detailedDescription, name, attributes);
		break;

	case ElemKind_InBodyDescription:
		m_parser->pushType<DescriptionType> (&m_member->m_inBodyDescription, name, attributes);
		break;

	case ElemKind_Location:
		m_parser->pushType<LocationType> (&m_member->m_location, name, attributes);
		break;

	case ElemKind_References:
	case ElemKind_ReferencedBy:
		break;

	case ElemKind_Modifiers:
		m_parser->pushType<StringType> (&m_member->m_modifiers, name, attributes);
		break;
	}

	return true;
}

//..............................................................................

bool
DescriptionType::create(
	DoxyXmlParser* parser,
	Description* description,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_description = description;
	return true;
}

bool
DescriptionType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Title:
		m_parser->pushType<StringType> (&m_description->m_title, name, attributes);
		break;

	case ElemKind_Para:
		m_parser->pushType<DocParaType> (&m_description->m_docBlockList, name, attributes);
		break;

	case ElemKind_Sect1:
	case ElemKind_Internal:
		m_parser->pushType<DocSectionBlockType> (&m_description->m_docBlockList, name, attributes);
		break;
	}

	return true;
}

//..............................................................................

bool
LocationType::create(
	DoxyXmlParser* parser,
	Location* location,
	const char* name,
	const char** attributes
	)
{
	sl::StringHashTableIterator<Member*> mapIt;
	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_File:
			location->m_file = attributes[1];
			break;

		case AttrKind_Line:
			location->m_line = atoi(attributes[1]);
			break;

		case AttrKind_Column:
			location->m_column = atoi(attributes[1]);
			break;

		case AttrKind_BodyFile:
			location->m_bodyFile = attributes[1];
			break;

		case AttrKind_BodyStart:
			location->m_bodyStartLine = atoi(attributes[1]);
			break;

		case AttrKind_BodyEnd:
			location->m_bodyEndLine = atoi(attributes[1]);
			break;
		}

		attributes += 2;
	};

	return true;
}

//..............................................................................

bool
DocSectionBlockType::create(
	DoxyXmlParser* parser,
	sl::List<DocBlock>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_sectionBlock = AXL_MEM_NEW(DocSectionBlock);
	m_sectionBlock->m_blockKind = name;
	list->insertTail(m_sectionBlock);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Id:
			m_sectionBlock->m_id = attributes[1];
			break;
		}

		attributes += 2;
	}

	return true;
}

bool
DocSectionBlockType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Title:
		m_parser->pushType<StringType> (&m_sectionBlock->m_title, name, attributes);
		break;

	case ElemKind_Para:
		m_parser->pushType<DocParaType> (&m_sectionBlock->m_childBlockList, name, attributes);
		break;

	case ElemKind_Sect1:
	case ElemKind_Sect2:
	case ElemKind_Sect3:
	case ElemKind_Sect4:
	case ElemKind_Internal:
		m_parser->pushType<DocSectionBlockType> (&m_sectionBlock->m_childBlockList, name, attributes);
		break;
	}

	return true;
}

//..............................................................................

bool
EnumValueType::create(
	DoxyXmlParser* parser,
	Member* member,
	const char* name,
	const char** attributes
	)
{
	Module* module = parser->getModule();

	m_parser = parser;
	m_enumValue = AXL_MEM_NEW(EnumValue);
	m_enumValue->m_parentEnum = member;
	member->m_enumValueList.insertTail(m_enumValue);

	sl::StringHashTableIterator<EnumValue*> mapIt;
	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Id:
			m_enumValue->m_id = attributes[1];
			ASSERT(member->m_parentCompound);
			if (member->m_parentCompound->m_compoundKind == CompoundKind_Group)
				break; // doxy groups contain duplicated definitions of members

			mapIt = module->m_enumValueMap.visit(m_enumValue->m_id);
			if (!mapIt->m_value)
			{
				mapIt->m_value = m_enumValue;
			}
			else
			{
				EnumValue* prevEnumValue = mapIt->m_value;
				printf(
					"warning: duplicate enum value id %s (%s)\n",
					m_enumValue->m_id.sz(),
					prevEnumValue->m_name.sz()
					);

				if (prevEnumValue->m_detailedDescription.isEmpty() && prevEnumValue->m_briefDescription.isEmpty())
				{
					printf("  replacing old enum value it has no documentation\n");
					mapIt->m_value = m_enumValue;
					prevEnumValue->m_isDuplicate = true;
				}
				else
				{
					m_enumValue->m_isDuplicate = true;
				}
			}

			break;

		case AttrKind_Prot:
			m_enumValue->m_protectionKind = ProtectionKindMap::findValue(attributes[1], ProtectionKind_Undefined);
			break;
		}

		attributes += 2;
	}

	return true;
}

bool
EnumValueType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Name:
		m_parser->pushType<StringType> (&m_enumValue->m_name, name, attributes);
		break;

	case ElemKind_Initializer:
		m_parser->pushType<LinkedTextType> (&m_enumValue->m_initializer, name, attributes);
		break;

	case ElemKind_BriefDescription:
		m_parser->pushType<DescriptionType> (&m_enumValue->m_briefDescription, name, attributes);
		break;

	case ElemKind_DetailedDescription:
		m_parser->pushType<DescriptionType> (&m_enumValue->m_detailedDescription, name, attributes);
		break;
	}

	return true;
}

//..............................................................................

bool
TemplateParamListType::create(
	DoxyXmlParser* parser,
	sl::List<Param>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_list = list;
	return true;
}

bool
TemplateParamListType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Param:
		m_parser->pushType<ParamType> (m_list, name, attributes);
		break;
	}

	return true;
}

//..............................................................................

bool
ParamType::create(
	DoxyXmlParser* parser,
	sl::List<Param>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_param = AXL_MEM_NEW(Param);
	list->insertTail(m_param);

	return true;
}

bool
ParamType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Type:
		m_parser->pushType<LinkedTextType> (&m_param->m_type, name, attributes);
		break;

	case ElemKind_DeclName:
		m_parser->pushType<StringType> (&m_param->m_declarationName, name, attributes);
		break;

	case ElemKind_DefName:
		m_parser->pushType<StringType> (&m_param->m_definitionName, name, attributes);
		break;

	case ElemKind_Array:
		m_parser->pushType<StringType> (&m_param->m_array, name, attributes);
		break;

	case ElemKind_DefVal:
		m_parser->pushType<LinkedTextType> (&m_param->m_defaultValue, name, attributes);
		break;

	case ElemKind_TypeConstraint:
		m_parser->pushType<LinkedTextType> (&m_param->m_typeConstraint, name, attributes);
		break;

	case ElemKind_BriefDescription:
		m_parser->pushType<DescriptionType> (&m_param->m_briefDescription, name, attributes);
		break;
	}

	return true;
}

//..............................................................................

bool
LinkedTextType::create(
	DoxyXmlParser* parser,
	LinkedText* linkedText,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_linkedText = linkedText;
	m_refText = AXL_MEM_NEW(RefText);
	m_linkedText->m_refTextList.insertTail(m_refText);

	return true;
}

bool
LinkedTextType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Ref:
		m_parser->pushType<RefTextType> (m_linkedText, name, attributes);
		break;
	}

	m_refText = AXL_MEM_NEW(RefText);
	m_linkedText->m_refTextList.insertTail(m_refText);
	return true;
}

//..............................................................................

bool
RefTextType::create(
	DoxyXmlParser* parser,
	LinkedText* linkedText,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_refText = AXL_MEM_NEW(RefText);
	linkedText->m_refTextList.insertTail(m_refText);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_RefId:
			m_refText->m_id = attributes[1];
			break;

		case AttrKind_KindRef:
			m_refText->m_refKind = RefKindMap::findValue(attributes[1], RefKind_Undefined);
			break;

		case AttrKind_External:
			m_refText->m_external = attributes[1];
			break;

		case AttrKind_Tooltip:
			m_refText->m_tooltip = attributes[1];
			break;
		}

		attributes += 2;
	}

	return true;
}

//..............................................................................

bool
DocParaType::create(
	DoxyXmlParser* parser,
	sl::List<DocBlock>* blockList,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_paragraphBlock = AXL_MEM_NEW(DocBlock);
	m_paragraphBlock->m_blockKind = name;
	blockList->insertTail(m_paragraphBlock);

	m_textBlock = AXL_MEM_NEW(DocBlock);
	m_paragraphBlock->m_childBlockList.insertTail(m_textBlock);

	return true;
}

bool
DocParaType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Ref:
		m_parser->pushType<DocRefTextType> (&m_paragraphBlock->m_childBlockList, name, attributes);
		break;

	case ElemKind_Anchor:
		m_parser->pushType<DocAnchorType> (&m_paragraphBlock->m_childBlockList, name, attributes);
		break;

	case ElemKind_Image:
		m_parser->pushType<DocImageType> (&m_paragraphBlock->m_childBlockList, name, attributes);
		break;

	case ElemKind_Ulink:
		m_parser->pushType<DocUlinkType> (&m_paragraphBlock->m_childBlockList, name, attributes);
		break;

	case ElemKind_SimpleSect:
		m_parser->pushType<DocSimpleSectionType> (&m_paragraphBlock->m_childBlockList, name, attributes);
		break;

	default:
		m_parser->pushType<DocParaType> (&m_paragraphBlock->m_childBlockList, name, attributes);
	}

	m_textBlock = AXL_MEM_NEW(DocBlock);
	m_paragraphBlock->m_childBlockList.insertTail(m_textBlock);
	return true;
}

//..............................................................................

bool
DocRefTextType::create(
	DoxyXmlParser* parser,
	sl::List<DocBlock>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_refBlock = AXL_MEM_NEW(DocRefBlock);
	m_refBlock->m_module = m_parser->getModule();
	m_refBlock->m_blockKind = name;
	list->insertTail(m_refBlock);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_RefId:
			m_refBlock->m_id = attributes[1];
			break;

		case AttrKind_KindRef:
			m_refBlock->m_refKind = RefKindMap::findValue(attributes[1], RefKind_Undefined);
			break;

		case AttrKind_External:
			m_refBlock->m_external = attributes[1];
			break;
		}

		attributes += 2;
	}

	return true;
}

//..............................................................................

bool
DocAnchorType::create(
	DoxyXmlParser* parser,
	sl::List<DocBlock>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_anchorBlock = AXL_MEM_NEW(DocAnchorBlock);
	m_anchorBlock->m_blockKind = name;
	list->insertTail(m_anchorBlock);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Id:
			m_anchorBlock->m_id = attributes[1];
			break;
		}

		attributes += 2;
	}

	Compound* compound = parser->getCurrentCompound();
	if (m_anchorBlock->m_id.isPrefix("_")) // doxygen citelist pages seem to omit page ids in acnhors; restore those
		m_anchorBlock->m_id.insert(0, compound->m_id);

	m_anchorBlock->m_id.makeLowerCase(); // there is no configuration setting to force Doxygen to case-fold anchors
	return true;
}

//..............................................................................

bool
DocImageType::create(
	DoxyXmlParser* parser,
	sl::List<DocBlock>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_imageBlock = AXL_MEM_NEW(DocImageBlock);
	m_imageBlock->m_blockKind = name;
	list->insertTail(m_imageBlock);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Type:
			m_imageBlock->m_imageKind = ImageKindMap::findValue(attributes[1], ImageKind_Undefined);
			break;

		case AttrKind_Name:
			m_imageBlock->m_name = attributes[1];
			break;

		case AttrKind_Width:
			m_imageBlock->m_width = atoi(attributes[1]);
			break;

		case AttrKind_Height:
			m_imageBlock->m_height = atoi(attributes[1]);
			break;
		}

		attributes += 2;
	}

	return true;
}

//..............................................................................

bool
DocUlinkType::create(
		DoxyXmlParser* parser,
		sl::List<DocBlock>* list,
		const char* name,
		const char** attributes
		)
{
	m_parser = parser;
	m_ulinkBlock = AXL_MEM_NEW(DocUlinkBlock);
	m_ulinkBlock->m_blockKind = name;
	list->insertTail(m_ulinkBlock);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
			case AttrKind_Url:
				m_ulinkBlock->m_url = attributes[1];
				break;

			// add as needed
		}

		attributes += 2;
	}

	return true;
}

//..............................................................................

bool
DocSimpleSectionType::create(
	DoxyXmlParser* parser,
	sl::List<DocBlock>* list,
	const char* name,
	const char** attributes
	)
{
	m_parser = parser;
	m_sectionBlock = AXL_MEM_NEW(DocSimpleSectionBlock);
	m_sectionBlock->m_blockKind = name;
	list->insertTail(m_sectionBlock);

	while (*attributes)
	{
		AttrKind attrKind = AttrKindMap::findValue(attributes[0], AttrKind_Undefined);
		switch (attrKind)
		{
		case AttrKind_Kind:
			m_sectionBlock->m_simpleSectionKind = attributes[1];
			break;
		}

		attributes += 2;
	}

	return true;
}

bool
DocSimpleSectionType::onStartElement(
	const char* name,
	const char** attributes
	)
{
	ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
	switch (elemKind)
	{
	case ElemKind_Para:
		m_parser->pushType<DocParaType> (&m_sectionBlock->m_childBlockList, name, attributes);
		break;
	}

	return true;
}

//..............................................................................
