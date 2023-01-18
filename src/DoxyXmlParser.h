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

#include "DoxyXmlType.h"
#include "Module.h"

#define _PRINT_XML 0

//..............................................................................

enum DoxyXmlFileKind {
	DoxyXmlFileKind_Index,
	DoxyXmlFileKind_Compound,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DoxyXmlParser: public xml::ExpatParser<DoxyXmlParser> {
	friend class xml::ExpatParser<DoxyXmlParser>;

protected:
	struct TypeStackEntry {
		DoxyXmlType* m_type;
		size_t m_depth;
	};

	enum ElemKind {
		ElemKind_Undefined,
		ElemKind_DoxygenIndex,
		ElemKind_DoxygenCompound,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_ENTRY("doxygenindex", ElemKind_DoxygenIndex)
		AXL_SL_HASH_TABLE_ENTRY("doxygen",      ElemKind_DoxygenCompound)
	AXL_SL_END_HASH_TABLE()

protected:
	Module* m_module;
	DoxyXmlFileKind m_fileKind;
	sl::String m_filePath;
	sl::String m_baseDir;
	sl::Array<TypeStackEntry> m_typeStack;
	sl::Array<Compound*> m_compoundStack;

#if (_PRINT_XML)
	size_t m_indent;
#endif

public:
	DoxyXmlParser();

	~DoxyXmlParser() {
		clear();
	}

	Module*
	getModule() {
		return m_module;
	}

	const sl::String&
	getFilePath() {
		return m_filePath;
	}

	const sl::String&
	getBaseDir() {
		return m_baseDir;
	}

	bool
	parseFile(
		Module* module,
		DoxyXmlFileKind fileKind,
		const sl::StringRef& fileName,
		size_t blockSize = -1
	);

	bool
	parseFile(
		Module* module,
		const sl::StringRef& fileName,
		size_t blockSize = -1
	) {
		return parseFile(module, DoxyXmlFileKind_Index, fileName, blockSize);
	}

	void
	clear();

	sl::String
	getLocationString() const {
		return sl::formatString("%s(%d)", m_filePath.sz(), getLineNumber());
	}

	template <typename T>
	bool
	pushType(
		const char* name,
		const char** attributes
	) {
		T* type = AXL_MEM_NEW(T);
		TypeStackEntry entry = { type, 0 };
		m_typeStack.append(entry);
		return type->create(this, name, attributes);
	}

	template <
		typename T,
		typename Context
	>
	bool
	pushType(
		Context* context,
		const char* name,
		const char** attributes
	) {
		T* type = AXL_MEM_NEW(T);
		if (type->create(this, context, name, attributes)) {
			TypeStackEntry entry = { type, 0 };
			m_typeStack.append(entry);
			return true;
		}
		else {
			return false;
		}
	}

	Compound*
	getCurrentCompound() {
		return !m_compoundStack.isEmpty() ? m_compoundStack.getBack() : NULL;
	}

	size_t
	pushCompound(Compound* compound) {
		return m_compoundStack.append(compound);
	}

	Compound*
	popCompound() {
		return m_compoundStack.getBackAndPop();
	}

protected:
	void
	onStartElement(
		const char* name,
		const char** attributes
	);

	void
	onEndElement(const char* name);

	void
	onCharacterData(
		const char* string,
		size_t length
	);

	void
	popType();

private:
	void
	printLastError() {
		fprintf(stderr, "%s: warning: %s\n", getLocationString().sz(), err::getLastErrorDescription().sz());
	}

#if (_PRINT_XML)
	void
	printIndent();

	void
	printElement(
		const char* name,
		const char** attributes
	);
#endif
};

//..............................................................................
