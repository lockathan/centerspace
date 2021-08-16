function exitToWindows(element)
	GameManager:getSingletonPtr():requestShutdown()
end

function loadsingleplayer(element)
	MainMenuState:getSingletonPtr():loadSinglePlayer()
end