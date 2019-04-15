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

function getNormalizedCppString(string)
	local s = string

	s = string.gsub(s, "%s*%*", "*")
	s = string.gsub(s, "%s*&", "&")
	s = string.gsub(s, "<%s*", " <")
	s = string.gsub(s, "%s+>", ">")
	s = string.gsub(s, "%(%s*", " (")
	s = string.gsub(s, "%s+%)", ")")

	return s
end

function getLinkedTextString(text, isRef)
	if not text then
		return ""
	end

	if not isRef then
		return text.m_plainText
	end

	local s = ""

	for i = 1, #text.m_refTextArray do
		local refText = text.m_refTextArray[i]
		local text = getNormalizedCppString(refText.m_text)

		if refText.m_id ~= "" then
			text = string.gsub(text, "<", "\\<") -- escape left chevron
			s = s .. ":ref:`" .. text .. "<doxid-" .. refText.m_id .. ">`"
		else
			s = s .. text
		end
	end

	s = string.gsub(s, "\n", " ") -- callsites of getLinkedTextString don't expect newline chars

	return s
end

function getParamString(param, isRef)
	local s = ""
	local name

	if not param.m_type.m_isEmpty then
		s = s .. getLinkedTextString(param.m_type, isRef)
	end

	if param.m_declarationName ~= "" then
		name = param.m_declarationName
	else
		name = param.m_definitionName
	end

	if name ~= "" then
		if s ~= "" then
			s = s .. " "
		end

		s = s .. getNormalizedCppString(name)
	end

	if param.m_array ~= "" then
		s = s .. " " .. param.m_array
	end

	if not param.m_defaultValue.m_isEmpty then
		s = s .. " = " .. getLinkedTextString(param.m_defaultValue, isRef)
	end

	return s
end

function getParamArrayString_sl(paramArray, isRef, lbrace, rbrace)
	local s
	local count = #paramArray

	if count == 0 then
		s = lbrace .. rbrace
	else
		s = lbrace .. getParamString(paramArray[1], isRef)

		for i = 2, count do
			s = s .. ", " .. getParamString(paramArray[i], isRef)
		end

		s = s .. rbrace
	end

	return s
end

function getParamArrayString_ml(paramArray, isRef, lbrace, rbrace, indent, nl)
	local s
	local count = #paramArray

	if count == 0 then
		s = lbrace .. rbrace
	elseif count == 1  then
		s = lbrace .. getParamString(paramArray[1], isRef) .. rbrace
	else
		s = lbrace .. nl .. indent .. "    "

		for i = 1, count do
			s = s .. getParamString(paramArray[i], isRef)

			if i ~= count then
				s = s .. ","
			end

			s = s .. nl .. indent .. "    "
		end
		s = s .. rbrace
	end

	return s
end

function getFunctionParamArrayString(paramArray, isRef, indent)
	return getParamArrayString_ml(paramArray, isRef, "(", ")", indent, "\n")
end

function getItemKindString(item, itemKindString)
	local s = ""

	if item.m_modifiers ~= "" then
		s = item.m_modifiers .. " "
	end

	s = s .. itemKindString
	return s
end

function getItemSimpleName(item)
	return item.m_name
end

function getItemQualifiedName(item)
	local name = string.gsub(item.m_path, "/", ".")
	return name
end

getItemName = getItemQualifiedName

function getItemNameForOverview(item)
	if hasItemRefTarget(item) then
		return ":ref:`" .. getItemName(item) .. "<doxid-" .. item.m_id .. ">`"
	else
		return getItemName(item)
	end
end

function getGroupName(group)
	local s
	if string.len(group.m_title) ~= 0 then
		s = group.m_title
	else
		s = group.m_name
	end

	return s
end

g_itemCidMap = {}
g_itemFileNameMap = {}

function ensureUniqueItemName(item, name, map, sep)
	local mapValue = map[name]

	if mapValue == nil then
		mapValue = {}
		mapValue.m_itemMap = {}
		mapValue.m_itemMap[item.m_id] = 1
		mapValue.m_count = 1
		map[name] = mapValue
	else
		local index = mapValue.m_itemMap[item.m_id]

		if index == nil then
			index = mapValue.m_count + 1
			mapValue.m_itemMap[item.m_id] = index
			mapValue.m_count = mapValue.m_count + 1
		end

		if index ~= 1 then
			name = name .. sep .. index

			if map[name] then
				-- solution - try some other separator on collision; but when a proper naming convention is followed, this should never happen.
				error("name collision at: " .. name)
			end
		end
	end

	return name
