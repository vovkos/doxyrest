use strict;

my $parent = "Compound";

my @memberArray = (
	"m_declarationName",
	"m_definitionName",
	"m_array",
	"m_type",
	"m_defaultValue",
	"m_typeConstraint",
	"m_briefDescription",
	);

foreach my $m (@memberArray)
{
	print (":ref:`$m <cid-$parent.$m>`\n");
}

print ("\n");

foreach my $m (@memberArray)
{
	print (
		".. _cid-$parent.$m:\n" .
		".. code-block:: lua\n" .
		"	:class: title-code-block\n" .
		"\n" .
		"	$m\n" .
		"\n" .
		"Detailed documentation for ``$m``.\n" .
		"\n"
		);
}