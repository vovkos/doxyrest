#pragma once

#include "DoxyXmlEnum.h"

//.............................................................................

enum CmdLineFlag
{
	CmdLineFlag_Help    = 0x0001,
	CmdLineFlag_Version = 0x0002,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Define: sl::ListLink
{
	sl::String m_name;
	sl::String m_value;
};

struct CmdLine
{
	uint_t m_flags;
	ProtectionKind m_protectionFilter;

	sl::String m_inputFileName;
	sl::String m_outputFileName;
	sl::String m_frameFileName;
	sl::String m_namespaceSep;
	sl::BoxList <sl::String> m_frameDirList;
	sl::StdList <Define> m_defineList;

	CmdLine ();
};

//.............................................................................

enum CmdLineSwitchKind
{
	CmdLineSwitchKind_Undefined = 0,
	CmdLineSwitchKind_Help,
	CmdLineSwitchKind_Version,

	CmdLineSwitchKind_OutputFileName = sl::CmdLineSwitchFlag_HasValue,
	CmdLineSwitchKind_FrameFileName,
	CmdLineSwitchKind_FrameDir,
	CmdLineSwitchKind_NamespaceSep,
	CmdLineSwitchKind_Define,
	CmdLineSwitchKind_ProtectionFilter,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SL_BEGIN_CMD_LINE_SWITCH_TABLE (CmdLineSwitchTable, CmdLineSwitchKind)
	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_Help,
		"h", "help", NULL,
		"Display this help"
		)
	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_Version,
		"v", "version", NULL,
		"Display version of doxyrest"
		)

	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_OutputFileName,
		"o", "output", "<file>",
		"Specify master (index) output file"
		)

	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_FrameFileName,
		"f", "frame", "<file>",
		"Specify Lua master (index) frame file"
		)
			
	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_FrameDir,
		"F", "frame-dir", "<dir>",
		"Add Lua frame directory (multiple allowed)"
		)

	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_NamespaceSep,
		"s", "namespace-sep", "<sep>",
		"Specify namespace separator (defaults to '_')"
		)

	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_ProtectionFilter,
		"p", "protection", "<protection>",
		"Exclude protected items"
		)

	AXL_SL_CMD_LINE_SWITCH_2 (
		CmdLineSwitchKind_Define,
		"D", "define", "<name>[=<value>]",
		"Define a Lua variable"
		)

AXL_SL_END_CMD_LINE_SWITCH_TABLE ()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CmdLineParser: public sl::CmdLineParser <CmdLineParser, CmdLineSwitchTable>
{
	friend class sl::CmdLineParser <CmdLineParser, CmdLineSwitchTable>;

protected:
	CmdLine* m_cmdLine;

public:
	CmdLineParser (CmdLine* cmdLine)
	{
		m_cmdLine = cmdLine;
	}

protected:
	bool
	onValue (const char* value)
	{
		m_cmdLine->m_inputFileName = value;
		return true;
	}

	bool
	onSwitch (
		SwitchKind switchKind,
		const char* value
		);

	bool
	finalize ();
};

//.............................................................................
