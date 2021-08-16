#include <Entities/BattlefieldGrid.h>
#include <Ogre.h>
#include <Controllers/MCUController.h>
#include <Entities/BattlefieldGridCell.h>
#include <Entities/Enums.h>
#include <Entities/GridSquare.h>
#include <Entities/GridSquareTemplateVector.h>
#include <Entities/GridSquareVector.h>
#include <Entities/Mission.h>
#include <Managers/GameManager.h>
#include <Managers/SQLiteDBManager.h>
#include <LuaBindings/LuaMCUDeque.h>
#include <StateChange/PlayerPreBattleState.h>
#include <AI/AIGroup.h>
#include <Templates/TerrainTemplate.h>
#include <hash_map>
#include <vector>

using namespace Ogre;

const float BattlefieldGrid::UnitSize = 10.f;

BattlefieldGrid::BattlefieldGrid()
	:
	mSceneManager(GameManager::getSingletonPtr()->getSceneManager()),
	mMaxHeight(0),
	mHeightDataLength(0),
	mHeightDataSideLength(0),
	mMinVertexHeight(10000.f),
	mMaxVertexHeight(-10.f),
	mMaxRoughness(0.f),
	mSearchCurrentX(0),
	mSearchCurrentZ(0),
	mSearchXDirection(0),
	mSearchZDirection(0),
	mTerrainTemplate(0),
	mBattlefield(0),
	mGridSquareTemplateVector(0),
	mMission(0)
{

}

BattlefieldGrid::~BattlefieldGrid()
{

	std::vector<BattlefieldGridCell*>::iterator cells;
	for(cells = mGridCells.begin(); cells != mGridCells.end(); ++cells)
	{
		delete (*cells);
	}
}

ManualObject* BattlefieldGrid::getBattlefield()
{
	if(!mBattlefield)
	{
		GridSquareVector* vector = new GridSquareVector(this);
		mBattlefield = vector->getBattlefield();
		delete vector;
	}
	return mBattlefield;
}

void BattlefieldGrid::setBattlefield(ManualObject* manual)
{
	mBattlefield = manual;
}

TerrainTemplate* BattlefieldGrid::getTerrainTemplate()
{
	return mTerrainTemplate;
}

void BattlefieldGrid::setTerrainTemplate(TerrainTemplate* terrain)
{
	mTerrainTemplate = terrain;
}

int BattlefieldGrid::getSideLength()
{
	return mHeightDataSideLength;
}

void BattlefieldGrid::setSideLength(int length)
{
	mHeightDataSideLength = length;
	mHeightDataLength = mHeightDataSideLength * mHeightDataSideLength;
	
	mGridCells.resize(mHeightDataLength);
}


float BattlefieldGrid::getMaxRoughness()
{
	return mMaxRoughness;
}

void BattlefieldGrid::setMaxRoughness(float value)
{
	mMaxRoughness = value;
}

float BattlefieldGrid::getMaxVertexHeight()
{
	return mMaxVertexHeight;
}

void BattlefieldGrid::setMaxVertexHeight(float value)
{
	mMaxVertexHeight = value;
}

BattlefieldGridCell* BattlefieldGrid::getGridCell(int index)
{
	if(index > mHeightDataLength)
		int temp = 1;
	return mGridCells[index];
}

BattlefieldGridCell* BattlefieldGrid::getGridCell(int x, int z)
{
	if((mHeightDataSideLength * z) + x > mHeightDataLength)
		int temp = 1;
	return mGridCells[(mHeightDataSideLength * z) + x];
}

BattlefieldGridCell* BattlefieldGrid::getGridCell(float x, float z)
{
	int xLoc = x / UnitSize;
	int zLoc = z / UnitSize;

	if(xLoc >= mHeightDataSideLength || zLoc >= mHeightDataSideLength)
		int temp = 0;

	return mGridCells[(mHeightDataSideLength * zLoc) + xLoc];
}

void BattlefieldGrid::seedEmptyGridCellSearch(int x, int z)
{
	mSearchCurrentX = x;
	mSearchCurrentZ = z;
	mSearchXDirection = 0;
	mSearchZDirection = -2;
	if(!getGridCell(mSearchCurrentX, mSearchCurrentZ)->isAvailable())
	{
		getNextEmptyGridCell();
	}
}

