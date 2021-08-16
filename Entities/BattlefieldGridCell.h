#ifndef BattlefieldGridCell_h
#define BattlefieldGridCell_h

#include <Entities/Enums.h>
#include <Ogre.h>


class GridSquare;
class MCUController;

class BattlefieldGridCell
{
public:
	BattlefieldGridCell();
	BattlefieldGridCell(GridSquare* square, float roughness);
	BattlefieldGridCell(int x, int y, int z, float unitSize, SquareType type = SquareType::FLAT, float roughness = 1.0f);
	
	~BattlefieldGridCell();
	
	void occupyCell(MCUController* mcu);
	void leaveCell();
	bool isOccupied();
	MCUController* getOccupant();

	float getRoughness();
	void setRoughness(float roughness);

	SquareType getSquareType();
	void setSquareType(SquareType slope);
	
	bool isPassable();
	void setPassable(bool value);

	int getX();
	int getY();
	int getZ();

	bool isPointInside(float x, float z);
	Ogre::Vector3 getMiddle();
	Ogre::Vector3 getMiddle2();

	Ogre::Vector3 getMiddleFloor();
	float getMovementCost();

	Ogre::Vector3 getNegXNegZVertex();
	void setNegXNegZVertex(const Ogre::Vector3& vertex);
	
	Ogre::Vector3 getNegXPosZVertex();
	void setNegXPosZVertex(const Ogre::Vector3& vertex);

	Ogre::Vector3 getPosXNegZVertex();
	void setPosXNegZVertex(const Ogre::Vector3& vertex);

	Ogre::Vector3 getPosXPosZVertex();
	void setPosXPosZVertex(const Ogre::Vector3& vertex);

	bool isMarkedToBeOccupied();
	void setMarkedToBeOccupied(bool value);

	bool isAvailable();

	bool requiresJump(BattlefieldGridCell* neighbour);

	std::string getTypeString();

protected:
	MCUController* mCurrentOccupant;
	float mRoughness;
	SquareType mSquareType;

	bool mPassable;
	bool mMarkedToBeOccupied;

	int mX;
	int mY;
	int mZ;

	float mUnitSize;

	float mNegXExtent;
	float mPosXExtent;
	float mNegZExtent;
	float mPosZExtent;

	Ogre::Vector3 mNegXNegZVertex;
	Ogre::Vector3 mNegXPosZVertex;
	Ogre::Vector3 mPosXNegZVertex;
	Ogre::Vector3 mPosXPosZVertex;
};
#endif