#include "pch.h"
#include "Generator.h"
#include "Module.h"
#include "CmdLine.h"

//.............................................................................

void
Generator::prepare (GlobalNamespace* globalNamespace)
{
	m_stringTemplate.create ();
	globalNamespace->luaExport (&m_stringTemplate.m_luaState);
	m_stringTemplate.m_luaState.registerFunction ("includeFile", includeFile_lua, (intptr_t) this);
	m_stringTemplate.m_luaState.registerFunction ("generateFile", generateFile_lua, (intptr_t) this);
	m_stringTemplate.m_luaState.registerFunction ("getItem", generateFile_lua, (intptr_t) this);
}

bool
Generator::generate (
	const char* targetFileName,
	const char* frameFileName
	)
{
	bool result;

	sl::String targetFilePath = io::getFullFilePath (targetFileName);
	sl::String frameFilePath = io::findFilePath (frameFileName, NULL, &m_cmdLine->m_frameDirList);
	if (frameFilePath.isEmpty ())
	{
		err::setFormatStringError ("frame file '%s' not found", frameFileName);
		return false;
	}

	m_frameDir = io::getDir (frameFilePath);
	m_targetDir = io::getDir (targetFilePath);

	m_stringTemplate.m_luaState.setGlobalString ("g_frameDir", m_frameDir);
	m_stringTemplate.m_luaState.setGlobalString ("g_targetDir", m_targetDir);
	m_stringBuffer.clear ();

	result = m_stringTemplate.processFile (&m_stringBuffer, frameFilePath);
	if (!result)
		return false;

	io::File targetFile;
	result = targetFile.open (targetFileName);
	if (!result)
		return false;

	size_t size = m_stringBuffer.getLength ();

	result = targetFile.write (m_stringBuffer, size) != -1;
	if (!result)
		return false;

	targetFile.setSize (size);
	return true;
}

bool
Generator::processFile (
	const char* targetFileName,
	const char* frameFileName,
	size_t baseArgCount
	)
{
	bool result;

	sl::String frameFilePath = io::findFilePath (frameFileName, m_frameDir, &m_cmdLine->m_frameDirList);
	if (frameFilePath.isEmpty ())
	{
		err::setFormatStringError ("frame '%s' not found", frameFileName);
		return false;
	}

	sl::String prevFrameDir = m_frameDir;

	m_frameDir = io::getDir (frameFilePath);
	m_stringTemplate.m_luaState.setGlobalString ("g_frameDir", m_frameDir);

	int top = m_stringTemplate.m_luaState.getTop ();
	ASSERT ((size_t) top >= baseArgCount);
	m_stringTemplate.setArgCount (top - baseArgCount);

	if (!targetFileName)
	{
		sl::String stringBuffer;
		result = m_stringTemplate.processFile (&stringBuffer, frameFilePath);
		if (!result)
			return false;

		m_stringBuffer.append (stringBuffer);
	}
	else
	{
		sl::String targetFilePath = m_targetDir + "/" + targetFileName;
		result = m_stringTemplate.processFileToFile (targetFilePath, frameFilePath);
		if (!result)
			return false;
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

	const char* fileName = luaState.getString (1);

	bool result = self->processFile (NULL, fileName, 1);
	if (!result)
	{
		luaState.setStringError (err::getLastErrorDescription ());
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

	const char* targetFileName = luaState.getString (1);
	const char* frameFileName = luaState.getString (2);

	bool result = self->processFile (targetFileName, frameFileName, 2);
	if (!result)
	{
		luaState.setStringError (err::getLastErrorDescription ());
		return -1;
	}

	return 0;
}

//.............................................................................
