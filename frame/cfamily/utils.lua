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

dofile(g_frameDir .. "/../common/string.lua")
dofile(g_frameDir .. "/../common/table.lua")
dofile(g_frameDir .. "/../common/item.lua")
dofile(g_frameDir .. "/../common/doc.lua")
dofile(g_frameDir .. "/../common/toc.lua")

if not LANGUAGE then
	LANGUAGE = "cpp"
end

if not INDEX_TITLE then
	INDEX_TITLE = "My Project Documentation"
end

if not EXTRA_PAGE_LIST then
	EXTRA_PAGE_LIST = {}
end

if not IMPORT_URL_MAP then
	IMPORT_URL_MAP = {}
end

if ML_PARAM_LIST_COUNT_THRESHOLD then
	ML_PARAM_LIST_COUNT_THRESHOLD = tonumber(ML_PARAM_LIST_COUNT_THRESHOLD)
end

if ML_PARAM_LIST_LENGTH_THRESHOLD then
	ML_PARAM_LIST_LENGTH_THRESHOLD = tonumber(ML_PARAM_LIST_LENGTH_THRESHOLD)
end

if string.match(LANGUAGE, "^c[px+]*$") then
	g_nameDelimiter = "::"
else
	g_nameDelimiter = "."
end

if PROTECTION_FILTER then
	g_protectionFilterValue = g_protectionKindMap[PROTECTION_FILTER]
else
	g_protectionFilterValue = 0
end

-------------------------------------------------------------------------------

function getNormalizedCppString(string)
	local s = string
	local space = ""

	if PRE_PARAM_LIST_SPACE then
		space = " "
	end

	s = string.gsub(s, "%s*%*", "*")
	s = string.gsub(s, "%s*&", "&")
	s = string.gsub(s, "<%s*", space .. "<")
	s = string.gsub(s, "%s+>", ">")
	s = string.gsub(s, "%(%s*", space .. "(")
	s = string.gsub(s, "%s+%)", ")")

	return s
end

function getLinkedTextString(text, isRef)
	if not text then
		return ""
	end

	if not isRef then
		return text.plainText
	end

	local s = ""

	for i = 1, #text.refTextArray do
		local refText = text.refTextArray[i]
		local text = getNormalizedCppString(refText.text)

		if refText.id ~= "" then
			text = string.gsub(text, "<", "\\<") -- escape left chevron
			s = s .. ":ref:`" .. text .. "<doxid-" .. refText.id .. ">`"
		else
			s = s .. text
		end
	end

	s = string.gsub(s, "\n", " ") -- callsites of getLinkedTextString don't expect newline chars

	return s
end

-------------------------------------------------------------------------------

-- param array formatting

function getParamString(param, isRef)
	local s = ""
	local name

	if not param.type.isEmpty then
		s = s .. getLinkedTextString(param.type, isRef)
	end

	if param.declarationName ~= "" then
		name = param.declarationName
	else
		name = param.definitionName
	end

	if name ~= "" then
		if s ~= "" then
			s = s .. " "
		end

		s = s .. getNormalizedCppString(name)
	end

	if param.array ~= "" then
		s = s .. " " .. param.array
	end

	if not param.defaultValue.isEmpty then
		s = s .. " = " .. getLinkedTextString(param.defaultValue, isRef)
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

function getParamArrayString(prefix, paramArray, isRef, lbrace, rbrace, indent, nl)
	if not lbrace then
		lbrace = "("
	end

	if not rbrace then
		rbrace = ")"
	end

	if not indent then
		indent = "\t"
	end

	if not nl then
		nl = "\n"
	end

	local s = ""
	local space = ""

	if PRE_PARAM_LIST_SPACE then
		space = " "
	end

	if ML_PARAM_LIST_COUNT_THRESHOLD and
		#paramArray > ML_PARAM_LIST_COUNT_THRESHOLD then
		s = getParamArrayString_ml(paramArray, isRef, lbrace, rbrace, indent, nl)
	else
		s = getParamArrayString_sl(paramArray, isRef, lbrace, rbrace)

		if ML_PARAM_LIST_LENGTH_THRESHOLD then
			local decl = prefix .. space .. s
			if isRef then
				decl = string.gsub(decl, ":ref:`[^`]*`", "")
			end

			if string.len(decl) > ML_PARAM_LIST_LENGTH_THRESHOLD then
				s = getParamArrayString_ml(paramArray, isRef, lbrace, rbrace, indent, nl)
			end
		end
	end

	s = space .. s
	return s
