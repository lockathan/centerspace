#ifndef StoryMissionTemplate_h
#define StoryMissionTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>
#include <hash_map>

class AIGroupTemplate;
class FactionHubTemplate;
class BattlefieldTemplate;
class TerrainTemplate;


class StoryMissionTemplate : public Template
{
public:
	StoryMissionTemplate();
	~StoryMissionTemplate();

	TerrainTemplate* getTerrainTemplate();
	void setTerrainTemplate(TerrainTemplate* terrain);

	FactionHubTemplate* getFactionHubTemplate();
	void setFactionHubTemplate(FactionHubTemplate* hub);

	int getSideLength();
	void setSideLength(int length);

	int getRequiredRank();
	void setRequiredRank(int rank);

	std::string getMissionScriptFilename();
	void setMissionScriptFilename(const std::string& filename);

	std::string getTerrainGenerationFilename();
	void setTerrainGenerationFilename(const std::string& filename);

	std::string getTerrainGenerationEntryPoint();
	void setTerrainGenerationEntryPoint(const std::string& entry);
	
	std::string getEnemyGenerationFilename();
	void setEnemyGenerationFilename(const std::string& filename);

	std::string getEnemyGenerationEntryPoint();
	void setEnemyGenerationEntryPoint(const std::string& entry);
	
	std::string getPlacementFilename();
	void setPlacementFilename(const std::string& filename);

	std::string getPlayerPlacementEntryPoint();
	void setPlayerPlacementEntryPoint(const std::string& entry);
	
	std::string getEnemyPlacementEntryPoint();
	void setEnemyPlacementEntryPoint(const std::string& entry);
	
	int getPlayerLocationX();
	void setPlayerLocationX(int x);
	
	int getPlayerLocationZ();
	void setPlayerLocationZ(int z);
	
	int getEnemy1LocationX();
	void setEnemy1LocationX(int x);
	
	int getEnemy1LocationZ();
	void setEnemy1LocationZ(int z);
	
	int getEnemy2LocationX();
	void setEnemy2LocationX(int x);
	
	int getEnemy2LocationZ();
	void setEnemy2LocationZ(int z);

	float getLuaParam1();
	void setLuaParam1(float param);
	
	float getLuaParam2();
	void setLuaParam2(float param);

	float getLuaParam3();
	void setLuaParam3(float param);

	float getLuaParam4();
	void setLuaParam4(float param);
	

private:
	TerrainTemplate* mTerrainTemplate;
	FactionHubTemplate* mFactionHubTemplate;

	int mSideLength;
	int mRequiredRank;
	std::string mMissionScriptFilename;
	std::string mTerrainGenerationFilename;
	std::string mTerrainGenerationEntryPoint;
	std::string mEnemyGenerationFilename;
	std::string mEnemyGenerationEntryPoint;
	std::string mPlacementFilename;
	std::string mPlayerPlacementEntryPoint;
	std::string mEnemyPlacementEntryPoint;
	int mPlayerLocationX;
	int mPlayerLocationZ;
	int mEnemy1LocationX;
	int mEnemy1LocationZ;
	int mEnemy2LocationX;
	int mEnemy2LocationZ;
	float mLuaParam1;
	float mLuaParam2;
	float mLuaParam3;
	float mLuaParam4;
};


#endif