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

function updateTarget() {
	var target = $(":target");

	if (target.is("span")) // sphinx uses spans to inject multiple ids
		target = target.parent();

	g_oldTarget.removeClass("target-highlight");
	target.addClass("target-highlight");
	g_oldTarget = target;
}

$(document).ready(updateTarget);
$(window).on('hashchange', updateTarget);

} ());

//..............................................................................
