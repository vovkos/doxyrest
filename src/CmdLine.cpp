#include "pch.h"
#include "CmdLine.h"

//..............................................................................

CmdLine::CmdLine ()
{
	m_flags = 0;
	m_protectionFilter = ProtectionKind_Public;
}

//..............................................................................

bool
CmdLineParser::onSwitch (
	SwitchKind switchKind,
	const sl::StringRef& value
	)
{
	switch (switchKind)
	{
	case CmdLineSwitchKind_Help:
		m_cmdLine->m_flags |= CmdLineFlag_Help;
		break;

	case CmdLineSwitchKind_Version:
		m_cmdLine->m_flags |= CmdLineFlag_Version;
		break;

	case CmdLineSwitchKind_OutputFileName:
		m_cmdLine->m_outputFileName = value;
		break;

	case CmdLineSwitchKind_FrameFileName:
		m_cmdLine->m_frameFileName = value;
		break;

	case CmdLineSwitchKind_FrameDir:
		m_cmdLine->m_frameDirList.insertTail (value);
		break;

	case CmdLineSwitchKind_ProtectionFilter:
		m_cmdLine->m_protectionFilter = ProtectionKindMap::findValue (value, ProtectionKind_Undefined);
		if (!m_cmdLine->m_protectionFilter)
		{
			err::setFormatStringError ("unknown protection '%s'", value.sz ());
			return false;
		}

		break;

	case CmdLineSwitchKind_Define:
		Define* define = AXL_MEM_NEW (Define);
		size_t i = value.find ('=');

		if (i == -1)
		{
			define->m_name = value;
		}
		else
		{
			define->m_name = value.getSubString (0, i);
			define->m_value = value.getSubString (i + 1);
		}

		m_cmdLine->m_defineList.insertTail (define);
		break;
	}

	return true;
}

bool
CmdLineParser::finalize ()
{
	if (m_cmdLine->m_inputFileName.isEmpty ())
	{
		if (!m_cmdLine->m_flags)
			m_cmdLine->m_flags = CmdLineFlag_Help;
	}
	else
	{
		if (m_cmdLine->m_outputFileName.isEmpty ())
			m_cmdLine->m_outputFileName = io::getFileName (m_cmdLine->m_inputFileName) + ".rst";

		if (m_cmdLine->m_frameFileName.isEmpty ())
		{
			err::setError ("master frame file name missing\n");
			return false;
		}
	}

	return true;
}

//..............................................................................
