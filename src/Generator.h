// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

struct CmdLine;
struct Module;
class GlobalNamespace;

//..............................................................................

class Generator
{
protected:
	const CmdLine* m_cmdLine;

	st::LuaStringTemplate m_stringTemplate;
	sl::String m_frameFilePath;
	sl::String m_frameDir;
	sl::String m_targetDir;

public:
	Generator (const CmdLine* cmdLine)
	{
		m_cmdLine = cmdLine;
	}

	void
	prepare (
		Module* module,
		GlobalNamespace* globalNamespace
		);

	bool
	generate (
		const sl::StringRef& targetFileName,
		const sl::StringRef& frameFileName
		);

	bool
	generate (
		Module* module,
		GlobalNamespace* globalNamespace,
		const sl::StringRef& targetFileName,
		const sl::StringRef& frameFileName
		)
	{
		prepare (module, globalNamespace);
		return generate (targetFileName, frameFileName);
	}

protected:
	static
	int
	includeFile_lua (lua_State* h);

	static
	int
	generateFile_lua (lua_State* h);

	bool
	processFile (
		const sl::StringRef& targetFileName,
		const sl::StringRef& frameFileName,
		size_t baseArgCount
		);
};

//..............................................................................
