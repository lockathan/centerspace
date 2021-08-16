#ifndef GridSquareJoinChecker_h
#define GridSquareJoinChecker_h

#include <Ogre.h>
#include <Entities/Enums.h>
#include <vector>
#include <deque>


class GridSquareTemplate;

enum HeightDifference
{
	SAME_LEVEL = 0,
	LOWER_LEVEL = 1,
	HIGHER_LEVEL = 2
};

struct ValidJoin
{
	ValidJoin(SquareType negType, SquareType posType, HeightDifference diff) : NegType(negType), PosType(posType), HeightDiff(diff) {}
	SquareType NegType;
	SquareType PosType;
	HeightDifference HeightDiff;
};

class GridSquareJoinChecker
{
public:
	~GridSquareJoinChecker();

	bool checkValidity(GridSquareTemplate* neg, GridSquareTemplate* pos);
	std::deque<SquareType> getAllowedTypes(GridSquareTemplate* neg, GridSquareTemplate* pos, bool recommendForPositive = true);
	
	static GridSquareJoinChecker* getSingletonPtr();
private:
	GridSquareJoinChecker();
	
	std::vector<ValidJoin> mValidXJoins;
	std::vector<ValidJoin> mValidZJoins;
	
	//All diagonal joins go from Negative Z to Positive Z
	std::vector<ValidJoin> mValidDiagonalNegXJoins;
	std::vector<ValidJoin> mValidDiagonalPosXJoins;

	static GridSquareJoinChecker* mGridSquareJoinChecker;
};

#endif