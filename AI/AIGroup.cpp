#include <AI/AIGroup.h>
#include <Controllers/MCUController.h>
#include <Entities/BattlefieldGridCell.h>
#include <Entities/Mission.h>
#include <Entities/MCUPlacement.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/WeaponHardpoint.h>
#include <Managers/LuaStateManager.h>
#include <AI/GroupAttackOrder.h>
#include <AI/GroupMoveOrder.h>
#include <AI/GroupMoveToAttackOrder.h>
#include <AI/MCUOrderStatus.h>
#include <LuaBindings/LuaMCUDeque.h>
#include <States/InBattleState.h>
#include <Templates/ReactorTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Terrain/TerrainManager.h>
#include <Ogre.h>

#include <deque>

using namespace Ogre;

AIGroup::AIGroup(int id)
	:
	mCurrentGoal(0),
	mGroupAttackOrder(0),
	mGroupMoveOrder(0),
	mGroupMoveToAttackOrder(0),
	mPlayerBattleState(0),
	mShortWeaponRange(0),
	mLongWeaponRange(0),
	mLuaMCUDeque(0),
	mFinishedTurn(true),
	mId(id)
{

}

AIGroup::~AIGroup()
{
	std::deque<MCUPlacement*>::iterator iter;

	for(iter = mInitialPlacements.begin(); iter != mInitialPlacements.end(); ++iter)
	{
		delete (*iter);
	}

	delete mLuaMCUDeque;
	if(mGroupAttackOrder)
	{
		delete mGroupAttackOrder;
	}
	if(mGroupMoveOrder)
	{
		delete mGroupMoveOrder;
	}
	if(mGroupMoveToAttackOrder)
	{
		delete mGroupMoveToAttackOrder;
	}
}

int AIGroup::getAverageMCUXPos()
{
	std::deque<MCUController*>::iterator iter;

	int total(0), num(0);

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		total+=(*iter)->getCurrentLocation()->getX();
		++num;
	}

	if(num > 0)
	{
		return total / num;
	}
	return 0;
}

int AIGroup::getAverageMCUZPos()
{
	std::deque<MCUController*>::iterator iter;

	int total(0), num(0);

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		total+=(*iter)->getCurrentLocation()->getZ();
		++num;
	}

	if(num > 0)
	{
		return total / num;
	}
	return 0;
}

std::deque<MCUController*> AIGroup::getMCUs()
{
	return mMCUs;
}

void AIGroup::setMCUs(std::deque<MCUController*> mcus)
{
	mMCUs = mcus;
	mMCUIterator = mMCUs.begin();
}

BattlefieldGridCell* AIGroup::getCurrentGoal()
{
	return mCurrentGoal;
}

void AIGroup::setCurrentGoal(BattlefieldGridCell* goal)
{
	mCurrentGoal = goal;
}

bool AIGroup::hasMoreMCUControllers()
{
	return mMCUIterator != mMCUs.end();
}

MCUController* AIGroup::getNextMCUController()
{
	MCUController* next = (*mMCUIterator);
	++mMCUIterator;
	return next;
}

bool AIGroup::hasFinishedTurn()
{
	return mFinishedTurn;
}

void AIGroup::startTurn()
{
	mFinishedTurn = false;
	if(mGroupAttackOrder)
		mGroupAttackOrder->resetOrderStatus();
	if(mGroupMoveOrder)
		mGroupMoveOrder->resetOrderStatus();
	if(mGroupMoveToAttackOrder)
		mGroupMoveToAttackOrder->resetOrderStatus();

	mMCUIterator = mMCUs.begin();
	
	int num(0);
	float shortRange(1000.f), longRange(0.f);
	
	std::deque<MCUController*>::iterator iter;
	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter)->getShortestWeaponRange() < shortRange)
			shortRange = (*iter)->getShortestWeaponRange();
		longRange+=(*iter)->getLongestWeaponRange();
	}
	if(num > 0)
	{
		mShortWeaponRange = shortRange;
		mLongWeaponRange = longRange / (float)(num);
	}
}

