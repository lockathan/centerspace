#include <Entities/GridSquareJoinChecker.h>
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

GridSquareJoinChecker::GridSquareJoinChecker()
{
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::EMPTY, HeightDifference::SAME_LEVEL));
		
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_NEG_X, HeightDifference::HIGHER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	//mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	//mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::HIGHER_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	//mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	//mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_NEG_X, HeightDifference::HIGHER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_POS_X, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::CLIFF_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_Z, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::CORNER_CLIFF_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_Z, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::CLIFF_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_POS_Z, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::HIGHER_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::TRANS_CLIFF_SLOPE_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_Z, SquareType::TRANS_CLIFF_SLOPE_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_NEG_Z, SquareType::CLIFF_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_POS_Z, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::TRANS_SLOPE_CLIFF_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::TRANS_SLOPE_CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_NEG_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_POS_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_NEG_X, HeightDifference::HIGHER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_NEG_X, HeightDifference::HIGHER_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_NEG_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_NEG_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_POS_X, HeightDifference::SAME_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_POS_X, HeightDifference::SAME_LEVEL));

	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_POS_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_POS_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));


	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::EMPTY, HeightDifference::SAME_LEVEL));
	
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	
	//mValidZJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	//mValidZJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	//mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	//mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::CLIFF_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::CLIFF_POS_X, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::CLIFF_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::CLIFF_POS_X, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::TRANS_CLIFF_SLOPE_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::TRANS_CLIFF_SLOPE_POS_X, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_NEG_X, SquareType::CLIFF_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_POS_X, SquareType::CLIFF_POS_X, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::TRANS_SLOPE_CLIFF_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::TRANS_SLOPE_CLIFF_POS_X, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_NEG_X, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_POS_X, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_POS_Z, HeightDifference::SAME_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidZJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_NEG_X, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_NEG_X, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::CLIFF_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalNegXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::CLIFF_POS_X, HeightDifference::SAME_LEVEL));

	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_POS_X, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_POS_X, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_CLIFF_SLOPE_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::TRANS_SLOPE_CLIFF_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::TRANS_CLIFF_SLOPE_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::TRANS_SLOPE_CLIFF_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_POS_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_CLIFF_POS_X_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_POS_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::FLAT, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::HIGHER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::FLAT, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::HIGHER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::FLAT, HeightDifference::LOWER_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_NEG_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::INV_CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::SLOPED_POS_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_NEG_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_NEG_Z, SquareType::SLOPED_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_POS_X_POS_Z, SquareType::SLOPED_NEG_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::INV_CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::INV_CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::INV_CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::INV_CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::CORNER_NEG_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_X, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::CORNER_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_POS_Z, SquareType::CORNER_POS_X_NEG_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_X, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::SLOPED_NEG_Z, SquareType::CORNER_POS_X_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));

	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::CLIFF_NEG_X, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));
	
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_POS_X, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CLIFF_NEG_Z, SquareType::CORNER_CLIFF_NEG_X_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::CLIFF_POS_Z, HeightDifference::SAME_LEVEL));
	mValidDiagonalPosXJoins.push_back(ValidJoin(SquareType::CORNER_CLIFF_POS_X_NEG_Z, SquareType::CLIFF_NEG_X, HeightDifference::SAME_LEVEL));

}

GridSquareJoinChecker::~GridSquareJoinChecker()
{

}

