--------------------------------------------------------------------------------
--
--  This file is part of the Doxyrest toolkit.
--
--  Doxyrest is distributed under the MIT license.
--  For details see accompanying license.txt file,
--  the public copy of which is also available at:
--  http://tibbo.com/downloads/archive/doxyrest/license.txt
--
--------------------------------------------------------------------------------

function captialize(string)
	local s = string.gsub(string, "^%l", string.upper)
	return s
end

function trimLeadingWhitespace(string)
	local s = string.gsub(string, "^%s+", "")
	return s
end

function trimTrailingWhitespace(string)
	local s = string.gsub(string, "%s+$", "")
	return s
end

function trimWhitespace(string)
	local s = trimLeadingWhitespace(string)
	return trimTrailingWhitespace(s)
end

function getTitle(title, underline)
	if not title or title == "" then
		title = "<Untitled>"
	end

	return title .. "\n" .. string.rep(underline, #title)
end

function replaceCommonSpacePrefix(source, replacement)
	local s = "\n" .. source -- add leading '\n'
	local prefix = nil
	local len = 0

	for newPrefix in string.gmatch(s, "(\n[ \t]*)[^%s]") do
		if not prefix then
			prefix = newPrefix
			len = string.len(prefix)
		else
			local newLen = string.len(newPrefix)
			if newLen < len then
				len = newLen
			end

			for i = 2, len do
				if string.byte(prefix, i) ~= string.byte(newPrefix, i) then
					len = i - 1
					break
				end
			end

			prefix = string.sub(prefix, 1, len)
		end

		if len < 2 then
			break
		end
	end

	if not prefix or len < 2 and replacement == "" then
		return source
	end

	s = string.gsub(s, prefix, "\n" .. replacement) -- replace common prefix
	s = string.sub(s, 2) -- remove leading '\n'

	return s
end

-------------------------------------------------------------------------------