LuaMCUDeque* AIGroup::getLuaMCUDeque()
{
	if(!mLuaMCUDeque)
	{
		mLuaMCUDeque = new LuaMCUDeque(mInitialPlacements);
	}
	return mLuaMCUDeque;
}
	
std::deque<MCUPlacement*>& AIGroup::getMCUPlacements()
{
	return mInitialPlacements;
}

void AIGroup::setMCUPlacements(std::deque<MCUPlacement*>& placements)
{
	mInitialPlacements = placements;
	
	std::deque<MCUPlacement*>::iterator iter;
	for(iter = mInitialPlacements.begin(); iter != mInitialPlacements.end(); ++iter)
	{
		(*iter)->OwnerAIGroup = this;
	}
}

void AIGroup::addMCUPlacement(MechanizedCombatUnit* mcu, int x, int z)
{
	MCUPlacement* placement = new MCUPlacement(x, z, mcu);
	placement->OwnerAIGroup = this;
	mInitialPlacements.push_back(placement);
}

PlayerBattleState* AIGroup::getPlayerBattleState()
{
	return mPlayerBattleState;
}

void AIGroup::setPlayerBattleState(PlayerBattleState* state)
{
	mPlayerBattleState = state;
}

void AIGroup::addMCUController(MCUController* mcu)
{
	mMCUs.push_back(mcu);
	mcu->setAIGroup(this);
}

void AIGroup::removeMCU(MCUController* mcu)
{
	if(mGroupAttackOrder)
		mGroupAttackOrder->removeMCU(mcu);
	if(mGroupMoveOrder)
		mGroupMoveOrder->removeMCU(mcu);
	if(mGroupMoveToAttackOrder)
		mGroupMoveToAttackOrder->removeMCU(mcu);
	std::deque<MCUController*>::iterator iter;
	
	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter) == mcu)
		{
			mMCUs.erase(iter);
			break;
		}
	}
}

bool AIGroup::hasAttackOrder()
{
	if(mGroupAttackOrder)
		return true;
	return false;
}

GroupAttackOrder* AIGroup::getGroupAttackOrder()
{
	return mGroupAttackOrder;
}

void AIGroup::setGroupAttackOrder(GroupAttackOrder* order)
{
	mGroupAttackOrder = order;
}

void AIGroup::giveGroupAttackOrder(MCUController* target)
{
	if(mGroupAttackOrder)
	{
		delete mGroupAttackOrder;
		mGroupAttackOrder = 0;
	}
	if(!mFinishedTurn && InBattleState::getSingletonPtr()->isMyTurn(mPlayerBattleState))
	{
		mGroupAttackOrder = new GroupAttackOrder(this, target);
		LuaStateManager::getSingletonPtr()->executeGroupAttackOrder(InBattleState::getSingletonPtr(), mGroupAttackOrder);
	}
}

void AIGroup::executeGroupAttackOrder()
{
	if(!mFinishedTurn && InBattleState::getSingletonPtr()->isMyTurn(mPlayerBattleState))
	{
		if(mGroupAttackOrder)
		{
			LuaStateManager::getSingletonPtr()->executeGroupAttackOrder(InBattleState::getSingletonPtr(), mGroupAttackOrder);
		}
	}
}

void AIGroup::executeGroupAttackOrderIfNecessary()
{
	if(!mFinishedTurn && InBattleState::getSingletonPtr()->isMyTurn(mPlayerBattleState))
	{
		if(mGroupAttackOrder)
		{
			if(mGroupAttackOrder->awaitingOrders())
				LuaStateManager::getSingletonPtr()->executeGroupAttackOrder(InBattleState::getSingletonPtr(), mGroupAttackOrder);
		}
	}
}

void AIGroup::finishedGroupAttackOrder(bool issueNewOrders)
{
	if(mGroupAttackOrder)
	{
		delete mGroupAttackOrder;
		mGroupAttackOrder = 0;

		LuaStateManager::getSingletonPtr()->issueOrders(InBattleState::getSingletonPtr(), this);
	}
}

