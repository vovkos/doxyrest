// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

struct CmdLine;

//.............................................................................

class Generator
{
protected:
	const CmdLine* m_cmdLine;

	st::LuaStringTemplate m_stringTemplate;
	sl::String m_frameFilePath;
	sl::String m_frameDir;
	sl::String m_targetDir;
	sl::String m_stringBuffer;

public:
	Generator (const CmdLine* cmdLine)
	{
		m_cmdLine = cmdLine;
	}

	void
	prepare (class GlobalNamespace* globalNamespace);
	
	bool
	generate (
		const char* targetFileName,
		const char* frameFileName
		);

	bool
	generate (
		class GlobalNamespace* globalNamespace,
		const char* targetFileName,
		const char* frameFileName
		)
	{
		prepare (globalNamespace);
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
		const char* targetFileName,
		const char* frameFileName
		);
};

//.............................................................................
