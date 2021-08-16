#ifndef GridSquareTemplateVector_h
#define GridSquareTemplateVector_h

#include <Ogre.h>
#include <Entities/Enums.h>
#include <vector>
#include <deque>

class GridSquareTemplate;
class GridSquareJoinTemplate;
class TerrainTemplate;


struct TerrainSquare;

class GridSquareTemplateVector
{
public:
	GridSquareTemplateVector();
	GridSquareTemplateVector(TerrainTemplate* terrain, int size);
	~GridSquareTemplateVector();

	int getSideLength();
	GridSquareTemplate* getGridSquare(int index);
	GridSquareTemplate* getGridSquare(int x, int z);

	void setDefaultGridHeight(int y);
	void changeSquare(int x, int z, int yDiff, bool createCliff = false);
	void changeSquare(int x, int z, SquareType newType);
	void changeSquare(int x, int z, int yDiff, SquareType newType);

	void createValleyOrRidge(int startX, int startZ, int endX, int endZ, int width, int targetY, bool createCliff = false);
	void createPlateau(int startX, int startZ, int minRadius, int maxRadius, int targetY, bool createCliff = false);
	std::deque<GridSquareTemplate*> getSquaresBetweenPoints(int x1, int z1, int x2, int z2);

	bool checkEntireGrid();
	
	bool fixSquareType(GridSquareTemplate* square);
	void checkSurroundings(GridSquareTemplate* square, bool forceOnce, bool createCliff = false, TerrainChange change = TerrainChange::TC_NO_CHANGE, bool changeTransitions = true, bool isCircle = false);
	void adjustHeight(GridSquareTemplate* square, bool forceOnce, bool createCliff = false, TerrainChange change = TerrainChange::TC_NO_CHANGE, bool changeTransitions = true, bool isCircle = false);
	void fixType(GridSquareTemplate* square, bool forceOnce, bool createCliff = false, TerrainChange change = TerrainChange::TC_NO_CHANGE, bool changeTransitions = true, bool isCircle = false);
	void startRecursiveSubdivision();

	void writeToFile(const std::string& fileName);
	void loadFromFile(const std::string& fileName);

	std::deque<GridSquareJoinTemplate*>& getGridSquareJoinTemplates();
	GridSquareJoinTemplate* getGridSquareJoinTemplate(SquareType type);

private:
	std::deque<GridSquareJoinTemplate*> mGridSquareJoinTemplates;
	void recursivelySubdivide(int topLeftX, int topLeftZ, int topRightX, int topRightZ, int bottomRightX, int bottomRightZ, int bottomLeftX, int bottomLeftZ);
	SquareType getSquareType(const TerrainSquare& square, const TerrainSquare& negX, const TerrainSquare& posX, const TerrainSquare& negZ, const TerrainSquare& posZ);
	bool contains(std::vector<SquareType> types, SquareType type);
	void equalizeHeight(GridSquareTemplate* square, bool raised, bool createCliff = false);
	bool isValid(GridSquareTemplate* square1, GridSquareTemplate* square2);
	bool isValid2(GridSquareTemplate* square, GridSquareTemplate* originator);
	
	std::vector<GridSquareTemplate*> mGridSquares;
	int mSideLength;
	TerrainTemplate* mTerrainTemplate;

};

#endif