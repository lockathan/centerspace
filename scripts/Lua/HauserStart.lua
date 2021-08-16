function start(campaign)
	campaign:getInventoryState():setRemainingCash(10000)
	mcuRepos = MCUTemplateRepository:getSingletonPtr()
	Interceptor = mcuRepos:findByNameOrID("Interceptor")
	Warthog = mcuRepos:findByNameOrID("Warthog")
	Medusa = mcuRepos:findByNameOrID("Medusa")
	campaign:getInventoryState():addMCU(Interceptor:instantiate(true))
	campaign:getInventoryState():addMCU(Interceptor:instantiate(true))
	
	campaign:getInventoryState():addMCU(Warthog:instantiate(true))
	campaign:getInventoryState():addMCU(Warthog:instantiate(true))
	
	campaign:getInventoryState():addMCU(Medusa:instantiate(true))
	
	
end