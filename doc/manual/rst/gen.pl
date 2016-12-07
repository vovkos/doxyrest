use strict;

my $parent = "RefText";

my @memberArray = (
	"m_refKind",
	"m_text",
	"m_id",
	"m_external",
	"m_tooltip",
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