end

-------------------------------------------------------------------------------

function getItemKindString(item, itemKindString)
	local s = ""

	if item.modifiers ~= "" then
		s = item.modifiers .. " "
	end

	s = s .. itemKindString
	return s
end

function getTemplateSpecSuffix(prefix, item)
	if not item.templateSpecParamArray or #item.templateSpecParamArray == 0 then
		return ""
	end

	return getParamArrayString(prefix, item.templateSpecParamArray, false, "<", ">")
end

function getItemSimpleName(item)
	return item.name .. getTemplateSpecSuffix(item.name, item)
end

function getItemQualifiedName(item)
	local name = string.gsub(item.path, "/", g_nameDelimiter)
	return name .. getTemplateSpecSuffix(name, item)
end

getItemName = getItemQualifiedName

function getItemNameForOverview(item)
	if hasItemRefTarget(item) then
		return ":ref:`" .. getItemName(item) .. "<doxid-" .. item.id .. ">`"
	else
		return getItemName(item)
	end
end

function getEnumOverviewRefTargetString(enum)
	local s = ""

	for i = 1, #enum.enumValueArray do
		local enumValue = enum.enumValueArray[i]
		if not hasItemRefTarget(enumValue) then
			s = s .. getItemRefTargetString(enumValue)
		end
	end

	return s
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

function getFunctionModifierDelimiter(indent)
	if ML_SPECIFIER_MODIFIER_LIST then
		return "\n" .. indent
	else
		return " "
	end
end

function getPropertyDeclString(item, isRef, indent)
	local s = getLinkedTextString(item.returnType, true)

	if item.modifiers ~= "" then
		s = string.gsub(s, "property", item.modifiers .. " property")
	end

	s = s .. getFunctionModifierDelimiter(indent)

	if isRef then
		s = s .. ":ref:`" .. getItemName(item)  .. "<doxid-" .. item.id .. ">`"
	else
		s = s .. getItemName(item)
	end

	if #item.paramArray > 0 then
		s = s .. getParamArrayString(s, item.paramArray, true, "(", ")", indent)
	end

	return s
end

function getFunctionDeclStringImpl(item, returnType, isRef, indent)
	local s = ""

	if item.templateParamArray and #item.templateParamArray > 0 or
		item.templateSpecParamArray and #item.templateSpecParamArray > 0 then
		s = "template "
		if not PRE_PARAM_LIST_SPACE then
			s = s .. " "
		end
		s = s .. getParamArrayString(s, item.templateParamArray, false, "<", ">") .. "\n" .. indent
	end

	if string.find(item.flags, "static") then
		s = s .. "static" .. getFunctionModifierDelimiter(indent)
	elseif item.virtualKind == "pure-virtual" then
		s = s .. "virtual" .. getFunctionModifierDelimiter(indent)
	elseif item.virtualKind ~= "non-virtual" then
		s = s .. item.virtualKind .. getFunctionModifierDelimiter(indent)
	end

	if returnType and returnType ~= "" then
		s = s .. returnType .. getFunctionModifierDelimiter(indent)
	end

	if item.modifiers ~= "" then
		s = s .. item.modifiers .. getFunctionModifierDelimiter(indent)
	end

	if isRef then
		s = s .. ":ref:`" .. getItemName(item)  .. "<doxid-" .. item.id .. ">`"
	else
		s = s .. getItemName(item)
	end

	s = s .. getParamArrayString(s, item.paramArray, true, "(", ")", indent)

	if string.find(item.flags, "const") then
		s = s .. " const"
	end

	if item.virtualKind == "pure-virtual" then
		s = s .. " = 0"
	end

	return s
