#include <Managers/PathFindingManager.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <Entities/PathFindingNode.h>
#include <Templates/DriveTemplate.h>

#include <hash_map>
#include <Ogre.h>
#include <math.h>

using namespace Ogre;

PathFindingManager::PathFindingManager(BattlefieldGrid* grid)
{
	mGrid = grid;
	mSideLength = mGrid->getSideLength();
	
	mMaxRoughness = mGrid->getMaxRoughness();
	mAll.resize(mSideLength * mSideLength);
	mClosed.resize(mSideLength * mSideLength);
	mOpen.resize(mSideLength * mSideLength);

}

PathFindingManager::~PathFindingManager() 
{

	
}


PathFindingNode* PathFindingManager::findPath(BattlefieldGridCell* start, BattlefieldGridCell* end, DriveTemplate* drive)
{
	for(int i =0; i < mSideLength * mSideLength;++i)
	{
		BattlefieldGridCell* current = mGrid->getGridCell(i);
		
		mAll[i] = new PathFindingNode(current, current == start);
		
	}
	
	PathFindingNode* begin = mAll[(start->getZ() * mSideLength) + start->getX()];
	PathFindingNode* finish = mAll[(end->getZ() * mSideLength) + end->getX()];
	begin->setTotalMovementCost(begin->getMovementCost());
	begin->setEstimatedCostToGoal(estimateCostToGoal(begin,finish));

	mOpen[(begin->getZ() * mSideLength) + begin->getX()] = begin;
	PathFindingNode* current = 0;
	float cost = 0;
	float neighbourMovementCost = 0;
	std::vector<PathFindingNode*> neighbours;
	std::vector<PathFindingNode*>::iterator iter;

	while(findLowest(mOpen) != finish)
	{
		current = removeLowest(mOpen);
		mClosed[(current->getZ() * mSideLength) + current->getX()] = current;
		
		neighbours = getNeighbours(current);

		for(iter = neighbours.begin();iter != neighbours.end();++iter)
		{
			if(current->isDiagonal((*iter)))
			{
				neighbourMovementCost = (*iter)->getMovementCost() * 1.414;
			}
			else
			{
				neighbourMovementCost = (*iter)->getMovementCost();
			}

			if(current->isSloped((*iter)))
			{
				neighbourMovementCost = neighbourMovementCost * 1.414;
			}

			if(current->isCliff((*iter)))
			{
				neighbourMovementCost = neighbourMovementCost * drive->getJumpingEfficiency();
			}

			cost = current->getTotalMovementCost() + neighbourMovementCost;

			if(mOpen[((*iter)->getZ() * mSideLength) + (*iter)->getX()])
			{
				if(cost < (*iter)->getTotalMovementCost())
				{
					mOpen[((*iter)->getZ() * mSideLength) + (*iter)->getX()] = 0;
				}
			}
			
			if(mClosed[((*iter)->getZ() * mSideLength) + (*iter)->getX()])
			{
				if(cost < (*iter)->getTotalMovementCost())
				{
					mClosed[((*iter)->getZ() * mSideLength) + (*iter)->getX()] = 0;
				}
			}

			if(!mOpen[((*iter)->getZ() * mSideLength) + (*iter)->getX()] && !mClosed[((*iter)->getZ() * mSideLength) + (*iter)->getX()])
			{
				(*iter)->setTotalMovementCost(cost);
				(*iter)->setEstimatedCostToGoal(estimateCostToGoal((*iter),finish));
				(*iter)->setParent(current);
				mOpen[((*iter)->getZ() * mSideLength) + (*iter)->getX()] = (*iter);
			}

		}
	}
	resetAll();

	return finish;
}

PathFindingNode* PathFindingManager::findLowest(std::vector<PathFindingNode*> &set)
{
	std::vector<PathFindingNode*>::iterator iter = set.begin();

	while((*iter) == 0)
	{
		++iter;
	}

	PathFindingNode* current = (*iter);

	while(iter != set.end())
	{
		if((*iter) != 0)
		{
			if((*iter)->getWholePathCost() < current->getWholePathCost())
				current = (*iter);
		}
		++iter;
	}

	return current;
}

PathFindingNode* PathFindingManager::removeLowest(std::vector< PathFindingNode*> &set)
{
	std::vector< PathFindingNode*>::iterator iter = set.begin();

	while((*iter) == 0)
	{
		++iter;
	}

	PathFindingNode* current = (*iter);

	while(iter != set.end())
	{
		if((*iter) != 0)
		{
			if((*iter)->getWholePathCost() < current->getWholePathCost())
				current = (*iter);
		}
		++iter;
	}
	
	set[(current->getZ() * mSideLength) + current->getX()] = 0;

	return current;
}
std::vector<PathFindingNode*> PathFindingManager::getNeighbours(PathFindingNode* search)
{
	std::vector<PathFindingNode*> result;

	if(search->getX() > 0)
	{
		result.push_back(mAll[(search->getZ() * mSideLength) + ( search->getX() - 1)]);
		
		if(search->getZ() > 0)
			result.push_back(mAll[((search->getZ() - 1) * mSideLength) + search->getX() - 1]);

		if(search->getZ() < (mSideLength - 1))
			result.push_back(mAll[((search->getZ() + 1) * mSideLength) + search->getX() - 1]);

	}
	if(search->getZ() > 0)
	{
		result.push_back(mAll[((search->getZ() - 1) * mSideLength) + search->getX()]);

		if(search->getX() < (mSideLength - 1))
			result.push_back(mAll[((search->getZ() - 1) * mSideLength) + search->getX() + 1]);
	}

	if(search->getX() < (mSideLength - 1))
	{
		result.push_back(mAll[(search->getZ() * mSideLength) + search->getX() + 1]);

		if(search->getZ() < (mSideLength - 1))
			result.push_back(mAll[((search->getZ() + 1) * mSideLength) + search->getX() + 1]);
	}

	if(search->getZ() < (mSideLength - 1))
	{
		result.push_back(mAll[((search->getZ() + 1) * mSideLength) + search->getX()]);
	}

	return result;
}

float PathFindingManager::estimateCostToGoal(PathFindingNode* current, PathFindingNode* goal)
{
	/*
	h_diagonal(n) = min(abs(n.x-goal.x), abs(n.y-goal.y))
	h_straight(n) = (abs(n.x-goal.x) + abs(n.y-goal.y))
	h(n) = D2 * h_diagonal(n) + D * (h_straight(n) - 2*h_diagonal(n)))
	*/
	float diagonal = std::min(abs(goal->getX() - current->getX()), abs(goal->getZ() - current->getZ()));
	float straight = abs(goal->getX() - current->getX()) + abs(goal->getZ() - current->getZ());
		
	return (mMaxRoughness * 1.414f * diagonal) + (mMaxRoughness * (straight - 2 * diagonal));

}

void PathFindingManager::resetAll()
{
	std::vector< PathFindingNode*>::iterator iter;

	for(iter = mAll.begin(); iter != mAll.end();++iter)
	{
		
		if((*iter))
		{
			if(mOpen[((*iter)->getZ() * mSideLength) + (*iter)->getX()] == 0 && mClosed[((*iter)->getZ() * mSideLength) + (*iter)->getX()] == 0)
				delete (*iter);
		}

	}

	mOpen.clear();
	mClosed.clear();
	mAll.clear();

	mAll.resize(mSideLength * mSideLength);
	mClosed.resize(mSideLength * mSideLength);
	mOpen.resize(mSideLength * mSideLength);
}
