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

function getNormalizedCppString (string)
	local s = string

	s = string.gsub (s, "%s*%*", "*")
	s = string.gsub (s, "%s*&", "&")
	s = string.gsub (s, "<%s*", " <")
	s = string.gsub (s, "%s+>", ">")
	s = string.gsub (s, "%(%s*", " (")
	s = string.gsub (s, "%s+%)", ")")

	return s
end

function getLinkedTextString (text, isRef)
	if not text then
		return ""
	end

	if not isRef then
		return text.m_plainText
	end

	local s = ""

	for i = 1, #text.m_refTextArray do
		local refText = text.m_refTextArray [i]
		local text = getNormalizedCppString (refText.m_text)

		if refText.m_id ~= "" then
			s = s .. ":ref:`" .. text .. "<doxid-" .. refText.m_id .. ">`"
		else
			s = s .. text
		end
	end

	return s
end

function getParamString (param, isRef)
	local s = ""
	local name

	if not param.m_type.m_isEmpty then
		s = s .. getLinkedTextString (param.m_type, isRef)
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

		s = s .. getNormalizedCppString (name)
	end

	if param.m_array ~= "" then
		s = s .. " " .. param.m_array
	end

	if not param.m_defaultValue.m_isEmpty then
		s = s .. " = " .. getLinkedTextString (param.m_defaultValue, isRef)
	end

	return s
end

function getParamArrayString_sl (paramArray, isRef, lbrace, rbrace)
	local s
	local count = #paramArray

	if count == 0 then
		s = lbrace .. rbrace
	else
		s = lbrace .. getParamString (paramArray [1], isRef)

		for i = 2, count do
			s = s .. ", " .. getParamString (paramArray [i], isRef)
		end

		s = s .. rbrace
	end

	return s
end

function getParamArrayString_ml (paramArray, isRef, lbrace, rbrace, indent)
	local s
	local count = #paramArray

	if count == 0 then
		s = lbrace .. rbrace
	elseif count == 1  then
		s = lbrace .. getParamString (paramArray [1], isRef) .. rbrace
	else
		s = lbrace .. "\n" .. indent .. "    "

		for i = 1, count do
			s = s .. getParamString (paramArray [i], isRef)

			if i ~= count then
				s = s .. ","
			end

			s = s .. "\n" .. indent .. "    "
		end
		s = s .. rbrace
	end

	return s
end

function getFunctionParamArrayString (paramArray, isRef, indent)
	return getParamArrayString_ml (paramArray, isRef, "(", ")", indent)
end

function getTemplateParamArrayString (paramArray, isRef)
	return getParamArrayString_sl (paramArray, isRef, "<", ">")
end

function getDefineParamArrayString (paramArray, isRef)
	return getParamArrayString_sl (paramArray, isRef, "(", ")")
end

function getItemName (item)
	local s = ""
	local parent = item.m_parent

	while parent do
		if parent.m_compoundKind == "group" then
			parent = parent.m_parent
		else
			if parent.m_compoundKind == "namespace" and parent.m_path ~= "" then
				s = string.gsub (parent.m_path, "/", ".") .. "." -- only add prefix to namespaces, not classes/structs
			end

			break
		end
	end

	s = s .. item.m_name

	if item.m_templateParamArray and #item.m_templateParamArray > 0 then
		s = s .. " " .. getTemplateParamArrayString (item.m_templateParamArray)
	end

	if item.m_templateSpecParamArray and #item.m_templateSpecParamArray > 0 then
		s = s .. " " .. getTemplateParamArrayString (item.m_templateSpecParamArray)
	end

	return s
end

g_itemFileNameMap = {}
g_itemCidMap = {}

function ensureUniqueItemName (item, name, map, sep)
	local mapValue = map [name]

	if mapValue == nil then
		mapValue = {}
		mapValue.m_itemMap = {}
		mapValue.m_itemMap [item] = 1
		mapValue.m_count = 1
		map [name] = mapValue
	else
		local index = mapValue.m_itemMap [item]

		if index == nil then
			index = mapValue.m_count + 1
			mapValue.m_itemMap [item] = index
			mapValue.m_count = mapValue.m_count + 1
		end

		if index ~= 1 then
			name = name .. sep .. index

			if map [name] then
				-- solution - try some other separator on collision; but when a proper naming convention is followed, this should never happen.
				error ("name collision at: " .. name)
			end
		end
	end

	return name
