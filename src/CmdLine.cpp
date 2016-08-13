#include "pch.h"
#include "CmdLine.h"

//.............................................................................

CmdLine::CmdLine ()
{
	m_flags = 0;
	m_namespaceSep = "_";
}

//.............................................................................

bool
CmdLineParser::onSwitch (
	SwitchKind switchKind,
	const char* value
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

	case CmdLineSwitchKind_NamespaceSep:
		m_cmdLine->m_namespaceSep = value;
		break;

	case CmdLineSwitchKind_Define:
		Define* define = AXL_MEM_NEW (Define);
		const char* p = strchr (value, '=');

		if (!p)
		{
			define->m_name = sl::String (value);
		}
		else
		{
			define->m_name = sl::String (value, p - value);
			define->m_value = p + 1;
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
			err::setStringError ("master frame file name missing\n");
			return false;
		}
	}

	return true;
}

//.............................................................................