bool AIGroup::hasMoveOrder()
{
	if(mGroupMoveOrder)
		return true;
	return false;
}

GroupMoveOrder* AIGroup::getGroupMoveOrder()
{
	return mGroupMoveOrder;
}

void AIGroup::setGroupMoveOrder(GroupMoveOrder* order)
{
	mGroupMoveOrder = order;
}

void AIGroup::giveGroupMoveOrder(BattlefieldGridCell* dest)
{
	if(!dest)
		return;

	if(mGroupMoveOrder)
	{
		delete mGroupMoveOrder;
		mGroupMoveOrder = 0;
	}
	
	if(!mFinishedTurn && InBattleState::getSingletonPtr()->isMyTurn(mPlayerBattleState))
	{
		mGroupMoveOrder = new GroupMoveOrder(this, dest);
		LuaStateManager::getSingletonPtr()->executeGroupMoveOrder(InBattleState::getSingletonPtr(), mGroupMoveOrder);
	}
}

void AIGroup::finishedGroupMoveOrder(bool issueNewOrders)
{
	if(mGroupMoveOrder)
	{
		delete mGroupMoveOrder;
		mGroupMoveOrder = 0;
	}
	std::deque<MCUController*>::iterator iter;
	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		(*iter)->giveStopOrder();
	}

	if(issueNewOrders)
		LuaStateManager::getSingletonPtr()->issueOrders(InBattleState::getSingletonPtr(), this);
}

bool AIGroup::hasMoveToAttackOrder()
{
	if(mGroupMoveToAttackOrder)
		return true;
	return false;
}

GroupMoveToAttackOrder* AIGroup::getGroupMoveToAttackOrder()
{
	return mGroupMoveToAttackOrder;
}

void AIGroup::setGroupMoveToAttackOrder(GroupMoveToAttackOrder* order)
{
	if(mGroupMoveToAttackOrder)
	{
		delete mGroupMoveToAttackOrder;
		mGroupMoveToAttackOrder = 0;
	}
	mGroupMoveToAttackOrder = order;
}

void AIGroup::giveGroupMoveToAttackOrder(MCUController* target)
{
	if(mGroupMoveToAttackOrder)
	{
		delete mGroupMoveToAttackOrder;
		mGroupMoveToAttackOrder = 0;
	}
	
	if(!mFinishedTurn && InBattleState::getSingletonPtr()->isMyTurn(mPlayerBattleState))
	{
		mGroupMoveToAttackOrder = new GroupMoveToAttackOrder(this, target);
		LuaStateManager::getSingletonPtr()->executeGroupMoveToAttackOrder(InBattleState::getSingletonPtr(), mGroupMoveToAttackOrder);
	}
}

void AIGroup::finishedGroupMoveToAttackOrder(bool issueNewOrders)
{
	MCUController* target(0);
	if(mGroupMoveToAttackOrder)
	{
		target = mGroupMoveToAttackOrder->getTarget();
		delete mGroupMoveToAttackOrder;
		mGroupMoveToAttackOrder = 0;
	}
	std::deque<MCUController*>::iterator iter;
	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		(*iter)->giveStopOrder();
	}
	if(issueNewOrders && target)
	{
		this->giveGroupAttackOrder(target);
	}
}	

bool AIGroup::hasOrders()
{
	if(mGroupAttackOrder || mGroupMoveOrder || mGroupMoveToAttackOrder)
		return true;
	return false;
}

bool AIGroup::hasLowMovementEnergy()
{
	std::deque<MCUController*>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter)->getRemainingReactorPower() > (*iter)->getMechanizedCombatUnit()->getReactor()->getMaxEnergy() * 0.1f)
			return false;
	}

	return true;
}

bool AIGroup::hasFiredAllWeapons()
{
	
	std::deque<MCUController*>::iterator iter;
	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter)->canFireAtLeastOneWeapon())
			return false;
	}

	return true;
}

