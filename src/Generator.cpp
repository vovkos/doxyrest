#include "pch.h"
#include "Generator.h"
#include "Module.h"
#include "CmdLine.h"

//.............................................................................

void
Generator::prepare (
	Module* module,	
	GlobalNamespace* globalNamespace
	)
{
	m_stringTemplate.create ();
	m_stringTemplate.m_luaState.registerFunction ("includeFile", includeFile_lua, (intptr_t) this);
	m_stringTemplate.m_luaState.registerFunction ("generateFile", generateFile_lua, (intptr_t) this);
	m_stringTemplate.m_luaState.registerFunction ("getItem", generateFile_lua, (intptr_t) this);

	globalNamespace->luaExport (&m_stringTemplate.m_luaState);

	sl::Iterator <Define> it = m_cmdLine->m_defineList.getHead ();
	for (; it; it++)
	{
		Define* define = *it;
		if (define->m_value.isEmpty ())
			m_stringTemplate.m_luaState.setGlobalBoolean (define->m_name, true);
		else
			m_stringTemplate.m_luaState.setGlobalString (define->m_name, define->m_value);
	}
}

bool
Generator::generate (
	const sl::StringRef& targetFileName,
	const sl::StringRef& frameFileName
	)
{
	bool result;

	sl::String targetFilePath = io::getFullFilePath (targetFileName);
	sl::String targetDir = io::getDir (targetFilePath);
	
	result = io::ensureDirExists (targetDir);
	if (!result)
		return false;

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
	m_stringTemplate.m_luaState.setGlobalString ("g_targetFileName", targetFileName);	
	
	sl::String stringBuffer;

	result = m_stringTemplate.processFile (&stringBuffer, frameFilePath);
	if (!result)
		return false;

	sl::String dir = io::getDir (targetFileName);
	if (!dir.isEmpty ())
	{
		result = io::ensureDirExists (dir);
		if (!result)
			return false;
	}

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
		err::setFormatStringError ("frame '%s' not found", frameFileName);
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
		luaState.error (err::getLastErrorDescription ());
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
		luaState.error (err::getLastErrorDescription ());
		ASSERT (false);
		return -1;
	}

	return 0;
}

//.............................................................................
