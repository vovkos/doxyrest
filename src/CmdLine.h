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

#include "DoxyXmlEnum.h"

//..............................................................................

enum CmdLineFlag
{
	CmdLineFlag_Help              = 0x0001,
	CmdLineFlag_Version           = 0x0002,
	CmdLineFlag_AllowMemberGroups = 0x0004,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Define: sl::ListLink
{
	sl::String m_name;
	sl::String m_value;
	bool m_hasValue;

	Define ()
	{
		m_hasValue = false;
	}
};

struct CmdLine
{
	uint_t m_flags;
	sl::String m_inputFileName;
	sl::String m_outputFileName;
	sl::String m_frameFileName;
	sl::BoxList <sl::String> m_frameDirList;
	sl::List <Define> m_defineList;

	CmdLine ()
	{
		m_flags = 0;
	}
};

//..............................................................................

enum CmdLineSwitchKind
{
	CmdLineSwitchKind_Undefined = 0,
	CmdLineSwitchKind_Help,
	CmdLineSwitchKind_Version,
	CmdLineSwitchKind_OutputFileName,
	CmdLineSwitchKind_FrameFileName,
	CmdLineSwitchKind_FrameDir,
	CmdLineSwitchKind_Define,
	CmdLineSwitchKind_AllowMemberGroups,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		CmdLineSwitchKind_Define,
		"D", "define", "<name>[=<value>]",
		"Define a Lua variable"
		)

	AXL_SL_CMD_LINE_SWITCH (
		CmdLineSwitchKind_AllowMemberGroups,
		"allow-member-groups", NULL,
		"Allow grouping for type members"
		)

AXL_SL_END_CMD_LINE_SWITCH_TABLE ()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	onValue (const sl::StringRef& value)
	{
		m_cmdLine->m_inputFileName = value;
		return true;
	}

	bool
	onSwitch (
		SwitchKind switchKind,
		const sl::StringRef& value
		);

	bool
	finalize ();
};

//..............................................................................