end

function getItemFileName(item, suffix)
	local s

	if item.m_compoundKind == "struct" then
		s = "type_"
	elseif item.m_compoundKind then
		s = item.m_compoundKind .. "_"
	elseif item.m_memberKind then
		s = item.m_memberKind .. "_"
	else
		s = "undef_"
	end

	if item.m_compoundKind == "group" then
		s = s .. item.m_name
		-- s = string.gsub(s, '-', "_") -- groups can contain dashes, get rid of those
	else
		local path = string.gsub(item.m_path, "/operator[%s%p]+$", "/operator")
		s = s .. string.gsub(path, "/", "_")
	end

	s = ensureUniqueItemName(item, s, g_itemFileNameMap, "-")

	if not suffix then
		suffix = ".rst"
	end

	s = s .. suffix

	return s
end

function getItemCid(item)
	local s

	if item.m_compoundKind == "group" then
		s = item.m_name
	else
		s = string.gsub(item.m_path, "/operator[%s%p]+$", "/operator")
		s = string.gsub(s, "@[0-9]+/", "")
		s = string.gsub(s, "/", ".")
	end

	s = string.lower(s)
	s = ensureUniqueItemName(item, s, g_itemCidMap, "-")

	return s
end

function getItemImportArray(item)
	if item.m_importArray and next(item.m_importArray) ~= nil then
		return item.m_importArray
	end

	local text = getItemInternalDocumentation(item)
	local importArray = {}
	local i = 1
	for import in string.gmatch(text, ":import:([^:]+)") do
		importArray[i] = import
		i = i + 1
	end

	return importArray
end

function getItemImportString(item)
	local importArray = getItemImportArray(item)
	if next(importArray) == nil then
		return ""
	end

	local importPrefix
	local importSuffix

	if string.match(g_language, "^c[px+]*$") or g_language == "idl" then
		importPrefix = "\t#include <"
		importSuffix = ">\n"
	elseif string.match(g_language, "^ja?ncy?$") then
		importPrefix = "\timport \""
		importSuffix = "\"\n"
	else
		importPrefix = "\timport "
		importSuffix = "\n"
	end

	local s =
		".. code-block:: " .. g_language .. "\n" ..
		"\t:class: overview-code-block\n\n"

	for i = 1, #importArray do
		local import = importArray[i]
		s = s .. importPrefix .. import .. importSuffix
	end

	return s
end

function getItemRefTargetString(item)
	local s =
		".. _doxid-" .. item.m_id .. ":\n" ..
		".. _cid-" .. getItemCid(item) .. ":\n"

	if item.m_isSubGroupHead then
		for j = 1, #item.m_subGroupSlaveArray do
			slaveItem = item.m_subGroupSlaveArray[j]

			s = s ..
				".. _doxid-" .. slaveItem.m_id .. ":\n" ..
				".. _cid-" .. getItemCid(slaveItem) .. ":\n"
		end
	end

	return s
end

function hasItemRefTarget(item)
	return item.m_hasDocumentation or item.m_subGroupHead
end

function getItemArrayOverviewRefTargetString(itemArray)
	local s = ""

	for i = 1, #itemArray do
		local item = itemArray[i]
		if not hasItemRefTarget(item) then
			s = s .. getItemRefTargetString(item)
		end
	end

	return  s
end

function getEnumOverviewRefTargetString(enum)
	local s = ""

	for i = 1, #enum.m_enumValueArray do
		local enumValue = enum.m_enumValueArray[i]
		if not hasItemRefTarget(enumValue) then
			s = s .. getItemRefTargetString(enumValue)
		end
	end

	return  s
end

function getEnumArrayOverviewRefTargetString(enumArray)
	local s = ""

	for i = 1, #enumArray do
		local enum = enumArray[i]
		if isUnnamedItem(enum) then
			s = s .. getEnumOverviewRefTargetString(enum)
		end
	end

	return  s
end

function isTocTreeItem(compound, item)
	return not item.m_groupId or item.m_groupId == compound.m_id
end

