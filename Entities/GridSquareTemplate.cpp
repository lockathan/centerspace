#include <Entities/GridSquareTemplate.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/Enums.h>
#include <Entities/GridSquare.h>
#include <Entities/GridSquareJoinTemplate.h>
#include <Entities/GridSquareTemplateVector.h>
#include <Entities/GridSquareVector.h>
#include <Managers/GameManager.h>
#include <Managers/SQLiteDBManager.h>
#include <Templates/TerrainTemplate.h>
#include <Ogre.h>
#include <vector>
#include <deque>


using namespace Ogre;

GridSquareTemplate::GridSquareTemplate(GridSquareTemplateVector* parent)
	:
	mX(0),
	mY(0),
	mZ(0),
	mType(SquareType::FLAT),
	mRaised(false),
	mChange(TerrainChange::TC_NOT_SET),
	mParent(parent)
{


}

GridSquareTemplate::GridSquareTemplate(int x, int y, int z, SquareType type, GridSquareTemplateVector* parent)
	:
	mX(x),
	mY(y),
	mZ(z),
	mType(type),
	mRaised(false),
	mChange(TerrainChange::TC_NOT_SET),
	mParent(parent)
{

}

GridSquareTemplate::~GridSquareTemplate()
{

}

int GridSquareTemplate::getX()
{
	return mX;
}

int GridSquareTemplate::getY()
{
	return mY;
}

int GridSquareTemplate::getZ()
{
	return mZ;
}

void GridSquareTemplate::setY(int y)
{
	mY = y;
	
}

bool GridSquareTemplate::wasRaised()
{
	return mRaised;
}
void GridSquareTemplate::setRaised(bool value)
{
	mRaised = value;
}

SquareType GridSquareTemplate::getSquareType()
{
	return mType;
}

void GridSquareTemplate::setSquareType(SquareType type)
{
	mType = type;
	
}

TerrainChange GridSquareTemplate::getTerrainChange()
{
	return mChange;
}

void GridSquareTemplate::setTerrainChange(TerrainChange change)
{
	mChange = change;
}

bool GridSquareTemplate::isNegXJoinValid(GridSquareTemplate* neighbour)
{
	int heightDiff = this->getY() - neighbour->getY();
	if(Math::Abs(heightDiff) >=2)
		return false;

	if(mType == SquareType::EMPTY || neighbour->getSquareType() == SquareType::EMPTY)
		return true;

	GridSquareJoinTemplate* thisTemplate = mParent->getGridSquareJoinTemplate(mType);
	GridSquareJoinTemplate* neighbourTemplate = mParent->getGridSquareJoinTemplate(neighbour->getSquareType());

	if(heightDiff == 0)
	{
		if(thisTemplate->getNegXNegZ() == neighbourTemplate->getPosXNegZ() &&
			thisTemplate->getNegXNegZLower() == neighbourTemplate->getPosXNegZLower() &&
			thisTemplate->getNegXPosZ() == neighbourTemplate->getPosXPosZ() &&
			thisTemplate->getNegXPosZLower() == neighbourTemplate->getPosXPosZLower())
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == 1)
	{
		if(
			thisTemplate->getNegXNegZLower() == neighbourTemplate->getPosXNegZ() && neighbourTemplate->getPosXNegZ()  == true && 
			thisTemplate->getNegXPosZLower() == neighbourTemplate->getPosXPosZ() && neighbourTemplate->getPosXPosZ()  == true)
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == -1)
	{
		if(
			thisTemplate->getNegXNegZ() == neighbourTemplate->getPosXNegZLower() && neighbourTemplate->getPosXNegZLower()  == true &&
			thisTemplate->getNegXPosZ() == neighbourTemplate->getPosXPosZLower() && neighbourTemplate->getPosXPosZLower()  == true)
		{
			return true;
		}
		return false;
	}
}

