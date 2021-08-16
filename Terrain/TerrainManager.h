#ifndef TerrainManager_h
#define TerrainManager_h

#include <Ogre.h>
#include <Entities/Mission.h>
#include <Entities/MCUPlacement.h>
#include <States/PlayerBattleState.h>
#include <Templates/TerrainTemplate.h>
#include <Templates/BattlefieldTemplate.h>

class BattlefieldGrid;
class MechanizedCombatUnit;

class TerrainManager
{
public:
	~TerrainManager();
	std::deque<MCUPlacement> placeMCUs(int sideLength, int x, int z, std::deque<MechanizedCombatUnit*> mcus, BattlefieldGrid* grid);
	
	static float calculateDistance(int x1, int z1, int x2, int z2);
	static float calculateSquareDistance(int x1, int z1, int x2, int z2);
	static TerrainManager* getSingletonPtr();
private:
	TerrainManager();
	Ogre::SceneManager* mSceneManager;

	bool isOccupied(int x, int z);
	bool isAvailable(int x, int z);

	std::deque<MCUPlacement> mMCUs;
	BattlefieldGrid* mBattlefieldGrid;

	bool mTerrainsImported;
	
	static TerrainManager* mTerrainManager;
};
#endif