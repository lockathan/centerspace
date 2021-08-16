function SquareDist(x1, z1, x2, z2)
	return ((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1))
end

function AttackTarget(state, target)
	while state:hasMoreAIGroups() do
		group = state:getNextAIGroup()
		
		groupXPos = group:getAverageMCUXPos()
		groupZPos = group:getAverageMCUZPos()
		
		targetXPos = target:getCurrentLocation():getX()
		targetZPos = target:getCurrentLocation():getZ()
		
		squareDist = SquareDist(groupXPos, groupZPos, targetXPos, targetZPos)
		
		while state:hasMoreVisibleMCUs() do
			mcu = state:getNextVisibleMCU()
			
			mcuXPos = mcu:getCurrentLocation():getX()
			mcuZPos = mcu:getCurrentLocation():getZ()
			compareSquareDist = SquareDist(groupXPos, groupZPos, mcuXPos, mcuZPos)
			
		end
		group:loopFinished()
	end
	state:loopFinished()
end

function MovementFailedDueToBlockage(mcu, blockage, destination)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
	path = pathfinder:findPath(mcu:getCurrentLocation(), destination )
	mcu:setCurrentPath( path )
	mcu:moveAlongPath()
end

function WithinShortWeaponRange(group, target)
	groupXPos = group:getAverageMCUXPos()
	groupZPos = group:getAverageMCUZPos()
	
	targetXPos = target:getCurrentLocation():getX()
	targetZPos = target:getCurrentLocation():getZ()
	squareDist = SquareDist(groupXPos, groupZPos, targetXPos, targetZPos)
	
	if group:getShortWeaponRange * group:getShortWeaponRange < squareDist then
		return true
	else
		return false
	end
end

function WithinLongWeaponRange(group, target)
	groupXPos = group:getAverageMCUXPos()
	groupZPos = group:getAverageMCUZPos()
	
	targetXPos = target:getCurrentLocation():getX()
	targetZPos = target:getCurrentLocation():getZ()
	squareDist = SquareDist(groupXPos, groupZPos, targetXPos, targetZPos)
	
	if group:getLongWeaponRange * group:getLongWeaponRange < squareDist then
		return true
	else
		return false
	end
end

function MoveCloser(mcu, target)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	cell = grid:getCloserGridCell(mcu:getCurrentLocation():getX(), mcu:getCurrentLocation():getZ(),target:getCurrentLocation():getX(), target:getCurrentLocation():getZ(), mcu:getShortestWeaponRange)
	
	pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
	path = pathfinder:findPath(mcu:getCurrentLocation(), cell )
	mcu:setCurrentPath( path )
	mcu:moveAlongPath()
end

function ExecuteGroupAttackOrder(order)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	group = order:getOwner()
	target = order:getTarget()
	
	groupXPos = group:getAverageMCUXPos()
	groupZPos = group:getAverageMCUZPos()
	
	squareDistance = SquareDist(groupXPos, groupZPos, target:getCurrentLocation():getX(), target:getCurrentLocation:getZ())
	
	while group:hasMoreMCUControllers() do
		mcu = group:getNextMCUController()
		if mcu:isFiring then
			--only one MCU in a group is allowed to be firing at a time
			return
		end
		mcu:setCurrentTarget(target)
		currentSquareDistance = SquareDist(mcu:getCurrentLocation():getX(), mcu:getCurrentLocation():getZ(), target:getCurrentLocation():getX(), target:getCurrentLocation:getZ())
		shortestWeaponRange = mcu:getShortestWeaponRange()
		longestWeaponRange = mcu:getLongestWeaponRange()
		
		if shortestWeaponRange * shortestWeaponRange < squareDistance then
			if target:shieldsUp() then
				weapon = mcu:getBestAntiShieldWeapon()
				if not weapon == nil then
					InBattleState:getSingletonPtr():fireWeapon(mcu, weapon)
					return
				else
					order:setOrderStatus(mcu, OrderStatus:OS_FAILED)
				end
			else
				weapon = mcu:getBestAntiArmorWeapon()
				if not weapon == nil then
					InBattleState:getSingletonPtr():fireWeapon(mcu, weapon)
					return
				else
					order:setOrderStatus(mcu, OrderStatus:OS_FAILED)
				end
			end
		elseif longestWeaponRange * longestWeaponRange < squareDistance then
			if mcu:lowMovementEnergy() then
				if target:shieldsUp() then
					weapon = mcu:getBestAntiShieldWeapon()
					if not weapon == nil then
						InBattleState:getSingletonPtr():fireWeapon(mcu, weapon)
						return
					else
						order:setOrderStatus(mcu, OrderStatus:OS_FAILED)
					end
				else
					weapon = mcu:getBestAntiArmorWeapon()
					if not weapon == nil then
						InBattleState:getSingletonPtr():fireWeapon(mcu, weapon)
						return
					else
						order:setOrderStatus(mcu, OrderStatus:OS_FAILED)
					end
				end
			else
				MoveCloser(mcu, target)
			end
		else
			MoveCloser(mcu, target)
		end
	end
	group:loopFinished()
