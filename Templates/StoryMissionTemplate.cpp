#include <Templates/StoryMissionTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <string>
#include <hash_map>

StoryMissionTemplate::StoryMissionTemplate()
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
	mEnemy2LocationZ(0)
{

}

StoryMissionTemplate::~StoryMissionTemplate()
{

}

TerrainTemplate* StoryMissionTemplate::getTerrainTemplate()
{
	return mTerrainTemplate;
}

void StoryMissionTemplate::setTerrainTemplate(TerrainTemplate* terrain)
{
	mTerrainTemplate = terrain;
}

FactionHubTemplate* StoryMissionTemplate::getFactionHubTemplate()
{
	return mFactionHubTemplate;
}

void StoryMissionTemplate::setFactionHubTemplate(FactionHubTemplate* hub)
{
	mFactionHubTemplate = hub;
}

int StoryMissionTemplate::getSideLength()
{
	return mSideLength;
}

void StoryMissionTemplate::setSideLength(int length)
{
	mSideLength = length;
}

int StoryMissionTemplate::getRequiredRank()
{
	return mRequiredRank;
}

void StoryMissionTemplate::setRequiredRank(int rank)
{
	mRequiredRank = rank;
}

std::string StoryMissionTemplate::getMissionScriptFilename()
{
	return mMissionScriptFilename;
}

void StoryMissionTemplate::setMissionScriptFilename(const std::string& filename)
{
	mMissionScriptFilename = filename;
}

std::string StoryMissionTemplate::getTerrainGenerationFilename()
{
	return mTerrainGenerationFilename;
}

void StoryMissionTemplate::setTerrainGenerationFilename(const std::string& filename)
{
	mTerrainGenerationFilename = filename;
}

std::string StoryMissionTemplate::getTerrainGenerationEntryPoint()
{
	return mTerrainGenerationEntryPoint;
}

void StoryMissionTemplate::setTerrainGenerationEntryPoint(const std::string& entry)
{
	mTerrainGenerationEntryPoint = entry;
}
	
std::string StoryMissionTemplate::getEnemyGenerationFilename()
{
	return mEnemyGenerationFilename;
}

void StoryMissionTemplate::setEnemyGenerationFilename(const std::string& filename)
{
	mEnemyGenerationFilename = filename;
}

std::string StoryMissionTemplate::getEnemyGenerationEntryPoint()
{
	return mEnemyGenerationEntryPoint;
}

void StoryMissionTemplate::setEnemyGenerationEntryPoint(const std::string& entry)
{
	mEnemyGenerationEntryPoint = entry;
}
	
std::string StoryMissionTemplate::getPlacementFilename()
{
	return mPlacementFilename;
}

void StoryMissionTemplate::setPlacementFilename(const std::string& filename)
{
	mPlacementFilename = filename;
}

std::string StoryMissionTemplate::getPlayerPlacementEntryPoint()
{
	return mPlayerPlacementEntryPoint;
}

void StoryMissionTemplate::setPlayerPlacementEntryPoint(const std::string& entry)
{
	mPlayerPlacementEntryPoint = entry;
}
	
std::string StoryMissionTemplate::getEnemyPlacementEntryPoint()
{
	return mEnemyPlacementEntryPoint;
}

void StoryMissionTemplate::setEnemyPlacementEntryPoint(const std::string& entry)
{
	mEnemyPlacementEntryPoint = entry;
}
	
int StoryMissionTemplate::getPlayerLocationX()
{
	return mPlayerLocationX;
}

void StoryMissionTemplate::setPlayerLocationX(int x)
{
	mPlayerLocationX = x;
}
	
int StoryMissionTemplate::getPlayerLocationZ()
{
	return mPlayerLocationZ;
}

void StoryMissionTemplate::setPlayerLocationZ(int z)
{
	mPlayerLocationZ = z;
}
	
int StoryMissionTemplate::getEnemy1LocationX()
{
	return mEnemy1LocationX;
}

void StoryMissionTemplate::setEnemy1LocationX(int x)
{
	mEnemy1LocationX = x;
}
	
int StoryMissionTemplate::getEnemy1LocationZ()
{
	return mEnemy1LocationZ;
}

void StoryMissionTemplate::setEnemy1LocationZ(int z)
{
	mEnemy1LocationZ = z;
}
	
int StoryMissionTemplate::getEnemy2LocationX()
{
	return mEnemy2LocationX;
}

void StoryMissionTemplate::setEnemy2LocationX(int x)
{
	mEnemy2LocationX = x;
}
	
int StoryMissionTemplate::getEnemy2LocationZ()
{
	return mEnemy2LocationZ;
}

void StoryMissionTemplate::setEnemy2LocationZ(int z)
{
	mEnemy2LocationZ = z;
}

float StoryMissionTemplate::getLuaParam1()
{
	return mLuaParam1;
}

void StoryMissionTemplate::setLuaParam1(float param)
{
	mLuaParam1 = param;
}
	
float StoryMissionTemplate::getLuaParam2()
{
	return mLuaParam2;
}

void StoryMissionTemplate::setLuaParam2(float param)
{
	mLuaParam2 = param;
}

float StoryMissionTemplate::getLuaParam3()
{
	return mLuaParam3;
}

void StoryMissionTemplate::setLuaParam3(float param)
{
	mLuaParam3 = param;
}

float StoryMissionTemplate::getLuaParam4()
{
	return mLuaParam4;
}

void StoryMissionTemplate::setLuaParam4(float param)
{
	mLuaParam4 = param;
}
