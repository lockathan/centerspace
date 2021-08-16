#include <Terrain/TerrainManager.h>
#include <Terrain/TerrainImgGenerator.h>
#include <Entities/Mission.h>
#include <Entities/MCUPlacement.h>
#include <Ogre.h>
#include <Templates/BattlefieldTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <Managers/PhysicsManager.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <States/PlayerBattleState.h>
#include <Managers/GameManager.h>
#include <hash_map>
#include <vector>

TerrainManager::TerrainManager()
{
	GameManager* manager = GameManager::getSingletonPtr();
	mSceneManager = manager->getSceneManager();
	manager = 0;
}

TerrainManager::~TerrainManager()
{

}

std::deque<MCUPlacement> TerrainManager::placeMCUs(int sideLength, int x, int z, std::deque<MechanizedCombatUnit*> mcus, BattlefieldGrid* grid)
{
	mMCUs.clear();
	mBattlefieldGrid = grid;
	
	if(x < 0 || x >= sideLength || z < 0 || z >= sideLength)
		return mMCUs;

	int currentX = x;
	int currentZ = z;
	int xDirection = 0;
	int zDirection = -2;

	std::deque<MechanizedCombatUnit*>::iterator mcuIterator;

	for(mcuIterator = mcus.begin();mcuIterator != mcus.end(); ++mcuIterator)
	{
		if(currentX >= 0 && currentX < sideLength && currentZ >= 0 && currentZ < sideLength)
		{
			if(!isOccupied(currentX, currentZ))
			{
				MCUPlacement placement(currentX, currentZ, (*mcuIterator));
				mMCUs.push_back(placement);
			}
		}

		currentX+= xDirection;
		currentZ+= zDirection;

		if(zDirection != 0)
		{
			if(currentX - zDirection >= 0 && currentX - zDirection < sideLength)
			{
				//can we turn?
				//if(!result[(sideLength * currentZ) + currentX - zDirection])
				if(isAvailable(currentX, currentZ - zDirection))
				{
					xDirection = -zDirection;
					zDirection = 0;
				}
				//carry on straight
			}
			else
			//if we are about to go out of bounds
			if(currentZ + zDirection < 0 || currentZ + zDirection >= sideLength)
			{
				xDirection = -zDirection;
				zDirection = 0;
				
				currentX+=xDirection;
				currentZ+=zDirection;
				bool found = false;
				while(!found)
				{
					if(currentX >= 0 && currentX < sideLength && currentZ >= 0 && currentZ < sideLength)
					{
						//if(result[(sideLength * currentZ) + currentX])
						if(!isAvailable(currentX, currentZ))
						{
							currentX+=xDirection;
							currentZ+=zDirection;
						}
						else
						{
							found = true;
						}
					}
					else
					{
						zDirection = -xDirection;
						xDirection = 0;
					}
				}
			}
		}
		else
		if(xDirection != 0)
		{
			if(currentZ - xDirection >= 0 && currentZ - xDirection < sideLength)
			{
				//can we turn?
				//if(!result[(sideLength * currentZ) + currentX - xDirection])
				if(isAvailable(currentX - xDirection, currentZ))
				{
					zDirection = -xDirection;
					xDirection = 0;
				}
				//carry on straight
			}
			else
			//if we are about to go out of bounds
			if(currentX + xDirection < 0 || currentX + xDirection >= sideLength)
			{
				zDirection = -xDirection;
				xDirection = 0;
				
				currentX+=xDirection;
				currentZ+=zDirection;

				bool found = false;
				while(!found)
				{
					if(currentX >= 0 && currentX < sideLength && currentZ >= 0 && currentZ < sideLength)
					{
						//if(result[(sideLength * currentZ) + currentX])
						if(!isAvailable(currentX, currentZ))
						{
							currentX+=xDirection;
							currentZ+=zDirection;
						}
						else
						{
							found = true;
						}
					}
					else
					{
						xDirection = -zDirection;
						zDirection = 0;
					}

				}
			}
		}
	}

	return mMCUs;
}

bool TerrainManager::isOccupied(int x, int z)
{
	std::deque<MCUPlacement>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter).X == x && (*iter).Z == z)
			return true;
	}
	return false;
}

bool TerrainManager::isAvailable(int x, int z)
{
	if(isOccupied(x, z))
		return false;

	if(mBattlefieldGrid->getGridCell(x,z)->isPassable())
		return true;

	return false;
}

float TerrainManager::calculateDistance(int x1, int z1, int x2, int z2)
{
	return sqrt((float)((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1)));
}

float TerrainManager::calculateSquareDistance(int x1, int z1, int x2, int z2)
{
	return (float)((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1));
}

TerrainManager* TerrainManager::mTerrainManager;

TerrainManager* TerrainManager::getSingletonPtr()
{
	if(!mTerrainManager)
	{
		mTerrainManager = new TerrainManager();
	}
	return mTerrainManager;
}