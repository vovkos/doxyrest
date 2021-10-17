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

//..............................................................................

bool
Generator::create(const CmdLine* cmdLine) {
	bool result = m_stringTemplate.create();
	if (!result)
		return false;

	if (!cmdLine->m_configFileName.isEmpty()) {
		result = m_stringTemplate.m_luaState.doFile(cmdLine->m_configFileName);
		if (!result)
			return false;
	}

	sl::ConstIterator<Define> it = cmdLine->m_defineList.getHead();
	for (; it; it++) {
		const Define* define = *it;
		if (define->m_hasValue)
			m_stringTemplate.m_luaState.setGlobalString(define->m_name, define->m_value);
		else
			m_stringTemplate.m_luaState.setGlobalBoolean(define->m_name, true);
	}

	m_stringTemplate.m_luaState.getGlobal("FRAME_DIR_LIST");
	size_t count = m_stringTemplate.m_luaState.getRawLen();
	for (size_t i = 1; i <= count; i++) {
		m_stringTemplate.m_luaState.getArrayElement(i);
		sl::String dir = m_stringTemplate.m_luaState.popString();
		m_frameDirList.insertTail(dir);
	}

	sl::ConstBoxIterator<sl::String> it2 = cmdLine->m_frameDirList.getHead();
	for (; it2; it2++)
		m_frameDirList.insertTail(*it2);

	m_stringTemplate.m_luaState.pop();

	m_frameFileName = !cmdLine->m_frameFileName.isEmpty() ?
		cmdLine->m_frameFileName :
		m_stringTemplate.m_luaState.getGlobalString("FRAME_FILE");

	if (m_frameFileName.isEmpty())
		m_frameFileName = g_defaultFrameFileName;

	if (!io::findFilePath(m_frameFileName, &m_frameDirList)) {
		err::setFormatStringError("master frame file %s name missing", m_frameFileName.sz());
		return false;
	}

	m_outputFileName = !cmdLine->m_outputFileName.isEmpty() ?
		cmdLine->m_outputFileName :
		m_stringTemplate.m_luaState.getGlobalString("OUTPUT_FILE");

	if (m_outputFileName.isEmpty())
		m_outputFileName = g_defaultOutputFileName;

	return true;
}

bool
Generator::luaExport(
	Module* module,
	GlobalNamespace* globalNamespace
) {
	m_stringTemplate.m_luaState.registerFunction("includeFile", includeFile_lua, this);
	m_stringTemplate.m_luaState.registerFunction("includeFileWithIndent", includeFileWithIndent_lua, this);
	m_stringTemplate.m_luaState.registerFunction("generateFile", generateFile_lua, this);
	m_stringTemplate.m_luaState.registerFunction("findMemberById", findMemberById_lua, module);
	m_stringTemplate.m_luaState.registerFunction("findCompoundById", findCompoundById_lua, module);

	m_stringTemplate.m_luaState.createTable();
	m_stringTemplate.m_luaState.setGlobal("g_exportCache");

	globalNamespace->luaExport(&m_stringTemplate.m_luaState);
	m_stringTemplate.m_luaState.setGlobal("g_globalNamespace");

	luaExportArray(&m_stringTemplate.m_luaState, module->m_groupArray);
	m_stringTemplate.m_luaState.setGlobal("g_groupArray");

	luaExportArray(&m_stringTemplate.m_luaState, module->m_pageArray);
	m_stringTemplate.m_luaState.setGlobal("g_pageArray");

	luaExportArray(&m_stringTemplate.m_luaState, module->m_exampleArray);
	m_stringTemplate.m_luaState.setGlobal("g_exampleArray");

	return true;
}

bool
Generator::generate(
	const sl::StringRef& targetFileName,
	const sl::StringRef& frameFileName
) {
	bool result;

	sl::String frameFilePath = io::findFilePath(frameFileName, &m_frameDirList);
	if (frameFilePath.isEmpty()) {
		err::setFormatStringError("frame file '%s' not found", frameFileName.sz());
		return false;
	}

	sl::String targetDir = io::getDir(targetFileName);
	result = io::ensureDirExists(targetDir);
	if (!result)
		return false;

	m_frameDir = io::getDir(frameFilePath);
	m_targetDir = io::getFullFilePath(targetDir);

	m_stringTemplate.m_luaState.setGlobalString("g_frameDir", m_frameDir);
	m_stringTemplate.m_luaState.setGlobalString("g_targetDir", m_targetDir);
	m_stringTemplate.m_luaState.setGlobalString("g_targetFileName", targetFileName);

	return m_stringTemplate.processFileToFile(targetFileName, frameFilePath);
}

