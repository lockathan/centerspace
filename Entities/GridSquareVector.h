#ifndef GridSquareVector_h
#define GridSquareVector_h

#include <Ogre.h>
#include <vector>
#include <hash_map>
#include <Entities/GridSquare.h>

class BattlefieldGrid;
class GridSquareTemplate;
class GridSquareTemplateVector;

class GridSquareVector
{
public:
	GridSquareVector(BattlefieldGrid* grid);
	~GridSquareVector();
	
	void initialize();
	void resetGridSquares();

	GridSquare* getGridSquare(int x, int z);

	void computeIndices();

	bool hasMoreVertices();
	TexturedVertex* getNextVertex();

	bool hasMoreIndices();
	int getNextIndex();
	
	void insertVertex(TexturedVertex* vertex, GridSquare* owner);
	void insertVertex(unsigned int i, TexturedVertex* vertex, GridSquare* owner);
	TexturedVertex* getVertex(unsigned int i);
	void updateTextureCoordinates(TexturedVertex* vertex);

	Ogre::ManualObject* getBattlefield();
	BattlefieldGrid* getBattlefieldGrid();
	GridSquareTemplateVector* getGridSquareTemplateVector();

private:

	void addSquare(GridSquareTemplate* squareTemplate);

	void setGridCells();

	Ogre::Real getFogTextureCoord(const Ogre::Real& inCoord);
	std::vector<GridSquare*> mGridSquares;
	std::vector<TexturedVertex*> mVertexCache;
	
	int mCurrentVertex;
	int mCurrentIndex;
	int mNumVertices;
	int mSideLength;
	int mTotalLength;

	BattlefieldGrid* mBattlefieldGrid;
	Ogre::ManualObject* mBattlefield;
	GridSquareTemplateVector* mGridSquareTemplateVector;

};
#endif