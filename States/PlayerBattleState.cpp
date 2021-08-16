#include "PlayerBattleState.h"
#include <Ogre.h>
#include <BtOgre/BtOgreExtras.h>
#include <Controllers/KinematicCharacterController.h>
#include <Controllers/MCUController.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <Entities/DataProvider.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MovableEntity.h>
#include <Entities/PlayerProfile.h>
#include <Entities/Projectile.h>
#include <Managers/LuaStateManager.h>
#include <Managers/SQLiteDBManager.h>
#include <AI/CoverageMap.h>
#include <AI/GroupAttackOrder.h>
#include <AI/GroupMoveOrder.h>
#include <AI/GroupMoveToAttackOrder.h>
#include <AI/MCUOrderStatus.h>
#include <AI/AIGroup.h>
#include <StateChange/BattleResult.h>
#include <StateChange/MCUComponentStatus.h>
#include <StateChange/MCUStatus.h>
#include <StateChange/PlayerPreBattleState.h>
#include <StateChange/PlayerResult.h>
#include <States/InBattleState.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/SensorSystemTemplate.h>


#include <BulletCollision/CollisionDispatch/btGhostObject.h>

using namespace Ogre;

PlayerBattleState::PlayerBattleState()
	:
	mMyTurn(false),
	mPlayerProfile(0),
	mPlayerNumber(0),
	mTeam(0),
	mFactionTemplate(0),
	mLocal(false),
	mNeedsRender(false),
	mIsAI(false),
	mFirstVisibilityRun(true),
	mAIScriptName(""),
	mAIScriptEntryPoint(""),
	mCoverageMap(0)
{

}

PlayerBattleState::PlayerBattleState(PlayerProfile* player,FactionTemplate* faction, int team, int playerNumber)
	:
	mMyTurn(false),
	mPlayerProfile(player),
	mPlayerNumber(playerNumber),
	mTeam(team),
	mFactionTemplate(faction),
	mLocal(false),
	mNeedsRender(false),
	mIsAI(false),
	mFirstVisibilityRun(true),
	mAIScriptName(""),
	mAIScriptEntryPoint(""),
	mCoverageMap(0)
{

}

PlayerBattleState::~PlayerBattleState()
{

}

void PlayerBattleState::startTurn()
{
	if(mIsAI)
	{
		mAIGroupIterator = mAIGroups.begin();
		mNextVisibleMCU = mVisibleMCUs.begin();
		std::deque<AIGroup*>::iterator iter;

		for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
		{
			(*iter)->startTurn();
		}

		runAIScript();
	}
}

void PlayerBattleState::endTurn()
{
	std::deque<MCUController*>::iterator mcuIterator;

	for(mcuIterator = mMCUs.begin();mcuIterator != mMCUs.end(); ++mcuIterator)
	{
		(*mcuIterator)->endTurn();
	}
}

void PlayerBattleState::endMyTurn()
{
	InBattleState::getSingletonPtr()->endTurn();
}

PlayerProfile* PlayerBattleState::getPlayerProfile()
{
	return mPlayerProfile;
}

void PlayerBattleState::setPlayer(PlayerProfile* player)
{
	mPlayerProfile = player;
}

int PlayerBattleState::getTeam()
{
	return mTeam;
}

void PlayerBattleState::setTeam(int team)
{
	mTeam = team;
}

bool PlayerBattleState::isMyTurn()
{
	return mMyTurn;
}

void PlayerBattleState::setMyTurn(bool turn)
{
	mMyTurn = turn;
}

int PlayerBattleState::getPlayerNumber()
{
	return mPlayerNumber;
}

void PlayerBattleState::setPlayerNumber(int n)
{
	mPlayerNumber = n;
}

std::deque<MCUController*> PlayerBattleState::getMCUs()
{
	return mMCUs;
}

void PlayerBattleState::addMCU(MCUController* mcu)
{
	mMCUs.push_back(mcu);
}

void PlayerBattleState::setMCUs(std::deque<MCUController*> mcus)
{
	mMCUs = mcus;
}

