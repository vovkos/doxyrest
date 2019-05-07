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

(function () {

var g_oldTarget = $([]);

function getPreFromSpan(span) {
	var parent = span.parent();

	// ascend to the top-level <span>

	while (parent.is("span"))
	{
		span = parent;
		parent = parent.parent();
	}

	// check the direct parent of the top-level <span>

	if (parent.is("pre"))
		return parent;

	// nope, skip all sibling spans

	var next = span.next();
	while (next.is("span"))
		next = next.next();

	if (next.is("pre"))
		return next;

	return null;
}

function updateTarget() {
	var target = $(":target");

	// sphinx uses spans to inject multiple ids

	if (target.is("span")) {
		pre = getPreFromSpan(target);
		if (pre)
			target = pre;
	}

	g_oldTarget.removeClass("target-highlight");
	target.addClass("target-highlight");
	g_oldTarget = target;
}

$(document).ready(updateTarget);
$(window).on('hashchange', updateTarget);

} ());

//..............................................................................
