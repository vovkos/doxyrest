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

function concatDocBlockContents(s1, s2)
	local length1 = string.len(s1)
	local length2 = string.len(s2)

	if length1 == 0 then
		return s2
	elseif length2 == 0 then
		return s1
	end

	local last = string.sub(s1, -1, -1)
	local first = string.sub(s2, 1, 1)

	if string.match(last, "%s") or string.match(first, "%s") then
		return s1 .. s2
	else
		return s1 .. " " .. s2
	end
end

function processListDocBlock(block, context, bullet)
	local s

	local prevBullet = context.listItemBullet
	context.listItemBullet = bullet

	s = getDocBlockListContentsImpl(block.childBlockList, context)
	s = "\n\n" .. trimTrailingWhitespace(s) .. "\n\n"

	context.listItemBullet = prevBullet

	return s
end

function processDlListDocBlock(block, context)
	local prevList = context.dlList
	context.dlList = {}

	getDocBlockListContentsImpl(block.childBlockList, context)

	local s =
		"\n\n.. list-table::\n" ..
		"\t:widths: 20 80\n\n"

	for i = 1, #context.dlList do
		s = s .. "\t*\n"

		local entry = context.dlList[i]
		local title = replaceCommonSpacePrefix(entry.title, "\t\t  ")
		local description = replaceCommonSpacePrefix(entry.description, "\t\t  ")

		s = s .. "\t\t- " .. trimLeadingWhitespace(title) .. "\n\n"
		s = s .. "\t\t- " .. trimLeadingWhitespace(description) .. "\n\n"
	end

	context.dlList = prevList

	return s
end

function getCodeDocBlockContents(block, context)
	context.codeBlockKind = block.blockKind
	local s = getDocBlockListContentsImpl(block.childBlockList, context)
	context.codeBlockKind = nil

	return s
end

