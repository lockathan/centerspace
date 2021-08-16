function start(campaign)
	
	campaign:getInventoryState():setRemainingCash(10000)
	mcuRepos = MCUTemplateRepository:getSingletonPtr()
	Aphid = mcuRepos:findByNameOrID("Aphid")
	Wraith = mcuRepos:findByNameOrID("Wraith")
	Raptor = mcuRepos:findByNameOrID("Raptor")
	campaign:getInventoryState():addMCU(Aphid:instantiate(true))
	campaign:getInventoryState():addMCU(Aphid:instantiate(true))
	campaign:getInventoryState():addMCU(Wraith:instantiate(true))
	campaign:getInventoryState():addMCU(Wraith:instantiate(true))
	campaign:getInventoryState():addMCU(Raptor:instantiate(true))
end