void PlayerBattleState::removeMCU(MCUController* mcu)
{
	std::deque<MCUController*>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter) == mcu)
		{
			mMCUs.erase(iter);
			break;
		}
	}

	if(mMCUs.size() == 0)
	{
		InBattleState::getSingletonPtr()->getBattleResult()->getPlayerResult(mPlayerProfile)->setPlayerOutcome(PlayerOutcome::PO_DEFEAT);
		InBattleState::getSingletonPtr()->removePlayer(this);
	}
}

FactionTemplate* PlayerBattleState::getFactionTemplate()
{
	return mFactionTemplate;
}

void PlayerBattleState::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

bool PlayerBattleState::isLocal()
{
	return mLocal;
}

void PlayerBattleState::setLocal(bool value)
{
	mLocal = value;
}

bool PlayerBattleState::needsRender()
{
	return mNeedsRender;
}

bool PlayerBattleState::isAI()
{
	return mIsAI;
}

void PlayerBattleState::setAI(bool value)
{
	mIsAI = value;
}

void PlayerBattleState::setNeedsRender(bool value)
{
	mNeedsRender = value;
}

std::deque<Projectile*> PlayerBattleState::getProjectiles()
{
	return mProjectiles;
}

void PlayerBattleState::addProjectile(Projectile* proj)
{
	mProjectiles.push_back(proj);
}

void PlayerBattleState::removeProjectile(Projectile* proj)
{
	std::deque<Projectile*>::iterator iter;

	for(iter = mProjectiles.begin(); iter != mProjectiles.end(); ++iter)
	{
		if((*iter) == proj)
		{
			iter = mProjectiles.erase(iter);
			break;
		}
	}
}

void PlayerBattleState::loadScript(const std::string& scriptName, const std::string& scriptEntryPoint)
{
	mAIScriptName = scriptName;
	mAIScriptEntryPoint = scriptEntryPoint;
	LuaStateManager::getSingletonPtr()->loadScript(InBattleState::getSingletonPtr(), scriptName);
}

std::string PlayerBattleState::getAIScriptEntryPoint()
{
	return mAIScriptEntryPoint;
}

std::string PlayerBattleState::getAIScriptName()
{
	return mAIScriptName;
}

