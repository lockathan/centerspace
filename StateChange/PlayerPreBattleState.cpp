#include <StateChange/PlayerPreBattleState.h>
#include <Ogre.h>
#include <BtOgre/BtOgreExtras.h>
#include <Controllers/KinematicCharacterController.h>
#include <Controllers/MCUController.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/DataProvider.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MovableEntity.h>
#include <Entities/PlayerProfile.h>
#include <Entities/Projectile.h>
#include <Managers/LuaStateManager.h>
#include <Managers/SQLiteDBManager.h>
#include <AI/AIGroup.h>
#include <States/InBattleState.h>
#include <Templates/AIGroupTemplate.h>
#include <Templates/AIMCUTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/PlayerProfile.h>
#include <LuaBindings/LuaMCUDeque.h>
#include <deque>

using namespace Ogre;

PlayerPreBattleState::PlayerPreBattleState()
	:
	mPlayerNumber(0),
	mPlayerProfile(0),
	mTeam(0),
	mFactionTemplate(0),
	mLocal(true),
	mNeedsRender(false),
	mIsAI(false),
	mLuaMCUDeque(0),
	mAIScriptName(""),
	mAIScriptEntryPoint(""),
	mFetchedMCUPlacements(false)
{

}

PlayerPreBattleState::PlayerPreBattleState(PlayerProfile* player,FactionTemplate* faction, int team, int playerNumber)
	:
	mPlayerNumber(playerNumber),
	mPlayerProfile(player),
	mTeam(team),
	mFactionTemplate(faction),
	mLocal(true),
	mNeedsRender(false),
	mIsAI(player->isAI()),
	mLuaMCUDeque(new LuaMCUDeque()),
	mAIScriptName(""),
	mAIScriptEntryPoint(""),
	mFetchedMCUPlacements(false)
{

}

PlayerPreBattleState::~PlayerPreBattleState()
{
	if(mIsAI)
	{
		delete mPlayerProfile;
		std::deque<AIGroup*>::iterator iter;

		for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
		{
			delete (*iter);
		}

	}
}

PlayerProfile* PlayerPreBattleState::getPlayerProfile()
{
	return mPlayerProfile;
}

void PlayerPreBattleState::setPlayer(PlayerProfile* player)
{
	mPlayerProfile = player;
}

int PlayerPreBattleState::getTeam()
{
	return mTeam;
}

void PlayerPreBattleState::setTeam(int team)
{
	mTeam = team;
}

int PlayerPreBattleState::getPlayerNumber()
{
	return mPlayerNumber;
}

void PlayerPreBattleState::setPlayerNumber(int n)
{
	mPlayerNumber = n;
}

FactionTemplate* PlayerPreBattleState::getFactionTemplate()
{
	return mFactionTemplate;
}

void PlayerPreBattleState::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}
		
bool PlayerPreBattleState::isLocal()
{
	return mLocal;
}

void PlayerPreBattleState::setLocal(bool value)
{
	mLocal = value;
}
	
bool PlayerPreBattleState::needsRender()
{
	return mNeedsRender;
}

void PlayerPreBattleState::setNeedsRender(bool value)
{
	mNeedsRender = value;
}

bool PlayerPreBattleState::isAI()
{
	return mIsAI;
}

void PlayerPreBattleState::setAI(bool value)
{
	mIsAI = value;
}

std::string PlayerPreBattleState::getAIScriptName()
{
	return mAIScriptName;	
}

void PlayerPreBattleState::setAIScriptName(const std::string& script)
{
	mAIScriptName = script;
}

std::string PlayerPreBattleState::getAIScriptEntryPoint()
{
	return mAIScriptEntryPoint;
}

void PlayerPreBattleState::setAIScriptEntryPoint(const std::string& entry)
{
	mAIScriptEntryPoint = entry;
}

std::deque<MCUPlacement*>& PlayerPreBattleState::getMCUPlacements()
{
	if(mIsAI)
	{
		if(!mFetchedMCUPlacements)
		{
			std::deque<AIGroup*>::iterator iter;

			for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
			{
				std::deque<MCUPlacement*>& groupPlacements = (*iter)->getMCUPlacements();
				mAllMCUPlacements.insert(mAllMCUPlacements.begin(), groupPlacements.begin(), groupPlacements.end());
			}
			mFetchedMCUPlacements = true;
		}
		return mAllMCUPlacements;
	}
	else
	{
		return mLuaMCUDeque->getMCUPlacements();
	}
}

std::deque<AIGroupTemplate*>& PlayerPreBattleState::getAIGroupTemplates()
{
	return mAIGroupTemplates;
}

void PlayerPreBattleState::addAIGroupTemplate(AIGroupTemplate* group)
{
	mAIGroupTemplates.push_back(group);
	AIGroup* aiGroup = new AIGroup(mAIGroupTemplates.size());
	mAIGroups.push_back(aiGroup);
	std::deque<MechanizedCombatUnit*> mcus = group->getMCUs();
	std::deque<MechanizedCombatUnit*>::iterator iter;
	for(iter = mcus.begin(); iter != mcus.end(); ++iter)
	{
		aiGroup->addMCUPlacement((*iter), 0, 0);
	}
}

std::deque<AIGroup*>& PlayerPreBattleState::getAIGroups()
{
	return mAIGroups;
}

void PlayerPreBattleState::addAIGroup(AIGroup* group)
{
	mAIGroups.push_back(group);
}

LuaMCUDeque* PlayerPreBattleState::getLuaMCUDeque()
{
	return mLuaMCUDeque;
}

void PlayerPreBattleState::setLuaMCUDeque(LuaMCUDeque* deque)
{
	mLuaMCUDeque = deque;
}


bool PlayerPreBattleState::isCellOccupied(int x, int z)
{
	if(mIsAI)
	{
		std::deque<AIGroup*>::iterator iter;
		
		for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
		{
			if((*iter)->getLuaMCUDeque()->isCellOccupied(x,z))
				return true;
		}
		return false;
	}
	else
	{
		return mLuaMCUDeque->isCellOccupied(x,z);
	}
}

void PlayerPreBattleState::resetAIGroupMCUs()
{
	if(mIsAI)
	{
		std::deque<AIGroup*>::iterator iter;
		
		for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
		{
			(*iter)->setMCUs(std::deque<MCUController*>());
		}
	}
}
