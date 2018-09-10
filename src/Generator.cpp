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
#include "Generator.h"
#include "Module.h"
#include "CmdLine.h"

//..............................................................................

void
Generator::prepare (
	Module* module,
	GlobalNamespace* globalNamespace
	)
{
	m_stringTemplate.create ();
	m_stringTemplate.m_luaState.registerFunction ("includeFile", includeFile_lua, this);
	m_stringTemplate.m_luaState.registerFunction ("generateFile", generateFile_lua, this);

	m_stringTemplate.m_luaState.createTable ();
	m_stringTemplate.m_luaState.setGlobal ("g_exportCache");

	globalNamespace->luaExport (&m_stringTemplate.m_luaState);
	m_stringTemplate.m_luaState.setGlobal ("g_globalNamespace");

	luaExportArray (&m_stringTemplate.m_luaState, module->m_groupArray);
	m_stringTemplate.m_luaState.setGlobal ("g_groupArray");

	luaExportArray (&m_stringTemplate.m_luaState, module->m_pageArray);
	m_stringTemplate.m_luaState.setGlobal ("g_pageArray");

	luaExportArray (&m_stringTemplate.m_luaState, module->m_exampleArray);
	m_stringTemplate.m_luaState.setGlobal ("g_exampleArray");

	sl::ConstIterator <Define> it = m_cmdLine->m_defineList.getHead ();
	for (; it; it++)
	{
		const Define* define = *it;
		if (define->m_hasValue)
			m_stringTemplate.m_luaState.setGlobalString (define->m_name, define->m_value);
		else
			m_stringTemplate.m_luaState.setGlobalBoolean (define->m_name, true);
	}

	// export cache is only needed during export-time

	m_stringTemplate.m_luaState.pushNil ();
	m_stringTemplate.m_luaState.setGlobal ("g_exportCache");
}

bool
Generator::generate (
	const sl::StringRef& targetFileName,
	const sl::StringRef& frameFileName
	)
{
	bool result;

	sl::String frameFilePath = io::findFilePath (frameFileName, &m_cmdLine->m_frameDirList);
	if (frameFilePath.isEmpty ())
	{
		err::setFormatStringError ("frame file '%s' not found", frameFileName.sz ());
		return false;
	}

	sl::String targetDir = io::getDir (targetFileName);
	result = io::ensureDirExists (targetDir);
	if (!result)
		return false;

	m_frameDir = io::getDir (frameFilePath);
	m_targetDir = io::getFullFilePath (targetDir);

	m_stringTemplate.m_luaState.setGlobalString ("g_frameDir", m_frameDir);
	m_stringTemplate.m_luaState.setGlobalString ("g_targetDir", m_targetDir);
	m_stringTemplate.m_luaState.setGlobalString ("g_targetFileName", targetFileName);

	sl::String stringBuffer;

	result = m_stringTemplate.processFile (&stringBuffer, frameFilePath);
	if (!result)
		return false;

	io::File targetFile;
	result = targetFile.open (targetFileName);
	if (!result)
		return false;

	size_t size = stringBuffer.getLength ();

	result = targetFile.write (stringBuffer, size) != -1;
	if (!result)
		return false;

	targetFile.setSize (size);
	return true;
}

bool
Generator::processFile (
	const sl::StringRef& targetFileName,
	const sl::StringRef& frameFileName,
	size_t baseArgCount
	)
{
	bool result;

	sl::String frameFilePath = io::findFilePath (frameFileName, m_frameDir, &m_cmdLine->m_frameDirList);
	if (frameFilePath.isEmpty ())
	{
		err::setFormatStringError ("frame '%s' not found", frameFileName.sz ());
		return false;
	}

	sl::String prevFrameDir = m_stringTemplate.m_luaState.getGlobalString ("g_frameDir");

	m_frameDir = io::getDir (frameFilePath);
	m_stringTemplate.m_luaState.setGlobalString ("g_frameDir", m_frameDir);

	int top = m_stringTemplate.m_luaState.getTop ();
	ASSERT ((size_t) top >= baseArgCount);
	m_stringTemplate.setArgCount (top - baseArgCount);

	if (targetFileName.isEmpty ())
	{
		result = m_stringTemplate.processFile (NULL, frameFilePath);
		if (!result)
			return false;
	}
	else
	{
		sl::String prevTargetFileName = m_stringTemplate.m_luaState.getGlobalString ("g_targetFileName");
		m_stringTemplate.m_luaState.setGlobalString ("g_targetFileName", targetFileName);

		sl::String targetFilePath = m_targetDir + "/" + targetFileName;
		result = m_stringTemplate.processFileToFile (targetFilePath, frameFilePath);
		if (!result)
			return false;

		m_stringTemplate.m_luaState.setGlobalString ("g_targetFileName", prevTargetFileName);
	}

	m_frameDir = prevFrameDir;
	m_stringTemplate.m_luaState.setGlobalString ("g_frameDir", prevFrameDir);

	return true;
}

int
Generator::includeFile_lua (lua_State* h)
{
	lua::LuaNonOwnerState luaState (h);
	Generator* self = (Generator*) luaState.getContext ();
	ASSERT (self->m_stringTemplate.m_luaState == h);

	sl::StringRef fileName = luaState.getString (1);

	bool result = self->processFile (NULL, fileName, 1);
	if (!result)
	{
		luaState.prepareLastErrorString ();
		luaState.error ();
		ASSERT (false);
		return -1;
	}

	return 0;
}

int
Generator::generateFile_lua (lua_State* h)
{
	lua::LuaNonOwnerState luaState (h);
	Generator* self = (Generator*) luaState.getContext ();
	ASSERT (self->m_stringTemplate.m_luaState == h);

	sl::StringRef targetFileName = luaState.getString (1);
	sl::StringRef frameFileName = luaState.getString (2);

	bool result = self->processFile (targetFileName, frameFileName, 2);
	if (!result)
	{
		luaState.prepareLastErrorString ();
		luaState.error ();
		ASSERT (false);
		return -1;
	}

	return 0;
}

//..............................................................................
