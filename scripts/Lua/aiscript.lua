function SquareDist(x1, z1, x2, z2)
	return ((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1))
end

function ReturnFire(mcu)

	GameManager:getSingletonPtr():luaDebugReport("Returning fire", 0)
	target = mcu:getCurrentTarget()
	
	if mcu:canFireUpon(target) then
		if target:shieldsUp() then
			weapon = mcu:getBestAntiShieldWeapon()
			
			if weapon then
				GameManager:getSingletonPtr():luaDebugReport("Firing anti shield weapon (return fire)", 0)
				InBattleState:getSingletonPtr():fireWeapon(mcu, weapon, true)
				return
			else
				return
			end
		else
			weapon = mcu:getBestAntiArmorWeapon()
			if weapon then
				GameManager:getSingletonPtr():luaDebugReport("Firing anti armor weapon (return fire)", 0)
				InBattleState:getSingletonPtr():fireWeapon(mcu, weapon, true)
				return
			else
				return
			end
		end
	else
		GameManager:getSingletonPtr():luaDebugReport("Cannot fire upon", 0)
	end
end

function MovementFailedDueToBlockage(mcu, blockage, destination)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
	path = pathfinder:findPath(mcu:getCurrentLocation(), destination, mcu:getMechanizedCombatUnit():getDrive() )
	mcu:setCurrentPath( path )
	mcu:moveAlongPath()
end

function WithinShortWeaponRange(group, target)
	groupXPos = group:getAverageMCUXPos()
	groupZPos = group:getAverageMCUZPos()
	
	targetXPos = target:getCurrentLocation():getX()
	targetZPos = target:getCurrentLocation():getZ()
	squareDist = SquareDist(groupXPos, groupZPos, targetXPos, targetZPos)
	
	if group:getShortWeaponRange() * group:getShortWeaponRange() < squareDist then
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
	
	if group:getLongWeaponRange() * group:getLongWeaponRange() < squareDist then
		return true
	else
		return false
	end
end

function MoveCloser(mcu, target, rangeTarget)
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	cell = grid:getCloserGridCell(mcu:getCurrentLocation():getX(), mcu:getCurrentLocation():getZ(),target:getCurrentLocation():getX(), target:getCurrentLocation():getZ(), rangeTarget)
	
	if mcu:getCurrentLocation() == cell then
		GameManager:getSingletonPtr():luaDebugReport("Already there", 0)
		group = mcu:getAIGroup()
		group:notifyOfMovementChange(mcu)
		return
	end
	
	GameManager:getSingletonPtr():luaDebugReport("Moving mcu closer = sending to: x" .. cell:getX() .. " z" .. cell:getZ(), 0)
	pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
	path = pathfinder:findPath(mcu:getCurrentLocation(), cell, mcu:getMechanizedCombatUnit():getDrive() )
	mcu:setCurrentPath( path )
	mcu:moveAlongPath()
end

