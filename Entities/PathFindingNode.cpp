#include <Entities/PathFindingNode.h>
#include <Entities/BattlefieldGridCell.h>
#include <math.h>

PathFindingNode::PathFindingNode()
{
	mMovementCost = 0;
	mEstimatedCostToGoal = 0;
	mTotalMovementCost = 0;
	mOccupied = false;
	mIsStart = false;
	mX = 0;
	mZ = 0;
	mParent = 0;
}

PathFindingNode::PathFindingNode(BattlefieldGridCell* actual, bool isStart)
{
	mActual = actual;
	mMovementCost = mActual->getMovementCost();
	mEstimatedCostToGoal = 0;
	mTotalMovementCost = 0;
	mParent = 0;
	mOccupied = mActual->isOccupied();
	mIsStart = isStart;
	if(mOccupied && !mIsStart)
		mMovementCost = 999999.f;
	

	mX = mActual->getX();
	mZ = mActual->getZ();
}

PathFindingNode::PathFindingNode(PathFindingNode* parent, float totalMovementCost, BattlefieldGridCell* actual, bool isStart)
{
	mParent = parent;
	mActual = actual;
	mMovementCost = mActual->getMovementCost();
	mTotalMovementCost = totalMovementCost;
	mOccupied = mActual->isOccupied();
	mIsStart = isStart;
	if(mOccupied && !mIsStart)
		mMovementCost = 999999.f;
	mX = mActual->getX();
	mZ = mActual->getZ();
}

PathFindingNode::~PathFindingNode()
{

}

PathFindingNode* PathFindingNode::getParent()
{
	return mParent;
}

void PathFindingNode::setParent(PathFindingNode* parent)
{
	mParent = parent;
}

float PathFindingNode::getMovementCost()
{
	return mMovementCost;
}

void PathFindingNode::setMovementCost(float movement)
{
	mMovementCost = movement;
}

float PathFindingNode::getTotalMovementCost()
{
	return mTotalMovementCost;
}

void PathFindingNode::setTotalMovementCost(float total)
{
	mTotalMovementCost = total;
	mWholeCost = mEstimatedCostToGoal + mTotalMovementCost;
}


int PathFindingNode::getX()
{
	return mX;
}

void PathFindingNode::setX(int x)
{
	mX = x;
}

int PathFindingNode::getZ()
{
	return mZ;
}

void PathFindingNode::setZ(int z)
{
	mZ = z;
}

float PathFindingNode::getEstimatedCostToGoal()
{
	return mEstimatedCostToGoal;
}

void PathFindingNode::setEstimatedCostToGoal(float value)
{
	mEstimatedCostToGoal = value;
	mWholeCost = mEstimatedCostToGoal + mTotalMovementCost;
}

float PathFindingNode::getWholePathCost()
{
	return mTotalMovementCost + mEstimatedCostToGoal;
}

BattlefieldGridCell* PathFindingNode::getActual()
{
	return mActual;
}

void PathFindingNode::setActual(BattlefieldGridCell* actual)
{
	mActual = actual;
	mMovementCost = mActual->getMovementCost();
	mX = mActual->getX();
	mZ = mActual->getZ();
}

bool PathFindingNode::isDiagonal(PathFindingNode* neighbour)
{
	if(abs(mX - neighbour->getX()) == 1 && abs(mZ - neighbour->getZ()) == 1)
		return true;

	return false;
}

bool PathFindingNode::isSloped(PathFindingNode* neighbour)
{
	if(	mActual->getSquareType() == SquareType::SLOPED_NEG_X ||
		mActual->getSquareType() == SquareType::SLOPED_POS_X || 
		mActual->getSquareType() == SquareType::SLOPED_NEG_Z || 
		mActual->getSquareType() == SquareType::SLOPED_POS_Z ||
		mActual->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z ||
		mActual->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || 
		mActual->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || 
		mActual->getSquareType() == SquareType::CORNER_POS_X_POS_Z)
		return true;

	if(	neighbour->getActual()->getSquareType() == SquareType::SLOPED_NEG_X ||
		neighbour->getActual()->getSquareType() == SquareType::SLOPED_POS_X || 
		neighbour->getActual()->getSquareType() == SquareType::SLOPED_NEG_Z || 
		neighbour->getActual()->getSquareType() == SquareType::SLOPED_POS_Z ||
		neighbour->getActual()->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z ||
		neighbour->getActual()->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || 
		neighbour->getActual()->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || 
		neighbour->getActual()->getSquareType() == SquareType::CORNER_POS_X_POS_Z)
		return true;

	return false;

}

bool PathFindingNode::isCliff(PathFindingNode* neighbour)
{
	if(mActual->getX() > neighbour->getActual()->getX() && mActual->getZ() == neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CLIFF_NEG_X || mActual->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || mActual->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CLIFF_POS_X || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
	}
	else if(mActual->getX() < neighbour->getActual()->getX() && mActual->getZ() == neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CLIFF_POS_X || mActual->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z || mActual->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CLIFF_NEG_X || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z)
				return true;
		}
	}
	else if(mActual->getX() == neighbour->getActual()->getX() && mActual->getZ() > neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CLIFF_NEG_Z || mActual->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || mActual->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CLIFF_POS_Z || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
	}
	else if(mActual->getX() == neighbour->getActual()->getX() && mActual->getZ() < neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CLIFF_POS_Z || mActual->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z || mActual->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CLIFF_NEG_Z || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z)
				return true;
		}
	}
	else if(mActual->getX() > neighbour->getActual()->getX() && mActual->getZ() > neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
	}
	else if(mActual->getX() < neighbour->getActual()->getX() && mActual->getZ() > neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z)
				return true;
		}
	}
	else if(mActual->getX() < neighbour->getActual()->getX() && mActual->getZ() < neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z)
				return true;
		}
	}
	else if(mActual->getX() > neighbour->getActual()->getX() && mActual->getZ() < neighbour->getZ())
	{
		if(mActual->getY() > neighbour->getActual()->getY())
		{
			if(mActual->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
		else if(mActual->getY() < neighbour->getActual()->getY())
		{
			if(neighbour->getActual()->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z)
				return true;
		}
	}
	
	return false;
}