end

function ExecuteGroupMoveOrder(order)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	group = order:getOwner()
	cell = order:getDestination()
	grid:seedEmptyGridCellSearch(cell:getX(), cell:getZ())
	
	while group:hasMoreMCUControllers() do
		mcu = group:getNextMCUController()
		pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
		path = pathfinder:findPath(mcu:getCurrentLocation(), grid:getNextEmptyGridCell() )
		mcu:setCurrentPath( path )
		mcu:moveAlongPath()
	end
	group:loopFinished()
end

function ExecuteGroupMoveToAttackOrder(order)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	group = order:getOwner()
	target = order:getTarget()
	
	if WithinShortWeaponRange(group, target) then
		group:finishedGroupMoveToAttackOrder()
		return
	elseif WithinLongWeaponRange(group, target) then
		if group:hasLowMovementEnergy() then
			--try to shoot anyway
			group:finishedGroupMoveToAttackOrder()
			return
		end
	else
		if group:hasLowMovementEnergy() then
			group:finishedTurn()
			return
		end
	end
	
	groupXPos = group:getAverageMCUXPos()
	groupZPos = group:getAverageMCUZPos()
	cell = grid:getCloserGridCell(groupXPos,groupZPos, target:getCurrentLocation():getX(), target:getCurrentLocation:getZ(), group) 
	
	grid:seedEmptyGridCellSearch(cell:getX(), cell:getZ())
	
	while group:hasMoreMCUControllers() do
		mcu = group:getNextMCUController()
		pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
		path = pathfinder:findPath(mcu:getCurrentLocation(), grid:getNextEmptyGridCell() )
		mcu:setCurrentPath( path )
		mcu:moveAlongPath()
	end
	group::loopFinished()
end

function Seek(state, group)
	
	aiXPos = state:getAverageMCUXPos()
	aiZPos = state:getAverageMCUZPos()
	
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	groupXPos = group:getAverageMCUXPos()
	groupZPos = group:getAverageMCUZPos()
	
	dest = state:getCoverageMap():getNearestHotSpot(groupXPos,groupZPos)
	
	group:giveGroupMoveOrder(dest)
end

function IssueGroupOrders(group)
	if state:hasMoreVisibleMCUs() then
		target = group:getClosestVisibleEnemyMCU()
		
		if group:hasAttackOrder() then
				order = group:getGroupAttackOrder()
			if WithinShortWeaponRange(group, order:getTarget()) then
				if not group:canFireUpon(order:getTarget()) then
					group:finishedTurn()
				end
			elseif WithinLongWeaponRange(group, order:getTarget()) then
				if group:hasLowMovementEnergy() then
					if not group:canFireUpon(order:getTarget()) then
						group:finishedTurn()
					end
				else
					group:finishedGroupAttackOrder()
					group:giveGroupMoveToAttackOrder(order:getTarget())
				end
			else
				if group:hasLowMovementEnergy() then
					group:finishedTurn()
				else
					group:finishedGroupAttackOrder()
					group:giveGroupMoveToAttackOrder(order:getTarget())
				end
			end
		else
			if WithinShortWeaponRange(group, target) then
				if group:canFireUpon(target) then
					group:giveGroupAttackOrder(target)
				else
					--move to safety?
					group:finishedTurn()
				end
			elseif WithinLongWeaponRange(group, target) then
				if group:hasLowMovementEnergy() then
					if group:canFireUpon(target)) then
						group:giveGroupAttackOrder(target)
					else
						group:finishedTurn()
					end
				else
					group:giveGroupMoveToAttackOrder(target)
				end
			else
				if group:hasLowMovementEnergy() then
					group:finishedTurn()
				else
					group:giveGroupMoveToAttackOrder(target)
				end
			end
		end
		
	else
		if group:hasLowMovementEnergy() then
			--we cant move and there is no one to shoot at
			group:finishedTurn()
		else
			if group:hasMoveOrder() then
				ExecuteGroupMoveOrder(group:getGroupMoveOrder())
			else
				Seek(group:getPlayerBattleState(), group)
			end
		end
	end
	group:loopFinished()
end

function IssueOrders(state)
	while state:hasMoreAIGroups() do
		group = state:getNextAIGroup()
		IssueGroupOrders(group)
	end
	state:loopFinished()
end