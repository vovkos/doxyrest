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

#include "CmdLine.h"

struct Module;
class GlobalNamespace;

//..............................................................................

class Generator
{
protected:
	st::LuaStringTemplate m_stringTemplate;
	sl::String m_frameFilePath;
	sl::String m_frameDir;
	sl::String m_targetDir;
	sl::BoxList<sl::String> m_frameDirList;
	sl::String m_frameFileName;
	sl::String m_outputFileName;

public:
	bool
	create(const CmdLine* cmdLine);

	sl::String
	getConfigValue(const sl::StringRef& name)
	{
		return m_stringTemplate.m_luaState.getGlobalString(name);
	}

	bool
	luaExport(
		Module* module,
		GlobalNamespace* globalNamespace
		);

	bool
	generate()
	{
		return generate(m_outputFileName, m_frameFileName);
	}

	bool
	generate(
		const sl::StringRef& targetFileName,
		const sl::StringRef& frameFileName
		);

protected:
	static
	int
	includeFile_lua(lua_State* h);

	static
	int
	includeFileWithIndent_lua(lua_State* h);

	static
	int
	generateFile_lua(lua_State* h);

	bool
	processFile(
		const sl::StringRef& indent,
		const sl::StringRef& targetFileName,
		const sl::StringRef& frameFileName,
		size_t baseArgCount
		);
};

//..............................................................................
