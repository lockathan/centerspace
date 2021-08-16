#include <Entities/Mission.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/MCUPlacement.h>
#include <AI/AIGroup.h>
#include <StateChange/PlayerPreBattleState.h>
#include <Templates/RandomMissionTypeTemplate.h>
#include <Templates/StoryMissionTemplate.h>
#include <Templates/BattlefieldTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <deque>
#include <string>

AIPreBattleState::AIPreBattleState()
{

}

AIPreBattleState::~AIPreBattleState()
{
	std::deque<AIGroup*>::iterator iter;
	
	for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
	{
		if((*iter))
			delete (*iter);
		iter = mAIGroups.erase(iter);
	}
}

std::deque<AIGroup*> AIPreBattleState::getEnemyAIGroups()
{
	return mAIGroups;
}

void AIPreBattleState::setEnemyAIGroups(std::deque<AIGroup*> aigroups)
{
	mAIGroups = aigroups;
}

void AIPreBattleState::addEnemyAIGroup(AIGroup* aigroup)
{
	mAIGroups.push_back(aigroup);
}

std::deque<MCUPlacement*> AIPreBattleState::getAllMCUPlacements()
{
	std::deque<MCUPlacement*> result;
	std::deque<AIGroup*>::iterator iter;
	
	for(iter = mAIGroups.begin(); iter != mAIGroups.end(); ++iter)
	{
		std::deque<MCUPlacement*> placements = (*iter)->getMCUPlacements();
		result.insert(result.end(), placements.begin(), placements.end());	
	}

	return result;
}


Mission::Mission()
	:
	mTerrainTemplate(0),
	mFactionHubTemplate(0),
	mSideLength(0),
	mRequiredRank(0),
	mMissionScriptFilename(""),
	mTerrainGenerationFilename(""),
	mTerrainGenerationEntryPoint(""),
	mEnemyGenerationFilename(""),
	mEnemyGenerationEntryPoint(""),
	mPlacementFilename(""),
	mPlayerPlacementEntryPoint(""),
	mEnemyPlacementEntryPoint(""),
	mPlayerLocationX(0),
	mPlayerLocationZ(0),
	mEnemy1LocationX(0),
	mEnemy1LocationZ(0),
	mEnemy2LocationX(0),
	mEnemy2LocationZ(0),
	mLuaParam1(0),
	mLuaParam2(0),
	mLuaParam3(0),
	mLuaParam4(0)
{

}
Mission::Mission(StoryMissionTemplate* mission)
	:
	mTerrainTemplate(mission->getTerrainTemplate()),
	mFactionHubTemplate(mission->getFactionHubTemplate()),
	mSideLength(mission->getSideLength()),
	mRequiredRank(mission->getRequiredRank()),
	mMissionScriptFilename(mission->getMissionScriptFilename()),
	mTerrainGenerationFilename(mission->getTerrainGenerationFilename()),
	mTerrainGenerationEntryPoint(mission->getTerrainGenerationEntryPoint()),
	mEnemyGenerationFilename(mission->getEnemyGenerationFilename()),
	mEnemyGenerationEntryPoint(mission->getEnemyGenerationEntryPoint()),
	mPlacementFilename(mission->getPlacementFilename()),
	mPlayerPlacementEntryPoint(mission->getPlayerPlacementEntryPoint()),
	mEnemyPlacementEntryPoint(mission->getEnemyPlacementEntryPoint()),
	mPlayerLocationX(mission->getPlayerLocationX()),
	mPlayerLocationZ(mission->getPlayerLocationZ()),
	mEnemy1LocationX(mission->getEnemy1LocationX()),
	mEnemy1LocationZ(mission->getEnemy1LocationZ()),
	mEnemy2LocationX(mission->getEnemy2LocationX()),
	mEnemy2LocationZ(mission->getEnemy2LocationZ()),
	mLuaParam1(mission->getLuaParam1()),
	mLuaParam2(mission->getLuaParam2()),
	mLuaParam3(mission->getLuaParam3()),
	mLuaParam4(mission->getLuaParam4())
{

}

Mission::Mission(RandomMissionTypeTemplate* type, int sideLength, int requiredRank, TerrainTemplate* terrain, FactionHubTemplate* factionHub)
	:
	mTerrainTemplate(terrain),
	mFactionHubTemplate(factionHub),
	mSideLength(sideLength),
	mRequiredRank(requiredRank),
	mMissionScriptFilename(""),
	mTerrainGenerationFilename(terrain->getLuaScriptFile()),
	mTerrainGenerationEntryPoint(terrain->getLuaEntryPoint()),
	mEnemyGenerationFilename(type->getEnemyGenerationFilename()),
	mEnemyGenerationEntryPoint(type->getEnemyGenerationEntryPoint()),
	mPlacementFilename(type->getPlacementScriptFilename()),
	mPlayerPlacementEntryPoint(type->getPlayerPlacementEntryPoint()),
	mEnemyPlacementEntryPoint(type->getEnemyPlacementEntryPoint()),
	mPlayerLocationX(0),
	mPlayerLocationZ(0),
	mEnemy1LocationX(0),
	mEnemy1LocationZ(0),
	mEnemy2LocationX(0),
	mEnemy2LocationZ(0),
	mLuaParam1(terrain->getLuaParam1()),
	mLuaParam2(terrain->getLuaParam2()),
	mLuaParam3(terrain->getLuaParam3()),
	mLuaParam4(terrain->getLuaParam4())
{
	
}
Mission::~Mission()
{
	std::deque<PlayerPreBattleState*>::iterator iter;

	for(iter = mPlayerPreBattleStates.begin(); iter != mPlayerPreBattleStates.end(); ++iter)
	{
		delete (*iter);
	}
}