end

function getItemFileName (item, suffix)
	local s
	local parent = item.m_parent

	if item.m_compoundKind then
		s = item.m_compoundKind .. "_"
	elseif item.m_memberKind then
		s = item.m_memberKind .. "_"
	else
		s = "undef_"
	end

	if parent and parent.m_path ~= "" then
		s = s .. string.gsub (parent.m_path, "/", "_") .. "_"
	end

	s = s .. string.gsub (item.m_name, '-', "_") -- groups can contain dashes
	s = ensureUniqueItemName (item, s, g_itemFileNameMap, "_")

	if not suffix then
		suffix = ".rst"
	end

	s = s .. suffix

	return s
end

function getItemCid (item)
	local s = ""
	local parent = item.m_parent

	if parent and parent.m_path ~= "" then
		s = s .. string.gsub (parent.m_path, "/", ".") .. "."
	end

	s = string.lower (s .. item.m_name)
	s = ensureUniqueItemName (item, s, g_itemCidMap, "-")

	return s
end

function getCompoundTocTree (compound, indent)
	local s = ""

	for i = 1, #compound.m_groupArray do
		local item = compound.m_groupArray [i]
		local fileName = getItemFileName (item)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_namespaceArray do
		local item = compound.m_namespaceArray [i]
		local fileName = getItemFileName (item)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_enumArray do
		local item = compound.m_enumArray [i]
		if not isUnnamedItem (item) then
			local fileName = getItemFileName (item)
			s = s .. indent .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_structArray do
		local item = compound.m_structArray [i]
		local fileName = getItemFileName (item)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_unionArray do
		local item = compound.m_unionArray [i]
		local fileName = getItemFileName (item)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_classArray do
		local item = compound.m_classArray [i]
		local fileName = getItemFileName (item)
		s = s .. indent .. fileName .. "\n"
	end

	return s
end

function getDoubleSectionName (title1, count1, title2, count2)
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