function getDocBlockContents(block, context)
	local s = ""

	if block.blockKind == "computeroutput" then
		if context.codeBlockKind then
			s = block.text .. getDocBlockListContentsImpl(block.childBlockList, context)
		else
			local code = block.text .. getCodeDocBlockContents(block, context)

			if not string.find(code, "\n") then
				s = "``" .. trimWhitespace(code) .. "``"
			else
				code = replaceCommonSpacePrefix(code, "\t")
				code = trimTrailingWhitespace(code)
				s = "\n\n.. code-block:: none\n\n" .. code
			end
		end
	elseif block.blockKind == "programlisting" then
		local code = getCodeDocBlockContents(block, context)
		code = replaceCommonSpacePrefix(code, "\t")
		code = trimTrailingWhitespace(code)

		s = "\n\n.. ref-code-block:: " .. LANGUAGE .. "\n\n" .. code .. "\n\n"
	elseif block.blockKind == "preformatted" then
		local code = getCodeDocBlockContents(block, context)
		code = replaceCommonSpacePrefix(code, "\t")
		code = trimTrailingWhitespace(code)

		-- raw seems like a better approach, but need to figure something out with indents
		s = "\n\n.. code-block:: none\n\n" .. code .. "\n\n"
	elseif block.blockKind == "formula" then
		local code = getCodeDocBlockContents(block, context)
		local isInline = string.sub(code, 1, 1) == "$"

		-- take away framing tokens
		code = string.gsub(code, "^\\?[$%[]", "")
		code = string.gsub(code, "\\?[$%]]$", "")

		if isInline then
			s = ":math:`" .. code .. "`"
		else
			code = replaceCommonSpacePrefix(code, "\t")
			code = trimTrailingWhitespace(code)

			-- raw seems like a better approach, but need to figure something out with indents
			s = "\n\n.. math::\n\n" .. code .. "\n\n"
		end
	elseif block.blockKind == "verbatim" and VERBATIM_TO_CODE_BLOCK then
		local code = getCodeDocBlockContents(block, context)
		code = replaceCommonSpacePrefix(code, "\t")
		code = trimTrailingWhitespace(code)

		-- probably also need to assign some div class
		s = "\n\n.. code-block:: " .. VERBATIM_TO_CODE_BLOCK .. "\n\n" .. code .. "\n\n"
	elseif block.blockKind == "itemizedlist" then
		s = processListDocBlock(block, context, "*")
	elseif block.blockKind == "orderedlist" then
		s = processListDocBlock(block, context, "#.")
	elseif block.blockKind == "variablelist" then
		s = processDlListDocBlock(block, context)
	else
		local text = block.text
		local childContents = getDocBlockListContentsImpl(block.childBlockList, context)

		if not context.codeBlockKind then
			if ESCAPE_ASTERISKS then
				text = string.gsub(text, "%*", "\\*")
			end

			text = trimWhitespace(text)
			text = concatDocBlockContents(text, childContents)
		else
			text = text .. childContents

			if context.codeBlockKind ~= "programlisting" then
				return text
			end
		end

		if block.blockKind == "linebreak" then
			s = "\n\n"
		elseif block.blockKind == "ref" then
			text = string.gsub(text, "<", "\\<") -- escape left chevron
			s = ":ref:`" .. text .. " <doxid-" .. block.id .. ">`"
		elseif block.blockKind == "anchor" then
			s = ":target:`doxid-" .. block.id .. "`" .. text
		elseif block.blockKind == "image" then
			s = "\n\n.. image:: " .. block.name .. "\n"
			if block.width ~= 0 then
				s = s .. "\t:width: " .. block.width .. "\n"
			end
			if block.height ~= 0 then
				s = s .. "\t:height: " .. block.height .. "\n"
			end
			if block.text ~= "" then
				s = s .. "\t:alt: " .. block.text .. "\n"
			end
			s = s .. "\n"
		elseif block.blockKind == "bold" then
			if not string.find(text, "\n") then -- single line only
				s = "**" .. text .. "**"
			else
				s = text
			end
		elseif block.blockKind == "emphasis" then
			if not string.find(text, "\n") then -- single line only
				s = "*" .. text .. "*"
			else
				s = text
			end
		elseif block.blockKind == "heading" then
			s = ".. rubric:: " .. text .. "\n\n"
		elseif block.blockKind == "sp" then
			s = " "
		elseif block.blockKind == "varlistentry" then
			if not context.dlList then
				error("unexpected <varlistentry>")
			end

			local count = #context.dlList
			local entry = {}
			entry.title = trimWhitespace(text)
			entry.description = ""
			context.dlList[count + 1] = entry
		elseif block.blockKind == "listitem" then
			if context.dlList then
				local count = #context.dlList
				local entry = context.dlList[count]
				if entry then
					entry.description = trimWhitespace(text)
				end
			else
				if not context.listItemBullet then
					error("unexpected <listitem>")
				end

				s = context.listItemBullet .. " "
				local indent = string.rep(' ', string.len(s))

				text = replaceCommonSpacePrefix(text, indent)
				text = trimWhitespace(text)

				s = s .. text .. "\n\n"
			end
		elseif block.blockKind == "para" then
			s = trimWhitespace(text)
			if s ~= "" then
				s = s .. "\n\n"
			end
		elseif block.blockKind == "parametername" then
			text = trimWhitespace(text)

			if not context.paramSection then
				context.paramSection = {}
			end

			local i = #context.paramSection + 1

			context.paramSection[i] = {}
			context.paramSection[i].name = text
			context.paramSection[i].description = ""
		elseif block.blockKind == "parameterdescription" then
			text = trimWhitespace(text)

			if string.find(text, "\n") then
				text = "\n" .. replaceCommonSpacePrefix(text, "\t\t  ") -- add paramter table offset "- "
			end

			if context.paramSection then
				local count = #context.paramSection
				context.paramSection[count].description = text
			end
		elseif string.match(block.blockKind, "sect[1-4]") then
			if block.id and block.id ~= "" then
				s = ".. _doxid-" .. block.id .. ":\n"
			end

			if block.title and block.title ~= "" then
				s = s .. ".. rubric:: " .. block.title .. ":\n"
			end

			if s ~= "" then
				s = s .. "\n" .. text
			else
				s = text
			end
		elseif block.blockKind == "simplesect" then
			if block.simpleSectionKind == "return" then
				if not context.returnSection then
					context.returnSection = {}
				end

				local count = #context.returnSection
				context.returnSection[count + 1] = text
			elseif block.simpleSectionKind == "see" then
				if not context.seeSection then
					context.seeSection = {}
				end

				local count = #context.seeSection
				context.seeSection[count + 1] = text
			else
				s = text
			end
		elseif block.m_blockKind == "ulink" then
			s = "`" .. block.m_text .. "<" .. block.m_url .. ">`_"
		elseif block.m_blockKind == "table" then
			s = getSimpleTableContents(block.m_childBlockList, context)
		else
			s = text
		end
	end

	return s
