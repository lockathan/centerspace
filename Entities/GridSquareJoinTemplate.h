#ifndef GridSquareJoinTemplate_h
#define GridSquareJoinTemplate_h

#include <Entities/Enums.h>
#include <vector>
#include <deque>


class GridSquareTemplate;


class GridSquareJoinTemplate
{
public:
	GridSquareJoinTemplate(SquareType type);
	~GridSquareJoinTemplate();

	bool getNegXNegZ();
	bool getNegXPosZ();
	bool getPosXNegZ();
	bool getPosXPosZ();

	bool getNegXNegZLower();
	bool getNegXPosZLower();
	bool getPosXNegZLower();
	bool getPosXPosZLower();

	SquareType getSquareType();
private:
	SquareType mSquareType;
	bool mNegXNegZ;
	bool mNegXPosZ;
	bool mPosXNegZ;
	bool mPosXPosZ;

	bool mNegXNegZLower;
	bool mNegXPosZLower;
	bool mPosXNegZLower;
	bool mPosXPosZLower;

};

#endif