bool GridSquareTemplate::isPosXJoinValid(GridSquareTemplate* neighbour)
{
	int heightDiff = this->getY() - neighbour->getY();
	if(Math::Abs(heightDiff) >=2)
		return false;

	if(mType == SquareType::EMPTY || neighbour->getSquareType() == SquareType::EMPTY)
		return true;

	GridSquareJoinTemplate* thisTemplate = mParent->getGridSquareJoinTemplate(mType);
	GridSquareJoinTemplate* neighbourTemplate = mParent->getGridSquareJoinTemplate(neighbour->getSquareType());

	if(heightDiff == 0)
	{
		if(thisTemplate->getPosXNegZ() == neighbourTemplate->getNegXNegZ() &&
			thisTemplate->getPosXNegZLower() == neighbourTemplate->getNegXNegZLower() &&
			thisTemplate->getPosXPosZ() == neighbourTemplate->getNegXPosZ() &&
			thisTemplate->getPosXPosZLower() == neighbourTemplate->getNegXPosZLower())
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == 1)
	{
		if(
			thisTemplate->getPosXNegZLower() == neighbourTemplate->getNegXNegZ() && neighbourTemplate->getNegXNegZ()  == true &&
			thisTemplate->getPosXPosZLower() == neighbourTemplate->getNegXPosZ() && neighbourTemplate->getNegXPosZ()  == true)
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == -1)
	{
		if(
			thisTemplate->getPosXNegZ() == neighbourTemplate->getNegXNegZLower() && neighbourTemplate->getNegXNegZLower()  == true &&
			thisTemplate->getPosXPosZ() == neighbourTemplate->getNegXPosZLower() && neighbourTemplate->getNegXPosZLower()  == true)
		{
			return true;
		}
		return false;
	}
}

bool GridSquareTemplate::isNegZJoinValid(GridSquareTemplate* neighbour)
{
	int heightDiff = this->getY() - neighbour->getY();
	if(Math::Abs(heightDiff) >=2)
		return false;

	if(mType == SquareType::EMPTY || neighbour->getSquareType() == SquareType::EMPTY)
		return true;

	GridSquareJoinTemplate* thisTemplate = mParent->getGridSquareJoinTemplate(mType);
	GridSquareJoinTemplate* neighbourTemplate = mParent->getGridSquareJoinTemplate(neighbour->getSquareType());

	if(heightDiff == 0)
	{
		if(	thisTemplate->getNegXNegZ() == neighbourTemplate->getNegXPosZ() &&
			thisTemplate->getNegXNegZLower() == neighbourTemplate->getNegXPosZLower() &&
			thisTemplate->getPosXNegZ() == neighbourTemplate->getPosXPosZ() &&
			thisTemplate->getPosXNegZLower() == neighbourTemplate->getPosXPosZLower())
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == 1)
	{
		if(	
			thisTemplate->getNegXNegZLower() == neighbourTemplate->getNegXPosZ() && neighbourTemplate->getNegXPosZ()  == true &&
			thisTemplate->getPosXNegZLower() == neighbourTemplate->getPosXPosZ() && neighbourTemplate->getPosXPosZ()  == true)
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == -1)
	{
		if(	
			thisTemplate->getNegXNegZ() == neighbourTemplate->getNegXPosZLower() && neighbourTemplate->getNegXPosZLower()  == true &&
			thisTemplate->getPosXNegZ() == neighbourTemplate->getPosXPosZLower() && neighbourTemplate->getPosXPosZLower()  == true)
		{
			return true;
		}
		return false;
	}
}

bool GridSquareTemplate::isPosZJoinValid(GridSquareTemplate* neighbour)
{
	int heightDiff = this->getY() - neighbour->getY();
	if(Math::Abs(heightDiff) >=2)
		return false;
	
	if(mType == SquareType::EMPTY || neighbour->getSquareType() == SquareType::EMPTY)
		return true;

	GridSquareJoinTemplate* thisTemplate = mParent->getGridSquareJoinTemplate(mType);
	GridSquareJoinTemplate* neighbourTemplate = mParent->getGridSquareJoinTemplate(neighbour->getSquareType());

	if(heightDiff == 0)
	{
		if(	thisTemplate->getNegXPosZ() == neighbourTemplate->getNegXNegZ() &&
			thisTemplate->getNegXPosZLower() == neighbourTemplate->getNegXNegZLower() &&
			thisTemplate->getPosXPosZ() == neighbourTemplate->getPosXNegZ() &&
			thisTemplate->getPosXPosZLower() == neighbourTemplate->getPosXNegZLower())
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == 1)
	{
		if(	
			thisTemplate->getNegXPosZLower() == neighbourTemplate->getNegXNegZ() && neighbourTemplate->getNegXNegZ()  == true &&
			thisTemplate->getPosXPosZLower() == neighbourTemplate->getPosXNegZ() && neighbourTemplate->getPosXNegZ()  == true)
		{
			return true;
		}
		return false;
	}
	else if(heightDiff == -1)
	{
		if(	
			thisTemplate->getNegXPosZ() == neighbourTemplate->getNegXNegZLower() && neighbourTemplate->getNegXNegZLower()  == true &&
			thisTemplate->getPosXPosZ() == neighbourTemplate->getPosXNegZLower() && neighbourTemplate->getPosXNegZLower()  == true)
		{
			return true;
		}
		return false;
	}
}

