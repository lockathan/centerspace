#ifndef BattlefieldGrid_h
#define BattlefieldGrid_h

#include <Ogre.h>
#include <hash_map>
#include <vector>

class AIGroup;
class BattlefieldGridCell;
class GridSquare;
class GridSquareTemplateVector;
class LuaMCUDeque;
class Mission;
class MCUController;
class TerrainTemplate;

class BattlefieldGrid
{
public:
	BattlefieldGrid();
	~BattlefieldGrid();
	
	TerrainTemplate* getTerrainTemplate();
	void setTerrainTemplate(TerrainTemplate* terrain);

	int getSideLength();

	void setSideLength(int length);
	
	Ogre::ManualObject* getBattlefield();
	void setBattlefield(Ogre::ManualObject* manual);

	float getMaxRoughness();
	void setMaxRoughness(float value);

	float getMaxVertexHeight();
	void setMaxVertexHeight(float value);

	BattlefieldGridCell* getGridCell(int index);
	BattlefieldGridCell* getGridCell(int x, int z);
	BattlefieldGridCell* getGridCell(float x, float z);
	void seedEmptyGridCellSearch(int x, int z);
	BattlefieldGridCell* getNextEmptyGridCell();
	BattlefieldGridCell* getCloserGridCell(int x1, int z1, int x2, int z2, AIGroup* group);
	BattlefieldGridCell* getCloserGridCell(int x1, int z1, int x2, int z2, MCUController* controller);
	BattlefieldGridCell* getCloserGridCell(int x1, int z1, int x2, int z2, float range);

	void reset();

	void setGridCell(int index, BattlefieldGridCell* cell);

	GridSquareTemplateVector* getGridSquareTemplateVector();
	void setGridSquareTemplateVector(GridSquareTemplateVector* vector);

	bool isCentre(int x, int z);
	float getSquareDistance(BattlefieldGridCell* a, BattlefieldGridCell* b);

	Mission* getMission();
	void setMission(Mission* mission);

	bool isAvailable(int x, int z);
	bool isPassable(int x, int z);
	bool isOccupied(int x, int z);

	void placeMCUs(LuaMCUDeque* mcus, int x, int z);

	static float calculateDistance(int x1, int z1, int x2, int z2);
	static float calculateSquareDistance(int x1, int z1, int x2, int z2);

	static BattlefieldGrid* createBattlefieldGrid(GridSquareTemplateVector* templateVector, TerrainTemplate* terrain);
	static const float UnitSize;
private:


	float mMinX, mMinZ, mMaxX, mMaxZ;
	float mMinVertexHeight, mMaxVertexHeight;
	float mCellSizeX, mCellSizeZ;
	float mMaxRoughness;

	int mHeightDataLength;
	int mHeightDataSideLength;
	int mMaxHeight;

	int mSearchCurrentX;
	int mSearchCurrentZ;
	int mSearchXDirection;
	int mSearchZDirection;

	TerrainTemplate* mTerrainTemplate;
	std::vector<BattlefieldGridCell*> mGridCells;

	GridSquareTemplateVector* mGridSquareTemplateVector;
	Ogre::SceneManager* mSceneManager;
	Ogre::ManualObject* mBattlefield;
	Mission* mMission;
};
#endif