void PlayerBattleState::updateVisibilityState()
{
	std::deque<PlayerBattleState*> otherPlayers = InBattleState::getSingletonPtr()->getAllPlayers();
	std::deque<MCUController*> enemyMCUs;
	std::deque<Projectile*> enemyProjectiles;

	std::deque<PlayerBattleState*>::iterator player;

	for(player = otherPlayers.begin(); player != otherPlayers.end(); ++player)
	{
		if((*player) == this)
			continue;

		std::deque<MCUController*> mcus = (*player)->getMCUs();
		enemyMCUs.insert(enemyMCUs.begin(), mcus.begin(), mcus.end());
		
		std::deque<Projectile*> projectiles = (*player)->getProjectiles();
		enemyProjectiles.insert(enemyProjectiles.begin(), projectiles.begin(), projectiles.end());
	}
	

	std::deque<MCUController*>::iterator enemy;
	std::deque<MCUController*>::iterator friendly;
	
	bool couldSeeEnemies = mVisibleMCUs.size() > 0;
	mVisibleMCUs.clear();
	mNotVisibleMCUs.clear();
	mRadarVisibleMCUs.clear();

	if(mCoverageMap)
	{
		mCoverageMap->clearSightings();
	}

	for(enemy = enemyMCUs.begin(); enemy != enemyMCUs.end(); ++enemy)
	{
		bool canSee = false;
		bool radarVisible = false;
		for(friendly = mMCUs.begin(); friendly != mMCUs.end(); ++friendly)
		{
			
			Real sightRange = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getSightRange();
			Real maxSightSquaredDistance = sightRange * sightRange;

			Real radarRange(0);
			if((*friendly)->sensorActive())
				radarRange = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getRadarRange();

			Real maxRadarSquaredDistance = radarRange * radarRange;

			Vector3 enemyLoc = (*enemy)->getMiddle();
			Vector3 friendlyLoc = (*friendly)->getMiddle();

			Vector3 direction = enemyLoc - friendlyLoc;
			Real squaredDistance = direction.squaredLength();

			if(squaredDistance >= maxRadarSquaredDistance)
			{
				continue;
			}
			else if(squaredDistance < maxRadarSquaredDistance && squaredDistance >= maxSightSquaredDistance)
			{
				radarVisible = true;
				continue;
			}

			if((*friendly)->canSee((*enemy)))
			{
				mVisibleMCUs.push_back((*enemy));
				if(mNeedsRender)
					(*enemy)->setVisible(true);
				canSee = true;
				if(!couldSeeEnemies)
					notifyOfEnemyVisibilityChange((*friendly), (*enemy));
				if(mCoverageMap)
					mCoverageMap->addEnemySighting((*enemy)->getCurrentLocation());

				break;
			}
		}
		if(!canSee && !radarVisible)
		{
			mNotVisibleMCUs.push_back((*enemy));
			if(mNeedsRender)
			{
				(*enemy)->setRadarVisible(false);
				(*enemy)->setVisible(false);
			}
		}
		if(!canSee && radarVisible)
		{
			mRadarVisibleMCUs.push_back((*enemy));
			if(mNeedsRender)
				(*enemy)->setRadarVisible(true);
			if(mCoverageMap)
					mCoverageMap->addEnemySighting((*enemy)->getCurrentLocation());

		}
	}

	if(mNeedsRender)
	{
		std::deque<Projectile*>::iterator projIter;

		for(projIter = enemyProjectiles.begin(); projIter != enemyProjectiles.end(); ++projIter)
		{
			for(friendly = mMCUs.begin(); friendly != mMCUs.end(); ++friendly)
			{
				Real maxSquaredDistance = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getSightRange();
				maxSquaredDistance*= maxSquaredDistance;

				Vector3 friendlyLoc = (*friendly)->getMiddle();

				(*projIter)->setVisible((*projIter)->isCloserThan(friendlyLoc, maxSquaredDistance));
			}
		}

		std::deque<MCUController*>::iterator mcuIterator;

		for(mcuIterator = mMCUs.begin();mcuIterator != mMCUs.end(); ++mcuIterator)
		{
			(*mcuIterator)->renderFogOfWar();
		}
	
	}
	mNextVisibleMCU = mVisibleMCUs.begin();
}

void PlayerBattleState::updateProjectileVisibility()
{
	if(mNeedsRender)
	{
		std::deque<MovableEntity*>& movableEntities = InBattleState::getSingletonPtr()->getMovableEntities();
		std::deque<MovableEntity*>::iterator mov;
		
		std::deque<MCUController*>::iterator friendly;

		for(mov = movableEntities.begin(); mov != movableEntities.end(); ++mov)
		{
			if((*mov)->usesVisibilityCalculations())
			{
				for(friendly = mMCUs.begin(); friendly != mMCUs.end(); ++friendly)
				{
					Real maxSquaredDistance = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getSightRange();
					maxSquaredDistance*= maxSquaredDistance;

					Vector3 friendlyLoc = (*friendly)->getMiddle();

					bool visible = (*mov)->isCloserThan(friendlyLoc, maxSquaredDistance);
					(*mov)->setVisible(visible);
					
					if(visible)
						break;
				
				}
			}
		}

		/*
		std::deque<PlayerBattleState*> otherPlayers = InBattleState::getSingletonPtr()->getAllPlayers();
		std::deque<Projectile*> enemyProjectiles;

		std::deque<PlayerBattleState*>::iterator player;

		for(player = otherPlayers.begin(); player != otherPlayers.end(); ++player)
		{				
			std::deque<Projectile*> projectiles = (*player)->getProjectiles();
			enemyProjectiles.insert(enemyProjectiles.begin(), projectiles.begin(), projectiles.end());
		}
	
	
		std::deque<Projectile*>::iterator projIter;
		for(projIter = enemyProjectiles.begin(); projIter != enemyProjectiles.end();++projIter)
		{
			for(friendly = mMCUs.begin(); friendly != mMCUs.end(); ++friendly)
			{
				Real maxSquaredDistance = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getSightRange();
				maxSquaredDistance*= maxSquaredDistance;

				Vector3 friendlyLoc = (*friendly)->getMiddle();

				bool visible = (*projIter)->isCloserThan(friendlyLoc, maxSquaredDistance);
				(*projIter)->setVisible(visible);
					
				if(visible)
					break;
				
			}
		}
		*/
	}
}

