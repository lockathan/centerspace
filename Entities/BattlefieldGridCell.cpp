#include <Entities/BattlefieldGridCell.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/Enums.h>
#include <Entities/GridSquare.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Ogre.h>
using namespace Ogre;

BattlefieldGridCell::BattlefieldGridCell()
{
	mX = 0;
	mZ = 0;
	mNegXExtent = 0;
	mPosXExtent = 0;
	mNegZExtent = 0;
	mPosZExtent = 0;
	mPassable = true;
	mMarkedToBeOccupied = false;
	mUnitSize = 0;
	mCurrentOccupant = 0;
}

BattlefieldGridCell::BattlefieldGridCell(GridSquare* square, float roughness)
	:
	mUnitSize(BattlefieldGrid::UnitSize),
	mX(square->getCenterX()),
	mY(square->getCenterY()),
	mZ(square->getCenterZ()),
	mNegXExtent(mX * mUnitSize),
	mPosXExtent((mX + 1) * mUnitSize),
	mNegZExtent(mZ * mUnitSize),
	mPosZExtent((mZ + 1) * mUnitSize),
	mSquareType(square->Type),
	mPassable(true),
	mMarkedToBeOccupied(false),
	mCurrentOccupant(0),
	mRoughness(roughness)
{
	if(mSquareType == SquareType::CORNER_NEG_X_NEG_Z ||
		mSquareType == SquareType::CORNER_NEG_X_POS_Z ||
		mSquareType == SquareType::CORNER_POS_X_NEG_Z ||
		mSquareType == SquareType::CORNER_POS_X_POS_Z ||
		mSquareType == SquareType::INV_CORNER_NEG_X_NEG_Z ||
		mSquareType == SquareType::INV_CORNER_NEG_X_POS_Z ||
		mSquareType == SquareType::INV_CORNER_POS_X_NEG_Z ||
		mSquareType == SquareType::INV_CORNER_POS_X_POS_Z ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_NEG_X ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_NEG_Z ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_POS_X ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_POS_Z ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_NEG_X ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_NEG_Z ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_POS_X ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_POS_Z)
		mPassable = false;
}

BattlefieldGridCell::BattlefieldGridCell(int x, int y, int z, float unitSize, SquareType type, float roughness)
{
	
	mCurrentOccupant = 0;
	mUnitSize = unitSize;
	mX = x;
	mY = y;
	mZ = z;
	mNegXExtent = (mX * mUnitSize);
	mPosXExtent = ((mX + 1) * mUnitSize);

	mNegZExtent = (mZ * mUnitSize);
	mPosZExtent = ((mZ + 1) * mUnitSize);

	mSquareType = type;
	mPassable = true;

	if(mSquareType == SquareType::CORNER_NEG_X_NEG_Z ||
		mSquareType == SquareType::CORNER_NEG_X_POS_Z ||
		mSquareType == SquareType::CORNER_POS_X_NEG_Z ||
		mSquareType == SquareType::CORNER_POS_X_POS_Z ||
		mSquareType == SquareType::INV_CORNER_NEG_X_NEG_Z ||
		mSquareType == SquareType::INV_CORNER_NEG_X_POS_Z ||
		mSquareType == SquareType::INV_CORNER_POS_X_NEG_Z ||
		mSquareType == SquareType::INV_CORNER_POS_X_POS_Z ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_NEG_X ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_NEG_Z ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_POS_X ||
		mSquareType == SquareType::TRANS_CLIFF_SLOPE_POS_Z ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_NEG_X ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_NEG_Z ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_POS_X ||
		mSquareType == SquareType::TRANS_SLOPE_CLIFF_POS_Z)
		mPassable = false;

	mRoughness = roughness;

}

BattlefieldGridCell::~BattlefieldGridCell()
{

}

void BattlefieldGridCell::occupyCell(MCUController* mcu)
{
	mCurrentOccupant = mcu;
	mMarkedToBeOccupied = false;
}

void BattlefieldGridCell::leaveCell()
{
	//just set the pointer to nothing.
	//DO NOT delete the MCU!!!-
	mCurrentOccupant = 0;
	mMarkedToBeOccupied = false;
}

bool BattlefieldGridCell::isOccupied()
{
	return mCurrentOccupant != 0;
}

MCUController* BattlefieldGridCell::getOccupant()
{
	return mCurrentOccupant;
}


float BattlefieldGridCell::getRoughness()
{
	return mRoughness;
}

void BattlefieldGridCell::setRoughness(float roughness)
{
	mRoughness = roughness;
}

SquareType BattlefieldGridCell::getSquareType()
{
	return mSquareType;
}

void BattlefieldGridCell::setSquareType(SquareType type)
{
	mSquareType = type;
}

bool BattlefieldGridCell::isPassable()
{
	return mPassable;
}

void BattlefieldGridCell::setPassable(bool value)
{
	mPassable = value;
}