BattlefieldGridCell* BattlefieldGrid::getNextEmptyGridCell()
{
	//wont change direction without remembering state between calls. Build up collection of MCU placements anyway?
	BattlefieldGridCell* result(0);

	if(mSearchCurrentX >= 0 && mSearchCurrentX < mHeightDataSideLength && mSearchCurrentZ >= 0 && mSearchCurrentZ < mHeightDataSideLength)
	{
		if(getGridCell(mSearchCurrentX, mSearchCurrentZ)->isAvailable())
		{
			result = getGridCell(mSearchCurrentX, mSearchCurrentZ);
		}
	}

	mSearchCurrentX+= mSearchXDirection;
	mSearchCurrentZ+= mSearchZDirection;
	if(mSearchCurrentX < 0)
	{
		mSearchCurrentX = 0;
		if(mSearchXDirection != 0)
		{
			mSearchXDirection = 2;
			mSearchZDirection = 0;
		}
	}
	if(mSearchCurrentX  >= mHeightDataSideLength)
	{
		mSearchCurrentX = mHeightDataSideLength - 1;
		if(mSearchXDirection != 0)
		{
			mSearchXDirection = -2;
			mSearchZDirection = 0;
		}
	}
	if(mSearchCurrentZ < 0)
	{
		mSearchCurrentZ = 0;
		if(mSearchZDirection != 0)
		{
			mSearchXDirection = 0;
			mSearchZDirection = 2;
		}
	}
	if(mSearchCurrentZ >= mHeightDataSideLength)
	{
		mSearchCurrentZ = mHeightDataSideLength - 1;
		if(mSearchZDirection != 0)
		{
			mSearchXDirection = 0;
			mSearchZDirection = -2;
		}
	}

	if(mSearchZDirection != 0)
	{
		if(mSearchCurrentX - mSearchZDirection >= 0 && mSearchCurrentX - mSearchZDirection < mHeightDataSideLength)
		{
			//can we turn?
			//if(!result[(mHeightDataSideLength * mSearchCurrentZ) + mSearchCurrentX - mSearchZDirection])
			if(getGridCell(mSearchCurrentX - mSearchZDirection, mSearchCurrentZ)->isAvailable())
			{
				mSearchXDirection = -mSearchZDirection;
				mSearchZDirection = 0;
			}
			//carry on straight
		}
		else
		//if we are about to go out of bounds
		if(mSearchCurrentZ + mSearchZDirection < 0 || mSearchCurrentZ + mSearchZDirection >= mHeightDataSideLength)
		{
			mSearchXDirection = -mSearchZDirection;
			mSearchZDirection = 0;
				
			mSearchCurrentX+=mSearchXDirection;
			mSearchCurrentZ+=mSearchZDirection;
			bool found = false;
			while(!found)
			{
				if(mSearchCurrentX >= 0 && mSearchCurrentX < mHeightDataSideLength && mSearchCurrentZ >= 0 && mSearchCurrentZ < mHeightDataSideLength)
				{
					//if(result[(mHeightDataSideLength * mSearchCurrentZ) + mSearchCurrentX])
					if(!getGridCell(mSearchCurrentX, mSearchCurrentZ)->isAvailable())
					{
						mSearchCurrentX+=mSearchXDirection;
						mSearchCurrentZ+=mSearchZDirection;
					}
					else
					{
						found = true;
					}
				}
				else
				{
					mSearchZDirection = -mSearchXDirection;
					mSearchXDirection = 0;
				}
			}
		}
	}
	else
	if(mSearchXDirection != 0)
	{
		if(mSearchCurrentZ - mSearchXDirection >= 0 && mSearchCurrentZ - mSearchXDirection < mHeightDataSideLength)
		{
			//can we turn?
			//if(!result[(mHeightDataSideLength * mSearchCurrentZ) + mSearchCurrentX - mSearchXDirection])
			if(getGridCell(mSearchCurrentX, mSearchCurrentZ - mSearchXDirection)->isAvailable())
			{
				mSearchZDirection = -mSearchXDirection;
				mSearchXDirection = 0;
			}
			//carry on straight
		}
		else
		//if we are about to go out of bounds
		if(mSearchCurrentX + mSearchXDirection < 0 || mSearchCurrentX + mSearchXDirection >= mHeightDataSideLength)
		{
			mSearchZDirection = -mSearchXDirection;
			mSearchXDirection = 0;
				
			mSearchCurrentX+=mSearchXDirection;
			mSearchCurrentZ+=mSearchZDirection;

			bool found = false;
			while(!found)
			{
				if(mSearchCurrentX >= 0 && mSearchCurrentX < mHeightDataSideLength && mSearchCurrentZ >= 0 && mSearchCurrentZ < mHeightDataSideLength)
				{
					//if(result[(mHeightDataSideLength * mSearchCurrentZ) + mSearchCurrentX])
					if(!getGridCell(mSearchCurrentX, mSearchCurrentZ)->isAvailable())
					{
						mSearchCurrentX+=mSearchXDirection;
						mSearchCurrentZ+=mSearchZDirection;
					}
					else
					{
						found = true;
					}
				}
				else
				{
					mSearchXDirection = -mSearchZDirection;
					mSearchZDirection = 0;
				}

			}
		}
	}
	
	return result;
}

