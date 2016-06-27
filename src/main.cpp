#include "pch.h"
#include "CmdLine.h"
#include "DoxyXmlParser.h"
#include "Module.h"
#include "Generator.h"
#include "version.h"

#define _PRINT_USAGE_IF_NO_ARGUMENTS
#define _PRINT_MODULE 0

//.............................................................................

void
printVersion ()
{
	printf (
		"doxyrest v%d.%d.%d (%s%s)\n",
		VERSION_MAJOR,
		VERSION_MINOR,
		VERSION_REVISION,
		_AXL_CPU_STRING,
		_AXL_DEBUG_SUFFIX
		);
}

void
printUsage ()
{
	printVersion ();

	sl::String helpString = CmdLineSwitchTable::getHelpString ();
	printf ("Usage: doxyrest <options>...\n%s", helpString.cc ());
}

#if _PRINT_MODULE
inline 
void
printIndent (size_t indent)
{
	for (size_t i = 0; i < indent; i++)
		printf ("  ");
}

void
printDocBlock (
	DocBlock const* block,
	size_t indent,
	size_t level
	)
{
	printIndent (indent);

	sl::Iterator <DocBlock> it;

	switch (block->m_docBlockKind)
	{
	case DocBlockKind_Paragraph:
		if (!block->m_title.isEmpty ())
		{
			printf ("\\paragraph %s\n", block->m_title.cc ());
			printIndent (indent);
		}
		else
		{
			printf ("\\paragraph\n");
			printIndent (indent);
		}

		printf ("%s\n", ((DocParagraphBlock*) block)->m_contents.cc ());
		break;
		
	case DocBlockKind_Section:
		if (!block->m_title.isEmpty ())
			printf ("\\sect%d %s\n", level, block->m_title.cc ());
		else
			printf ("\\sect%d\n", level);

		it = ((DocSectionBlock*) block)->m_childBlockList.getHead ();
		for (; it; it++)
			printDocBlock (*it, indent + 1, level + 1);

		break;

	case DocBlockKind_Internal:
		printf ("\\internal\n");

		it = ((DocSectionBlock*) block)->m_childBlockList.getHead ();
		for (; it; it++)
			printDocBlock (*it, indent + 1, level + 1);

		break;
	}

}

void
printDescription (
	Description const* description,
	size_t indent
	)
{
	if (!description->m_title.isEmpty ())
	{
		printIndent (indent);
		printf ("\\title %s\n", description->m_title.cc ());
	}

	sl::Iterator <DocBlock> it = description->m_docBlockList.getHead ();
	for (; it; it++)
		printDocBlock (*it, indent, 1);
}

void 
printEnumValue (EnumValue* enumValue)
{
	printf (
		"  enumValue\n"
		"    name:         %s\n"
		"    initializer:  %s\n",

		enumValue->m_name.cc (),
		enumValue->m_initializer.cc ()
		);

	if (!enumValue->m_briefDescription.isEmpty ())
	{
		printf ("    briefDescription\n");
		printDescription (&enumValue->m_briefDescription, 3);
	}

	if (!enumValue->m_detailedDescription.isEmpty ())
	{
		printf ("    detailedDescription\n");
		printDescription (&enumValue->m_detailedDescription, 3);
	}
}

void 
printEnumValueList (const sl::ConstList <EnumValue>& list)
{
	sl::Iterator <EnumValue> it = list.getHead ();
	for (; it; it++)
		printEnumValue (*it);
}

void
printParam (Param* param)
{
	printf (
		"  param\n"
		"    declarationName: %s\n"
		"    definitionName:  %s\n"
		"    type:            %s\n"
		"    array:           %s\n"
		"    defaultValue:    %s\n"
		"    typeConstraint:  %s\n",

		param->m_declarationName.cc (),
		param->m_definitionName.cc (),
		param->m_type.cc (),
		param->m_array.cc (),
		param->m_defaultValue.cc (),
		param->m_typeConstraint.cc ()
		);

	if (!param->m_briefDescription.isEmpty ())
	{
		printf ("    briefDescription\n");
		printDescription (&param->m_briefDescription, 3);
	}
}

