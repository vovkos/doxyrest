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
#include "DoxyXmlParser.h"
#include "DoxyXmlType.h"

//..............................................................................

DoxyXmlParser::DoxyXmlParser() {
	m_module = NULL;
	m_fileKind = DoxyXmlFileKind_Index;

#if (_PRINT_XML)
	m_indent = 0;
#endif
}

bool
DoxyXmlParser::parseFile(
	Module* module,
	DoxyXmlFileKind fileKind,
	const sl::StringRef& fileName,
	size_t blockSize
) {
	m_module = module;
	m_fileKind = fileKind;
	m_filePath = io::getFullFilePath(fileName);
	m_baseDir = io::getDir(m_filePath);

	return xml::ExpatParser<DoxyXmlParser>::parseFile(fileName);
}

void
DoxyXmlParser::clear() {
	size_t count = m_typeStack.getCount();
	for (size_t i = 0; i < count; i++) {
		DoxyXmlType* type = m_typeStack[i].m_type;
		delete type;
	}

	m_typeStack.clear();
}

void
DoxyXmlParser::popType() {
	ASSERT(!m_typeStack.isEmpty());

	DoxyXmlType* type = m_typeStack.getBack().m_type;
	type->onPopType();
	delete type;

	m_typeStack.pop();
}

void
DoxyXmlParser::onStartElement(
	const char* name,
	const char** attributes
) {
#if (_PRINT_XML)
	printElement(name, attributes);
	m_indent++;
#endif

	if (m_typeStack.isEmpty()) {
		ElemKind elemKind = ElemKindMap::findValue(name, ElemKind_Undefined);
		switch (elemKind) {
		case ElemKind_DoxygenIndex:
			pushType<DoxygenIndexType>(name, attributes);
			break;

		case ElemKind_DoxygenCompound:
			pushType<DoxygenCompoundType>(name, attributes);
			break;
		}
	} else {
		TypeStackEntry* entry = &m_typeStack.getBack();

		if (entry->m_depth != 0) {
			entry->m_depth++;
		} else {
			bool result = entry->m_type->onStartElement(name, attributes);
			if (!result)
				printLastError();

			if (entry == &m_typeStack.getBack()) // no new types were added
				entry->m_depth++;
		}
	}
}

void
DoxyXmlParser::onEndElement(const char* name) {
#if (_PRINT_XML)
	m_indent--;
	printIndent();
	printf("</%s>\n", name);
#endif

	if (m_typeStack.isEmpty())
		return;

	TypeStackEntry* entry = &m_typeStack.getBack();
	if (entry->m_depth != 0) {
		entry->m_depth--;
	} else {
		bool result = entry->m_type->onEndElement(name);
		if (!result)
			printLastError();

		popType();
	}
}

void
DoxyXmlParser::onCharacterData(
	const char* string,
	size_t length
) {
#if (_PRINT_XML)
	printIndent();
	printf("%s\n", sl::String(string, length).sz());
#endif

	if (m_typeStack.isEmpty())
		return;

	TypeStackEntry* entry = &m_typeStack.getBack();
	if (entry->m_depth == 0) {
		bool result = entry->m_type->onCharacterData(string, length);
		if (!result)
			printLastError();
	}
}

#if (_PRINT_XML)
void
DoxyXmlParser::printIndent() {
	for (size_t i = 0; i < m_indent; i++)
		printf("  ");
}

void
DoxyXmlParser::printElement(
	const char* name,
	const char** attributes
) {
	printIndent();
	printf("<%s", name);

	if (*attributes) {
		printf("\n");
		m_indent++;

		while (*attributes) {
			printIndent();
			printf("%s = %s\n", attributes [0], attributes [1]);
			attributes += 2;
		}

		m_indent--;
		printIndent();
	}

	printf(">\n");
}
#endif

//..............................................................................
