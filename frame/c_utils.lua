-------------------------------------------------------------------------------

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

	if not param.m_type.m_isEmpty then
		s = s .. getLinkedTextString (param.m_type, isRef)
	end

	if param.m_declarationName ~= "" then
		if s ~= "" then
			s = s .. " "
		end

		s = s .. getNormalizedCppString (param.m_declarationName)
	end

	if not param.m_defaultValue.m_isEmpty then
		s = s .. " = "
		s = s .. getLinkedTextString (param.m_defaultValue, isRef)
	end

	return s
end

function getFunctionParamArrayString (paramArray, isRef, indent)
	local s
	local count = #paramArray

	if count == 0 then
		s = "()"
	elseif count == 1  then
		s = "(" .. getParamString (paramArray [1], isRef) .. ")"
	else
		s = "(\n" .. indent .. "    "

		for i = 1, count do
			s = s .. getParamString (paramArray [i], isRef)

			if i ~= count then
				s = s .. ","
			end

			s = s .. "\n" .. indent .. "    "
		end
		s = s .. ")"
	end

	return s
end

function getTemplateParamArrayString (paramArray, isRef)
	local s
	local count = #paramArray

	if count == 0 then
		s = "<>"
	else
		s = "<" .. getParamString (paramArray [1], isRef)

		for i = 2, count do
			s = s .. ", " .. getParamString (paramArray [i], isRef)
		end

		s = s .. ">"
	end

	return s
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
		local fileName = getItemFileName (compound.m_groupArray [i])
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_namespaceArray do
		local fileName = getItemFileName (compound.m_namespaceArray [i])
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_enumArray do
		local fileName = getItemFileName (compound.m_enumArray [i])
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_structArray do
		local fileName = getItemFileName (compound.m_structArray [i])
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_unionArray do
		local fileName = getItemFileName (compound.m_unionArray [i])
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_classArray do
		local fileName = getItemFileName (compound.m_classArray [i])
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

	s = s .. getFunctionParamArrayString (
		item.m_paramArray,
		true,
		indent
		)

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

	s = s .. getLinkedTextString (typedef.m_type, true) .. "\n"

	if isRef then
		s = s .. indent .. ":ref:`" .. getItemName (typedef)  .. "<doxid-" .. typedef.m_id .. ">` ("
	else
		s = s .. indent .. getItemName (typedef) ..  " ("
	end

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

function concatenateDescription (description)
	local s = ""

	for i = 1, #description.m_docBlockList do
		local block = description.m_docBlockList [i]
		s = s .. block.m_contents.m_plainText

		if i ~= #description.m_docBlockList then
			s = s .. "\n"
		end
	end

	return s
end

function getItemBriefDocumentation (item, detailsRefPrefix)
	local s

	if not item.m_briefDescription.m_isEmpty then
		s = concatenateDescription (item.m_briefDescription)
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
	local s = concatenateDescription (item.m_briefDescription)

	if item.m_detailedDescription.m_isEmpty then
		return s
	end

	if s ~= "" then
		s = s .. "\n\n"
	end

	s = s .. concatenateDescription (item.m_detailedDescription)
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
