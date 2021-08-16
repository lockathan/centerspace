function MovementFailedDueToBlockage(mcu, blockage, destination)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
	path = pathfinder:findPath(mcu:getCurrentLocation(), destination )
	mcu:setCurrentPath( path )
	mcu:moveAlongPath()
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

function IssueOrders(state)
	while state:hasMoreAIGroups() do
		group = state:getNextAIGroup()
		IssueGroupOrders(group)
	end
	state:loopFinished()
end


		if mcu:isFiring() then
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