bool GridSquareTemplate::allJoinsValid()
{
	GridSquareTemplate* negX(0); 
	GridSquareTemplate* posX(0); 
	GridSquareTemplate* negZ(0); 
	GridSquareTemplate* posZ(0);
	

	if(this->getX() > 0)
	{
		negX = mParent->getGridSquare((mParent->getSideLength() * this->getZ()) + this->getX() - 1);
	}

	if(this->getZ() > 0)
	{
		negZ = mParent->getGridSquare((mParent->getSideLength() * (this->getZ() - 1)) + this->getX());
	}

	if(this->getX() < (mParent->getSideLength() - 1))
	{
		posX = mParent->getGridSquare((mParent->getSideLength() * this->getZ()) + this->getX() + 1);
	}

	if(this->getZ() < (mParent->getSideLength() - 1))
	{
		posZ = mParent->getGridSquare((mParent->getSideLength() * (this->getZ() + 1)) + this->getX());
	}

	return allJoinsValid(negX, posX, negZ, posZ);
}

bool GridSquareTemplate::allJoinsValid(GridSquareTemplate* negX, GridSquareTemplate* posX, GridSquareTemplate* negZ, GridSquareTemplate* posZ)
{
	int numNeighbours(0);
	int numCorrect(0);

	if(negX)
		++numNeighbours;
	if(posX)
		++numNeighbours;
	if(negZ)
		++numNeighbours;
	if(posZ)
		++numNeighbours;

	if(negX && this->isNegXJoinValid(negX))
	{
		++numCorrect;
	}
	if(posX && this->isPosXJoinValid(posX))
	{
		++numCorrect;
	}
	if(negZ && this->isNegZJoinValid(negZ))
	{
		++numCorrect;
	}
	if(posZ && this->isPosZJoinValid(posZ))
	{
		++numCorrect;
	}

	if(numCorrect == numNeighbours)
		return true;

	return false;
}

bool GridSquareTemplate::correctType(int iterations)
{
	
	GridSquareTemplate* negX(0); 
	GridSquareTemplate* posX(0); 
	GridSquareTemplate* negZ(0); 
	GridSquareTemplate* posZ(0);
	

	if(this->getX() > 0)
	{
		negX = mParent->getGridSquare((mParent->getSideLength() * this->getZ()) + this->getX() - 1);
	}

	if(this->getZ() > 0)
	{
		negZ = mParent->getGridSquare((mParent->getSideLength() * (this->getZ() - 1)) + this->getX());
	}

	if(this->getX() < (mParent->getSideLength() - 1))
	{
		posX = mParent->getGridSquare((mParent->getSideLength() * this->getZ()) + this->getX() + 1);
	}

	if(this->getZ() < (mParent->getSideLength() - 1))
	{
		posZ = mParent->getGridSquare((mParent->getSideLength() * (this->getZ() + 1)) + this->getX());
	}
	return correctType(negX, posX, negZ, posZ, iterations);
}

