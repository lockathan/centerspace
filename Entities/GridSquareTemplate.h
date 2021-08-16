#ifndef GridSquareTemplate_h
#define GridSquareTemplate_h

#include <Ogre.h>
#include <Entities/Enums.h>
#include <vector>
#include <deque>


class GridSquareTemplateVector;

class GridSquareTemplate
{
public:
	GridSquareTemplate(GridSquareTemplateVector* parent);
	GridSquareTemplate(int x, int y, int z, SquareType type, GridSquareTemplateVector* parent);
	~GridSquareTemplate();

	int getX();
	int getY();
	int getZ();

	void setY(int y);

	bool wasRaised();
	void setRaised(bool value);

	SquareType getSquareType();
	void setSquareType(SquareType type);

	TerrainChange getTerrainChange();
	void setTerrainChange(TerrainChange change);
	
	bool isNegXJoinValid(GridSquareTemplate* neighbour);
	bool isPosXJoinValid(GridSquareTemplate* neighbour);
	bool isNegZJoinValid(GridSquareTemplate* neighbour);
	bool isPosZJoinValid(GridSquareTemplate* neighbour);
	bool allJoinsValid();
	bool allJoinsValid(GridSquareTemplate* negX, GridSquareTemplate* posX, GridSquareTemplate* negZ, GridSquareTemplate* posZ);
	bool correctType(int iterations = -1);
	bool correctType(GridSquareTemplate* negX, GridSquareTemplate* posX, GridSquareTemplate* negZ, GridSquareTemplate* posZ, int iterations = -1);

	int numInvalidJoins();
	bool isSloped();
	std::string getSquareTypeAsString();

private:
	GridSquareTemplateVector* mParent;
	TerrainChange mChange;
	bool mRaised;
	SquareType mType;
	int mX;
	int mY;
	int mZ;

};

#endif