function ExecuteGroupAttackOrder(order)
	GameManager:getSingletonPtr():luaDebugReport("Executing attack order", group:getId())
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	group = order:getOwner()
	target = order:getTarget()
	
	groupXPos = group:getAverageMCUXPos()
	groupZPos = group:getAverageMCUZPos()
	
	while group:hasMoreMCUControllers() do
		mcu = group:getNextMCUController()
		if mcu:isFiring() then
			--only one MCU in a group is allowed to be firing at a time
			GameManager:getSingletonPtr():luaDebugReport("Is firing", group:getId())
			return
		end
		if mcu:isMoving() then
			--only one MCU in a group is allowed to be firing at a time
			GameManager:getSingletonPtr():luaDebugReport("Is moving", group:getId())
			return
		end
		
		if order:getOrderStatus(mcu) == OS_FAILED then
			GameManager:getSingletonPtr():luaDebugReport("Order already failed", group:getId())
			group:executeGroupAttackOrderIfNecessary()
			return
		end
		if order:getOrderStatus(mcu) == OS_IN_PROGRESS then
			GameManager:getSingletonPtr():luaDebugReport("Order already in progress", group:getId())
			group:executeGroupAttackOrderIfNecessary()
			return
		end
		mcu:setCurrentTarget(target)
		currentSquareDistance = SquareDist(mcu:getCurrentLocation():getX(), mcu:getCurrentLocation():getZ(), target:getCurrentLocation():getX(), target:getCurrentLocation():getZ())
		shortestWeaponRange = mcu:getShortestWeaponRange()
		longestWeaponRange = mcu:getLongestWeaponRange()
		GameManager:getSingletonPtr():luaDebugReport("Current square distance",currentSquareDistance)
		GameManager:getSingletonPtr():luaDebugReport("Squared short distance",shortestWeaponRange * shortestWeaponRange)
		GameManager:getSingletonPtr():luaDebugReport("Squared long distance",longestWeaponRange * longestWeaponRange)
		if shortestWeaponRange * shortestWeaponRange > currentSquareDistance then
			if target:shieldsUp() then
				weapon = mcu:getBestAntiShieldWeapon()
				
				if weapon then
					accuracy = mcu:calculateAccuracy(weapon)
					
					if accuracy < 0.5 and not mcu:lowMovementEnergy() then
						GameManager:getSingletonPtr():luaDebugReport("Accuracy sucks, moving closer", group:getId())
						MoveCloser(mcu, target, 2)
						order:setOrderStatus(mcu, OS_IN_PROGRESS)
						return
					end
					
					GameManager:getSingletonPtr():luaDebugReport("Firing anti shield weapon", group:getId())
					InBattleState:getSingletonPtr():fireWeapon(mcu, weapon, false)
					order:setOrderStatus(mcu, OS_IN_PROGRESS)
					group:loopFinished()
					return
				else
					GameManager:getSingletonPtr():luaDebugReport("Order failed - no suitable anti shield weapon", group:getId())
					order:setOrderStatus(mcu, OS_FAILED)
					return
				end
			else
				weapon = mcu:getBestAntiArmorWeapon()
				if weapon then
					accuracy = mcu:calculateAccuracy(weapon)
					
					if accuracy < 0.5 and not mcu:lowMovementEnergy() then
						GameManager:getSingletonPtr():luaDebugReport("Accuracy sucks, moving closer", group:getId())
						MoveCloser(mcu, target, 2)
						order:setOrderStatus(mcu, OS_IN_PROGRESS)
						return
					end
					
					GameManager:getSingletonPtr():luaDebugReport("Firing anti armor weapon", group:getId())
					InBattleState:getSingletonPtr():fireWeapon(mcu, weapon, false)
					order:setOrderStatus(mcu, OS_IN_PROGRESS)
					group:loopFinished()
					return
				else
					GameManager:getSingletonPtr():luaDebugReport("Order failed - no suitable anti armor weapon", group:getId())
					order:setOrderStatus(mcu, OS_FAILED)
					return
				end
			end
		elseif longestWeaponRange * longestWeaponRange > currentSquareDistance then
			if mcu:lowMovementEnergy() then
				if target:shieldsUp() then
					weapon = mcu:getBestAntiShieldWeapon()
					if weapon then
						GameManager:getSingletonPtr():luaDebugReport("Firing anti shield weapon", group:getId())
						InBattleState:getSingletonPtr():fireWeapon(mcu, weapon, false)
						order:setOrderStatus(mcu, OS_IN_PROGRESS)
						group:loopFinished()
						return
					else
						GameManager:getSingletonPtr():luaDebugReport("Order failed - no suitable anti shield weapon. Outside short range", group:getId())
						order:setOrderStatus(mcu, OS_FAILED)
						return
					end
				else
					weapon = mcu:getBestAntiArmorWeapon()
					if weapon then
						GameManager:getSingletonPtr():luaDebugReport("Firing anti armor weapon", group:getId())
						InBattleState:getSingletonPtr():fireWeapon(mcu, weapon, false)
						order:setOrderStatus(mcu, OS_IN_PROGRESS)
						group:loopFinished()
						return
					else
						GameManager:getSingletonPtr():luaDebugReport("Order failed - no suitable anti armor weapon. Outside short range", group:getId())
						order:setOrderStatus(mcu, OS_FAILED)
						return
					end
				end
			else
				GameManager:getSingletonPtr():luaDebugReport("In range but moving closer", group:getId())
				MoveCloser(mcu, target, mcu:getShortestWeaponRange())
				order:setOrderStatus(mcu, OS_IN_PROGRESS)
				return
			end
		else
			if mcu:lowMovementEnergy() then
				GameManager:getSingletonPtr():luaDebugReport("Out of range, moving closer", group:getId())
				MoveCloser(mcu, target, mcu:getShortestWeaponRange())
				order:setOrderStatus(mcu, OS_IN_PROGRESS)
				return
			else
				GameManager:getSingletonPtr():luaDebugReport("Order failed - insufficient energy to move closer", group:getId())
				order:setOrderStatus(mcu, OS_FAILED)
				return
			end
		end
	end
	GameManager:getSingletonPtr():luaDebugReport("Loop finished", group:getId())
	group:loopFinished()
	group:executeGroupAttackOrderIfNecessary()