end

function getDocBlockListContentsImpl(blockList, context)
	local s = ""

	for i = 1, #blockList do
		local block = blockList[i]
		if block.blockKind ~= "internal" then
			local blockContents = getDocBlockContents(block, context)
			if not context.codeBlockKind then
				s = concatDocBlockContents(s, blockContents)
			else
				s = s .. blockContents
			end
		end
	end

	return s
end

function getDocBlockListContents(blockList)
	local context = {}
	local s = getDocBlockListContentsImpl(blockList, context)

	if context.paramSection then
		s = s .. "\n\n.. rubric:: Parameters:\n\n"
		s = s .. ".. list-table::\n"
		s = s .. "\t:widths: 20 80\n\n"

		for i = 1, #context.paramSection do
			s = s .. "\t*\n"
			s = s .. "\t\t- " .. context.paramSection[i].name .. "\n\n"
			s = s .. "\t\t- " .. context.paramSection[i].description .. "\n\n"
		end
	end

	if context.returnSection then
		s = s .. "\n\n.. rubric:: Returns:\n\n"

		for i = 1, #context.returnSection do
			s = s .. context.returnSection[i]
		end
	end

	if context.seeSection then
		s = s .. "\n\n.. rubric:: See also:\n\n"

		for i = 1, #context.seeSection do
			s = s .. context.seeSection[i]
		end
	end

	s = trimTrailingWhitespace(s)

	return replaceCommonSpacePrefix(s, "")
end

function getSimpleDocBlockListContents(blockList)
	local s = ""

	for i = 1, #blockList do
		local block = blockList[i]
		s = s .. block.text .. getSimpleDocBlockListContents(block.childBlockList)
	end

	return s
end

function getSimpleTableContents(blockList, context)
	local tbl = {}
	local maxwidths = {}

	for i = 1, #blockList do
		local block = blockList [i]
		if block.m_blockKind == 'row' then
			local rowblock = block.m_childBlockList
			local row = {}
			for rownum = 1, #rowblock do
				if rowblock[rownum].m_blockKind == 'entry' then
					local entryblock = rowblock[rownum].m_childBlockList
					for entry = 1, #entryblock do
						if entryblock[entry].m_blockKind == 'para' then
							local t = trimWhitespace(entryblock[entry].m_childBlockList[1].m_text)
							local tlen = string.len(t)
							table.insert(row, t)
							if maxwidths[#row] == nil or maxwidths[#row] < tlen then
								maxwidths[#row] = tlen
							end
						end
					end
				end
			end
			table.insert(tbl, row)
		end
	end

	if #tbl == 0 then
		return ''
	end

	local headfoot = ''
	for i = 1, #maxwidths do
		headfoot = headfoot .. string.rep('=', maxwidths[i]) .. '  '
	end

	s = headfoot .. '\n'

	for r = 1, #tbl do
		for c = 1, #tbl[r] do
			s = s .. tbl[r][c] .. string.rep(' ', 2 + maxwidths[c] - string.len(tbl[r][c]))
		end
		s = s .. '\n'
		if r == 1 or r == #tbl then
			s = s .. headfoot .. '\n'
		end
	end

	return s
end

function isDocumentationEmpty(description)
	if description.isEmpty then
		return true
	end

	local text = getDocBlockListContents(description.docBlockList)
	return string.len(text) == 0
end

-------------------------------------------------------------------------------
