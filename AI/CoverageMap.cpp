#include <AI/CoverageMap.h>
#include <Controllers/MCUController.h>
#include <Entities/FogSquare.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <States/InBattleState.h>
#include <deque>

CoverageMap::CoverageMap(unsigned int sideLength, unsigned int size, PlayerBattleState* state)
	:
	mSideLength(sideLength),
	mSize(size),
	mCoverageStatusMap(mSize),
	mGrid(InBattleState::getSingletonPtr()->getBattlefieldGrid()),
	mPlayerBattleState(state),
	mFirstXVisible(mSideLength, mSideLength),
	mFirstZVisible(mSideLength, mSideLength),
	mLastZVisible(mSideLength, 0)
{
	
}

CoverageMap::~CoverageMap()
{
	
}

void CoverageMap::resetCoverageMap()
{
	mCoverageStatusMap = std::vector<CoverageSquare>(mSize);

	for(unsigned int i = 0; i < mSideLength; ++i)
	{
		mFirstXVisible[i] = mSideLength;
		mFirstZVisible[i] = mSideLength;
		mLastZVisible[i] = 0;
	}
	mSightingHotSpots.clear();
	mNonSightingHotSpots.clear();
}

void CoverageMap::updateCoverageMap()
{
	
}

void CoverageMap::clearSightings()
{
	mEnemySightings.clear();
}

void CoverageMap::setVisibility(FogSquareGrid* grid)
{
	while(grid->hasMoreFogSquares())
	{
		FogSquare* square = grid->getNext();
		if(square->X < mSideLength && square->Z < mSideLength)
		{
			BattlefieldGridCell* cell = mGrid->getGridCell(square->X, square->Z);

			mCoverageStatusMap[(mSideLength * square->Z) + square->X].Status = CoverageStatus::CS_VISIBLE_EMPTY;
			mCoverageStatusMap[(mSideLength * square->Z) + square->X].Interest = 0;
		
			if(mFirstXVisible[square->Z] > square->X)
				mFirstXVisible[square->Z] = square->X;

			if(mFirstZVisible[square->X] > square->Z)
				mFirstZVisible[square->X] = square->Z;

			if(mLastZVisible[square->X] < square->Z)
				mLastZVisible[square->X] = square->Z;
		}
	}
	grid->reset();
}

void CoverageMap::computeCoverageMap()
{
	std::deque<HotSpot>::iterator sighting;
	for(sighting = mEnemySightings.begin(); sighting != mEnemySightings.end(); ++sighting)
	{
		mCoverageStatusMap[((*sighting).Z * mSideLength) + (*sighting).X].Status = CoverageStatus::CS_VISIBLE_LAST_SIGHTING;
	}
	mEnemySightings.clear();

	int lastInterest = 1;
	int direction = 1;
	int x(0), z(0);
	int maxInterestX(0);
	int maxInterestZ(0);
	
	for(unsigned int i = 0; i < mSize; ++i)
	{
		x = i % mSideLength;
		z = i / mSideLength;
		
		if(z < mFirstZVisible[x])
		{
			maxInterestZ = mFirstZVisible[x] / 2;
		}
		else if(z > mLastZVisible[x])
		{
			maxInterestZ = (mSideLength - mLastZVisible[x]) / 2;
		}

		if(x == 0)
		{
			maxInterestX = mFirstXVisible[z] / 2;
			lastInterest = 1;
			direction = 1;
		}

		if(mCoverageStatusMap[i].Status == CoverageStatus::CS_VISIBLE_EMPTY)
		{
			lastInterest = 1;
			direction = 1;
			mCoverageStatusMap[i].Interest = 0;
			maxInterestX = (mSideLength - x) / 2;
		}
		else
		{
			if(mCoverageStatusMap[i].Status != CoverageStatus::CS_VISIBLE_LAST_SIGHTING)
			{
				if(lastInterest >= std::min(maxInterestX, maxInterestZ))
				{
					direction = -1;
					mNonSightingHotSpots.push_back(HotSpot(x, z));
				}
				else if(lastInterest <= 0)
				{
					direction = 1;
				}

				lastInterest+= direction;
				
				mCoverageStatusMap[i].Interest = lastInterest;
			}
			else
			{
				lastInterest = std::min(maxInterestX, maxInterestZ);
				mCoverageStatusMap[i].Interest = lastInterest;
				mSightingHotSpots.push_back(HotSpot(x, z));
				direction = -1;
			}
		}
	}
	
}

void CoverageMap::addEnemySighting(int x, int z)
{
	mEnemySightings.push_back(HotSpot(x, z));
}

void CoverageMap::addEnemySighting(BattlefieldGridCell* location)
{
	addEnemySighting(location->getX(), location->getZ());
}

std::deque<HotSpot> CoverageMap::getEnemySightings()
{
	return mEnemySightings;
}

void CoverageMap::removeEnemySighting(const HotSpot& hotspot)
{
	std::deque<HotSpot>::iterator iter;
	for(iter = mEnemySightings.begin(); iter != mEnemySightings.end(); ++iter)
	{
		if((*iter).X == hotspot.X && (*iter).Z == hotspot.Z)
		{
			mEnemySightings.erase(iter);
			return;
		}
	}
}

BattlefieldGridCell* CoverageMap::getNearestHotSpot(BattlefieldGridCell* location)
{
	float currrentMinSquareDistance(1000000.f);

	BattlefieldGrid* grid = InBattleState::getSingletonPtr()->getBattlefieldGrid();

	BattlefieldGridCell* result(0);
	std::deque<HotSpot>::iterator iter;
	if(!mSightingHotSpots.empty())
	{
		for(iter = mSightingHotSpots.begin(); iter != mSightingHotSpots.end(); ++iter)
		{
			float squareDistance = grid->getSquareDistance(location, grid->getGridCell((*iter).X, (*iter).Z));
			if(squareDistance < currrentMinSquareDistance)
			{
				squareDistance = currrentMinSquareDistance;
				result = grid->getGridCell((*iter).X, (*iter).Z);
			}
		}
	}
	if(mSightingHotSpots.empty() && !mNonSightingHotSpots.empty())
	{
		for(iter = mNonSightingHotSpots.begin(); iter != mNonSightingHotSpots.end(); ++iter)
		{
			float squareDistance = grid->getSquareDistance(location, grid->getGridCell((*iter).X, (*iter).Z));
			if(squareDistance < currrentMinSquareDistance)
			{
				squareDistance = currrentMinSquareDistance;
				result = grid->getGridCell((*iter).X, (*iter).Z);
			}
		}
	}
	if(!result)
		int temp = 0;

	return result;
}

BattlefieldGridCell* CoverageMap::getNearestHotSpot(int x, int z)
{
	BattlefieldGrid* grid = InBattleState::getSingletonPtr()->getBattlefieldGrid();
	return getNearestHotSpot(grid->getGridCell(x, z));
}
