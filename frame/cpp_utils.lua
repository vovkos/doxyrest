-------------------------------------------------------------------------------

function getSingleArgString (param)
	local s = string.gsub (param.m_type, " %*", "\\*") .. " " .. param.m_declarationName

	if param.m_defaultValue ~= "" then
		s = s .. " = "
		s = s .. param.m_defaultValue
	end

	return s
end

function getFunctionArgString (paramArray)
	local s
	local count = #paramArray

	if count == 0 then
		s = "()"
	elseif count == 1 then
		s = "(" .. getSingleArgString (paramArray [1]) .. ")"
	else
		s = "(\n|\t"

		for i = 1, count do
			s = s .. getSingleArgString (paramArray [i])

			if i ~= count then
				s = s .. ","
			end

			s = s .. "\n|\t"
		end
		s = s .. ")"
	end

	return s
end

function getItemFileName (item, parentCompound)	
	local s
	
	if (item.m_compoundKind) then
		s = item.m_compoundKind .. "_"
	elseif (item.m_memberKind) then
		s = item.m_memberKind .. "_"
	else
		s = "undef_"
	end

	if parentCompound.m_name then
		s = s .. string.gsub (parentCompound.m_path, "/", "_") .. "_"
	end

	s = s .. item.m_name .. ".rst"

	return s
end

function getCompoundTocTree (compound, indent)
	local s = ""

	for i = 1, #compound.m_namespaceArray do
		local fileName = getItemFileName (compound.m_namespaceArray [i], compound)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_enumArray do
		local fileName = getItemFileName (compound.m_enumArray [i], compound)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_structArray do
		local fileName = getItemFileName (compound.m_structArray [i], compound)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_unionArray do
		local fileName = getItemFileName (compound.m_unionArray [i], compound)
		s = s .. indent .. fileName .. "\n"
	end

	for i = 1, #compound.m_classArray do
		local fileName = getItemFileName (compound.m_classArray [i], compound)
		s = s .. indent .. fileName .. "\n"
	end

	return s
end

function getTitle (title, underline)
	return title .. "\n" .. string.rep (underline, #title)
end

-------------------------------------------------------------------------------