int BattlefieldGridCell::getX()
{
	return mX;
}

int BattlefieldGridCell::getY()
{
	return mY;
}

int BattlefieldGridCell::getZ()
{
	return mZ;
}

bool BattlefieldGridCell::isPointInside(float x, float z)
{
	return (mNegXExtent <= x && x <= mPosXExtent && mNegZExtent <= z && z <= mPosZExtent);
	
}

Vector3 BattlefieldGridCell::getMiddle()
{
	return Vector3((mNegXExtent + mPosXExtent) / 2.f, (mY + 0.52f) * mUnitSize, (mNegZExtent + mPosZExtent) / 2.f);
}

Vector3 BattlefieldGridCell::getMiddle2()
{
	Vector3 result = Vector3::ZERO;

	result.x = (mNegXNegZVertex.x + mNegXPosZVertex.x + mPosXNegZVertex.x + mPosXPosZVertex.x) / 4.0f;
	result.y = (mNegXNegZVertex.y + mNegXPosZVertex.y + mPosXNegZVertex.y + mPosXPosZVertex.y) / 4.0f - mUnitSize / 2;
	result.z = (mNegXNegZVertex.z + mNegXPosZVertex.z + mPosXNegZVertex.z + mPosXPosZVertex.z) / 4.0f;


	return result;
}

Vector3 BattlefieldGridCell::getMiddleFloor()
{
	return Vector3((mNegXExtent + mPosXExtent) / 2.f, mY* mUnitSize, (mNegZExtent + mPosZExtent) / 2.f);
}

float BattlefieldGridCell::getMovementCost()
{
	if(!mPassable)
		return 99999999.0f;

	if(mSquareType == SquareType::SLOPED_NEG_X || mSquareType == SquareType::SLOPED_NEG_Z ||
		mSquareType == SquareType::SLOPED_POS_X || mSquareType == SquareType::SLOPED_POS_X)
	{
		return 1.414f * mRoughness;
	}

	return mRoughness;
}

Vector3 BattlefieldGridCell::getNegXNegZVertex()
{
	return mNegXNegZVertex;
}

void BattlefieldGridCell::setNegXNegZVertex(const Vector3& vertex)
{
	mNegXNegZVertex = vertex;
}
	
Vector3 BattlefieldGridCell::getNegXPosZVertex()
{
	return mNegXPosZVertex;
}

void BattlefieldGridCell::setNegXPosZVertex(const Vector3& vertex)
{
	mNegXPosZVertex = vertex;
}

Vector3 BattlefieldGridCell::getPosXNegZVertex()
{
	return mPosXNegZVertex;
}

void BattlefieldGridCell::setPosXNegZVertex(const Vector3& vertex)
{
	mPosXNegZVertex = vertex;
}

Vector3 BattlefieldGridCell::getPosXPosZVertex()
{
	return mPosXPosZVertex;
}

void BattlefieldGridCell::setPosXPosZVertex(const Vector3& vertex)
{
	mPosXPosZVertex = vertex;
}

bool BattlefieldGridCell::isMarkedToBeOccupied()
{
	return mMarkedToBeOccupied;
}

void BattlefieldGridCell::setMarkedToBeOccupied(bool value)
{
	mMarkedToBeOccupied = value;
}

bool BattlefieldGridCell::isAvailable()
{
	return !mMarkedToBeOccupied && mPassable && mCurrentOccupant == 0;
}