end

function getFunctionDeclString(func, isRef, indent)
	return getFunctionDeclStringImpl(
		func,
		getLinkedTextString(func.returnType, true),
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
		s = s .. ":ref:`" .. define.name  .. "<doxid-" .. define.id .. ">`"
	else
		s = s .. define.name
	end

	if #define.paramArray > 0 then
		-- no space between name and params!

		s = s .. getParamArrayString(s, define.paramArray, false, "(", ")", indent, " \\\n")
	end

	return s
end

function getTypedefDeclString(typedef, isRef, indent)
	local s = "typedef"

	if next(typedef.paramArray) == nil then
		s = s .. " " .. getLinkedTextString(typedef.type, true) .. " "

		if isRef then
			s = s .. ":ref:`" .. getItemName(typedef)  .. "<doxid-" .. typedef.id .. ">`"
		else
			s = s .. getItemName(typedef)
		end

		if typedef.argString ~= "" then
			s = s .. formatArgDeclString(typedef.argString, indent)
		end

		return s
	end

 	s = s .. getFunctionModifierDelimiter(indent) .. getLinkedTextString(typedef.type, true) .. getFunctionModifierDelimiter(indent)

	if isRef then
		s = s .. ":ref:`" .. getItemName(typedef)  .. "<doxid-" .. typedef.id .. ">`"
	else
		s = s .. getItemName(typedef)
	end

	s = s .. getParamArrayString(s, typedef.paramArray, true, "(", ")", indent)
	return s
end

function getClassDeclString(class, isRef, indent)
	local s = ""

	if #class.templateParamArray > 0 or #class.templateSpecParamArray > 0 then
		s = "template"
		if not PRE_PARAM_LIST_SPACE then
			s = s .. " "
		end
		s = s .. getParamArrayString(s, class.templateParamArray, false, "<", ">", indent) .. "\n" .. indent
	end

	local name = getItemName(class)

	s = s .. class.compoundKind .. " "

	if isRef then
		s = s .. ":ref:`" .. name  .. "<doxid-" .. class.id .. ">` "
	else
		s = s .. name
	end

	return s
end

function getBaseClassString(class, protection)
	local s = ""

	if string.match(LANGUAGE, "^c[px+]*$") then
		s = protection .. " "
	end

	if class.id ~= "" then
		s = s .. ":ref:`" .. getItemQualifiedName(class) .. "<doxid-" .. class.id .. ">`"
	else
		local url = IMPORT_URL_MAP[class.importId]
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
	return item.name == "" or string.sub(item.name, 1, 1) == "@"
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
	bracket[0].result = ""

	local level = 0
	local pos = 1
	local len = string.len(decl)

	for i = 1, len do
		local c = string.sub(decl, i, i)

		if c == "(" or c == "[" or c == "<" or c == "{" then
			local chunk = trimLeadingWhitespace(string.sub(decl, pos, i))
			pos = i + 1

			bracket[level].result = bracket[level].result .. chunk

			level = level + 1
			bracket[level] = {}
			bracket[level].closingChar = g_closingBracketChar[c]
			bracket[level].result = ""
		elseif level > 0 then
			if c == ',' then
				if not bracket[level].delimiter then
					bracket[level].delimiter = "\n" .. indent .. string.rep("    ", level)
					bracket[level].result = bracket[level].delimiter .. bracket[level].result
				end

				local chunk = trimLeadingWhitespace(string.sub(decl, pos, i))
				pos = i + 1

				bracket[level].result = bracket[level].result .. chunk .. bracket[level].delimiter
				pos = i + 1
			elseif c == bracket[level].closingChar then
				local chunk = trimLeadingWhitespace(string.sub(decl, pos, i - 1))
				pos = i

				bracket[level].result = bracket[level].result .. chunk

				if bracket[level].delimiter then
					bracket[level].result = bracket[level].result .. bracket[level].delimiter
				end

				level = level - 1
				bracket[level].result = bracket[level].result .. bracket[level + 1].result
			end
		end
	end

	if pos <= len then
		bracket[0].result = bracket[0].result .. string.sub(decl, pos)
	end

	return bracket[0].result
end

-------------------------------------------------------------------------------

-- item filtering utils

g_protectionKindMap = {
	public    = 0,
	protected = 1,
	private   = 2,
	package   = 3,
}

g_protectionNameMap = {
	[0] = "public",
	[1] = "protected",
	[2] = "private",
	[3] = "package",
}

g_minProtectionValue = 0
g_maxProtectionValue = 3

function isItemExcludedByProtectionFilter(item)

	local protectionValue = g_protectionKindMap[item.protectionKind]
	if protectionValue and protectionValue > g_protectionFilterValue then
		return true
	end

	return false
end

function hasNonPublicItems(array)
	if next(array) == nil then
		return false
	end

	local lastItem = array[#array]
	local protectionValue = g_protectionKindMap[lastItem.protectionKind]
	return protectionValue and protectionValue > 0
end

function isItemExcludedByLocationFilter(item)
	if not item.location then
		return false
	end

	return
		-- exclude explicitly specified locations
		EXCLUDE_LOCATION_PATTERN and
		string.match(item.location.file, EXCLUDE_LOCATION_PATTERN)
end

function filterItemArray(itemArray)
	if next(itemArray) == nil then
		return
	end

	for i = #itemArray, 1, -1 do
		local item = itemArray[i]
		local isExcluded =
			isItemExcludedByProtectionFilter(item) or
			isItemExcludedByLocationFilter(item)

		if isExcluded then
			table.remove(itemArray, i)
		end
	end
end

function filterEnumArray(enumArray)
	if next(enumArray) == nil then
		return
	end

	for i = #enumArray, 1, -1 do
		local enum = enumArray[i]
		local isExcluded =
			isItemExcludedByProtectionFilter(enum) or
			isItemExcludedByLocationFilter(enum) or
			isUnnamedItem(enum) and #enum.enumValueArray == 0

		if isExcluded then
			table.remove(enumArray, i)
		end
	end
end

function filterNamespaceArray(namespaceArray)
	if next(namespaceArray) == nil then
		return
	end

	for i = #namespaceArray, 1, -1 do
		local item = namespaceArray[i]
		local isExcluded =
			isUnnamedItem(item) or
			isItemExcludedByLocationFilter(item)

		if isExcluded then
			table.remove(namespaceArray, i)
		end
	end
end

function filterConstructorArray(constructorArray)
	filterItemArray(constructorArray)

	if #constructorArray == 1 then
		local item = constructorArray[1]
		local isExcluded =
			isItemExcludedByProtectionFilter(item) or
			EXCLUDE_DEFAULT_CONSTRUCTOR and #item.paramArray == 0

		if isExcluded then
			table.remove(constructorArray, 1)
		end
	end
end

function filterDefineArray(defineArray)
	if next(defineArray) == nil then
		return
	end

	for i = #defineArray, 1, -1 do
		local item = defineArray[i]

		local isExcluded =
			isItemExcludedByLocationFilter(item) or
			EXCLUDE_EMPTY_DEFINES and item.initializer.isEmpty or
			EXCLUDE_DEFINE_PATTERN and string.match(item.name, EXCLUDE_DEFINE_PATTERN)

		if isExcluded then
			table.remove(defineArray, i)
		end
	end
end

function filterTypedefArray(typedefArray)
	if next(typedefArray) == nil then
		return
	end

	for i = #typedefArray, 1, -1 do
		local item = typedefArray[i]
		local isExcluded =
			isItemExcludedByProtectionFilter(item) or
			isItemExcludedByLocationFilter(item)

		if not isExcluded and EXCLUDE_PRIMITIVE_TYPEDEFS then
			local typeKind, name = string.match(
				item.type.plainText,
				"(%a+)%s+(%w[%w_]*)"
				)

			if name == item.name then
				isExcluded =
					item.briefDescription.isEmpty and
					item.detailedDescription.isEmpty
			end
		end

		if isExcluded then
			table.remove(typedefArray, i)
		end
	end
end

-------------------------------------------------------------------------------

-- base compound is an artificial compound holding all inherited members

function addToBaseCompound(baseCompound, baseTypeArray)
	for i = 1, #baseTypeArray do
		local baseType = baseTypeArray[i]

		-- prevent adding the same base type multiple times

		if not baseCompound.baseTypeMap[baseType] and
			baseType.compoundKind ~= "<undefined>" then

			baseCompound.baseTypeMap[baseType] = true
			prepareCompound(baseType)

			if next(baseType.baseTypeArray) ~= nil then
				addToBaseCompound(baseCompound, baseType.baseTypeArray)
			end

			concatenateTables(baseCompound.typedefArray, baseType.typedefArray)
			concatenateTables(baseCompound.enumArray, baseType.enumArray)
			concatenateTables(baseCompound.structArray, baseType.structArray)
			concatenateTables(baseCompound.unionArray, baseType.unionArray)
			concatenateTables(baseCompound.interfaceArray, baseType.interfaceArray)
			concatenateTables(baseCompound.protocolArray, baseType.protocolArray)
			concatenateTables(baseCompound.exceptionArray, baseType.exceptionArray)
			concatenateTables(baseCompound.classArray, baseType.classArray)
			concatenateTables(baseCompound.singletonArray, baseType.singletonArray)
			concatenateTables(baseCompound.serviceArray, baseType.serviceArray)
			concatenateTables(baseCompound.variableArray, baseType.variableArray)
			concatenateTables(baseCompound.propertyArray, baseType.propertyArray)
			concatenateTables(baseCompound.eventArray, baseType.eventArray)
			concatenateTables(baseCompound.functionArray, baseType.functionArray)
			concatenateTables(baseCompound.aliasArray, baseType.aliasArray)
		end
	end
end

function createPseudoCompound(compoundKind)
	local compound = {}

	compound.compoundKind = compoundKind
	compound.baseTypeMap = {}
	compound.namespaceArray = {}
	compound.typedefArray = {}
	compound.enumArray = {}
	compound.structArray = {}
	compound.unionArray = {}
	compound.interfaceArray = {}
	compound.protocolArray = {}
	compound.exceptionArray = {}
	compound.classArray = {}
	compound.singletonArray = {}
	compound.serviceArray = {}
	compound.variableArray = {}
	compound.propertyArray = {}
	compound.eventArray = {}
	compound.constructorArray = {}
	compound.functionArray = {}
	compound.aliasArray = {}
	compound.defineArray = {}

	return compound
end

function createBaseCompound(compound)
	local baseCompound = createPseudoCompound("base-compound")
	baseCompound.isBaseCompound = true
	addToBaseCompound(baseCompound, compound.baseTypeArray)
	baseCompound.hasItems = hasCompoundItems(baseCompound)
	handleCompoundProtection(baseCompound)

	compound.baseCompound = baseCompound
end

function sortByProtection(array)
	if next(array) == nil then
		return
	end

	local bucketArray  = {}
	for i = g_minProtectionValue, g_maxProtectionValue do
		bucketArray[i] = {}
	end

	for i = 1, #array do
		local item = array[i]
		local protectionValue = g_protectionKindMap[item.protectionKind]

		if not protectionValue then
			protectionValue = 0 -- assume public
		end

		table.insert(bucketArray[protectionValue], item)
	end

	local result = {}
	local k = 1

	for i = g_minProtectionValue, g_maxProtectionValue do
		local bucket = bucketArray[i]

		for j = 1, #bucket do
			array[k] = bucket[j]
			k = k + 1
		end
	end

	assert(k == #array + 1)

	return #array - #bucketArray[0]
end

function addToProtectionCompounds(compound, arrayName)
	local array = compound[arrayName]

	if next(array) == nil then
		return
	end

	for i = 1, #array do
		local item = array[i]
		local protectionValue = g_protectionKindMap[item.protectionKind]

		if not protectionValue then
			protectionValue = 0 -- assume public
		end

		local protectionCompound = compound.protectionCompoundArray[protectionValue]
		local targetArray = protectionCompound[arrayName]

		table.insert(targetArray, item)
		protectionCompound.isEmpty = nil
	end
end

function handleCompoundProtection(compound)

	compound.protectionCompoundArray = {}

	for i = g_minProtectionValue, g_maxProtectionValue do
		local protectionCompound = createPseudoCompound("protection-compound")
		protectionCompound.isEmpty = true
		protectionCompound.isBaseCompound = compound.isBaseCompound
		compound.protectionCompoundArray[i] = protectionCompound
	end

	addToProtectionCompounds(compound, "namespaceArray")
	addToProtectionCompounds(compound, "typedefArray")
	addToProtectionCompounds(compound, "enumArray")
	addToProtectionCompounds(compound, "structArray")
	addToProtectionCompounds(compound, "unionArray")
	addToProtectionCompounds(compound, "interfaceArray")
	addToProtectionCompounds(compound, "protocolArray")
	addToProtectionCompounds(compound, "exceptionArray")
	addToProtectionCompounds(compound, "classArray")
	addToProtectionCompounds(compound, "singletonArray")
	addToProtectionCompounds(compound, "serviceArray")
	addToProtectionCompounds(compound, "variableArray")
	addToProtectionCompounds(compound, "propertyArray")
	addToProtectionCompounds(compound, "eventArray")
	addToProtectionCompounds(compound, "constructorArray")
	addToProtectionCompounds(compound, "functionArray")
	addToProtectionCompounds(compound, "aliasArray")
	addToProtectionCompounds(compound, "defineArray")

	-- eliminate empty protection compounds

	for i = g_minProtectionValue, g_maxProtectionValue do
		if compound.protectionCompoundArray[i].isEmpty then
			compound.protectionCompoundArray[i] = nil
		end
	end
end

-------------------------------------------------------------------------------

-- compound & enum prep

function hasCompoundItems(compound)
	return
		#compound.namespaceArray ~= 0 or
		#compound.typedefArray ~= 0 or
		#compound.enumArray ~= 0 or
		#compound.structArray ~= 0 or
		#compound.unionArray ~= 0 or
		#compound.interfaceArray ~= 0 or
		#compound.protocolArray ~= 0 or
		#compound.exceptionArray ~= 0 or
		#compound.classArray ~= 0 or
		#compound.singletonArray ~= 0 or
		#compound.serviceArray ~= 0 or
		#compound.variableArray ~= 0 or
		#compound.propertyArray ~= 0 or
		#compound.eventArray ~= 0 or
		#compound.constructorArray ~= 0 or
		#compound.functionArray ~= 0 or
		#compound.aliasArray ~= 0 or
		#compound.defineArray ~= 0
end

function prepareCompound(compound)
	if compound.stats then
		return compound.stats
	end

	local stats = {}

	-- filter invisible items out

	if compound.derivedTypeArray then
		filterItemArray(compound.derivedTypeArray)
	end

	filterNamespaceArray(compound.namespaceArray)
	filterTypedefArray(compound.typedefArray)
	filterEnumArray(compound.enumArray)
	filterItemArray(compound.structArray)
	filterItemArray(compound.unionArray)
	filterItemArray(compound.interfaceArray)
	filterItemArray(compound.protocolArray)
	filterItemArray(compound.exceptionArray)
	filterItemArray(compound.classArray)
	filterItemArray(compound.singletonArray)
	filterItemArray(compound.serviceArray)
	filterItemArray(compound.variableArray)
	filterItemArray(compound.propertyArray)
	filterItemArray(compound.eventArray)
	filterConstructorArray(compound.constructorArray)
	filterItemArray(compound.functionArray)
	filterItemArray(compound.aliasArray)
	filterDefineArray(compound.defineArray)

	stats.hasItems = hasCompoundItems(compound)

	-- scan for documentation and create subgroups

	stats.hasUnnamedEnums = false
	stats.hasDocumentedUnnamedEnumValues = false

	for i = 1, #compound.enumArray do
		local item = compound.enumArray[i]

		if isUnnamedItem(item) then
			stats.hasUnnamedEnums = true

			if prepareItemArrayDocumentation(item.enumValueArray, compound) then
				stats.hasDocumentedUnnamedEnumValues = true
			end
		end
	end

	stats.hasDocumentedTypedefs = prepareItemArrayDocumentation(compound.typedefArray, compound)
	stats.hasDocumentedVariables = prepareItemArrayDocumentation(compound.variableArray, compound)
	stats.hasDocumentedProperties = prepareItemArrayDocumentation(compound.propertyArray, compound)
	stats.hasDocumentedEvents = prepareItemArrayDocumentation(compound.eventArray, compound)
	stats.hasDocumentedFunctions = prepareItemArrayDocumentation(compound.functionArray, compound)
	stats.hasDocumentedAliases = prepareItemArrayDocumentation(compound.aliasArray, compound)
	stats.hasDocumentedDefines = prepareItemArrayDocumentation(compound.defineArray, compound)

	stats.hasDocumentedConstruction =
		prepareItemArrayDocumentation(compound.constructorArray, compound) or
		not EXCLUDE_DESTRUCTORS and compound.destructor and prepareItemDocumentation(compound.destructor, compound)

	stats.hasDocumentedItems =
		stats.hasDocumentedUnnamedEnumValues or
		stats.hasDocumentedTypedefs or
		stats.hasDocumentedVariables or
		stats.hasDocumentedProperties or
		stats.hasDocumentedEvents or
		stats.hasDocumentedConstruction or
		stats.hasDocumentedFunctions or
		stats.hasDocumentedAliases or
		stats.hasDocumentedDefines

	stats.hasBriefDocumentation = not isDocumentationEmpty(compound.briefDescription)
	stats.hasDetailedDocumentation = not isDocumentationEmpty(compound.detailedDescription)

	-- sort items -- only the ones producing separate pages;
	-- also, defines, which always go to the global namespace

	table.sort(compound.groupArray, cmpIds)
	table.sort(compound.namespaceArray, cmpNames)
	table.sort(compound.enumArray, cmpNames)
	table.sort(compound.structArray, cmpNames)
	table.sort(compound.unionArray, cmpNames)
	table.sort(compound.interfaceArray, cmpNames)
	table.sort(compound.protocolArray, cmpNames)
	table.sort(compound.exceptionArray, cmpNames)
	table.sort(compound.classArray, cmpNames)
	table.sort(compound.serviceArray, cmpNames)
	table.sort(compound.defineArray, cmpNames)

	compound.stats = stats

	if compound.baseTypeArray and next(compound.baseTypeArray) ~= nil then
		createBaseCompound(compound)
	end

	handleCompoundProtection(compound)

	return stats
end

function prepareEnum(enum)
	local stats = {}

	stats.hasDocumentedEnumValues = prepareItemArrayDocumentation(enum.enumValueArray)
	stats.hasBriefDocumentation = not isDocumentationEmpty(enum.briefDescription)
	stats.hasDetailedDocumentation = not isDocumentationEmpty(enum.detailedDescription)

	return stats
end

-------------------------------------------------------------------------------