BattlefieldGridCell* BattlefieldGrid::getCloserGridCell(int x1, int z1, int x2, int z2, AIGroup* group)
{
	return getCloserGridCell(x1, z2, x2, z2, group->getShortWeaponRange());
}

BattlefieldGridCell* BattlefieldGrid::getCloserGridCell(int x1, int z1, int x2, int z2, MCUController* controller)
{
	return getCloserGridCell(x1, z1, x2, z2, controller->getShortestWeaponRange());
}

BattlefieldGridCell* BattlefieldGrid::getCloserGridCell(int x1, int z1, int x2, int z2, float range)
{
	Vector3 direction = Vector3(x2, 0, z2) - Vector3(x1, 0, z1);
	Real length = direction.normalise();
	
	if(length < range)
		return getGridCell(x1, z1);

	length-= range;
	Vector3 end = Vector3(x1, 0, z1) + direction * length;
	return getGridCell(end.x, end.z);
}

void BattlefieldGrid::reset()
{
	std::vector<BattlefieldGridCell*>::iterator cells;
	for(cells = mGridCells.begin(); cells != mGridCells.end(); ++cells)
	{
		(*cells)->setMarkedToBeOccupied(false);
		(*cells)->occupyCell(0);
	}
}


void BattlefieldGrid::setGridCell(int index, BattlefieldGridCell* cell)
{
	mGridCells[index] = cell;
}

GridSquareTemplateVector* BattlefieldGrid::getGridSquareTemplateVector()
{
	return mGridSquareTemplateVector;
}

void BattlefieldGrid::setGridSquareTemplateVector(GridSquareTemplateVector* vector)
{
	mGridSquareTemplateVector = vector;
}

bool BattlefieldGrid::isCentre(int x, int z)
{
	if(mHeightDataSideLength % 2 == 0)
		return false;
	if(x == (mHeightDataSideLength / 2) && (z == mHeightDataSideLength / 2))
		return true;
	
	return false;
}

float BattlefieldGrid::getSquareDistance(BattlefieldGridCell* a, BattlefieldGridCell* b)
{
	return (float)((a->getX() - b->getX()) * (a->getX() - b->getX()) + (a->getZ() - b->getZ()) * (a->getZ() - b->getZ()));
}

Mission* BattlefieldGrid::getMission()
{
	return mMission;
}

void BattlefieldGrid::setMission(Mission* mission)
{
	mMission = mission;
}

bool BattlefieldGrid::isAvailable(int x, int z)
{
	if(isOccupied(x,z))
		return false;

	if(isPassable(x,z))
		return true;

	return false;
}

bool BattlefieldGrid::isPassable(int x, int z)
{
	return getGridCell(x, z)->isPassable();
}

bool BattlefieldGrid::isOccupied(int x, int z)
{
	return mMission->isCellOccupied(x, z);
	//return getGridCell(x,z)->isOccupied();
}

