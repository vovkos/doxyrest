#pragma once

#include "DoxyXmlType.h"
#include "Module.h"

#define _PRINT_XML 0

//.............................................................................

enum DoxyXmlFileKind
{
	DoxyXmlFileKind_Index,
	DoxyXmlFileKind_Compound,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DoxyXmlParser: public xml::ExpatParser <DoxyXmlParser>
{
	friend class xml::ExpatParser <DoxyXmlParser>;

protected:
	struct TypeStackEntry
	{
		DoxyXmlType* m_type;
		size_t m_depth;
	};

	enum ElemKind
	{
		ElemKind_Undefined,
		ElemKind_DoxygenIndex,
		ElemKind_DoxygenCompound,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE_MAP (ElemKindMap, ElemKind)
		AXL_SL_HASH_TABLE_MAP_ENTRY ("doxygenindex", ElemKind_DoxygenIndex)
		AXL_SL_HASH_TABLE_MAP_ENTRY ("doxygen",      ElemKind_DoxygenCompound)
	AXL_SL_END_HASH_TABLE_MAP ()

protected:
	Module* m_module;
	DoxyXmlFileKind m_fileKind;
	sl::String m_filePath;
	sl::String m_baseDir;
	sl::Array <TypeStackEntry> m_typeStack;

#if _PRINT_XML
	size_t m_indent;
#endif

public:
	DoxyXmlParser ();

	~DoxyXmlParser ()
	{
		clear ();
	}

	Module* 
	getModule ()
	{
		return m_module;
	}

	sl::String 
	getFilePath ()
	{
		return m_filePath;
	}

	sl::String 
	getBaseDir ()
	{
		return m_baseDir;
	}

	bool
	parseFile (
		Module* module,
		DoxyXmlFileKind fileKind, 
		const char* fileName,
		size_t blockSize = -1
		);

	bool
	parseFile (
		Module* module,
		const char* fileName,
		size_t blockSize = -1
		)
	{
		return parseFile (module, DoxyXmlFileKind_Index, fileName, blockSize);
	}

	void
	clear ();

	template <typename T>
	bool
	pushType (
		const char* name,
		const char** attributes
		)
	{
		T* type = AXL_MEM_NEW (T);
		TypeStackEntry entry = { type, 0 };
		m_typeStack.append (entry);
		return type->create (this, name, attributes);		
	}

	template <
		typename T,
		typename Context
		>
	bool
	pushType (		
		Context* context,
		const char* name,
		const char** attributes
		)
	{
		T* type = AXL_MEM_NEW (T);
		TypeStackEntry entry = { type, 0 };
		m_typeStack.append (entry);
		return type->create (this, context, name, attributes);
	}

protected:
	void 
	onStartElement (
		const char* name,
		const char** attributes
		);

	void 
	onEndElement (const char* name);

	void 
	onCharacterData (
		const char* string,
		size_t length
		);

	void
	popType ();

private:
#if _PRINT_XML
	void 
	printIndent ();

	void
	printElement (
		const char* name,
		const char** attributes
		);
#endif
};

//.............................................................................