bool GridSquareJoinChecker::checkValidity(GridSquareTemplate* neg, GridSquareTemplate* pos)
{
	HeightDifference hDiff(HeightDifference::SAME_LEVEL);

	std::vector<ValidJoin>& joins = mValidXJoins;

	bool x = false;
	bool z = false;
	bool invert = false;

	if(neg->getX() != pos->getX() && neg->getZ() != pos->getZ())
	{
		x = true;
		z = true;
		invert = pos->getZ() < neg->getZ();
		bool xIncreasesAsZIncreases(false);

		if(!invert)
		{
			if(neg->getX() < pos->getX())
			{
				xIncreasesAsZIncreases = true;
			}
			else
			{
				xIncreasesAsZIncreases = false;
			}
		}
		else
		{
			if(neg->getX() < pos->getX())
			{
				xIncreasesAsZIncreases = false;
			}
			else
			{
				xIncreasesAsZIncreases = true;
			}
		}

		if(xIncreasesAsZIncreases)
		{
			joins = mValidDiagonalNegXJoins;
		}
		else
		{
			joins = mValidDiagonalPosXJoins;
		}
	}
	else if(neg->getX() != pos->getX())
	{
		x = true;
		invert = pos->getX() < neg->getX();
		joins = mValidXJoins;
	}
	else if(neg->getZ() != pos->getZ())
	{
		z = true;
		invert = pos->getZ() < neg->getZ();
		joins = mValidZJoins;
	}

	if(!x && !z)
		return true;
	
	if(invert)
	{
		GridSquareTemplate* temp(neg);
		neg = pos;
		pos = temp;
	}

	if(neg->getY() == pos->getY() + 1)
		hDiff = HeightDifference::LOWER_LEVEL;
	else if(neg->getY() == pos->getY() - 1)
		hDiff = HeightDifference::HIGHER_LEVEL;
	else if(abs(neg->getY() - pos->getY()) > 1)
		return false;

	SquareType negType = neg->getSquareType();
	SquareType posType = pos->getSquareType();


	std::vector<ValidJoin>::iterator iter;
	for(iter = joins.begin(); iter != joins.end(); ++iter)
	{
		if((*iter).NegType == negType && (*iter).PosType == posType && (*iter).HeightDiff == hDiff)
		{
			return true;
		}
	}

	return false;
}

std::deque<SquareType> GridSquareJoinChecker::getAllowedTypes(GridSquareTemplate* neg, GridSquareTemplate* pos, bool recommendForPositive)
{

	std::deque<SquareType> result;

	HeightDifference hDiff(HeightDifference::SAME_LEVEL);

	std::vector<ValidJoin>& joins = mValidXJoins;

	bool x = false;
	bool z = false;
	bool invert = false;

	if(neg->getX() != pos->getX() && neg->getZ() != pos->getZ())
	{
		x = true;
		z = true;
		invert = pos->getZ() < neg->getZ();
		bool xIncreasesAsZIncreases(false);

		if(!invert)
		{
			if(neg->getX() < pos->getX())
			{
				xIncreasesAsZIncreases = true;
			}
			else
			{
				xIncreasesAsZIncreases = false;
			}
		}
		else
		{
			if(neg->getX() < pos->getX())
			{
				xIncreasesAsZIncreases = false;
			}
			else
			{
				xIncreasesAsZIncreases = true;
			}
		}

		if(xIncreasesAsZIncreases)
		{
			joins = mValidDiagonalNegXJoins;
		}
		else
		{
			joins = mValidDiagonalPosXJoins;
		}
	}
	else if(neg->getX() != pos->getX())
	{
		x = true;
		invert = pos->getX() < neg->getX();
		joins = mValidXJoins;
	}
	else if(neg->getZ() != pos->getZ())
	{
		z = true;
		invert = pos->getZ() < neg->getZ();
		joins = mValidZJoins;
	}

	if(!x && !z)
		return result;

	if(invert)
	{
		GridSquareTemplate* temp(neg);
		neg = pos;
		pos = temp;
	}

	if(neg->getY() == pos->getY() + 1)
		hDiff = HeightDifference::LOWER_LEVEL;
	else if(neg->getY() == pos->getY() - 1)
		hDiff = HeightDifference::HIGHER_LEVEL;
	else if(abs(neg->getY() - pos->getY()) > 1)
		return result;

	SquareType negType = neg->getSquareType();
	SquareType posType = pos->getSquareType();

	std::vector<ValidJoin>::iterator iter;
	for(iter = joins.begin(); iter != joins.end(); ++iter)
	{
		if(recommendForPositive && (*iter).NegType == negType && (*iter).HeightDiff == hDiff)
		{
			result.push_back((*iter).PosType);
		}
		else if(!recommendForPositive && (*iter).PosType == posType && (*iter).HeightDiff == hDiff)
		{
			result.push_back((*iter).NegType);
		}
	}

	return result;
}

GridSquareJoinChecker* GridSquareJoinChecker::mGridSquareJoinChecker;

GridSquareJoinChecker* GridSquareJoinChecker::getSingletonPtr()
{
	if(!mGridSquareJoinChecker)
	{
		mGridSquareJoinChecker = new GridSquareJoinChecker();
	}
	return mGridSquareJoinChecker;
}