void BattlefieldGrid::placeMCUs(LuaMCUDeque* deque, int x, int z)
{
	if(x < 0 || x >= mHeightDataSideLength || z < 0 || z >= mHeightDataSideLength)
		return;

	int currentX = x;
	int currentZ = z;
	int xDirection = 0;
	int zDirection = -2;

	 
	deque->reset();
	while(deque->hasMoreItems())
	{
		
		MCUPlacement* mcu = deque->getNext();
		while(!mcu->Placed)
		{
			if(currentX >= 0 && currentX < mHeightDataSideLength && currentZ >= 0 && currentZ < mHeightDataSideLength)
			{
				if(!isOccupied(currentX, currentZ) && isPassable(currentX, currentZ))
				{
					mcu->X = currentX;
					mcu->Z = currentZ;
					mcu->Placed = true;
					break;
				}
			}

			currentX+= xDirection;
			currentZ+= zDirection;

			//are we already out of bounds
			if(currentX < 0)
			{
				currentX = 0;
				if(xDirection != 0)
				{
					xDirection = 2;
					zDirection = 0;
				}
			}
			if(currentX  >= mHeightDataSideLength)
			{
				currentX = mHeightDataSideLength - 1;
				if(xDirection != 0)
				{
					xDirection = -2;
					zDirection = 0;
				}
			}
			if(currentZ < 0)
			{
				currentZ = 0;
				if(zDirection != 0)
				{
					xDirection = 0;
					zDirection = 2;
				}
			}
			if(currentZ >= mHeightDataSideLength)
			{
				currentZ = mHeightDataSideLength - 1;
				if(zDirection != 0)
				{
					xDirection = 0;
					zDirection = -2;
				}
			}
			if(zDirection != 0)
			{
				if(currentX - zDirection >= 0 && currentX - zDirection < mHeightDataSideLength)
				{
					//can we turn?
					//if(!result[(mHeightDataSideLength * currentZ) + currentX - zDirection])
					if(isAvailable(currentX - zDirection, currentZ))
					{
						xDirection = -zDirection;
						zDirection = 0;
					}
					//carry on straight
				}
				else
				//if we are about to go out of bounds
				if(currentZ + zDirection < 0 || currentZ + zDirection >= mHeightDataSideLength)
				{
					xDirection = -zDirection;
					zDirection = 0;
				
					currentX+=xDirection;
					currentZ+=zDirection;
					bool found = false;
					while(!found)
					{
						if(currentX >= 0 && currentX < mHeightDataSideLength && currentZ >= 0 && currentZ < mHeightDataSideLength)
						{
							//if(result[(mHeightDataSideLength * currentZ) + currentX])
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
				if(currentZ - xDirection >= 0 && currentZ - xDirection < mHeightDataSideLength)
				{
					//can we turn?
					//if(!result[(mHeightDataSideLength * currentZ) + currentX - xDirection])
					if(isAvailable(currentX, currentZ - xDirection))
					{
						zDirection = -xDirection;
						xDirection = 0;
					}
					//carry on straight
				}
				else
				//if we are about to go out of bounds
				if(currentX + xDirection < 0 || currentX + xDirection >= mHeightDataSideLength)
				{
					zDirection = -xDirection;
					xDirection = 0;
				
					currentX+=xDirection;
					currentZ+=zDirection;

					bool found = false;
					while(!found)
					{
						if(currentX >= 0 && currentX < mHeightDataSideLength && currentZ >= 0 && currentZ < mHeightDataSideLength)
						{
							//if(result[(mHeightDataSideLength * currentZ) + currentX])
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
	}

}

float BattlefieldGrid::calculateDistance(int x1, int z1, int x2, int z2)
{
	return sqrt((float)((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1)));
}

float BattlefieldGrid::calculateSquareDistance(int x1, int z1, int x2, int z2)
{
	return (float)((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1));
}
BattlefieldGrid* BattlefieldGrid::createBattlefieldGrid(GridSquareTemplateVector* templateVector, TerrainTemplate* terrain)
{
	BattlefieldGrid* grid = new BattlefieldGrid();
	grid->setTerrainTemplate(terrain);
	grid->setSideLength(templateVector->getSideLength() - 1);
	grid->setGridSquareTemplateVector(templateVector);

	GridSquareVector* vector = new GridSquareVector(grid);
	grid->setBattlefield(vector->getBattlefield());

	delete vector;
	return grid;
}