bool AIGroup::canFireUpon(MCUController* target)
{
	if(hasFiredAllWeapons())
		return false;

	std::deque<MCUController*>::iterator iter;
	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter)->canFireUpon(target))
			return true;
	}

	return false;
}

void AIGroup::finishedTurn()
{
	mFinishedTurn = true;
	GameManager::getSingletonPtr()->luaDebugReport("Finished turn", this->getId());
	std::deque<MCUController*>::iterator iter;
	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		(*iter)->stopMoving(false, 0, false);
	}
	mPlayerBattleState->notifyOfFinishedTurn(this);
}

void AIGroup::notifyOfMovementChange(MCUController* controller)
{
	GameManager::getSingletonPtr()->luaDebugReport("Stopped moving", this->getId());
	if(mGroupMoveOrder)
	{
		if(controller->reachedDestination())
		{
			mGroupMoveOrder->setOrderStatus(controller, OrderStatus::OS_COMPLETED);
		}
		else
		{
			mGroupMoveOrder->setOrderStatus(controller, OrderStatus::OS_FAILED);
		}
		return;
	}
	else if(mGroupMoveToAttackOrder)
	{
		if(controller->reachedDestination())
		{
			mGroupMoveToAttackOrder->setOrderStatus(controller, OrderStatus::OS_COMPLETED);
		}
		else
		{
			mGroupMoveToAttackOrder->setOrderStatus(controller, OrderStatus::OS_FAILED);
		}
		return;
	}
	else if(mGroupAttackOrder)
	{
		if(controller->reachedDestination())
			mGroupAttackOrder->setOrderStatus(controller, OrderStatus::OS_STARTED);
		else
			mGroupAttackOrder->setOrderStatus(controller, OrderStatus::OS_FAILED);

		this->loopFinished();
		if(!mFinishedTurn)
			LuaStateManager::getSingletonPtr()->executeGroupAttackOrder(InBattleState::getSingletonPtr(), mGroupAttackOrder);

		return;
	}
	if(!mFinishedTurn)
		LuaStateManager::getSingletonPtr()->issueOrders(InBattleState::getSingletonPtr(), this);
}

void AIGroup::notifyOfMovementFailureDueToBlockage(MCUController* controller, MCUController* blockage, BattlefieldGridCell* destination)
{
	LuaStateManager::getSingletonPtr()->movementFailedDueToBlockage(InBattleState::getSingletonPtr(), controller, blockage, destination);
}

MCUController* AIGroup::getClosestVisibleEnemyMCU()
{
	int xpos = this->getAverageMCUXPos();
	int zpos = this->getAverageMCUZPos();
	float currentShortestDistance = 1000000.f;
	MCUController* result(0);

	std::deque<MCUController*> enemyMCUs = mPlayerBattleState->getAllVisibleMCUs();
	std::deque<MCUController*>::iterator iter;
	for(iter = enemyMCUs.begin(); iter != enemyMCUs.end(); ++iter)
	{
		float distance = TerrainManager::calculateSquareDistance(xpos, zpos, (*iter)->getCurrentLocation()->getX(), (*iter)->getCurrentLocation()->getZ());
		if(distance < currentShortestDistance)
		{
			result = (*iter);
			currentShortestDistance = distance;
		}
	}

	return result;
}

float AIGroup::getShortWeaponRange()
{
	return mShortWeaponRange;
}

float AIGroup::getLongWeaponRange()
{
	return mLongWeaponRange;
}

void AIGroup::loopFinished()
{
	mMCUIterator = mMCUs.begin();
	if(mGroupAttackOrder && mGroupAttackOrder->awaitingOrders())
	{
		this->executeGroupAttackOrder();
		return;
	}

	if(!hasOrders() && !mFinishedTurn)
		LuaStateManager::getSingletonPtr()->issueOrders(InBattleState::getSingletonPtr(), this);
}

int AIGroup::getId()
{
	return mId;
}