function getTitle (title, underline)
	if not title or  title == "" then
		title = "<Untitled>"
	end

	return title .. "\n" .. string.rep (underline, #title)
end

function getFunctionDeclStringImpl (item, returnType, isRef, indent)
	local s = ""

	if returnType then
		s = returnType

		if g_hasNewLineAfterReturnType then
			s = s .. "\n" .. indent
		else
			s = s .. " "
		end
	end

	if item.m_modifiers ~= "" then
		s = s .. item.m_modifiers

		if g_hasNewLineAfterReturnType then
			s = s .. "\n" .. indent
		else
			s = s .. " "
		end
	end

	if isRef then
		s = s .. ":ref:`" .. getItemName (item)  .. "<doxid-" .. item.m_id .. ">` "
	else
		s = s .. getItemName (item) ..  " "
	end

	s = s .. getFunctionParamArrayString (item.m_paramArray, true, indent)

	return s
end

function getFunctionDeclString (func, isRef, indent)
	return getFunctionDeclStringImpl (
		func,
		getLinkedTextString (func.m_returnType, true),
		isRef,
		indent
		)
end

function getEventDeclString (event, isRef, indent)
	return getFunctionDeclStringImpl (
		event,
		"event",
		isRef,
		indent
		)
end

function getDefineDeclString (define, isRef)
	local s = "#define "

	if isRef then
		s = s .. ":ref:`" .. define.m_name  .. "<doxid-" .. define.m_id .. ">`"
	else
		s = s .. define.m_name
	end

	if #define.m_paramArray > 0 then
		-- no space between name and params!

		s = s .. getDefineParamArrayString (define.m_paramArray, true)
	end

	return s
end

function getTypedefDeclString (typedef, isRef, indent)
	local s = "typedef"

	if typedef.m_argString == "" then
		s = s .. " " .. getLinkedTextString (typedef.m_type, true) .. " "

		if isRef then
			s = s .. ":ref:`" .. getItemName (typedef)  .. "<doxid-" .. typedef.m_id .. ">` "
		else
			s = s .. getItemName (typedef) ..  " "
		end

		return s
	end

	if g_hasNewLineAfterReturnType then
		s = s .. "\n" .. indent
	else
		s = s .. " "
	end

	s = s .. getLinkedTextString (typedef.m_type, true)

	if g_hasNewLineAfterReturnType then
		s = s .. "\n" .. indent
	else
		s = s .. " "
	end

	if isRef then
		s = s .. ":ref:`" .. getItemName (typedef)  .. "<doxid-" .. typedef.m_id .. ">` "
	else
		s = s .. getItemName (typedef) ..  " "
	end

	if #typedef.m_paramArray > 0 then
		s = s .. getFunctionParamArrayString (typedef.m_paramArray, true, indent)
		return s
	end

	s = s .. "("

	if not string.find (typedef.m_argString, ",") then
		local arg = string.match (typedef.m_argString, "%(([^()]+)%)")
		if arg then
			s = s .. arg .. ")"
		else
			s = s .. ")"
		end
	else
		s = s .. "\n"

		for arg, term in string.gmatch (typedef.m_argString, "%s*([^,()]+)([,)])") do
			s = s .. indent .. "    " .. arg
			if term == "," then
				s = s .. ",\n"
			else
				s = s .. "\n"
			end
		end

		s = s .. indent .. "    )"
	end

	return s
end

function isMemberOfUnnamedType (item)
	if item.m_detailedDescription.m_isEmpty then
		return nil
	end

	local block = item.m_detailedDescription.m_docBlockList [1]
	if block.m_blockKind ~= "internal" or
		#block.m_childBlockList < 1 or
		block.m_childBlockList [1].m_blockKind ~= "paragraph" then
		return nil
	end

	local s = block.m_childBlockList [1].m_contents.m_plainText
	return string.match (s, ":unnamed:([%w/:]+)")
end

function isUnnamedItem (item)
	return item.m_name == "" or string.sub (item.m_name, 1, 1) == "@"
end

function getDocBlockListContents (blockList)
	local s = ""

	for i = 1, #blockList do
		local block = blockList [i]

		if block.m_blockKind == "paragraph" then
			s = s .. block.m_contents.m_plainText
		elseif block.m_blockKind ~= "internal" then
			s = s .. getDocBlockListContents (block.m_childBlockList)
		end

		if i ~= #blockList then
			s = s .. "\n"
		end
	end

	return s
end

function getItemBriefDocumentation (item, detailsRefPrefix)
	local s

	if not item.m_briefDescription.m_isEmpty then
		s = getDocBlockListContents (item.m_briefDescription.m_docBlockList)
	elseif item.m_detailedDescription.m_isEmpty then
		return ""
	else
		local block = item.m_detailedDescription.m_docBlockList [1]
		s = block.m_contents.m_plainText

		local i = string.find (s, ".%s", 1, true)
		if i then
			s = string.sub (s, 1, i)
		end
	end

	s = string.match (s, "(.-)%s*$")  -- trim trailing whitespace
	s = string.gsub (s, "\t", "    ") -- replace tabs with spaces

	if detailsRefPrefix then
		s = s .. " :ref:`More...<" .. detailsRefPrefix .. "doxid-" .. item.m_id .. ">`"
	end

	return s
end

function getItemDetailedDocumentation (item)
	local s = getDocBlockListContents (item.m_briefDescription.m_docBlockList)

	if item.m_detailedDescription.m_isEmpty then
		return s
	end

	if s ~= "" then
		s = s .. "\n\n"
	end

	s = s .. getDocBlockListContents (item.m_detailedDescription.m_docBlockList)
	s = string.gsub (s, "\t", "    ") -- replace tabs with spaces

	return s
end

function removePrimitiveTypedefs (typedefArray)
	if next (typedefArray) == nil then
		return
	end

	for i = #typedefArray, 1, -1 do
		typedef = typedefArray [i]

		local typeKind, name = string.match (
			typedef.m_type.m_plainText,
			"(%a+)%s+(%w[%w_]*)"
			)

		if name == typedef.m_name then
			table.remove (typedefArray, i)
		end
	end
end

-------------------------------------------------------------------------------
