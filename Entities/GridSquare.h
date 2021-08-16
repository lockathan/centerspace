#ifndef GridSquare_h
#define GridSquare_h

#include <Ogre.h>
#include <Entities/Enums.h>

#include <vector>
#include <deque>


class GridSquareVector;

enum VertexXLocation
{
	VL_NEG_X = 0,
	VL_POS_X = 1
};

enum VertexYLocation
{
	VL_NEG_Y = 0,
	VL_POS_Y = 1,
	VL_UPPER_Y = 2
};

enum VertexZLocation
{
	VL_NEG_Z = 0,
	VL_POS_Z = 1
};

enum CliffFacing
{
	CF_NA = 0,
	CF_NEG_Z = 1,
	CF_POS_Z = 2,
	CF_NEG_X = 3,
	CF_POS_X = 4
};

class GridSquareTemplate;
class TexturedVertex;

class GridSquare
{
public:
	GridSquare(GridSquareVector* vector, GridSquareTemplate* squareTemplate);
	
	~GridSquare();

	void computeIndices();
	void reset();


	bool hasMoreIndices();
	int getNextIndex();
	void moveNextIndex();


	int getCenterX();
	int getCenterY();
	int getCenterZ();
	float getTextureStep();

	std::deque<TexturedVertex*> getVertices();
	void addVertex(TexturedVertex* vertex);

	TexturedVertex* findVertex(VertexXLocation xloc, VertexYLocation yLoc, VertexZLocation zLoc, CliffFacing cf = CliffFacing::CF_NA);
	TexturedVertex* findVertex(VertexXLocation xloc, VertexYLocation yLoc, VertexZLocation zLoc, bool hasTextureCoordinates,CliffFacing cf = CliffFacing::CF_NA );

	int findIndex(VertexXLocation xloc, VertexYLocation yLoc, VertexZLocation zLoc, CliffFacing cf = CliffFacing::CF_NA);
	
	void setAlphaSplatCoords(TexturedVertex* vertex);


	SquareType Type;
	static const int NumSquareTypes = 30;
private:
	float calculateSimpleTextureCoord(float location);
	float calculateSimpleTextureCoord(float location, float  y);

	float calculateSimpleTextureCoord(float existingValue, float existingLocation, float newLocation);
	float calculateComplexTextureCoord(float sourceValue, float sourceLocation, float sourceY, float destLocation, float destY);
	void calculateComplexTextureCoord(TexturedVertex *source, TexturedVertex *dest);

	bool isSloped();

	std::deque<TexturedVertex*> mVertices;
	
	std::deque<int> mIndices;

	GridSquareVector* mGrid;

	int mNumIndices;

	int mCurrentIndex;

	float mUnitSize;
	float mTextureStep;
	float mHypTextureStep;
	int mCenterX;
	int mCenterY;
	int mCenterZ;

};

class TexturedVertex
{
public:	
	TexturedVertex(float _x, float _y, float _z, VertexXLocation xLoc, VertexYLocation yLoc, VertexZLocation zLoc, GridSquare* owner, const Ogre::Vector3& normal = Ogre::Vector3(0,1,0))
		: 
		x(_x), 
		y(_y), 
		z(_z), 
		u(0), 
		v(0),
		EX(0),
		EY(0),
		EZ(0),
		EW(0),
		FX(0),
		FY(0),
		FZ(0),
		FW(0),
		XLocation(xLoc),
		YLocation(yLoc), 
		ZLocation(zLoc),
		setUCoord(false),
		setVCoord(false),
		CliffFace(CF_NA),
		Owner(owner),
		Normal(normal)
	{
		Owner->setAlphaSplatCoords(this);
	}

	TexturedVertex(TexturedVertex* vertex) 
		: 
		x(vertex->x), 
		y(vertex->y), 
		z(vertex->z),
		u(vertex->u),
		v(vertex->v),
		EX(vertex->EX),
		EY(vertex->EY),
		EZ(vertex->EZ),
		EW(vertex->EW),
		FX(vertex->FX),
		FY(vertex->FY),
		FZ(vertex->FZ),
		FW(vertex->FW),
		index(vertex->index),
		XLocation(vertex->XLocation), 
		YLocation(vertex->YLocation),
		ZLocation(vertex->ZLocation),
		setUCoord(vertex->setUCoord),
		setVCoord(vertex->setVCoord),
		CliffFace(vertex->CliffFace),
		Owner(vertex->Owner),
		Normal(vertex->Normal)
		{}

	~TexturedVertex(){}

	float x;
	float y;
	float z;
	float u;
	float v;

	float EX;
	float EY;
	float EZ;
	float EW;

	float FX;
	float FY;
	float FZ;
	float FW;

	int index;
	bool gotTextureCoords() {return setUCoord && setVCoord;}
	void gotTextureCoords(bool value) {setUCoord = value; setVCoord = value;}
	bool setUCoord;
	bool setVCoord;
	VertexXLocation XLocation;
	VertexYLocation YLocation;
	VertexZLocation ZLocation;
	Ogre::Vector3 Normal;
	CliffFacing CliffFace;
	GridSquare* Owner;
	void setCliffFace(CliffFacing cliffFace)
	{
		CliffFace = cliffFace;
		switch(CliffFace)
		{
		case CF_NA:
			Normal = Ogre::Vector3(0,1,0);
			break;
		case CF_NEG_Z:
			Normal = Ogre::Vector3(0,0,-1);
			break;
		case CF_POS_Z:
			Normal = Ogre::Vector3(0,0,1);
			break;
		case CF_NEG_X:
			Normal = Ogre::Vector3(-1,0,0);
			break;
		case CF_POS_X:
			Normal = Ogre::Vector3(1,0,0);
			break;
		}
	}
	Ogre::Vector3 getVertexXYZ() {return Ogre::Vector3(x,y,z);}
};
#endif