void 
printParamList (const sl::ConstList <Param>& list)
{
	sl::Iterator <Param> it = list.getHead ();
	for (; it; it++)
		printParam (*it);
}

void 
printMember(Member* member)
{
	printf (
		"%s %s\n"
		"  id:           %s\n"
		"  type:         %s\n"
		"  definition:   %s\n"
		"  argString:    %s\n"
		"  bitField:     %s\n"
		"  initializer:  %s\n"
		"  exceptions:   %s\n"
		"  flags:        %s\n",

		getMemberKindString (member->m_memberKind),
		member->m_name.cc (),
		member->m_id.cc (),
		member->m_type.cc (),
		member->m_definition.cc (),
		member->m_argString.cc (),
		member->m_bitField.cc (),
		member->m_initializer.cc (),
		member->m_exceptions.cc (),
		getProtectionKindString (member->m_protectionKind),
		getVirtualKindString (member->m_virtualKind),
		getMemberFlagString (member->m_flags).cc ()
		);

	if (!member->m_briefDescription.isEmpty ())
	{
		printf ("  briefDescription\n");
		printDescription (&member->m_briefDescription, 2);
	}

	if (!member->m_detailedDescription.isEmpty ())
	{
		printf ("  detailedDescription\n");
		printDescription (&member->m_detailedDescription, 2);
	}

	if (!member->m_detailedDescription.isEmpty ())
	{
		printf ("  inBodyDescription\n");
		printDescription (&member->m_inBodyDescription, 2);
	}

	if (member->m_memberKind == MemberKind_Enum)
	{
		printf ("  enumMembers {\n");

		sl::Iterator <EnumValue> it = member->m_enumValueList.getHead ();
		for (; it; it++)
			printEnumValue (*it);

		printf ("  }\n");
	}
	
	if (!member->m_templateParamList.isEmpty ())
	{
		printf ("  templateParamList <\n");
		printParamList (member->m_templateParamList);
		printf ("  >\n");
	}
	
	if (!member->m_paramList.isEmpty ())
	{
		printf ("  paramList (\n");
		printParamList (member->m_paramList);
		printf ("  )\n");
	}
}

void 
printMemberList (const sl::ConstList <Member>& list)
{
	sl::Iterator <Member> it = list.getHead ();
	for (; it; it++)
		printMember (*it);
}

void 
printMemberArray (const sl::Array <Member*>& array)
{
	size_t count = array.getCount ();
	for (size_t i = 0; i < count; i++)
		printMember (array [i]);
}

void 
printCompound (Compound* compound)
{
	printf (
		"%s %s\n"
		"  id:         %s\n"
		"  title:      %s\n"
		"  language:   %s\n"
		"  protection: %s\n"
		"  isFinal:    %s\n"
		"  isSealed:   %s\n"
		"  isAbstract: %s\n",

		getCompoundKindString (compound->m_compoundKind),
		compound->m_name.cc (),
		compound->m_id.cc (),
		compound->m_title.cc (),
		getLanguageKindString (compound->m_languageKind),
		getProtectionKindString (compound->m_protectionKind),
		compound->m_isFinal ? "yes" : "no",
		compound->m_isSealed ? "yes" : "no",
		compound->m_isAbstract ? "yes" : "no"
		);

	if (!compound->m_briefDescription.isEmpty ())
	{
		printf ("  briefDescription\n");
		printDescription (&compound->m_briefDescription, 2);
	}

	if (!compound->m_detailedDescription.isEmpty ())
	{
		printf ("  detailedDescription\n");
		printDescription (&compound->m_detailedDescription, 2);
	}

	sl::Iterator <Member> it = compound->m_memberList.getHead ();
	for (; it; it++)
		printMember (*it);

	printf ("\n");
}

void
printNamespaceContents (NamespaceContents* nspace);