bool
Generator::processFile(
	const sl::StringRef& indent,
	const sl::StringRef& targetFileName,
	const sl::StringRef& frameFileName,
	size_t baseArgCount
) {
	bool result;

	sl::String frameFilePath = io::findFilePath(frameFileName, m_frameDir, &m_frameDirList);
	if (frameFilePath.isEmpty()) {
		err::setFormatStringError("frame '%s' not found", frameFileName.sz());
		return false;
	}

	sl::String prevFrameDir = m_stringTemplate.m_luaState.getGlobalString("g_frameDir");

	m_frameDir = io::getDir(frameFilePath);
	m_stringTemplate.m_luaState.setGlobalString("g_frameDir", m_frameDir);

	int top = m_stringTemplate.m_luaState.getTop();
	ASSERT((size_t)top >= baseArgCount);
	m_stringTemplate.setArgCount(top - baseArgCount);

	if (!targetFileName.isEmpty()) {
		sl::String prevTargetFileName = m_stringTemplate.m_luaState.getGlobalString("g_targetFileName");
		m_stringTemplate.m_luaState.setGlobalString("g_targetFileName", targetFileName);

		sl::String targetFilePath = io::concatFilePath(m_targetDir, targetFileName);
		result = m_stringTemplate.processFileToFile(targetFilePath, frameFilePath);
		if (!result)
			return false;

		m_stringTemplate.m_luaState.setGlobalString("g_targetFileName", prevTargetFileName);
	} else if (!indent.isEmpty()) {
		sl::String contents;
		result = m_stringTemplate.processFile(&contents, frameFilePath);
		if (!result)
			return false;

		size_t offset = 0;

		for (;;) {
			contents.insert(offset, indent);
			offset = contents.find('\n', offset + indent.getLength());
			if (offset >= contents.getLength() - 1) // don't indent pre-eof
				break;

			offset++; // skip \n
		}

		m_stringTemplate.append(contents);
	} else {
		result = m_stringTemplate.processFile(NULL, frameFilePath);
		if (!result)
			return false;
	}

	m_frameDir = prevFrameDir;
	m_stringTemplate.m_luaState.setGlobalString("g_frameDir", prevFrameDir);

	return true;
}

int
Generator::includeFile_lua(lua_State* h) {
	lua::LuaNonOwnerState luaState(h);
	Generator* self = (Generator*)luaState.getContext();
	ASSERT(self->m_stringTemplate.m_luaState == h);

	sl::StringRef fileName = luaState.getString(1);

	bool result = self->processFile(NULL, NULL, fileName, 1);
	if (!result) {
		luaState.prepareLastErrorString();
		luaState.error();
		ASSERT(false);
		return -1;
	}

	return 0;
}

int
Generator::includeFileWithIndent_lua(lua_State* h) {
	lua::LuaNonOwnerState luaState(h);
	Generator* self = (Generator*)luaState.getContext();
	ASSERT(self->m_stringTemplate.m_luaState == h);

	sl::StringRef indent = luaState.getString(1);
	sl::StringRef fileName = luaState.getString(2);

	sl::String contents;
	bool result = self->processFile(indent, NULL, fileName, 2);
	if (!result) {
		luaState.prepareLastErrorString();
		luaState.error();
		ASSERT(false);
		return -1;
	}

	return 0;
}

int
Generator::generateFile_lua(lua_State* h) {
	lua::LuaNonOwnerState luaState(h);
	Generator* self = (Generator*)luaState.getContext();
	ASSERT(self->m_stringTemplate.m_luaState == h);

	sl::StringRef targetFileName = luaState.getString(1);
	sl::StringRef frameFileName = luaState.getString(2);

	bool result = self->processFile(NULL, targetFileName, frameFileName, 2);
	if (!result) {
		luaState.prepareLastErrorString();
		luaState.error();
		ASSERT(false);
		return -1;
	}

	return 0;
}

int
Generator::findMemberById_lua(lua_State* h) {
	lua::LuaNonOwnerState luaState(h);
	Module* module = (Module*)luaState.getContext();

	sl::StringRef id = luaState.getString(1);
	Member* member = module->m_memberMap.findValue(id, NULL);
	if (!member)
		luaState.pushNil(); // not found
	else {
		ASSERT(member->m_cacheIdx != -1); // should already be in the cache
		member->luaExport(&luaState);
	}

	return 1;
}

int
Generator::findCompoundById_lua(lua_State* h) {
	lua::LuaNonOwnerState luaState(h);
	Module* module = (Module*)luaState.getContext();

	sl::StringRef id = luaState.getString(1);
	Compound* compound = module->m_compoundMap.findValue(id, NULL);
	if (!compound)
		luaState.pushNil(); // not found
	else {
		ASSERT(compound->m_cacheIdx != -1); // should already be in the cache
		compound->luaExport(&luaState);
	}

	return 1;
}

//..............................................................................
