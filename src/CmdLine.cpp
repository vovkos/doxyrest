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
#include "CmdLine.h"

//..............................................................................

bool
CmdLineParser::onSwitch(
	SwitchKind switchKind,
	const sl::StringRef& value
) {
	switch (switchKind) {
	case CmdLineSwitchKind_Help:
		m_cmdLine->m_flags |= CmdLineFlag_Help;
		break;

	case CmdLineSwitchKind_Version:
		m_cmdLine->m_flags |= CmdLineFlag_Version;
		break;

	case CmdLineSwitchKind_ConfigFileName:
		m_cmdLine->m_configFileName = value;
		break;

	case CmdLineSwitchKind_OutputFileName:
		m_cmdLine->m_outputFileName = value;
		break;

	case CmdLineSwitchKind_FrameFileName:
		m_cmdLine->m_frameFileName = value;
		break;

	case CmdLineSwitchKind_FrameDir:
		m_cmdLine->m_frameDirList.insertTail(value);
		break;

	case CmdLineSwitchKind_Define:
		Define* define = new Define;
		size_t i = value.find('=');

		if (i == -1) {
			define->m_name = value;
		} else {
			define->m_name = value.getSubString(0, i);
			define->m_value = value.getSubString(i + 1);
			define->m_hasValue = true; // even if value empty
		}

		m_cmdLine->m_defineList.insertTail(define);
		break;
	}

	return true;
}

bool
CmdLineParser::finalize() {
	if (m_cmdLine->m_configFileName.isEmpty())
		if (io::doesFileExist(g_defaultConfigFileName))
			m_cmdLine->m_configFileName = g_defaultConfigFileName;

	if (m_cmdLine->m_inputFileName.isEmpty() &&
		m_cmdLine->m_configFileName.isEmpty () &&
		!m_cmdLine->m_flags)
		m_cmdLine->m_flags = CmdLineFlag_Help;

	return true;
}

//..............................................................................