void
printNamespaceArray (const sl::Array <Namespace*>& array)
{
	size_t count = array.getCount ();
	for (size_t i = 0; i < count; i++)
	{
		Namespace* nspace = array [i];

		printf ("namespace %s {\n", nspace->m_compound->m_name.cc ());
		printNamespaceContents (nspace);
		printf ("} // namespace %s {\n", nspace->m_compound->m_name.cc ());
	}
}

void
printNamespaceContents (NamespaceContents* nspace)
{
	if (!nspace->m_namespaceArray.isEmpty ())
	{
		printf ("NAMESPACES\n");
		printNamespaceArray (nspace->m_namespaceArray);
	}

	if (!nspace->m_enumArray.isEmpty ())
	{
		printf ("ENUMS\n");
		printMemberArray (nspace->m_enumArray);
	}

	if (!nspace->m_structArray.isEmpty ())
	{
		printf ("STRUCTS\n");
		printNamespaceArray (nspace->m_structArray);
	}

	if (!nspace->m_unionArray.isEmpty ())
	{
		printf ("UNIONS\n");
		printNamespaceArray (nspace->m_unionArray);
	}

	if (!nspace->m_classArray.isEmpty ())
	{
		printf ("CLASSES\n");
		printNamespaceArray (nspace->m_classArray);
	}

	if (!nspace->m_typedefArray.isEmpty ())
	{
		printf ("TYPEDEFS\n");
		printMemberArray (nspace->m_typedefArray);
	}

	if (!nspace->m_variableArray.isEmpty ())
	{
		printf ("VARIABLES\n");
		printMemberArray (nspace->m_variableArray);
	}

	if (!nspace->m_functionArray.isEmpty ())
	{
		printf ("FUNCTIONS\n");
		printMemberArray (nspace->m_functionArray);
	}

	if (!nspace->m_propertyArray.isEmpty ())
	{
		printf ("PROPERTIES\n");
		printMemberArray (nspace->m_propertyArray);
	}

	if (!nspace->m_eventArray.isEmpty ())
	{
		printf ("EVENTS\n");
		printMemberArray (nspace->m_eventArray);
	}
}
#endif

int
run (CmdLine* cmdLine)
{
	bool result;

	Module module;
	GlobalNamespace globalNamespace;
	DoxyXmlParser parser;
	Generator generator (cmdLine);

	printf ("parsing...\n");

	result = 
		parser.parseFile (&module, cmdLine->m_inputFileName) &&
		globalNamespace.build (&module);

	if (!result)
	{
		printf ("error: %s\n", err::getLastErrorDescription ().cc ());
		return -1;
	}

#if _PRINT_MODULE
	printf ("namespace :: {\n");	
	printNamespaceContents (&globalNamespace);
	printf ("} // namespace :: {\n");
#endif

	printf ("generating...\n");

	result = generator.generate (&globalNamespace, cmdLine->m_outputFileName, cmdLine->m_frameFileName);
	if (!result)
	{
		printf ("error: %s\n", err::getLastErrorDescription ().cc ());
		return -1;
	}

	return 0;
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)
int
wmain (
	int argc,
	wchar_t* argv []
	)
#else
int
main (
	int argc,
	char* argv []
	)
#endif
{
	int result;

	xml::registerExpatErrorProvider ();

	CmdLine cmdLine;
	CmdLineParser parser (&cmdLine);

#ifdef _PRINT_USAGE_IF_NO_ARGUMENTS
	if (argc < 2)
	{
		printUsage ();
		return 0;
	}
#endif

	result = parser.parse (argc, argv);
	if (!result)
	{
		printf ("error parsing command line: %s\n", err::getLastErrorDescription ().cc ());
		return -1;
	}

#if (_AXL_ENV == AXL_ENV_WIN)
	SetConsoleOutputCP (CP_UTF8);
#endif

	result = 0;

	if (cmdLine.m_flags & CmdLineFlag_Help)
		printUsage ();
	else if (cmdLine.m_flags & CmdLineFlag_Version)
		printVersion ();
	else 
		result = run (&cmdLine);

	return result;
}

//.............................................................................