end

function ExecuteGroupMoveOrder(order)
	
	grid = InBattleState:getSingletonPtr():getBattlefieldGrid()
	group = order:getOwner()
	cell = order:getDestination()
	grid:seedEmptyGridCellSearch(cell:getX(), cell:getZ())
	
	GameManager:getSingletonPtr():luaDebugReport("Executing move order to: x" .. cell:getX() .. " z" .. cell:getZ(),group:getId())
	
	while group:hasMoreMCUControllers() do
		mcu = group:getNextMCUController()
		if order:getOrderStatus(mcu) == OS_STARTED then
			pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
			targetCell = grid:getNextEmptyGridCell()
			if not targetCell then
				order:setOrderStatus(mcu, OS_FAILED)
				return
			end
			if targetCell == mcu:getCurrentLocation() then
				order:setOrderStatus(mcu, OS_COMPLETED)
				return
			end
			GameManager:getSingletonPtr():luaDebugReport("Sending MCU to: x" .. targetCell:getX() .. " z" .. targetCell:getZ(),group:getId())
			path = pathfinder:findPath(mcu:getCurrentLocation(), targetCell, mcu:getMechanizedCombatUnit():getDrive() )
			mcu:setCurrentPath( path )
			mcu:moveAlongPath()
			order:setOrderStatus(mcu, OS_IN_PROGRESS)
		end
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
	
	cell = grid:getCloserGridCell(groupXPos,groupZPos, target:getCurrentLocation():getX(), target:getCurrentLocation():getZ(), group) 
	GameManager:getSingletonPtr():luaDebugReport("Executing move to attack order to: x" .. cell:getX() .. " z" .. cell:getZ(),group:getId())
	grid:seedEmptyGridCellSearch(cell:getX(), cell:getZ())
	
	while group:hasMoreMCUControllers() do
		mcu = group:getNextMCUController()
		if order:getOrderStatus(mcu) == OS_STARTED then
			pathfinder = InBattleState:getSingletonPtr():getPathFindingManager()
			targetCell = grid:getNextEmptyGridCell()
			if targetCell == mcu:getCurrentLocation() then
				order:setOrderStatus(mcu, OS_COMPLETED)
				return
			end
			GameManager:getSingletonPtr():luaDebugReport("Sending MCU to: x" .. targetCell:getX() .. " z" .. targetCell:getZ(),group:getId())
			path = pathfinder:findPath(mcu:getCurrentLocation(), targetCell, mcu:getMechanizedCombatUnit():getDrive() )
			mcu:setCurrentPath( path )
			mcu:moveAlongPath()
			order:setOrderStatus(mcu, OS_IN_PROGRESS)
		end
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
	GameManager:getSingletonPtr():luaDebugReport("Start issue orders",group:getId())
	state = group:getPlayerBattleState()
	
	if state:hasMoreVisibleMCUs() then
		target = group:getClosestVisibleEnemyMCU()
		GameManager:getSingletonPtr():luaDebugReport("Enemies visible", group:getId())
		if group:hasAttackOrder() then
			GameManager:getSingletonPtr():luaDebugReport("Has attack order", group:getId())
			order = group:getGroupAttackOrder()
			if WithinShortWeaponRange(group, order:getTarget()) then
				GameManager:getSingletonPtr():luaDebugReport("Within short range", group:getId())
				if not group:canFireUpon(order:getTarget()) then
					GameManager:getSingletonPtr():luaDebugReport("Cannot fire", group:getId())
					group:finishedTurn()
				else
					ExecuteGroupAttackOrder(order)
				end
			elseif WithinLongWeaponRange(group, order:getTarget()) then
				GameManager:getSingletonPtr():luaDebugReport("Within long range", group:getId())
				if group:hasLowMovementEnergy() then
					GameManager:getSingletonPtr():luaDebugReport("low movement energy", group:getId())
					if not group:canFireUpon(order:getTarget()) then
					GameManager:getSingletonPtr():luaDebugReport("Cannot fire", group:getId())
						group:finishedTurn()
					end
				else
					GameManager:getSingletonPtr():luaDebugReport("Moving closer", group:getId())
					group:finishedGroupAttackOrder()
					group:giveGroupMoveToAttackOrder(order:getTarget())
				end
			else
				GameManager:getSingletonPtr():luaDebugReport("out of range", group:getId())
				if group:hasLowMovementEnergy() then
					GameManager:getSingletonPtr():luaDebugReport("low movement energy", group:getId())
					group:finishedTurn()
				else
					GameManager:getSingletonPtr():luaDebugReport("Moving closer", group:getId())
					group:finishedGroupAttackOrder()
					group:giveGroupMoveToAttackOrder(order:getTarget())
				end
			end
		else
			if WithinShortWeaponRange(group, target) then
				GameManager:getSingletonPtr():luaDebugReport("Within short range", group:getId())
				if group:canFireUpon(target) then
					GameManager:getSingletonPtr():luaDebugReport("Giving attack order", group:getId())
					group:giveGroupAttackOrder(target)
				else
					GameManager:getSingletonPtr():luaDebugReport("In short weapon range but cannot fire upon", group:getId())
					--move to safety?
					group:finishedTurn()
				end
			elseif WithinLongWeaponRange(group, target) then
				GameManager:getSingletonPtr():luaDebugReport("Within long range", group:getId())
				if group:hasLowMovementEnergy() then
					GameManager:getSingletonPtr():luaDebugReport("low movement energy", group:getId())
					if group:canFireUpon(target) then
						GameManager:getSingletonPtr():luaDebugReport("Giving attack order", group:getId())
						group:giveGroupAttackOrder(target)
					else
						GameManager:getSingletonPtr():luaDebugReport("In long weapon range but cannot fire upon", group:getId())
						group:finishedTurn()
					end
				else
					GameManager:getSingletonPtr():luaDebugReport("Moving closer", group:getId())
					group:giveGroupMoveToAttackOrder(target)
				end
			else
				if group:hasLowMovementEnergy() then
					GameManager:getSingletonPtr():luaDebugReport("Low movement energy", group:getId())
					group:finishedTurn()
				else
					GameManager:getSingletonPtr():luaDebugReport("Moving closer", group:getId())
					group:giveGroupMoveToAttackOrder(target)
				end
			end
		end
	else
		if group:hasLowMovementEnergy() then
			GameManager:getSingletonPtr():luaDebugReport("Nothing to do", group:getId())
			group:finishedTurn()
		else
			if group:hasMoveOrder() then
				GameManager:getSingletonPtr():luaDebugReport("Following move order", group:getId())
				ExecuteGroupMoveOrder(group:getGroupMoveOrder())
			else
				GameManager:getSingletonPtr():luaDebugReport("Seeking", group:getId())
				Seek(state, group)
			end
		end
	end
	group:loopFinished()
	--if no order was given, end the turn
	--group:finishedTurn()
end

function IssueOrders(state)
	while state:hasMoreAIGroups() do
		group = state:getNextAIGroup()
		IssueGroupOrders(group)
	end
	state:loopFinished()
end