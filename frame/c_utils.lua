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

function getItemKindString (item, itemKindString)
	local s = ""

	if item.m_modifiers ~= "" then
		s = item.m_modifiers .. " "
	end

	s = s .. itemKindString
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
				-- only add prefix to namespaces, not classes/structs
				s = string.gsub (parent.m_path, "/", g_nameDelimiter) .. g_nameDelimiter
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

function getItemNameForOverview (item)
	if hasItemRefTarget (item) then
		return ":ref:`" .. getItemName (item) .. "<doxid-" .. item.m_id .. ">`"
	else
		return getItemName (item)
	end
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
		s = s .. string.gsub (parent.m_path, "/", g_nameDelimiter) .. g_nameDelimiter
	end

	s = string.lower (s .. item.m_name)
	s = ensureUniqueItemName (item, s, g_itemCidMap, "-")

	return s
end

function getItemImportArray (item)
	if item.m_importArray and next (item.m_importArray) ~= nil then
		return item.m_importArray
	end

	local text = getItemInternalDocumentation (item)
	local importArray = {}
	local i = 1
	for import in string.gmatch (text, ":import:([^:]+)") do
		importArray [i] = import
		i = i + 1
	end

	return importArray
end

function getItemImportString (item)
	local importArray = getItemImportArray (item)
	if next (importArray) == nil then
		return ""
	end

	local importPrefix
	local importSuffix

	if string.match (g_language, "^c[px+]*$") then
		importPrefix = "\t#include <"
		importSuffix = ">\n"
	elseif string.match (g_language, "^ja?ncy?$") then
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
		local import = importArray [i]
		s = s .. importPrefix .. import .. importSuffix
	end

	return s
end

function getItemRefTargetString (item)
	local s =
		".. _doxid-" .. item.m_id .. ":\n" ..
		".. _cid-" .. getItemCid (item) .. ":\n"

	if item.m_isSubGroupHead then
		for j = 1, #item.m_subGroupSlaveArray do
			slaveItem = item.m_subGroupSlaveArray [j]

			s = s ..
				".. _doxid-" .. slaveItem.m_id .. ":\n" ..
				".. _cid-" .. getItemCid (slaveItem) .. ":\n"
		end
	end

	return s
end

function hasItemRefTarget (item)
	return item.m_hasDocumentation or item.m_subGroupHead
end

function getCompoundTocTree (compound)
	local s = ".. toctree::\n\t:hidden:\n\n"

	for i = 1, #compound.m_groupArray do
		local item = compound.m_groupArray [i]
		local fileName = getItemFileName (item)
		s = s .. "\t" .. fileName .. "\n"
	end

	for i = 1, #compound.m_namespaceArray do
		local item = compound.m_namespaceArray [i]
		local fileName = getItemFileName (item)
		s = s .. "\t" .. fileName .. "\n"
	end

	for i = 1, #compound.m_enumArray do
		local item = compound.m_enumArray [i]
		if not isUnnamedItem (item) then
			local fileName = getItemFileName (item)
			s = s .. "\t" .. fileName .. "\n"
		end
	end

	for i = 1, #compound.m_structArray do
		local item = compound.m_structArray [i]
		local fileName = getItemFileName (item)
		s = s .. "\t" .. fileName .. "\n"
	end

	for i = 1, #compound.m_unionArray do
		local item = compound.m_unionArray [i]
		local fileName = getItemFileName (item)
		s = s .. "\t" .. fileName .. "\n"
	end

	for i = 1, #compound.m_classArray do
		local item = compound.m_classArray [i]
		local fileName = getItemFileName (item)
		s = s .. "\t" .. fileName .. "\n"
	end

	s = string.gsub (s, "%s+$", "")   -- trim trailing whitespace
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