bool GridSquareTemplate::correctType(GridSquareTemplate* negX, GridSquareTemplate* posX, GridSquareTemplate* negZ, GridSquareTemplate* posZ, int iterations)
{
	

	if(this->getX() == 21 && this->getZ() == 19)
		int temp = 0;

	int numNeighbours(0);
	int numCorrect(0);

	if(negX)
		++numNeighbours;
	if(posX)
		++numNeighbours;
	if(negZ)
		++numNeighbours;
	if(posZ)
		++numNeighbours;

	if(negX && this->isNegXJoinValid(negX))
	{
		++numCorrect;
	}
	if(posX && this->isPosXJoinValid(posX))
	{
		++numCorrect;
	}
	if(negZ && this->isNegZJoinValid(negZ))
	{
		++numCorrect;
	}
	if(posZ && this->isPosZJoinValid(posZ))
	{
		++numCorrect;
	}

	if(numCorrect == numNeighbours)
		return true;

	std::deque<GridSquareJoinTemplate*>& joinTemplates = mParent->getGridSquareJoinTemplates();
	
	SquareType type(mType);
	int oldHeight(mY);
	
	std::deque<GridSquareJoinTemplate*>::iterator iter;
	for(iter = joinTemplates.begin(); iter != joinTemplates.end(); ++iter)
	{
		numCorrect = 0;
		mY = oldHeight;
		mType = (*iter)->getSquareType();
		if(mType == SquareType::EMPTY && mX != mParent->getSideLength() && mZ != mParent->getSideLength())
		{
			continue;
		}
		
		if(negX && this->isNegXJoinValid(negX))
		{
			++numCorrect;
		}
		if(posX && this->isPosXJoinValid(posX))
		{
			++numCorrect;
		}
		if(negZ && this->isNegZJoinValid(negZ))
		{
			++numCorrect;
		}
		if(posZ && this->isPosZJoinValid(posZ))
		{
			++numCorrect;
		}

		if(numCorrect == numNeighbours)
			return true;

		++mY;
		numCorrect = 0;

		if(negX && this->isNegXJoinValid(negX))
		{
			++numCorrect;
		}
		if(posX && this->isPosXJoinValid(posX))
		{
			++numCorrect;
		}
		if(negZ && this->isNegZJoinValid(negZ))
		{
			++numCorrect;
		}
		if(posZ && this->isPosZJoinValid(posZ))
		{
			++numCorrect;
		}
		
		if(numCorrect == numNeighbours)
			return true;
		
		mY-=2;
		if(mY < 0)
		{
			continue;
		}
		numCorrect = 0;

		if(negX && this->isNegXJoinValid(negX))
		{
			++numCorrect;
		}
		if(posX && this->isPosXJoinValid(posX))
		{
			++numCorrect;
		}
		if(negZ && this->isNegZJoinValid(negZ))
		{
			++numCorrect;
		}
		if(posZ && this->isPosZJoinValid(posZ))
		{
			++numCorrect;
		}
		
		if(numCorrect == numNeighbours)
			return true;
	}

	this->setY(oldHeight);
	this->setSquareType(type);
	
	std::deque<GridSquareTemplate*> neighboursToCorrect;

	if(negX && negX->numInvalidJoins() >=2)
	{
		neighboursToCorrect.push_back(negX);
		negX = 0;
		--numNeighbours;
	}
	if(posX && posX->numInvalidJoins() >=2)
	{
		neighboursToCorrect.push_back(posX);
		posX = 0;
		--numNeighbours;
	}
	if(negZ && negZ->numInvalidJoins() >=2)
	{
		neighboursToCorrect.push_back(negZ);
		negZ = 0;
		--numNeighbours;
	}
	if(posZ && posZ->numInvalidJoins() >=2)
	{
		neighboursToCorrect.push_back(posZ);
		posZ = 0;
		--numNeighbours;
	}
	numCorrect = 0;

	if(negX && this->isNegXJoinValid(negX))
	{
		++numCorrect;
	}
	if(posX && this->isPosXJoinValid(posX))
	{
		++numCorrect;
	}
	if(negZ && this->isNegZJoinValid(negZ))
	{
		++numCorrect;
	}
	if(posZ && this->isPosZJoinValid(posZ))
	{
		++numCorrect;
	}

	if(numCorrect != numNeighbours)
	{
		for(iter = joinTemplates.begin(); iter != joinTemplates.end(); ++iter)
		{
			numCorrect = 0;
			mY = oldHeight;
			mType = (*iter)->getSquareType();
			if(mType == SquareType::EMPTY && mX != mParent->getSideLength() && mZ != mParent->getSideLength())
			{
				continue;
			}
		
			if(negX && this->isNegXJoinValid(negX))
			{
				++numCorrect;
			}
			if(posX && this->isPosXJoinValid(posX))
			{
				++numCorrect;
			}
			if(negZ && this->isNegZJoinValid(negZ))
			{
				++numCorrect;
			}
			if(posZ && this->isPosZJoinValid(posZ))
			{
				++numCorrect;
			}

			if(numCorrect == numNeighbours)
				return true;

			++mY;
			numCorrect = 0;

			if(negX && this->isNegXJoinValid(negX))
			{
				++numCorrect;
			}
			if(posX && this->isPosXJoinValid(posX))
			{
				++numCorrect;
			}
			if(negZ && this->isNegZJoinValid(negZ))
			{
				++numCorrect;
			}
			if(posZ && this->isPosZJoinValid(posZ))
			{
				++numCorrect;
			}
		
			if(numCorrect == numNeighbours)
				return true;
		
			mY-=2;
			if(mY < 0)
			{
				continue;
			}
			numCorrect = 0;

			if(negX && this->isNegXJoinValid(negX))
			{
				++numCorrect;
			}
			if(posX && this->isPosXJoinValid(posX))
			{
				++numCorrect;
			}
			if(negZ && this->isNegZJoinValid(negZ))
			{
				++numCorrect;
			}
			if(posZ && this->isPosZJoinValid(posZ))
			{
				++numCorrect;
			}
		
			if(numCorrect == numNeighbours)
				return true;
		}

		this->setY(oldHeight);
		this->setSquareType(type);
	}


	if(iterations == 0)
	{
		return false;
	} 
	else if(iterations > 0)
	{
		--iterations;
	}
	else if(iterations < 0)
	{
		iterations = 2;
	}
	

	std::deque<GridSquareTemplate*>::iterator neighbour;

	for(neighbour = neighboursToCorrect.begin(); neighbour != neighboursToCorrect.end(); ++neighbour)
	{
		(*neighbour)->correctType(iterations);
	}
	return false;
}