int Mission::getId()
{
	return mId;
}

void Mission::setId(int id)
{
	mId = id;
}

std::string Mission::getName()
{
	return mName;
}

void Mission::setName(const std::string& name)
{
	mName = name;
}

TerrainTemplate* Mission::getTerrainTemplate()
{
	return mTerrainTemplate;
}

void Mission::setTerrainTemplate(TerrainTemplate* terrain)
{
	mTerrainTemplate = terrain;
}

FactionHubTemplate* Mission::getFactionHubTemplate()
{
	return mFactionHubTemplate;
}

void Mission::setFactionHubTemplate(FactionHubTemplate* hub)
{
	mFactionHubTemplate = hub;
}

int Mission::getSideLength()
{
	return mSideLength;
}

void Mission::setSideLength(int length)
{
	mSideLength = length;
}

int Mission::getRequiredRank()
{
	return mRequiredRank;
}

void Mission::setRequiredRank(int rank)
{
	mRequiredRank = rank;
}

std::string Mission::getMissionScriptFilename()
{
	return mMissionScriptFilename;
}

void Mission::setMissionScriptFilename(const std::string& filename)
{
	mMissionScriptFilename = filename;
}

std::string Mission::getTerrainGenerationFilename()
{
	return mTerrainGenerationFilename;
}

void Mission::setTerrainGenerationFilename(const std::string& filename)
{
	mTerrainGenerationFilename = filename;
}

std::string Mission::getTerrainGenerationEntryPoint()
{
	return mTerrainGenerationEntryPoint;
}

void Mission::setTerrainGenerationEntryPoint(const std::string& entry)
{
	mTerrainGenerationEntryPoint = entry;
}
	
std::string Mission::getEnemyGenerationFilename()
{
	return mEnemyGenerationFilename;
}

void Mission::setEnemyGenerationFilename(const std::string& filename)
{
	mEnemyGenerationFilename = filename;
}

std::string Mission::getEnemyGenerationEntryPoint()
{
	return mEnemyGenerationEntryPoint;
}

void Mission::setEnemyGenerationEntryPoint(const std::string& entry)
{
	mEnemyGenerationEntryPoint = entry;
}
	
std::string Mission::getPlacementFilename()
{
	return mPlacementFilename;
}

void Mission::setPlacementFilename(const std::string& filename)
{
	mPlacementFilename = filename;
}

std::string Mission::getPlayerPlacementEntryPoint()
{
	return mPlayerPlacementEntryPoint;
}

void Mission::setPlayerPlacementEntryPoint(const std::string& entry)
{
	mPlayerPlacementEntryPoint = entry;
}
	
std::string Mission::getEnemyPlacementEntryPoint()
{
	return mEnemyPlacementEntryPoint;
}

void Mission::setEnemyPlacementEntryPoint(const std::string& entry)
{
	mEnemyPlacementEntryPoint = entry;
}
	
int Mission::getPlayerLocationX()
{
	return mPlayerLocationX;
}

void Mission::setPlayerLocationX(int x)
{
	mPlayerLocationX = x;
}
	
int Mission::getPlayerLocationZ()
{
	return mPlayerLocationZ;
}

void Mission::setPlayerLocationZ(int z)
{
	mPlayerLocationZ = z;
}
	
int Mission::getEnemy1LocationX()
{
	return mEnemy1LocationX;
}

void Mission::setEnemy1LocationX(int x)
{
	mEnemy1LocationX = x;
}
	
int Mission::getEnemy1LocationZ()
{
	return mEnemy1LocationZ;
}

void Mission::setEnemy1LocationZ(int z)
{
	mEnemy1LocationZ = z;
}
	
int Mission::getEnemy2LocationX()
{
	return mEnemy2LocationX;
}

void Mission::setEnemy2LocationX(int x)
{
	mEnemy2LocationX = x;
}
	
int Mission::getEnemy2LocationZ()
{
	return mEnemy2LocationZ;
}

void Mission::setEnemy2LocationZ(int z)
{
	mEnemy2LocationZ = z;
}

float Mission::getLuaParam1()
{
	return mLuaParam1;
}

void Mission::setLuaParam1(float param)
{
	mLuaParam1 = param;
}
	
float Mission::getLuaParam2()
{
	return mLuaParam2;
}

void Mission::setLuaParam2(float param)
{
	mLuaParam2 = param;
}

float Mission::getLuaParam3()
{
	return mLuaParam3;
}

void Mission::setLuaParam3(float param)
{
	mLuaParam3 = param;
}

float Mission::getLuaParam4()
{
	return mLuaParam4;
}

void Mission::setLuaParam4(float param)
{
	mLuaParam4 = param;
}

std::deque<PlayerPreBattleState*>& Mission::getPlayerPreBattleStates()
{
	return mPlayerPreBattleStates;
}

void Mission::setPlayerPreBattleStates(std::deque<PlayerPreBattleState*>& states)
{
	mPlayerPreBattleStates = states;
}

bool Mission::isCellOccupied(int x, int z)
{
	std::deque<PlayerPreBattleState*>::iterator iter;

	for(iter = mPlayerPreBattleStates.begin(); iter != mPlayerPreBattleStates.end(); ++iter)
	{
		if((*iter)->isCellOccupied(x, z))
			return true;
	}
	return false;
}

void Mission::resetAIGroupMCUs()
{
	std::deque<PlayerPreBattleState*>::iterator iter;

	for(iter = mPlayerPreBattleStates.begin(); iter != mPlayerPreBattleStates.end(); ++iter)
	{
		(*iter)->resetAIGroupMCUs();
	}
}