void PlayerBattleState::updateMCUVisibility()
{
	std::deque<PlayerBattleState*> otherPlayers = InBattleState::getSingletonPtr()->getAllPlayers();
	std::deque<MCUController*> enemyMCUs;

	std::deque<PlayerBattleState*>::iterator player;

	for(player = otherPlayers.begin(); player != otherPlayers.end(); ++player)
	{
		if((*player) == this)
			continue;

		std::deque<MCUController*> mcus = (*player)->getMCUs();
		enemyMCUs.insert(enemyMCUs.begin(), mcus.begin(), mcus.end());
	}
	

	std::deque<MCUController*>::iterator enemy;
	std::deque<MCUController*>::iterator friendly;
	
	bool couldSeeEnemies = mVisibleMCUs.size() > 0 || mRadarVisibleMCUs.size() > 0;
	mVisibleMCUs.clear();
	mNotVisibleMCUs.clear();
	mRadarVisibleMCUs.clear();
	bool clearedSightings(false);
	for(enemy = enemyMCUs.begin(); enemy != enemyMCUs.end(); ++enemy)
	{
		bool canSee(false);
		bool radarVisible(false);
		for(friendly = mMCUs.begin(); friendly != mMCUs.end(); ++friendly)
		{
			
			Real sightRange = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getSightRange();
			Real maxSightSquaredDistance = sightRange * sightRange;
			Real radarRange = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getRadarRange();
			Real maxRadarSquaredDistance = radarRange * radarRange;

			Vector3 enemyLoc = (*enemy)->getMiddle();
			Vector3 friendlyLoc = (*friendly)->getMiddle();

			Vector3 direction = enemyLoc - friendlyLoc;
			Real squaredDistance = direction.squaredLength();

			if(squaredDistance >= maxRadarSquaredDistance)
			{
				continue;
			}
			else if(squaredDistance < maxRadarSquaredDistance && squaredDistance >= maxSightSquaredDistance)
			{
				radarVisible = true;
				if(!clearedSightings && mCoverageMap)
				{
					clearedSightings = true;
					mCoverageMap->clearSightings();
				}
				if(!couldSeeEnemies)
					notifyOfEnemyVisibilityChange((*friendly), (*enemy));
				if(mCoverageMap)
					mCoverageMap->addEnemySighting((*enemy)->getCurrentLocation());
				continue;
			}

			if((*friendly)->canSee((*enemy)))
			{
				mVisibleMCUs.push_back((*enemy));
				if(!clearedSightings && mCoverageMap)
				{
					clearedSightings = true;
					mCoverageMap->clearSightings();
				}
				if(mNeedsRender)
					(*enemy)->setVisible(true);
				canSee = true;
				if(!couldSeeEnemies)
					notifyOfEnemyVisibilityChange((*friendly), (*enemy));
				if(mCoverageMap)
					mCoverageMap->addEnemySighting((*enemy)->getCurrentLocation());

				break;
			}
		}
		if(!canSee && !radarVisible)
		{
			mNotVisibleMCUs.push_back((*enemy));
			if(mNeedsRender)
			{
				(*enemy)->setRadarVisible(false);
				(*enemy)->setVisible(false);
			}
		}
		if(!canSee && radarVisible)
		{
			mRadarVisibleMCUs.push_back((*enemy));
			if(mNeedsRender)
				(*enemy)->setRadarVisible(true);
			if(mCoverageMap)
					mCoverageMap->addEnemySighting((*enemy)->getCurrentLocation());

		}
	}

	if(mCoverageMap)
	{
		//if we can see an enemy, clear the sightings of the coverage map
		//if not, leave the last sightings in place
		if(mRadarVisibleMCUs.empty() && mVisibleMCUs.empty())
		{
			for(friendly = mMCUs.begin(); friendly != mMCUs.end(); ++friendly)
			{
				Real sightRange = BattlefieldGrid::UnitSize * (*friendly)->getMechanizedCombatUnit()->getSensorSystem()->getSightRange();
				Real maxSightSquaredDistance = sightRange * sightRange;

				std::deque<HotSpot> sightings = mCoverageMap->getEnemySightings();
				std::deque<HotSpot>::iterator sighting;
				for(sighting = sightings.begin(); sighting != sightings.end(); ++sighting)
				{
					Vector3 friendlyLoc = (*friendly)->getMiddle();
					BattlefieldGridCell* loc = InBattleState::getSingletonPtr()->getBattlefieldGrid()->getGridCell((*sighting).X, (*sighting).Z);
					Vector3 sightingLoc = loc->getMiddleFloor();
					Vector3 direction = sightingLoc - friendlyLoc;
					Real squaredDistance = direction.squaredLength();

					if(squaredDistance > maxSightSquaredDistance)
					{
						mCoverageMap->removeEnemySighting((*sighting));
					}
					
				}

			}
		}
	}

	if(mNeedsRender)
	{
		std::deque<MCUController*>::iterator mcuIterator;

		for(mcuIterator = mMCUs.begin();mcuIterator != mMCUs.end(); ++mcuIterator)
		{
			(*mcuIterator)->renderFogOfWar();
		}
	}

	mNextVisibleMCU = mVisibleMCUs.begin();
}

