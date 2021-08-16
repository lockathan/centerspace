function start(campaign)
	
	campaign:getInventoryState():setRemainingCash(10000)
	mcuRepos = MCUTemplateRepository:getSingletonPtr()
	Kindjal = mcuRepos:findByNameOrID("Kindjal")
	Rapier = mcuRepos:findByNameOrID("Rapier")
	Broadsword = mcuRepos:findByNameOrID("Broadsword")
	
	campaign:getInventoryState():addMCU(Kindjal:instantiate(true))
	campaign:getInventoryState():addMCU(Kindjal:instantiate(true))
	campaign:getInventoryState():addMCU(Rapier:instantiate(true))
	campaign:getInventoryState():addMCU(Rapier:instantiate(true))
	campaign:getInventoryState():addMCU(Broadsword:instantiate(true))
	
end