
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
function GenerateTerrain(grid, param1, param2, param3, param4)
	GameManager:getSingletonPtr():luaDebugReport("small craters", param1)
	sideLength = grid:getSideLength()
	numSmallCraters = math.random(param1 / 2, param1)
	numBigCraters = math.random(param2 / 2, param2)
	numCircles = math.random(param3 / 2, param3)
	
	
	seed = os.time()
	GameManager:getSingletonPtr():luaDebugReport("seed", seed)
	math.randomseed( seed )
	
	maxOuterCircleRadius = round(math.random(sideLength / 2, sideLength), 0)
	minOuterCircleRadius = math.random((maxOuterCircleRadius - 4), (maxOuterCircleRadius - 2))
	
	for i = 0, numCircles, 1 do
		x = math.random(1, sideLength -1)
		z = math.random(1, sideLength -1)
		outerRadius = math.random(minOuterCircleRadius + 1, maxOuterCircleRadius)
		innerRadius = math.random(minOuterCircleRadius, outerRadius)
		
		grid:createPlateau(x,z,innerRadius,outerRadius,1, false)
		
		GameManager:getSingletonPtr():luaDebugReport("grid:createPlateau(" .. x .. "," .. z .. "," .. innerRadius .. "," .. outerRadius .. ",1,false)" , 0)
		maxOuterCircleRadius = math.random(outerRadius ,maxOuterCircleRadius)
		minOuterCircleRadius = math.random((maxOuterCircleRadius - 4), (maxOuterCircleRadius - 2))
	end
	
	return
end

	grid:createValleyOrRidge(16,26,18,26,1,1,false)
	grid:createValleyOrRidge(23,11,23,8,4,2,false)
	grid:createValleyOrRidge(1,19,0,19,2,3,false)
	grid:createValleyOrRidge(4,8,4,7,3,3,false)
	grid:createValleyOrRidge(3,27,2,27,3,2,false)
	grid:createValleyOrRidge(3,15,3,13,2,1,false)
	grid:createValleyOrRidge(20,23,16,23,4,1,false)
	grid:createValleyOrRidge(9,1,9,0,2,1,false)
	grid:createValleyOrRidge(7,26,20,26,4,1,false)
	grid:createValleyOrRidge(23,11,24,11,2,1,false)
	grid:createValleyOrRidge(28,11,28,10,2,2,false)
	grid:createValleyOrRidge(7,14,7,25,4,1,false)
	grid:createValleyOrRidge(2,6,2,5,2,2,false)
	grid:createValleyOrRidge(15,26,17,26,1,3,false)
	grid:createValleyOrRidge(1,26,0,26,4,1,false)
	grid:createValleyOrRidge(14,8,14,18,4,1,false)
	grid:createValleyOrRidge(4,28,4,28,4,1,false)
	grid:createValleyOrRidge(13,3,13,0,4,1,false)
	grid:createValleyOrRidge(12,16,8,16,2,1,false)
	grid:createValleyOrRidge(4,21,6,21,1,3,false)
	grid:createValleyOrRidge(8,3,8,0,2,2,false)
	grid:createValleyOrRidge(14,23,14,21,1,2,false)
	grid:createValleyOrRidge(11,8,11,10,1,2,false)
	grid:createValleyOrRidge(9,16,9,18,1,2,false)
	grid:createValleyOrRidge(17,11,19,11,1,1,false)
	grid:createValleyOrRidge(12,23,12,21,1,3,false)
	grid:createValleyOrRidge(7,21,9,21,1,1,false)
	grid:createValleyOrRidge(1,17,3,17,1,3,false)
	grid:createValleyOrRidge(25,11,25,15,2,3,false)
	grid:createValleyOrRidge(25,17,27,17,1,3,false)
	grid:createValleyOrRidge(25,18,25,22,2,2,false)
	grid:createValleyOrRidge(19,13,15,13,2,2,false)