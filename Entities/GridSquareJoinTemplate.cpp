#include <Entities/GridSquareJoinTemplate.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/Enums.h>
#include <Entities/GridSquare.h>
#include <Entities/GridSquareTemplate.h>
#include <Entities/GridSquareVector.h>
#include <Templates/TerrainTemplate.h>
#include <Ogre.h>
#include <vector>
#include <deque>

using namespace Ogre;

GridSquareJoinTemplate::GridSquareJoinTemplate(SquareType type)
	:
	mSquareType(type),
	mNegXNegZ(false),
	mNegXPosZ(false),
	mPosXNegZ(false),
	mPosXPosZ(false),
	mNegXNegZLower(false),
	mNegXPosZLower(false),
	mPosXNegZLower(false),
	mPosXPosZLower(false)
{
	switch(mSquareType)
	{
		case SquareType::FLAT:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
		break;
		case SquareType::SLOPED_NEG_X:
			mNegXNegZLower = true;
			mNegXPosZLower = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
		break;
		case SquareType::SLOPED_POS_X:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZLower = true;
			mPosXPosZLower = true;
		break;
		case SquareType::SLOPED_NEG_Z:
			mNegXPosZ = true;
			mNegXNegZLower = true;
			mPosXNegZLower = true;
			mPosXPosZ = true;
		break;
		case SquareType::SLOPED_POS_Z:
			mNegXNegZ = true;
			mNegXPosZLower = true;
			mPosXNegZ = true;
			mPosXPosZLower = true;
		break;
		case SquareType::CORNER_NEG_X_NEG_Z:
			mNegXNegZLower = true;
			mNegXPosZLower = true;
			mPosXNegZLower = true;
			mPosXPosZ = true;
		break;
		case SquareType::CORNER_POS_X_NEG_Z:
			mNegXNegZLower = true;
			mNegXPosZ = true;
			mPosXNegZLower = true;
			mPosXPosZLower = true;
		break;
		case SquareType::CORNER_NEG_X_POS_Z:
			
			mNegXNegZLower = true;
			mNegXPosZLower = true;
			mPosXNegZ = true;
			mPosXPosZLower = true;
		break;
		case SquareType::CORNER_POS_X_POS_Z:
			mPosXPosZLower = true;
			mNegXPosZLower = true;
			mPosXNegZLower = true;
			mNegXNegZ = true;
			break;
		case SquareType::CLIFF_NEG_X:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mNegXPosZLower = true;
		break;
		case SquareType::CLIFF_POS_X:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mPosXNegZLower = true;
			mPosXPosZLower = true;
		break;
		case SquareType::CLIFF_NEG_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mPosXNegZLower = true;
		break;
		case SquareType::CLIFF_POS_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXPosZLower = true;
			mPosXPosZLower = true;
		break;

		case SquareType::CORNER_CLIFF_NEG_X_NEG_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mNegXPosZLower = true;
			mPosXNegZLower = true;
		break;
		case SquareType::CORNER_CLIFF_POS_X_NEG_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mPosXNegZLower = true;
			mPosXPosZLower = true;
			mNegXNegZLower = true;

		break;
		case SquareType::CORNER_CLIFF_NEG_X_POS_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mNegXPosZLower = true;
			mPosXPosZLower = true;
		break;
		case SquareType::CORNER_CLIFF_POS_X_POS_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mPosXNegZLower = true;
			mNegXPosZLower = true;
			mPosXPosZLower = true;
			
		break;
		case SquareType::INV_CORNER_NEG_X_NEG_Z:
			mNegXNegZLower = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
		break;
		case SquareType::INV_CORNER_POS_X_NEG_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZLower = true;
			mPosXPosZ = true;
		break;
		case SquareType::INV_CORNER_NEG_X_POS_Z:
			mNegXNegZ = true;
			mNegXPosZLower = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
		break;
		case SquareType::INV_CORNER_POS_X_POS_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZLower = true;
		break;
		case SquareType::TRANS_CLIFF_SLOPE_NEG_X:
			mNegXNegZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mNegXPosZLower = true;

		break;
		case SquareType::TRANS_CLIFF_SLOPE_POS_X:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXNegZLower = true;
			mPosXPosZLower = true;

		break;
		case SquareType::TRANS_CLIFF_SLOPE_NEG_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mPosXNegZLower = true;
		break;
		case SquareType::TRANS_CLIFF_SLOPE_POS_Z:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mNegXPosZLower = true;
			mPosXPosZLower = true;
		break;
		case SquareType::TRANS_SLOPE_CLIFF_NEG_X:
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mNegXPosZLower = true;
		break;
		case SquareType::TRANS_SLOPE_CLIFF_POS_X:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXPosZ = true;
			mPosXNegZLower = true;
			mPosXPosZLower = true;
		break;
		case SquareType::TRANS_SLOPE_CLIFF_NEG_Z:
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXNegZLower = true;
			mPosXNegZLower = true;
		break;
		case SquareType::TRANS_SLOPE_CLIFF_POS_Z:
			mNegXNegZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			mNegXPosZLower = true;
			mPosXPosZLower = true;
		
		break;
		case SquareType::EMPTY:
			mNegXNegZ = true;
			mNegXPosZ = true;
			mPosXNegZ = true;
			mPosXPosZ = true;
			
			mNegXNegZLower = true;
			mNegXPosZLower = true;
			mPosXNegZLower = true;
			mPosXPosZLower = true;
		break;
	}

}

GridSquareJoinTemplate::~GridSquareJoinTemplate()
{

}

bool GridSquareJoinTemplate::getNegXNegZ()
{
	return mNegXNegZ;
}

bool GridSquareJoinTemplate::getNegXPosZ()
{
	return mNegXPosZ;
}

bool GridSquareJoinTemplate::getPosXNegZ()
{
	return mPosXNegZ;
}

bool GridSquareJoinTemplate::getPosXPosZ()
{
	return mPosXPosZ;
}

bool GridSquareJoinTemplate::getNegXNegZLower()
{
	return mNegXNegZLower;
}

bool GridSquareJoinTemplate::getNegXPosZLower()
{
	return mNegXPosZLower;
}

bool GridSquareJoinTemplate::getPosXNegZLower()
{
	return mPosXNegZLower;
}

bool GridSquareJoinTemplate::getPosXPosZLower()
{
	return mPosXPosZLower;
}

SquareType GridSquareJoinTemplate::getSquareType()
{
	return mSquareType;
}