int GridSquareTemplate::numInvalidJoins()
{
	GridSquareTemplate* negX(0); 
	GridSquareTemplate* posX(0); 
	GridSquareTemplate* negZ(0); 
	GridSquareTemplate* posZ(0);
	

	if(this->getX() > 0)
	{
		negX = mParent->getGridSquare((mParent->getSideLength() * this->getZ()) + this->getX() - 1);
	}

	if(this->getZ() > 0)
	{
		negZ = mParent->getGridSquare((mParent->getSideLength() * (this->getZ() - 1)) + this->getX());
	}

	if(this->getX() < (mParent->getSideLength() - 1))
	{
		posX = mParent->getGridSquare((mParent->getSideLength() * this->getZ()) + this->getX() + 1);
	}

	if(this->getZ() < (mParent->getSideLength() - 1))
	{
		posZ = mParent->getGridSquare((mParent->getSideLength() * (this->getZ() + 1)) + this->getX());
	}

	int numNeighbours(0);
	int numCorrect(0);

	if(negX)
		++numNeighbours;
	if(posX)
		++numNeighbours;
	if(negZ)
		++numNeighbours;
	if(posZ)
		++numNeighbours;

	if(negX && this->isNegXJoinValid(negX))
	{
		++numCorrect;
	}
	if(posX && this->isPosXJoinValid(posX))
	{
		++numCorrect;
	}
	if(negZ && this->isNegZJoinValid(negZ))
	{
		++numCorrect;
	}
	if(posZ && this->isPosZJoinValid(posZ))
	{
		++numCorrect;
	}

	return numNeighbours - numCorrect;
}

bool GridSquareTemplate::isSloped()
{
	if(	mType == SquareType::SLOPED_NEG_X || 
		mType == SquareType::SLOPED_NEG_Z || 
		mType == SquareType::SLOPED_POS_X || 
		mType == SquareType::SLOPED_POS_Z || 
		mType == SquareType::CORNER_NEG_X_NEG_Z || 
		mType == SquareType::CORNER_NEG_X_POS_Z || 
		mType == SquareType::CORNER_POS_X_NEG_Z || 
		mType == SquareType::CORNER_POS_X_POS_Z ||
		mType == SquareType::INV_CORNER_NEG_X_NEG_Z || 
		mType == SquareType::INV_CORNER_NEG_X_POS_Z || 
		mType == SquareType::INV_CORNER_POS_X_NEG_Z || 
		mType == SquareType::INV_CORNER_POS_X_POS_Z ||
		mType == SquareType::TRANS_CLIFF_SLOPE_NEG_X || 
		mType == SquareType::TRANS_CLIFF_SLOPE_POS_X || 
		mType == SquareType::TRANS_CLIFF_SLOPE_NEG_Z || 
		mType == SquareType::TRANS_CLIFF_SLOPE_POS_Z ||
		mType == SquareType::TRANS_SLOPE_CLIFF_NEG_X || 
		mType == SquareType::TRANS_SLOPE_CLIFF_POS_X || 
		mType == SquareType::TRANS_SLOPE_CLIFF_NEG_Z || 
		mType == SquareType::TRANS_SLOPE_CLIFF_POS_Z)
		return true;

	return false;
}