function getCompoundTocTree(compound)
	local s = ".. toctree::\n\t:hidden:\n\n"

	for i = 1, #compound.m_groupArray do
		local item = compound.m_groupArray[i]
		local fileName = getItemFileName(item)
		s = s .. "\t" .. fileName .. "\n"
	end

	for i = 1, #compound.m_namespaceArray do
		local item = compound.m_namespaceArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_enumArray do
		local item = compound.m_enumArray[i]
		if isTocTreeItem(compound, item) and not isUnnamedItem(item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_structArray do
		local item = compound.m_structArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_unionArray do
		local item = compound.m_unionArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_interfaceArray do
		local item = compound.m_interfaceArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_protocolArray do
		local item = compound.m_protocolArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_exceptionArray do
		local item = compound.m_exceptionArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_classArray do
		local item = compound.m_classArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_singletonArray do
		local item = compound.m_singletonArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_serviceArray do
		local item = compound.m_serviceArray[i]
		if isTocTreeItem(compound, item) then
			local fileName = getItemFileName(item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	return trimTrailingWhitespace(s)
end

function getGroupTree(group, indent)
	local s = ""

	if not indent then
		indent = ""
	end

	local name = getGroupName(group)
	name = string.gsub(name, "<", "\\<") -- escape left chevron

	s = "|\t" .. indent .. ":ref:`" .. name .. "<doxid-" ..group.m_id .. ">`\n"

	for i = 1, #group.m_groupArray do
		s = s .. getGroupTree(group.m_groupArray[i], indent .. "\t")
	end

	return s
end

function getPageTree(page, fileName, indent)
	local s = ""

	if not indent then
		indent = ""
	end

	local docName = string.gsub(fileName, "%.rst$", "")

	s = "|\t" .. indent .. ":doc:`" .. docName .. "`\n"

	for i = 1, #page.m_subPageArray do
		local subPage = page.m_subPageArray[i]
		s = s .. getPageTree(subPage, getItemFileName(subPage), indent .. "\t")
	end

	return s
end

function getNamespaceTree(nspace, indent)
	local s = ""

	if not indent then
		indent = ""
	end

	s = "\t" .. indent .. "namespace :ref:`" .. getItemQualifiedName(nspace) .. "<doxid-" .. nspace.m_id ..">`\n"

	for i = 1, #nspace.m_namespaceArray do
		s = s .. getNamespaceTree(nspace.m_namespaceArray[i], indent .. "    ")
	end

	return s
end

function getDoubleSectionName(title1, count1, title2, count2)
	local s

	if count1 == 0 then
		if count2 == 0 then
			s = "<Empty>" -- should not really happen
		else
			s = title2
		end
	else
		if count2 == 0 then
			s = title1
		else
			s = title1 .. " & " .. title2
		end
	end

	return s
end

function getTitle(title, underline)
	if not title or  title == "" then
		title = "<Untitled>"
	end

	return title .. "\n" .. string.rep(underline, #title)
end

function getProtectionSuffix(item)
	if item.m_protectionKind and item.m_protectionKind ~= "public" then
		return " // " .. item.m_protectionKind
	else
		return ""
	end
end

function getFunctionModifierDelimiter(indent)
	if g_hasNewLineAfterReturnType then
		return "\n" .. indent
	else
		return " "
	end
end

function getPropertyDeclString(item, isRef, indent)
	local s = getLinkedTextString(item.m_returnType, true)

	if item.m_modifiers ~= "" then
		s = string.gsub(s, "property", item.m_modifiers .. " property")
	end

	s = s .. getFunctionModifierDelimiter(indent)

	if isRef then
		s = s .. ":ref:`" .. getItemName(item)  .. "<doxid-" .. item.m_id .. ">` "
	else
		s = s .. getItemName(item) ..  " "
	end

	if #item.m_paramArray > 0 then
		s = s .. getFunctionParamArrayString(item.m_paramArray, true, indent)
	end

	return s
end

function getFunctionDeclStringImpl(item, returnType, isRef, indent)
	local s = ""

	if string.find(item.m_flags, "static") then
		s = s .. "static" .. getFunctionModifierDelimiter(indent)
	elseif item.m_virtualKind == "pure-virtual" then
		s = s .. "virtual" .. getFunctionModifierDelimiter(indent)
	elseif item.m_virtualKind ~= "non-virtual" then
		s = s .. item.m_virtualKind .. getFunctionModifierDelimiter(indent)
	end

	if returnType and returnType ~= "" then
		s = s .. returnType .. getFunctionModifierDelimiter(indent)
	end

	if item.m_modifiers ~= "" then
		s = s .. item.m_modifiers .. getFunctionModifierDelimiter(indent)
	end

	if isRef then
		s = s .. ":ref:`" .. getItemName(item)  .. "<doxid-" .. item.m_id .. ">` "
	else
		s = s .. getItemName(item) ..  " "
	end

	s = s .. getFunctionParamArrayString(item.m_paramArray, true, indent)

	if string.find(item.m_flags, "const") then
		s = s .. " const"
	end

	if item.m_virtualKind == "pure-virtual" then
		s = s .. " = 0"
	end

	return s
end

function getFunctionDeclString(func, isRef, indent)
	return getFunctionDeclStringImpl(
		func,
		getLinkedTextString(func.m_returnType, true),
		isRef,
		indent
		)
end

function getVoidFunctionDeclString(func, isRef, indent)
	return getFunctionDeclStringImpl(
		func,
		nil,
		isRef,
		indent
		)
end

function getEventDeclString(event, isRef, indent)
	return getFunctionDeclStringImpl(
		event,
		"event",
		isRef,
		indent
		)
end

function getDefineDeclString(define, isRef, indent)
	local s = "#define "

	if isRef then
		s = s .. ":ref:`" .. define.m_name  .. "<doxid-" .. define.m_id .. ">`"
	else
		s = s .. define.m_name
	end

	if #define.m_paramArray > 0 then
		-- no space between name and params!

		s = s .. getParamArrayString_ml(define.m_paramArray, false, "(", ")", indent, " \\\n")
	end

	return s
end

function getTypedefDeclString(typedef, isRef, indent)
	local s = "typedef"

	if next(typedef.m_paramArray) == nil then
		s = s .. " " .. getLinkedTextString(typedef.m_type, true) .. " "

		if isRef then
			s = s .. ":ref:`" .. getItemName(typedef)  .. "<doxid-" .. typedef.m_id .. ">`"
		else
			s = s .. getItemName(typedef)
		end

		if typedef.m_argString ~= "" then
			local c = string.sub(typedef.m_argString, 1, 1)
			if c ~= ")" then -- nested declarator, e.g. int(*Func) (int a)
				s = s .. " "
			end

			s = s .. formatArgDeclString(typedef.m_argString, indent)
		end

		return s
	end

 	s = s .. getFunctionModifierDelimiter(indent) .. getLinkedTextString(typedef.m_type, true) .. getFunctionModifierDelimiter(indent)

	if isRef then
		s = s .. ":ref:`" .. getItemName(typedef)  .. "<doxid-" .. typedef.m_id .. ">` "
	else
		s = s .. getItemName(typedef) ..  " "
	end

	s = s .. getFunctionParamArrayString(typedef.m_paramArray, true, indent)
	return s
end

function getBaseClassString(class, protection)
	local s = ""

	if string.match(g_language, "^c[px+]*$") then
		s = protection .. " "
	end

	if class.m_id ~= "" then
		s = s .. ":ref:`" .. getItemQualifiedName(class) .. "<doxid-" .. class.m_id .. ">`"
	else
		local url = g_importUrlMap[class.m_importId]
		if url then
			s = s .. "`" .. getItemName(class) .. "<" .. url .. ">`__"
		else
			s = s .. getItemName(class)
		end
	end

	return s
end

function isMemberOfUnnamedType(item)
	local text = getItemInternalDocumentation(item)
	return string.match(text, ":unnamed:([%w/:]+)")
end

function isUnnamedItem(item)
	return item.m_name == "" or string.sub(item.m_name, 1, 1) == "@"
end

g_closingBracketChar =
{
	["("] = ")",
	["["] = "]",
	["<"] = ">",
	["{"] = "}",
}

function formatArgDeclString(decl, indent)
	local bracket = {}
	bracket[0] = {}
	bracket[0].m_result = ""

	local level = 0
	local pos = 1
	local len = string.len(decl)

	for i = 1, len do
		local c = string.sub(decl, i, i)

		if c == "(" or c == "[" or c == "<" or c == "{" then
			local chunk = trimLeadingWhitespace(string.sub(decl, pos, i))
			pos = i + 1

			bracket[level].m_result = bracket[level].m_result .. chunk

			level = level + 1
			bracket[level] = {}
			bracket[level].m_closingChar = g_closingBracketChar[c]
			bracket[level].m_result = ""
		elseif level > 0 then
			if c == ',' then
				if not bracket[level].m_delimiter then
					bracket[level].m_delimiter = "\n" .. indent .. string.rep("    ", level)
					bracket[level].m_result = bracket[level].m_delimiter .. bracket[level].m_result
				end

				local chunk = trimLeadingWhitespace(string.sub(decl, pos, i))
				pos = i + 1

				bracket[level].m_result = bracket[level].m_result .. chunk .. bracket[level].m_delimiter
				pos = i + 1
			elseif c == bracket[level].m_closingChar then
				local chunk = trimLeadingWhitespace(string.sub(decl, pos, i - 1))
				pos = i

				bracket[level].m_result = bracket[level].m_result .. chunk

				if bracket[level].m_delimiter then
					bracket[level].m_result = bracket[level].m_result .. bracket[level].m_delimiter
				end

				level = level - 1
				bracket[level].m_result = bracket[level].m_result .. bracket[level + 1].m_result
			end
		end
	end

	if pos <= len then
		bracket[0].m_result = bracket[0].m_result .. string.sub(decl, pos)
	end

	return bracket[0].m_result
end

-------------------------------------------------------------------------------

-- item documentation utils

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

	if not prefix then
		return source
	end

	if len < 2 and replacement == "" then
		return source
	end

	s = string.gsub(s, prefix, "\n" .. replacement) -- replace common prefix
	s = string.sub(s, 2) -- remove leading '\n'

	return s
end

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

	local prevBullet = context.m_listItemBullet
	context.m_listItemBullet = bullet

	s = getDocBlockListContentsImpl(block.m_childBlockList, context)
	s = "\n\n" .. trimTrailingWhitespace(s) .. "\n\n"

	context.m_listItemBullet = prevBullet

	return s
end

function processDlListDocBlock(block, context)
	local prevList = context.m_dlList
	context.m_dlList = {}

	getDocBlockListContentsImpl(block.m_childBlockList, context)

	local s =
		"\n\n.. list-table::\n" ..
		"\t:widths: 20 80\n\n"

	for i = 1, #context.m_dlList do
		s = s .. "\t*\n"

		local entry = context.m_dlList[i]
		local title = replaceCommonSpacePrefix(entry.m_title, "\t\t  ")
		local description = replaceCommonSpacePrefix(entry.m_description, "\t\t  ")

		s = s .. "\t\t- " .. trimLeadingWhitespace(title) .. "\n\n"
		s = s .. "\t\t- " .. trimLeadingWhitespace(description) .. "\n\n"
	end

	context.m_dlList = prevList

	return s
end

function getCodeDocBlockContents(block, context)
	context.m_codeBlockKind = block.m_blockKind
	local s = getDocBlockListContentsImpl(block.m_childBlockList, context)
	context.m_codeBlockKind = nil

	return s
end

function getDocBlockContents(block, context)
	local s = ""

	if block.m_blockKind == "computeroutput" then
		if context.m_codeBlockKind then
			s = block.m_text .. getDocBlockListContentsImpl(block.m_childBlockList, context)
		else
			local code = block.m_text .. getCodeDocBlockContents(block, context)

			if not string.find(code, "\n") then
				s = "``" .. trimWhitespace(code) .. "``"
			else
				code = replaceCommonSpacePrefix(code, "    ")
				code = trimTrailingWhitespace(code)
				s = "\n\n.. code-block:: none\n\n" .. code
			end
		end
	elseif block.m_blockKind == "programlisting" then
		local code = getCodeDocBlockContents(block, context)
		code = replaceCommonSpacePrefix(code, "    ")
		code = trimTrailingWhitespace(code)

		s = "\n\n.. ref-code-block:: " .. g_language .. "\n\n" .. code .. "\n\n"
	elseif block.m_blockKind == "preformatted" then
		local code = getCodeDocBlockContents(block, context)
		code = replaceCommonSpacePrefix(code, "    ")
		code = trimTrailingWhitespace(code)

		-- raw seems like a better approach, but need to figure something out with indents
		s = "\n\n.. code-block:: none\n\n" .. code .. "\n\n"
	elseif block.m_blockKind == "formula" then
		local code = getCodeDocBlockContents(block, context)
		local isInline = string.sub(code, 1, 1) == "$"

		-- take away framing tokens
		code = string.gsub(code, "^\\?[$%[]", "")
		code = string.gsub(code, "\\?[$%]]$", "")

		if isInline then
			s = ":math:`" .. code .. "`"
		else
			code = replaceCommonSpacePrefix(code, "    ")
			code = trimTrailingWhitespace(code)

			-- raw seems like a better approach, but need to figure something out with indents
			s = "\n\n.. math::\n\n" .. code .. "\n\n"
		end
	elseif block.m_blockKind == "verbatim" and VERBATIM_TO_CODE_BLOCK then
		local code = getCodeDocBlockContents(block, context)
		code = replaceCommonSpacePrefix(code, "    ")
		code = trimTrailingWhitespace(code)

		-- probably also need to assign some div class
		s = "\n\n.. code-block:: " .. VERBATIM_TO_CODE_BLOCK .. "\n\n" .. code .. "\n\n"
	elseif block.m_blockKind == "itemizedlist" then
		s = processListDocBlock(block, context, "*")
	elseif block.m_blockKind == "orderedlist" then
		s = processListDocBlock(block, context, "#.")
	elseif block.m_blockKind == "variablelist" then
		s = processDlListDocBlock(block, context)
	else
		local text = block.m_text
		local childContents = getDocBlockListContentsImpl(block.m_childBlockList, context)

		if not context.m_codeBlockKind then
			if g_escapeAsterisks then
				text = string.gsub(text, "%*", "\\*")
			end

			text = trimWhitespace(text)
			text = concatDocBlockContents(text, childContents)
		else
			text = text .. childContents

			if context.m_codeBlockKind ~= "programlisting" then
				return text
			end
		end

		if block.m_blockKind == "linebreak" then
			s = "\n\n"
		elseif block.m_blockKind == "ref" then
			text = string.gsub(text, "<", "\\<") -- escape left chevron
			s = ":ref:`" .. text .. " <doxid-" .. block.m_id .. ">`"
		elseif block.m_blockKind == "anchor" then
			s = ":target:`doxid-" .. block.m_id .. "`" .. text
		elseif block.m_blockKind == "image" then
			s = "\n\n.. image:: " .. block.m_name .. "\n"
			if block.m_width ~= 0 then
				s = s .. "\t:width: " .. block.m_width .. "\n"
			end
			if block.m_height ~= 0 then
				s = s .. "\t:height: " .. block.m_height .. "\n"
			end
			if block.m_text ~= "" then
				s = s .. "\t:alt: " .. block.m_text .. "\n"
			end
			s = s .. "\n"
		elseif block.m_blockKind == "bold" then
			if not string.find(text, "\n") then -- single line only
				s = "**" .. text .. "**"
			else
				s = text
			end
		elseif block.m_blockKind == "emphasis" then
			if not string.find(text, "\n") then -- single line only
				s = "*" .. text .. "*"
			else
				s = text
			end
		elseif block.m_blockKind == "heading" then
			s = ".. rubric:: " .. text .. "\n\n"
		elseif block.m_blockKind == "sp" then
			s = " "
		elseif block.m_blockKind == "varlistentry" then
			if not context.m_dlList then
				error("unexpected <varlistentry>")
			end

			local count = #context.m_dlList
			local entry = {}
			entry.m_title = trimWhitespace(text)
			entry.m_description = ""
			context.m_dlList[count + 1] = entry
		elseif block.m_blockKind == "listitem" then
			if context.m_dlList then
				local count = #context.m_dlList
				local entry = context.m_dlList[count]
				if entry then
					entry.m_description = trimWhitespace(text)
				end
			else
				if not context.m_listItemBullet then
					error("unexpected <listitem>")
				end

				s = context.m_listItemBullet .. " "
				local indent = string.rep(' ', string.len(s))

				text = replaceCommonSpacePrefix(text, indent)
				text = trimWhitespace(text)

				s = s .. text .. "\n\n"
			end
		elseif block.m_blockKind == "para" then
			s = trimWhitespace(text)
			if s ~= "" then
				s = s .. "\n\n"
			end
		elseif block.m_blockKind == "parametername" then
			text = trimWhitespace(text)

			if not context.m_paramSection then
				context.m_paramSection = {}
			end

			local i = #context.m_paramSection + 1

			context.m_paramSection[i] = {}
			context.m_paramSection[i].m_name = text
			context.m_paramSection[i].m_description = ""
		elseif block.m_blockKind == "parameterdescription" then
			text = trimWhitespace(text)

			if string.find(text, "\n") then
				text = "\n" .. replaceCommonSpacePrefix(text, "\t\t  ") -- add paramter table offset "- "
			end

			if context.m_paramSection then
				local count = #context.m_paramSection
				context.m_paramSection[count].m_description = text
			end
		elseif string.match(block.m_blockKind, "sect[1-4]") then
			if block.m_id and block.m_id ~= "" then
				s = ".. _doxid-" .. block.m_id .. ":\n"
			end

			if block.m_title and block.m_title ~= "" then
				s = s .. ".. rubric:: " .. block.m_title .. ":\n"
			end

			if s ~= "" then
				s = s .. "\n" .. text
			else
				s = text
			end
		elseif block.m_blockKind == "simplesect" then
			if block.m_simpleSectionKind == "return" then
				if not context.m_returnSection then
					context.m_returnSection = {}
				end

				local count = #context.m_returnSection
				context.m_returnSection[count + 1] = text
			elseif block.m_simpleSectionKind == "see" then
				if not context.m_seeSection then
					context.m_seeSection = {}
				end

				local count = #context.m_seeSection
				context.m_seeSection[count + 1] = text
			else
				s = text
			end
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
		if block.m_blockKind ~= "internal" then
			local blockContents = getDocBlockContents(block, context)
			if not context.m_codeBlockKind then
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

	if context.m_paramSection then
		s = s .. "\n\n.. rubric:: Parameters:\n\n"
		s = s .. ".. list-table::\n"
		s = s .. "\t:widths: 20 80\n\n"

		for i = 1, #context.m_paramSection do
			s = s .. "\t*\n"
			s = s .. "\t\t- " .. context.m_paramSection[i].m_name .. "\n\n"
			s = s .. "\t\t- " .. context.m_paramSection[i].m_description .. "\n\n"
		end
	end

	if context.m_returnSection then
		s = s .. "\n\n.. rubric:: Returns:\n\n"

		for i = 1, #context.m_returnSection do
			s = s .. context.m_returnSection[i]
		end
	end

	if context.m_seeSection then
		s = s .. "\n\n.. rubric:: See also:\n\n"

		for i = 1, #context.m_seeSection do
			s = s .. context.m_seeSection[i]
		end
	end

	s = trimTrailingWhitespace(s)
	s = string.gsub(s, "\t", "    ") -- replace tabs with spaces

	return replaceCommonSpacePrefix(s, "")
end

function getSimpleDocBlockListContents(blockList)
	local s = ""

	for i = 1, #blockList do
		local block = blockList[i]
		s = s .. block.m_text .. getSimpleDocBlockListContents(block.m_childBlockList)
	end

	return s
end

function getItemInternalDocumentation(item)
	local s = ""

	for i = 1, #item.m_detailedDescription.m_docBlockList do
		local block = item.m_detailedDescription.m_docBlockList[i]
		if block.m_blockKind == "internal" then
			s = s .. block.m_text .. getSimpleDocBlockListContents(block.m_childBlockList)
		end
	end

	return s
end

function getItemBriefDocumentation(item, detailsRefPrefix)
	local s = getDocBlockListContents(item.m_briefDescription.m_docBlockList)

	if string.len(s) == 0 then
		s = getDocBlockListContents(item.m_detailedDescription.m_docBlockList)
		if string.len(s) == 0 then
			return ""
		end

		-- generate brief description from first sentence only
		-- matching space is to handle qualified identifiers(e.g. io.File.open)

		local i = string.find(s, "%.%s", 1)
		if i then
			s = string.sub(s, 1, i)
		end

		s = trimTrailingWhitespace(s)
		s = string.gsub(s, "\t", "    ") -- replace tabs with spaces
	end

	if detailsRefPrefix then
		s = s .. " :ref:`More...<" .. detailsRefPrefix .. "doxid-" .. item.m_id .. ">`"
	end

	return s
end

function getItemDetailedDocumentation(item)
	local brief = getDocBlockListContents(item.m_briefDescription.m_docBlockList)
	local detailed = getDocBlockListContents(item.m_detailedDescription.m_docBlockList)

	if string.len(detailed) == 0 then
		return brief
	elseif string.len(brief) == 0 then
		return detailed
	else
		return brief .. "\n\n" .. detailed
	end
end

function isDocumentationEmpty(description)
	if description.m_isEmpty then
		return true
	end

	local text = getDocBlockListContents(description.m_docBlockList)
	return string.len(text) == 0
end

function prepareItemDocumentation(item, compound)
	local hasBriefDocuemtnation = not isDocumentationEmpty(item.m_briefDescription)
	local hasDetailedDocuemtnation = not isDocumentationEmpty(item.m_detailedDescription)

	item.m_hasDocumentation = hasBriefDocuemtnation or hasDetailedDocuemtnation
	if not item.m_hasDocumentation then
		return false
	end

	if hasDetailedDocuemtnation then
		local text = getItemInternalDocumentation(item)

		item.m_isSubGroupHead = string.match(text, ":subgroup:") ~= nil
		if item.m_isSubGroupHead then
			item.m_subGroupSlaveArray = {}
		end
	end

	return not compound or not item.m_groupId or item.m_groupId == compound.m_id
end

function prepareItemArrayDocumentation(itemArray, compound)

	local hasDocumentation = false
	local subGroupHead = nil

	for i = 1, #itemArray do
		local item = itemArray[i]

		local result = prepareItemDocumentation(item, compound)
		if result then
			hasDocumentation = true
			if item.m_isSubGroupHead then
				subGroupHead = item
			else
				subGroupHead = nil
			end
		elseif subGroupHead then
			table.insert(subGroupHead.m_subGroupSlaveArray, item)
			item.m_subGroupHead = subGroupHead
		end
	end

	return hasDocumentation
end

function isItemInCompoundDetails(item, compound)
	if not item.m_hasDocumentation then
		return false
	end

	return not item.m_groupId or item.m_groupId == compound.m_id
end

-------------------------------------------------------------------------------

-- item filtering utils

function isItemExcludedByLocationFilter(item)
	if not item.m_location then
		return false
	end

	return
		-- exclude explicitly specified locations
		EXCLUDE_LOCATION_PATTERN and
		string.match(item.m_location.m_file, EXCLUDE_LOCATION_PATTERN)
end

function filterItemArray(itemArray)
	if next(itemArray) == nil then
		return
	end

	for i = #itemArray, 1, -1 do
		local item = itemArray[i]
		local isExcluded = isItemExcludedByLocationFilter(item)

		if isExcluded then
			table.remove(itemArray, i)
		end
	end
end

-------------------------------------------------------------------------------

-- compound & enum prep

function cmpIds(i1, i2)
	return i1.m_id < i2.m_id
end

function cmpNames(i1, i2)
	return i1.m_name < i2.m_name
end

function cmpTitles(i1, i2)
	return i1.m_title < i2.m_title
end

function prepareCompound(compound)
	if compound.m_stats then
		return compound.m_stats
	end

	local stats = {}

	-- filter invisible items out

	filterItemArray(compound.m_structArray)
	filterItemArray(compound.m_variableArray)
	filterItemArray(compound.m_functionArray)

	stats.m_hasItems =
		#compound.m_structArray ~= 0 or
		#compound.m_variableArray ~= 0 or
		#compound.m_functionArray ~= 0

	-- collect documentation stats

	stats.m_hasBriefDocumentation = not isDocumentationEmpty(compound.m_briefDescription)
	stats.m_hasDetailedDocumentation = not isDocumentationEmpty(compound.m_detailedDescription)
	stats.m_hasDocumentedVariables = prepareItemArrayDocumentation(compound.m_variableArray, compound)
	stats.m_hasDocumentedFunctions = prepareItemArrayDocumentation(compound.m_functionArray, compound)
	stats.m_hasDocumentedItems = stats.m_hasDocumentedVariables or stats.m_hasDocumentedFunctions

	-- sort items -- only the ones producing separate pages;

	table.sort(compound.m_groupArray, cmpIds)
	table.sort(compound.m_structArray, cmpNames)

	compound.m_stats = stats

	return stats
end

-------------------------------------------------------------------------------

-- generic utils

function captialize(string)
	local s = string.gsub(string, "^%l", string.upper)
	return s
end

function concatenateTables(t1, t2)
	local j = #t1 + 1
	for i = 1, #t2 do
		t1 [j] = t2 [i]
		j = j + 1
	end
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

-------------------------------------------------------------------------------
