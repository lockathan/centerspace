function OnDataBind(dataset, id, element)
	if dataset:getDataSource() == MCU then
		if id == -1 then
			return
		end
		
		batteryEnergyRemaining = tonumber(dataset:getValue(id, "@RemainingBatteryPower@"))
		maxBatteryEnergy = tonumber(dataset:getValue(id, "@MaxBatteryPower@"))
		
		reactorEnergyRemaining = tonumber(dataset:getValue(id, "@RemainingReactorPower@"))
		maxReactorEnergy = tonumber(dataset:getValue(id, "@MaxReactorPower@"))
		
		shieldEnergyRemaining = tonumber(dataset:getValue(id, "@RemainingShieldPower@"))
		maxShieldEnergy = tonumber(dataset:getValue(id, "@MaxShieldPower@"))
		
		batteryStatus = math.floor(batteryEnergyRemaining / maxBatteryEnergy * 19) + 1
		reactorStatus = math.floor(reactorEnergyRemaining / maxReactorEnergy * 19) + 1
		shieldStatus = math.floor(shieldEnergyRemaining / maxShieldEnergy * 19) + 1
		
		batteryImageElement = element:getOwnerDocument():getElementById("batteryimage")
		reactorImageElement = element:getOwnerDocument():getElementById("reactorimage")
		shieldImageElement = element:getOwnerDocument():getElementById("shieldimage")
		
		if batteryImageElement ~= nil then
			batteryImageElement:setAttribute("src", "img/batterystatus" .. batteryStatus .. ".png")
		end
		if reactorImageElement ~= nil then
			reactorImageElement:setAttribute("src", "img/reactorstatus" .. reactorStatus .. ".png")
		end
		if shieldImageElement ~= nil then
			shieldImageElement:setAttribute("src", "img/shieldstatus" .. shieldStatus .. ".png")
		end
	end
end

function selectMCU(element)
	itemid = element:getItemId()
	
	if itemid ~= -1 then
		InBattleState:getSingletonPtr():selectMCU(itemid)
	end
end

function nextMCU(element)
	InBattleState:getSingletonPtr():nextMCU()
end

function prevMCU(element)
	InBattleState:getSingletonPtr():prevMCU()
end
function returnToBattle(element)
	RocketRenderer:getSingletonPtr():closeModalDocument()
end

function restart(element)
	InBattleState:getSingletonPtr():restart()
end

function endBattle(element)
	InBattleState:getSingletonPtr():endBattle()
end

function exitToWindows(element)
	GameManager:getSingletonPtr():requestShutdown()
end
