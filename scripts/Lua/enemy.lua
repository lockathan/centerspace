function generate(pstate, rank, faction, ownGroupStrength)
	
	GameManager:getSingletonPtr():luaDebugReport("ownGroupStrength: ", ownGroupStrength)
	if rank == 0 then
		rank = 1
	end
	
	maxTotalMaxGroupStrength =  rank * ownGroupStrength
	maxTotalMaxGroupStrength = maxTotalMaxGroupStrength - 4
	currentTotalGroupStrength = 0
	
	GameManager:getSingletonPtr():luaDebugReport("maxTotalMaxGroupStrength: ", maxTotalMaxGroupStrength)
	while currentTotalGroupStrength < maxTotalMaxGroupStrength do
	
		groupTemplate = AIGroupTemplateRepository:getSingletonPtr():getRandomAIGroupTemplate(faction:getId(), rank)
		groupStrength = groupTemplate:getStrength() * groupTemplate:getRequiredRank()
		GameManager:getSingletonPtr():luaDebugReport("groupStrength: ", groupStrength)
		if groupStrength < 1 then
			groupStrength = 1
		end
		
		pstate:addAIGroupTemplate(groupTemplate)
		currentTotalGroupStrength = currentTotalGroupStrength + groupStrength
		
	end
end