function getPropertyDeclString (item, isRef, indent)
	local s = getLinkedTextString (item.m_returnType, true)

	if item.m_modifiers ~= "" then
		s = string.gsub (s, "property", item.m_modifiers .. " property")
	end

	if g_hasNewLineAfterReturnType then
		s = s .. "\n" .. indent
	else
		s = s .. " "
	end

	if isRef then
		s = s .. ":ref:`" .. getItemName (item)  .. "<doxid-" .. item.m_id .. ">` "
	else
		s = s .. getItemName (item) ..  " "
	end

	if #item.m_paramArray > 0 then
		s = s .. getFunctionParamArrayString (item.m_paramArray, true, indent)
	end

	return s
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

function getVoidFunctionDeclString (func, isRef, indent)
	return getFunctionDeclStringImpl (
		func,
		nil,
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
	local text = getItemInternalDocumentation (item)
	return string.match (text, ":unnamed:([%w/:]+)")
end

function isUnnamedItem (item)
	return item.m_name == "" or string.sub (item.m_name, 1, 1) == "@"
end

function ensureExtraNewLine (s)
	local length = string.len (s)
	if s [length] ~= "\n" then
		return s .. "\n\n"
	elseif length < 2 or s [length - 1] ~= "\n" then
		return s .. "\n"
	else
		return s
	end
end

-------------------------------------------------------------------------------

-- item documentation utils

function removeCommonSpacePrefix (source)

	local prefix = nil

	local s = "\n" .. source -- add leading '\n'

	for newPrefix in string.gmatch (s, "\n([ \t]*)[^%s]") do
		if not prefix then
			prefix = newPrefix
		else
			local len = string.len (prefix)
			local newLen = string.len (newPrefix)
			if newLen < len then
				len = newLen
			end

			for i = 1, len do
				if string.byte (prefix, i) ~= string.byte (newPrefix, i) then
					len = i - 1
					break
				end
			end

			if len == 0 then
				return source
			else
				prefix = string.sub (prefix, 1, len)
			end
		end
	end

	if not prefix then
		return source
	end

	s = string.gsub (s, "\n" .. prefix, "\n") -- remove common prefix
	s = string.sub (s, 2) -- remove leading '\n'

	return s
end

function getDocBlockListContentsImpl (blockList, internalFilter)
	local s = ""

	for i = 1, #blockList do
		local block = blockList [i]
		local isInternal = block.m_blockKind == "internal"

		if isInternal == internalFilter then
			if block.m_blockKind == "simplesect" and block.m_simpleSectionKind == "see" then
				s = s .. ".. rubric:: See also:\n\n"
				s = s .. block.m_text .. getDocBlockListContents (block.m_childBlockList, false)
				s = ensureExtraNewLine (s)
			elseif block.m_blockDoxyKind == "computeroutput" then
				s = s .. "``" .. block.m_text .. "``"
			elseif block.m_blockDoxyKind == "bold" then
				s = s .. "**" .. block.m_text .. "**"
			elseif block.m_blockDoxyKind == "italic" then
				s = s .. "*" .. block.m_text .. "*"
			else
				s = s .. block.m_text .. getDocBlockListContents (block.m_childBlockList, false)

				if block.m_blockKind == "paragraph" then
					s = ensureExtraNewLine (s)
				end
			end
		end
	end

	s = string.gsub (s, "%s+$", "")   -- trim trailing whitespace
	s = string.gsub (s, "\t", "    ") -- replace tabs with spaces

	return removeCommonSpacePrefix (s)
end

function getDocBlockListContents (blockList)
	return getDocBlockListContentsImpl (blockList, false)
end

function getItemInternalDocumentation (item)
	return getDocBlockListContentsImpl (item.m_detailedDescription.m_docBlockList, true)
end

function getItemBriefDocumentation (item, detailsRefPrefix)
	local s = getDocBlockListContents (item.m_briefDescription.m_docBlockList)

	if string.len (s) == 0 then
		s = getDocBlockListContents (item.m_detailedDescription.m_docBlockList)
		if string.len (s) == 0 then
			return ""
		end

		local i = string.find (s, ".", 1, true) -- first sentence only
		if i then
			s = string.sub (s, 1, i)
		end

		s = string.gsub (s, "%s+$", "")   -- trim trailing whitespace
		s = string.gsub (s, "\t", "    ") -- replace tabs with spaces
	end

	if detailsRefPrefix then
		s = s .. " :ref:`More...<" .. detailsRefPrefix .. "doxid-" .. item.m_id .. ">`"
	end

	return s
end

function getItemDetailedDocumentation (item)
	local brief = getDocBlockListContents (item.m_briefDescription.m_docBlockList)
	local detailed = getDocBlockListContents (item.m_detailedDescription.m_docBlockList)

	if string.len (detailed) == 0 then
		return brief
	elseif string.len (brief) == 0 then
		return detailed
	else
		return brief .. "\n\n" .. detailed
	end
end

function isDocumentationEmpty (description)
	if description.m_isEmpty then
		return true
	end

	local text = getDocBlockListContents (description.m_docBlockList)
	return string.len (text) == 0
end

function prepareItemDocumentation (item)
	local hasBriefDocuemtnation = not isDocumentationEmpty (item.m_briefDescription)
	local hasDetailedDocuemtnation = not isDocumentationEmpty (item.m_detailedDescription)

	item.m_hasDocumentation = hasBriefDocuemtnation or hasDetailedDocuemtnation
	if not item.m_hasDocumentation then
		return false
	end

	if hasDetailedDocuemtnation then
		local text = getItemInternalDocumentation (item)

		item.m_isSubGroupHead = string.match (text, ":subgroup:") ~= nil
		if item.m_isSubGroupHead then
			item.m_subGroupSlaveArray = {}
		end
	end

	return true
end

function prepareItemArrayDocumentation (
	itemArray,
	compound
	)

	local hasDocumentation = false
	local subGroupHead = nil

	for i = 1, #itemArray do
		local item = itemArray [i]

		prepareItemDocumentation (item)

		if item.m_hasDocumentation then
			hasDocumentation = true
			if item.m_isSubGroupHead then
				subGroupHead = item
			else
				subGroupHead = nil
			end
		elseif subGroupHead then
			table.insert (subGroupHead.m_subGroupSlaveArray, item)
			item.m_subGroupHead = subGroupHead
		end
	end

	return hasDocumentation
end

-------------------------------------------------------------------------------

-- item filtering utils

g_protectionKindMap = {
	public    = 0,
	protected = 1,
	private   = 2,
	package   = 3,
}

function isItemExcludedByProtectionFilter (item)

	local protectionValue = g_protectionKindMap [item.m_protectionKind]
	if protectionValue and protectionValue > g_protectionFilterValue then
		return true
	end

	return false
end

function filterItemArray (itemArray)
	if next (itemArray) == nil then
		return
	end

	for i = #itemArray, 1, -1 do
		item = itemArray [i]
		local isExcluded = isItemExcludedByProtectionFilter (item)

		if isExcluded then
			print ("filtering out:", item.m_name)
			table.remove (itemArray, i)
		end
	end
end

function filterConstructorArray (constructorArray)
	if next (constructorArray) == nil then
		return
	end

	for i = #constructorArray, 1, -1 do
		item = constructorArray [i]
		local isExcluded =
			isItemExcludedByProtectionFilter (item) or
			not g_includeDefaultConstructors and #item.m_paramArray == 0

		if isExcluded then
			table.remove (defineArray, i)
		end
	end
end

function filterDefineArray (defineArray)
	if next (defineArray) == nil then
		return
	end

	for i = #defineArray, 1, -1 do
		item = defineArray [i]

		local isExcluded =
			not g_includeEmptyDefines and item.m_initializer.m_isEmpty or
			g_excludeDefinePattern and string.match (item.m_name, g_excludeDefinePattern)

		if isExcluded then
			table.remove (defineArray, i)
		end
	end
end

function filterTypedefArray (typedefArray)
	if next (typedefArray) == nil then
		return
	end

	for i = #typedefArray, 1, -1 do
		item = typedefArray [i]

		local isExcluded = isItemExcludedByProtectionFilter (item)
		if name == item.m_name then
			table.remove (typedefArray, i)
		end

		if not g_includePrimitiveTypedefs then
			local typeKind, name = string.match (
				item.m_type.m_plainText,
				"(%a+)%s+(%w[%w_]*)"
				)

			if name == item.m_name then
				table.remove (typedefArray, i)
			end
		end
	end
end

-------------------------------------------------------------------------------

-- item filtering utils

function cmpIds (g1, g2)
	return g1.m_id < g2.m_id
end

function cmpNames (g1, g2)
	return g1.m_name < g2.m_name
end

function prepareCompound (compound)
	local stats = {}

	-- filter invisible items out

	filterTypedefArray (compound.m_typedefArray)
	filterItemArray (compound.m_enumArray)
	filterItemArray (compound.m_structArray)
	filterItemArray (compound.m_unionArray)
	filterItemArray (compound.m_classArray)
	filterItemArray (compound.m_variableArray)
	filterItemArray (compound.m_propertyArray)
	filterItemArray (compound.m_eventArray)
	filterConstructorArray (compound.m_constructorArray)
	filterItemArray (compound.m_functionArray)
	filterItemArray (compound.m_aliasArray)
	filterDefineArray (compound.m_defineArray)

	-- scan for documentation and create subgroups

	stats.m_hasUnnamedEnums = false
	stats.m_hasDocumentedUnnamedEnumValues = false

	for i = 1, #compound.m_enumArray do
		local item = compound.m_enumArray [i]
		if isUnnamedItem (item) then
			stats.m_hasUnnamedEnums = true

			if prepareItemArrayDocumentation (item.m_enumValueArray) then
				stats.m_hasDocumentedUnnamedEnumValues = true
			end
		end
	end

	stats.m_hasDocumentedTypedefs = prepareItemArrayDocumentation (compound.m_typedefArray);
	stats.m_hasDocumentedVariables = prepareItemArrayDocumentation (compound.m_variableArray);
	stats.m_hasDocumentedProperties = prepareItemArrayDocumentation (compound.m_propertyArray);
	stats.m_hasDocumentedEvents = prepareItemArrayDocumentation (compound.m_eventArray);
	stats.m_hasDocumentedFunctions = prepareItemArrayDocumentation (compound.m_functionArray);
	stats.m_hasDocumentedAliases = prepareItemArrayDocumentation (compound.m_aliasArray);
	stats.m_hasDocumentedDefines = prepareItemArrayDocumentation (compound.m_defineArray);

	stats.m_hasDocumentedConstruction =
		prepareItemArrayDocumentation (compound.m_constructorArray) or
		g_includeDestructors and compound.m_destructor and prepareItemDocumentation (compound.m_destructor)

	stats.m_hasDocumentedItems =
		stats.m_hasDocumentedUnnamedEnumValues or
		stats.m_hasDocumentedTypedefs or
		stats.m_hasDocumentedVariables or
		stats.m_hasDocumentedProperties or
		stats.m_hasDocumentedEvents or
		stats.m_hasDocumentedConstruction or
		stats.m_hasDocumentedFunctions or
		stats.m_hasDocumentedAliases or
		stats.m_hasDocumentedDefines

	stats.m_hasBriefDocumentation = not isDocumentationEmpty (compound.m_briefDescription)
	stats.m_hasDetailedDocumentation = not isDocumentationEmpty (compound.m_detailedDescription)

	-- sort items (only the ones producing separate pages)

	table.sort (compound.m_groupArray, cmpIds)
	table.sort (compound.m_namespaceArray, cmpNames)
	table.sort (compound.m_enumArray, cmpNames)
	table.sort (compound.m_structArray, cmpNames)
	table.sort (compound.m_unionArray, cmpNames)
	table.sort (compound.m_classArray, cmpNames)

	return stats
end

-------------------------------------------------------------------------------