bool BattlefieldGridCell::requiresJump(BattlefieldGridCell* neighbour)
{
	if(mX > neighbour->getX() && mZ == neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CLIFF_NEG_X || mSquareType == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || mSquareType == SquareType::CORNER_CLIFF_NEG_X_POS_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CLIFF_POS_X || neighbour->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z || neighbour->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
	}
	else if(mX < neighbour->getX() && mZ == neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CLIFF_POS_X || mSquareType == SquareType::CORNER_CLIFF_POS_X_NEG_Z || mSquareType == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CLIFF_NEG_X || neighbour->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || neighbour->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z)
				return true;
		}
	}
	else if(mX == neighbour->getX() && mZ > neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CLIFF_NEG_Z || mSquareType == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || mSquareType == SquareType::CORNER_CLIFF_POS_X_NEG_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CLIFF_POS_Z || neighbour->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z || neighbour->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
	}
	else if(mX == neighbour->getX() && mZ < neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CLIFF_POS_Z || mSquareType == SquareType::CORNER_CLIFF_NEG_X_POS_Z || mSquareType == SquareType::CORNER_CLIFF_POS_X_POS_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CLIFF_NEG_Z || neighbour->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || neighbour->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z)
				return true;
		}
	}
	else if(mX > neighbour->getX() && mZ > neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || mSquareType == SquareType::CLIFF_NEG_X || mSquareType == SquareType::CLIFF_NEG_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z || neighbour->getSquareType() == SquareType::CLIFF_POS_X || neighbour->getSquareType() == SquareType::CLIFF_POS_Z)
				return true;
		}
	}
	else if(mX < neighbour->getX() && mZ > neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CORNER_CLIFF_POS_X_NEG_Z || mSquareType == SquareType::CLIFF_POS_X || mSquareType == SquareType::CLIFF_NEG_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z || neighbour->getSquareType() == SquareType::CLIFF_NEG_X || neighbour->getSquareType() == SquareType::CLIFF_POS_Z)
				return true;
		}
	}
	else if(mX < neighbour->getX() && mZ < neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CORNER_CLIFF_NEG_X_POS_Z || mSquareType == SquareType::CLIFF_NEG_X || mSquareType == SquareType::CLIFF_POS_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z || neighbour->getSquareType() == SquareType::CLIFF_POS_X || neighbour->getSquareType() == SquareType::CLIFF_NEG_Z)
				return true;
		}
	}
	else if(mX > neighbour->getX() && mZ < neighbour->getZ())
	{
		if(mY > neighbour->getY())
		{
			if(mSquareType == SquareType::CORNER_CLIFF_POS_X_POS_Z || mSquareType == SquareType::CLIFF_POS_X || mSquareType == SquareType::CLIFF_POS_Z)
				return true;
		}
		else if(mY < neighbour->getY())
		{
			if(neighbour->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z || neighbour->getSquareType() == SquareType::CLIFF_NEG_X || neighbour->getSquareType() == SquareType::CLIFF_NEG_Z)
				return true;
		}
	}
	
	return false;
}

std::string BattlefieldGridCell::getTypeString()
{
	switch(mSquareType)
	{
	case FLAT: return "FLAT";
		case EMPTY: return "FLAT";
		case SLOPED_NEG_X: return "SLOPED_NEG_X";
		case SLOPED_POS_X: return "SLOPED_POS_X";
		case SLOPED_NEG_Z: return "SLOPED_NEG_Z";
		case SLOPED_POS_Z: return "SLOPED_POS_Z";
		case CORNER_NEG_X_NEG_Z: return "CORNER_NEG_X_NEG_Z";
		case CORNER_POS_X_NEG_Z: return "CORNER_POS_X_NEG_Z";
		case CORNER_NEG_X_POS_Z: return "CORNER_NEG_X_POS_Z";
		case CORNER_POS_X_POS_Z: return "CORNER_POS_X_POS_Z";
		case CLIFF_NEG_X: return "CLIFF_NEG_X";
		case CLIFF_POS_X: return "CLIFF_POS_X";
		case CLIFF_NEG_Z: return "CLIFF_NEG_Z";
		case CLIFF_POS_Z: return "CLIFF_POS_Z";
		case CORNER_CLIFF_NEG_X_NEG_Z: return "CORNER_CLIFF_NEG_X_NEG_Z";
		case CORNER_CLIFF_POS_X_NEG_Z: return "CORNER_CLIFF_POS_X_NEG_Z";
		case CORNER_CLIFF_NEG_X_POS_Z: return "CORNER_CLIFF_NEG_X_POS_Z";
		case CORNER_CLIFF_POS_X_POS_Z: return "CORNER_CLIFF_POS_X_POS_Z";
		case INV_CORNER_NEG_X_NEG_Z: return "INV_CORNER_NEG_X_NEG_Z";
		case INV_CORNER_POS_X_NEG_Z: return "INV_CORNER_POS_X_NEG_Z";
		case INV_CORNER_NEG_X_POS_Z: return "INV_CORNER_NEG_X_POS_Z";
		case INV_CORNER_POS_X_POS_Z: return "INV_CORNER_POS_X_POS_Z";
		case TRANS_CLIFF_SLOPE_NEG_X: return "TRANS_CLIFF_SLOPE_NEG_X";
		case TRANS_CLIFF_SLOPE_POS_X: return "TRANS_CLIFF_SLOPE_POS_X";
		case TRANS_CLIFF_SLOPE_NEG_Z: return "TRANS_CLIFF_SLOPE_NEG_Z";
		case TRANS_CLIFF_SLOPE_POS_Z: return "TRANS_CLIFF_SLOPE_POS_Z";
		case TRANS_SLOPE_CLIFF_NEG_X: return "TRANS_SLOPE_CLIFF_NEG_X";
		case TRANS_SLOPE_CLIFF_POS_X: return "TRANS_SLOPE_CLIFF_POS_X";
		case TRANS_SLOPE_CLIFF_NEG_Z: return "TRANS_SLOPE_CLIFF_NEG_Z";
		case TRANS_SLOPE_CLIFF_POS_Z: return "TRANS_SLOPE_CLIFF_POS_Z";
		case INDETERMINATE: return "INDETERMINATE";
	}
	return "INDETERMINATE";
}
