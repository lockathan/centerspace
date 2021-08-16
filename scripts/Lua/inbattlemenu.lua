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