int PlayerBattleState::getAverageMCUXPos()
{
	std::deque<AIGroup*>::iterator iter;

	int total(0), num(0);

	for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
	{
		total+= (*iter)->getAverageMCUXPos();
		++num;
	}
	if(num > 0)
		return total / num;
	return 0;
}

int PlayerBattleState::getAverageMCUZPos()
{
	std::deque<AIGroup*>::iterator iter;

	int total(0), num(0);

	for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
	{
		total+= (*iter)->getAverageMCUZPos();
		++num;
	}
	if(num > 0)
		return total / num;
	return 0;
}

void PlayerBattleState::runAIScript()
{
	LuaStateManager::getSingletonPtr()->issueOrders(InBattleState::getSingletonPtr(), this);
}

void PlayerBattleState::updateCoverageMap(bool reset)
{
	if(mIsAI && mVisibleMCUs.empty())
	{
		if(mCoverageMap)
		{
			if(reset)
				mCoverageMap->resetCoverageMap();
		}
		else
		{
			int sideLength = InBattleState::getSingletonPtr()->getBattlefieldGrid()->getSideLength();
			int size = sideLength * sideLength;
			mCoverageMap = new CoverageMap(sideLength, size, this);
		}
		std::deque<MCUController*>::iterator friendly;
		for(friendly = mMCUs.begin(); friendly != mMCUs.end(); ++friendly)
		{
			mCoverageMap->setVisibility((*friendly)->getCurrentFogSquareGrid());
		}
		mCoverageMap->computeCoverageMap();
	}
	else if(mIsAI)
	{
		if(mCoverageMap)
		{
			mCoverageMap->resetCoverageMap();
		}
	}
}

std::hash_map<std::string,std::string> PlayerBattleState::getGuiValues()
{
	std::hash_map<std::string,std::string> result;

	result[FieldNames::Player::FactionName] = mFactionTemplate->getName();
	result[FieldNames::Player::PlayerName] = mPlayerProfile->getName();
	result[FieldNames::Player::PlayerNumber] = SQLiteDBManager::convertIntToString(mPlayerNumber);
	result[FieldNames::Player::TeamNumber] = SQLiteDBManager::convertIntToString(mTeam);

	return result;
}