std::string GridSquareTemplate::getSquareTypeAsString()
{
	switch(mType)
	{
		case SquareType::FLAT:
			return "FLAT";
		case SquareType::SLOPED_NEG_X:
			return "SLOPED_NEG_X";
		case SquareType::SLOPED_POS_X:
			return "SLOPED_POS_X";
		case SquareType::SLOPED_NEG_Z:
			return "SLOPED_NEG_Z";
		case SquareType::SLOPED_POS_Z:
			return "SLOPED_POS_Z";
		case SquareType::CORNER_NEG_X_NEG_Z:
			return "CORNER_NEG_X_NEG_Z";
		case SquareType::CORNER_POS_X_NEG_Z:
			return "CORNER_POS_X_NEG_Z";
		case SquareType::CORNER_NEG_X_POS_Z:
			return "CORNER_NEG_X_POS_Z";
		case SquareType::CORNER_POS_X_POS_Z:
			return "CORNER_POS_X_POS_Z";
		case SquareType::CLIFF_NEG_X:
			return "CLIFF_NEG_X";
		case SquareType::CLIFF_POS_X:
			return "CLIFF_POS_X";
		case SquareType::CLIFF_NEG_Z:
			return "CLIFF_NEG_Z";
		case SquareType::CLIFF_POS_Z:
			return "CLIFF_POS_Z";
		case SquareType::CORNER_CLIFF_NEG_X_NEG_Z:
			return "CORNER_CLIFF_NEG_X_NEG_Z";
		case SquareType::CORNER_CLIFF_POS_X_NEG_Z:
			return "CORNER_CLIFF_POS_X_NEG_Z";
		case SquareType::CORNER_CLIFF_NEG_X_POS_Z:
			return "CORNER_CLIFF_NEG_X_POS_Z";
		case SquareType::CORNER_CLIFF_POS_X_POS_Z:
			return "CORNER_CLIFF_POS_X_POS_Z";
		case SquareType::INV_CORNER_NEG_X_NEG_Z:
			return "INV_CORNER_NEG_X_NEG_Z";
		case SquareType::INV_CORNER_POS_X_NEG_Z:
			return "INV_CORNER_POS_X_NEG_Z";
		case SquareType::INV_CORNER_NEG_X_POS_Z:
			return "INV_CORNER_NEG_X_POS_Z";
		case SquareType::INV_CORNER_POS_X_POS_Z:
			return "INV_CORNER_POS_X_POS_Z";
		
		case SquareType::TRANS_CLIFF_SLOPE_NEG_X:
			return "TRANS_CLIFF_SLOPE_NEG_X";
		case SquareType::TRANS_CLIFF_SLOPE_POS_X:
			return "TRANS_CLIFF_SLOPE_POS_X";
		case SquareType::TRANS_CLIFF_SLOPE_NEG_Z:
			return "TRANS_CLIFF_SLOPE_NEG_Z";
		case SquareType::TRANS_CLIFF_SLOPE_POS_Z:
			return "TRANS_CLIFF_SLOPE_POS_Z";
		case SquareType::TRANS_SLOPE_CLIFF_NEG_X:
			return "TRANS_SLOPE_CLIFF_NEG_X";
		case SquareType::TRANS_SLOPE_CLIFF_POS_X:
			return "TRANS_SLOPE_CLIFF_POS_X";
		case SquareType::TRANS_SLOPE_CLIFF_NEG_Z:
			return "TRANS_SLOPE_CLIFF_NEG_Z";
		case SquareType::TRANS_SLOPE_CLIFF_POS_Z:
			return "TRANS_SLOPE_CLIFF_POS_Z";
		case SquareType::EMPTY:
			
			return "EMPTY";
	}
	return "";
}
