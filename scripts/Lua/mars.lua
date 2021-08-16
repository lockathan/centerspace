function round(num, idp)
    local mult = 10^(idp or 0)
    if num >= 0 then return math.floor(num * mult + 0.5) / mult
    else return math.ceil(num * mult - 0.5) / mult end
end

function GenerateTerrain(grid, param1, param2, param3, param4)
	GameManager:getSingletonPtr():luaDebugReport("small craters", param1)
	sideLength = grid:getSideLength()numSmallCraters = math.random(param1 / 2, param1)
	numBigCraters = math.random(param2 / 2, param2)
	
	seed = os.time()
	GameManager:getSingletonPtr():luaDebugReport("seed", seed)
	math.randomseed( seed )
	
	for i = 0, numBigCraters, 1 do
		x = math.random(1, sideLength -1)
		z = math.random(1, sideLength -1)
		wideOrHigh = math.random(0,1)
		xOrZ = math.random(0, 1)
		posOrNeg = math.random(0,1)
		x2 = x
		z2 = z
		distance = 0
		width = 0
		height = 0
		nonsquareFactor = math.random(2,5)
		if wideOrHigh == 0 then
			width = math.random(2,5)
			height = math.random(1,2)
		else
			width = math.random(1,4)
			height = math.random(1,3)
		end
		
		if posOrNeg == 0 then
			distance = width
		else
			distance = -1 * width
		end
		distance = (2 * distance) + nonsquareFactor
		GameManager:getSingletonPtr():luaDebugReport("width", width)
		GameManager:getSingletonPtr():luaDebugReport("distance", distance)
		if xOrZ == 0 then
			x2 = x2 + distance
			if x2 < 0 then
				x2 = 0
			end
			if x2 >= (sideLength -1) then
				x2 = (sideLength - 1)
			end
		else
			z2 = z2 + distance
			if z2 < 0 then
				z2 = 0
			end
			if z2 >= (sideLength -1) then
				z2 = (sideLength - 1)
			end
		end
		cliffOrSlope = math.random(0,1)
		GameManager:getSingletonPtr():luaDebugReport("grid:createValleyOrRidge(" .. x .. "," .. z .. "," .. x2 .. "," .. z2 .. "," .. width .. "," .. height .. ",false)" , 0)
		grid:createValleyOrRidge(x,z,x2,z2,width,height, false)
		
	end
	for i = 0, numSmallCraters, 1 do
		x = math.random(1, sideLength -1)
		z = math.random(1, sideLength -1)
		xOrZ = math.random(0, 1)
		posOrNeg = math.random(0,1)
		x2 = x
		z2 = z
		distance = 0
		width = math.random(1,2)
		if posOrNeg == 0 then
			distance = width
		else
			distance = -1 * width
		end
		distance = 2 * distance
		GameManager:getSingletonPtr():luaDebugReport("width", width)
		GameManager:getSingletonPtr():luaDebugReport("distance", distance)
		if xOrZ == 0 then
			x2 = x2 + distance
			if x2 < 0 then
				x2 = 0
			end
			if x2 >= (sideLength -1) then
				x2 = (sideLength - 1)
			end
		else
			z2 = z2 + distance
			if z2 < 0 then
				z2 = 0
			end
			if z2 >= (sideLength -1) then
				z2 = (sideLength - 1)
			end
		end
		height = math.random(1,3)
		GameManager:getSingletonPtr():luaDebugReport("grid:createValleyOrRidge(" .. x .. "," .. z .. "," .. x2 .. "," .. z2 .. "," .. width .. "," .. height .. ",false)" , 0)
		grid:createValleyOrRidge(x,z,x2,z2,width,height, false)
	end
	
	return
end