void PlayerBattleState::setAIGroups(std::deque<AIGroup*> formations)
{
	mAIGroups = formations;
	mAIGroupIterator = mAIGroups.begin();

	std::deque<AIGroup*>::iterator iter;
	for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
	{
		(*iter)->setPlayerBattleState(this);
	}
}

void PlayerBattleState::addAIGroup(AIGroup* formation)
{
	mAIGroups.push_back(formation);
}
	
bool PlayerBattleState::hasMoreAIGroups()
{
	return mAIGroupIterator != mAIGroups.end();
}

AIGroup* PlayerBattleState::getNextAIGroup()
{
	AIGroup* next = (*mAIGroupIterator);
	++mAIGroupIterator;
	return next;
}

bool PlayerBattleState::hasMoreVisibleMCUs()
{
	return mNextVisibleMCU != mVisibleMCUs.end();
}

MCUController* PlayerBattleState::getNextVisibleMCU()
{
	MCUController* next = (*mNextVisibleMCU);
	++mNextVisibleMCU;
	return next;
}

std::deque<MCUController*> PlayerBattleState::getAllVisibleMCUs()
{
	return mVisibleMCUs;
}

void PlayerBattleState::notifyOfMovementChange(MCUController* controller)
{
	if(mIsAI)
	{
		AIGroup* group =controller->getAIGroup();
		if(group)
		{
			if(group->hasMoveOrder())
			{
				if(controller->reachedDestination())
					group->getGroupMoveOrder()->setOrderStatus(controller, OrderStatus::OS_COMPLETED);
				else
					group->getGroupMoveOrder()->setOrderStatus(controller, OrderStatus::OS_FAILED);
			}
			if(group->hasMoveToAttackOrder())
			{
				if(controller->reachedDestination())
					group->getGroupMoveToAttackOrder()->setOrderStatus(controller, OrderStatus::OS_COMPLETED);
				else
					group->getGroupMoveToAttackOrder()->setOrderStatus(controller, OrderStatus::OS_FAILED);
			}
		}
	}
}

void PlayerBattleState::notifyOfEnemyVisibilityChange(MCUController* controller, MCUController* enemy)
{
	if(controller->isMoving())
		controller->giveStopOrder();

	if(mIsAI)
	{
		if(controller->getAIGroup())
		{
			if(controller->getAIGroup()->hasMoveOrder())
			{
				controller->getAIGroup()->finishedGroupMoveOrder(false);
				controller->getAIGroup()->giveGroupMoveToAttackOrder(enemy);
			}
		}
	}
}

void PlayerBattleState::notifyOfNewOrders(AIGroup* group, GroupOrder* order)
{
	
}

void PlayerBattleState::notifyOfOrderCompletion(AIGroup* group, GroupOrder* order)
{

}

void PlayerBattleState::notifyOfFinishedTurn(AIGroup* group)
{
	bool finished = true;
	std::deque<AIGroup*>::iterator iter;
	for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
	{
		if(!(*iter)->hasFinishedTurn())
			finished = false;
	}

	if(finished)
	{
		endMyTurn();
	}
}

void PlayerBattleState::loopFinished()
{
	mNextVisibleMCU = mVisibleMCUs.begin();
	mAIGroupIterator = mAIGroups.begin();
}

CoverageMap* PlayerBattleState::getCoverageMap()
{
	return mCoverageMap;
}

MCUController* PlayerBattleState::getNextMCU(MCUController* mcu)
{
	if(mcu == mMCUs.back())
		return mMCUs.front();

	std::deque<MCUController*>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter) == mcu)
			break;
	}
	++iter;
	return (*iter);
}

MCUController* PlayerBattleState::getPrevMCU(MCUController* mcu)
{
	if(mcu == mMCUs.front())
		return mMCUs.back();

	std::deque<MCUController*>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter) == mcu)
			break;
	}

	--iter;
	return (*iter);
}
