function placePlayer(mcudeque, grid, x, z)
	sideLength = grid:getSideLength()
	seed = os.time()
	GameManager:getSingletonPtr():luaDebugReport("seed", seed)
	math.randomseed( seed )
	
	seedX = math.random(0, (sideLength - 1))
	seedZ = math.random(0, (sideLength - 1))
	
	mcudeque:setSeedX(seedX)
	mcudeque:setSeedZ(seedZ)
	
	grid:placeMCUs(mcudeque, seedX, seedZ)
end --close function

function placeEnemy(mcudeque, grid, x, z)
	sideLength = grid:getSideLength()
	
	seedX = math.random(0, (sideLength - 1))
	seedZ = math.random(0, (sideLength - 1))
	
	mcudeque:setSeedX(seedX)
	mcudeque:setSeedZ(seedZ)
	
	grid:placeMCUs(mcudeque, seedX, seedZ)
end


