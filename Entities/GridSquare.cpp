#include <Entities/GridSquare.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/Enums.h>
#include <Entities/GridSquareVector.h>
#include <Entities/GridSquareTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <Ogre.h>
#include <vector>
#include <deque>


using namespace Ogre;

GridSquare::GridSquare(GridSquareVector* vector, GridSquareTemplate* squareTemplate)	
{
	mGrid = vector;
	mNumIndices = 0;

	mTextureStep = 1 / 20.f;
	mHypTextureStep = 1.414f * mTextureStep;

	Type = squareTemplate->getSquareType();
	mUnitSize = BattlefieldGrid::UnitSize;
	mCenterX = squareTemplate->getX();
	mCenterY = squareTemplate->getY();
	mCenterZ = squareTemplate->getZ();
	
	reset();


	float y = mCenterY * mUnitSize;
	float yLower = (mCenterY - 1) * mUnitSize;

	TexturedVertex* vertex = 0;
	switch(Type)
	{
		case SquareType::FLAT:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::SLOPED_NEG_X:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::SLOPED_POS_X:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::SLOPED_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::SLOPED_POS_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CORNER_NEG_X_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CORNER_POS_X_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CORNER_NEG_X_POS_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CORNER_POS_X_POS_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			break;
		case SquareType::CLIFF_NEG_X:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CLIFF_POS_X:
			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), yLower, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CLIFF_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			

		break;
		case SquareType::CLIFF_POS_Z:
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;

		case SquareType::CORNER_CLIFF_NEG_X_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CORNER_CLIFF_POS_X_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), yLower, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), yLower, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CORNER_CLIFF_NEG_X_POS_Z:			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::CORNER_CLIFF_POS_X_POS_Z:
			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), yLower, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), yLower, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), yLower, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		case SquareType::INV_CORNER_NEG_X_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		case SquareType::INV_CORNER_POS_X_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		case SquareType::INV_CORNER_NEG_X_POS_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::INV_CORNER_POS_X_POS_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		
		
		case SquareType::TRANS_CLIFF_SLOPE_NEG_X:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::TRANS_CLIFF_SLOPE_POS_X:
			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), yLower, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * (mCenterX + 1), y, mUnitSize * mCenterZ, VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_X);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
		case SquareType::TRANS_CLIFF_SLOPE_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			vertex->setCliffFace(CliffFacing::CF_NEG_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			

		break;
		case SquareType::TRANS_CLIFF_SLOPE_POS_Z:
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * (mCenterZ + 1), VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, this);
			vertex->setCliffFace(CliffFacing::CF_POS_Z);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		case SquareType::TRANS_SLOPE_CLIFF_NEG_X:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		case SquareType::TRANS_SLOPE_CLIFF_POS_X:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		case SquareType::TRANS_SLOPE_CLIFF_NEG_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		break;
		case SquareType::TRANS_SLOPE_CLIFF_POS_Z:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
		
		break;
		case SquareType::EMPTY:
			vertex = new TexturedVertex(mUnitSize * mCenterX, y, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);
			
			vertex = new TexturedVertex(mUnitSize * mCenterX, yLower, mUnitSize * mCenterZ, VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, this);
			mGrid->insertVertex(vertex, this);
			mVertices.push_back(vertex);

		break;
	}
}

GridSquare::~GridSquare()
{
	std::deque<TexturedVertex*>::iterator iter;

	for(iter = mVertices.begin(); iter != mVertices.end(); ++iter)
	{
		delete (*iter);
	}
}

void GridSquare::computeIndices()
{
	std::deque<TexturedVertex*>::iterator vIterator;

	for(vIterator = mVertices.begin(); vIterator!= mVertices.end(); ++vIterator)
	{
		if(!(*vIterator)->gotTextureCoords())
		{
			TexturedVertex* vertex = mGrid->getVertex((*vIterator)->index);

			if(vertex && vertex->gotTextureCoords())
			{
				(*vIterator)->u = vertex->u;
				(*vIterator)->v = vertex->v;

				(*vIterator)->EX = vertex->EX;
				(*vIterator)->EY = vertex->EY;
				(*vIterator)->EZ = vertex->EZ;
				(*vIterator)->EW = vertex->EW;
				
				(*vIterator)->FX = vertex->FX;
				(*vIterator)->FY = vertex->FY;
				(*vIterator)->FZ = vertex->FZ;
				(*vIterator)->FW = vertex->FW;

				(*vIterator)->gotTextureCoords(true);
			}
		}
	}
	

	if(mCenterX == 0)
	{
		for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
		{
			if((*vIterator)->XLocation == VertexXLocation::VL_NEG_X)
			{
				(*vIterator)->u = calculateSimpleTextureCoord((*vIterator)->x, (*vIterator)->y);
				(*vIterator)->setUCoord = true;
				mGrid->updateTextureCoordinates((*vIterator));
			}
		
		}
	}

	if(mCenterZ == 0)
	{
		for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
		{
			if((*vIterator)->XLocation == VertexXLocation::VL_NEG_X && (*vIterator)->YLocation != VertexYLocation::VL_UPPER_Y && (*vIterator)->ZLocation == VertexZLocation::VL_NEG_Z)
			{
				(*vIterator)->v = calculateSimpleTextureCoord((*vIterator)->z);
				(*vIterator)->setVCoord = true;
				mGrid->updateTextureCoordinates((*vIterator));
			}	
		}

	}

	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		if((*vIterator)->setUCoord && (*vIterator)->XLocation == VertexXLocation::VL_NEG_X && (*vIterator)->ZLocation == VertexZLocation::VL_NEG_Z)
		{
			TexturedVertex* vertex = findVertex(VertexXLocation::VL_POS_X, (*vIterator)->YLocation, VertexZLocation::VL_NEG_Z, false, (*vIterator)->CliffFace);

			if(vertex)
			{
				vertex->u = calculateSimpleTextureCoord((*vIterator)->u, (*vIterator)->x, vertex->x);
				vertex->v = (*vIterator)->v;
				vertex->gotTextureCoords(true);
				mGrid->updateTextureCoordinates(vertex);
			}
		}
	}

	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		if((*vIterator)->setUCoord && (*vIterator)->ZLocation == VertexZLocation::VL_NEG_Z)
		{
			TexturedVertex* vertex = findVertex((*vIterator)->XLocation, (*vIterator)->YLocation, VertexZLocation::VL_POS_Z, false, (*vIterator)->CliffFace);

			if(vertex)
			{
				vertex->u = (*vIterator)->u;
				vertex->v = calculateSimpleTextureCoord((*vIterator)->v, (*vIterator)->z, vertex->z);
				vertex->gotTextureCoords(true);
				mGrid->updateTextureCoordinates(vertex);
			}
		}
	}

	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		if((*vIterator)->setUCoord && (*vIterator)->XLocation == VertexXLocation::VL_NEG_X && (*vIterator)->ZLocation == VertexZLocation::VL_POS_Z)
		{
			TexturedVertex* vertex = findVertex(VertexXLocation::VL_POS_X, (*vIterator)->YLocation, VertexZLocation::VL_POS_Z,false, (*vIterator)->CliffFace);

			if(vertex)
			{
				vertex->u = calculateSimpleTextureCoord((*vIterator)->u, (*vIterator)->x, vertex->x);
				vertex->v = (*vIterator)->v;
				vertex->gotTextureCoords(true);
				mGrid->updateTextureCoordinates(vertex);
			}
		}
	}
	
	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		TexturedVertex* na = 0;

		if((*vIterator)->CliffFace != CliffFacing::CF_NA && (*vIterator)->YLocation != VertexYLocation::VL_UPPER_Y && (*vIterator)->Owner == this)
		{
			(*vIterator)->EX = 1;
			(*vIterator)->EY = 1;
			(*vIterator)->EZ = 0;
			(*vIterator)->EW = 0;
			(*vIterator)->FX = 0;
			(*vIterator)->FY = 0;
			(*vIterator)->FZ = 0;
			(*vIterator)->FW = 0;

			na = findVertex((*vIterator)->XLocation, (*vIterator)->YLocation, (*vIterator)->ZLocation,CliffFacing::CF_NA);
			
			na->EY = 0.8f;
			na->EZ = na->EZ * 0.2f;
			na->EW = na->EW * 0.2f;
			na->FX = na->FX * 0.2f;
			na->FY = na->FY * 0.2f;
			na->FZ = na->FZ * 0.2f;
			na->FW = na->FW * 0.2f;

			if(na)
			{	
				if((*vIterator)->CliffFace == CliffFacing::CF_NEG_X && (*vIterator)->YLocation == VertexYLocation::VL_NEG_Y && na->gotTextureCoords())
				{
					(*vIterator)->u = na->u;
					(*vIterator)->v = na->v;
					(*vIterator)->gotTextureCoords(true);
					mGrid->updateTextureCoordinates((*vIterator));

				}
				if((*vIterator)->CliffFace == CliffFacing::CF_NEG_Z && (*vIterator)->YLocation == VertexYLocation::VL_NEG_Y && na->gotTextureCoords())
				{
					(*vIterator)->u = na->u;
					(*vIterator)->v = na->v;
					(*vIterator)->gotTextureCoords(true);
					mGrid->updateTextureCoordinates((*vIterator));

				}
				if((*vIterator)->CliffFace == CliffFacing::CF_POS_X && (*vIterator)->YLocation == VertexYLocation::VL_NEG_Y && na->gotTextureCoords())
				{
					(*vIterator)->u = na->u;
					(*vIterator)->v = na->v;
					(*vIterator)->gotTextureCoords(true);
					mGrid->updateTextureCoordinates((*vIterator));

				}
				if((*vIterator)->CliffFace == CliffFacing::CF_POS_Z && (*vIterator)->YLocation == VertexYLocation::VL_NEG_Y && na->gotTextureCoords())
				{
					(*vIterator)->u = na->u;
					(*vIterator)->v = na->v;
					(*vIterator)->gotTextureCoords(true);
					mGrid->updateTextureCoordinates((*vIterator));
				}
			}
		}
	}
	
	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		if((*vIterator)->gotTextureCoords() && (*vIterator)->CliffFace != CliffFacing::CF_NA && (*vIterator)->Owner == this)
		{
			(*vIterator)->EX = 1;
			(*vIterator)->EY = 1;
			(*vIterator)->EZ = 0;
			(*vIterator)->EW = 0;
			(*vIterator)->FX = 0;
			(*vIterator)->FY = 0;
			(*vIterator)->FZ = 0;
			(*vIterator)->FW = 0;
			mGrid->updateTextureCoordinates((*vIterator));

			TexturedVertex* vertex = 0;
			int dir(0);
			if((*vIterator)->YLocation == VertexYLocation::VL_NEG_Y)
			{
				vertex = findVertex((*vIterator)->XLocation, VertexYLocation::VL_POS_Y, (*vIterator)->ZLocation,false, (*vIterator)->CliffFace);
				dir = 1;
				if(vertex)
				{
					switch((*vIterator)->CliffFace)
					{
					case CliffFacing::CF_NEG_X:
						vertex->u = (*vIterator)->u + mTextureStep * dir;
						vertex->v = (*vIterator)->v;
						vertex->gotTextureCoords(true);
						break;

					case CliffFacing::CF_NEG_Z:
						vertex->u = (*vIterator)->u;
						vertex->v = (*vIterator)->v + mTextureStep * dir;
						vertex->gotTextureCoords(true);
						break;
					case CliffFacing::CF_POS_X:
						vertex->u = (*vIterator)->u + mTextureStep * dir;
						vertex->v = (*vIterator)->v;
						vertex->gotTextureCoords(true);
						break;
					case CliffFacing::CF_POS_Z:
						vertex->u = (*vIterator)->u;
						vertex->v = (*vIterator)->v + mTextureStep * dir;
						vertex->gotTextureCoords(true);
						break;
						
					}
					vertex->EX = 1;
					vertex->EY = 1;
					vertex->EZ = 0;
					vertex->EW = 0;
					vertex->FX = 0;
					vertex->FY = 0;
					vertex->FZ = 0;
					vertex->FW = 0;

					mGrid->updateTextureCoordinates(vertex);
				}
			}
		}
	}
	
	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		TexturedVertex* na = 0;

		if((*vIterator)->CliffFace != CliffFacing::CF_NA && (*vIterator)->YLocation != VertexYLocation::VL_UPPER_Y && (*vIterator)->Owner == this)
		{
			na = findVertex((*vIterator)->XLocation, (*vIterator)->YLocation, (*vIterator)->ZLocation,CliffFacing::CF_NA);
				
			if(na)
			{
				if((*vIterator)->CliffFace == CliffFacing::CF_NEG_X && (*vIterator)->YLocation == VertexYLocation::VL_POS_Y && (*vIterator)->gotTextureCoords())
				{
					//na->u = (*vIterator)->u;
					//na->setUCoord = true;
					
					na->v = (*vIterator)->v;
					na->setVCoord = true;
					mGrid->updateTextureCoordinates(na);
				}

				if((*vIterator)->CliffFace == CliffFacing::CF_NEG_Z && (*vIterator)->YLocation == VertexYLocation::VL_POS_Y && (*vIterator)->gotTextureCoords())
				{
					na->u = (*vIterator)->u;
					na->setUCoord = true;
					//na->v = (*vIterator)->v;
					//na->setVCoord = true;
					mGrid->updateTextureCoordinates(na);
				}


			}
		}
	}
	
	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		if((*vIterator)->setUCoord && (*vIterator)->XLocation == VertexXLocation::VL_NEG_X)
		{
			TexturedVertex* vertex = findVertex(VertexXLocation::VL_POS_X, (*vIterator)->YLocation, (*vIterator)->ZLocation, false, (*vIterator)->CliffFace);

			if(vertex)
			{
				vertex->u = calculateSimpleTextureCoord((*vIterator)->u, (*vIterator)->x, vertex->x);
				vertex->v = (*vIterator)->v;
				vertex->gotTextureCoords(true);
				mGrid->updateTextureCoordinates(vertex);
			}

			if(this->isSloped())
			{
				if((*vIterator)->YLocation == VertexYLocation::VL_NEG_Y)
					vertex = findVertex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, false, (*vIterator)->CliffFace);
				else
					vertex = findVertex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, false, (*vIterator)->CliffFace);

				if(vertex && !vertex->gotTextureCoords())
				{
					calculateComplexTextureCoord((*vIterator), vertex);
					vertex->gotTextureCoords(true);
					mGrid->updateTextureCoordinates(vertex);
				}
			}
		}
	}

	for(vIterator = mVertices.begin(); vIterator != mVertices.end(); ++vIterator)
	{
		if((*vIterator)->setUCoord && (*vIterator)->ZLocation == VertexZLocation::VL_NEG_Z)
		{
			TexturedVertex* vertex = findVertex((*vIterator)->XLocation, (*vIterator)->YLocation, VertexZLocation::VL_POS_Z, false, (*vIterator)->CliffFace);

			if(vertex)
			{
				vertex->u = (*vIterator)->u;
				vertex->v = calculateSimpleTextureCoord((*vIterator)->v, (*vIterator)->z, vertex->z);
				vertex->gotTextureCoords(true);
				mGrid->updateTextureCoordinates(vertex);
			}
		}
	}

	switch(Type)
	{
		case SquareType::FLAT:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mNumIndices = 6;
			break;
		case SquareType::SLOPED_NEG_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mNumIndices = 6;
			
			break;
		case SquareType::SLOPED_POS_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mNumIndices = 6;
			
			break;
		case SquareType::SLOPED_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mNumIndices = 6;
			
			break;
		case SquareType::SLOPED_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mNumIndices = 6;
			
			break;
		case SquareType::CORNER_NEG_X_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));

			mNumIndices = 6;
			
			break;
		case SquareType::CORNER_POS_X_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));

			mNumIndices = 6;
			
			break;
		case SquareType::CORNER_NEG_X_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));

			mNumIndices = 6;
			
			break;
		case SquareType::CORNER_POS_X_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			
			mNumIndices = 6;
			
			break;
		case SquareType::CLIFF_NEG_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mNumIndices = 12;
			break;
		case SquareType::CLIFF_POS_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));
			
			mNumIndices = 12;
			
			break;
		case SquareType::CLIFF_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mNumIndices = 12;
			
			break;
		case SquareType::CLIFF_POS_Z:

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mNumIndices = 12;
			break;
		case SquareType::CORNER_CLIFF_NEG_X_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mNumIndices = 18;
			break;
		case SquareType::CORNER_CLIFF_POS_X_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			
			mNumIndices = 18;
			break;
		case SquareType::CORNER_CLIFF_NEG_X_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			
			mNumIndices = 18;
			break;
		case SquareType::CORNER_CLIFF_POS_X_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));
			
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			
			mNumIndices = 18;
			break;
			
		case SquareType::INV_CORNER_NEG_X_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mNumIndices = 6;
			break;
		case SquareType::INV_CORNER_POS_X_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));

			mNumIndices = 6;
			break;
		case SquareType::INV_CORNER_NEG_X_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mNumIndices = 6;
			break;
		case SquareType::INV_CORNER_POS_X_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mNumIndices = 6;
			break;
		
		case SquareType::TRANS_CLIFF_SLOPE_NEG_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mNumIndices = 9;
			break;
		case SquareType::TRANS_CLIFF_SLOPE_POS_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_POS_X));
			mNumIndices = 9;
			break;
		case SquareType::TRANS_CLIFF_SLOPE_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
		
			mNumIndices = 9;
			break;
		case SquareType::TRANS_CLIFF_SLOPE_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
		
			mNumIndices = 9;
			break;
		case SquareType::TRANS_SLOPE_CLIFF_NEG_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NEG_X));

			mNumIndices = 9;
			break;
		case SquareType::TRANS_SLOPE_CLIFF_POS_X:
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_X));
			mNumIndices = 9;
			break;
		case SquareType::TRANS_SLOPE_CLIFF_NEG_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NEG_Z));
		
			mNumIndices = 9;
			break;
		case SquareType::TRANS_SLOPE_CLIFF_POS_Z:
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z));

			mIndices.push_back(findIndex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
			mIndices.push_back(findIndex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_POS_Z));
		
			mNumIndices = 9;
		break;
		default:
			mNumIndices = 0;
			break;
	}

}

float GridSquare::calculateSimpleTextureCoord(float location)
{
	return mTextureStep * (location / mUnitSize);
}

float GridSquare::calculateSimpleTextureCoord(float location, float y)
{
	return mTextureStep * ((location / mUnitSize) + (y / mUnitSize));
}

float GridSquare::calculateSimpleTextureCoord(float existingValue, float existingLocation, float newLocation)
{
	float difference = ((newLocation / mUnitSize) - (existingLocation / mUnitSize)) * mTextureStep;
	
	return difference + existingValue;
}

float GridSquare::calculateComplexTextureCoord(float sourceValue, float sourceLocation, float sourceY, float destLocation, float destY)
{
	if(sourceLocation == destLocation && sourceY == destY)
		return sourceValue;

	if(sourceLocation == destLocation)
	{
		
		return sourceValue + mTextureStep;
	}
	else
	{
		if(sourceY != destY)
		{
			return sourceValue + 1.414f * mTextureStep;
		}
		else
		{
			return sourceValue + mTextureStep;
		}
	}
}

void GridSquare::calculateComplexTextureCoord(TexturedVertex* source, TexturedVertex* dest)
{
	int xStep = (dest->x - source->x) / mUnitSize;
	int yStep = (dest->y - source->y) / mUnitSize;
	int zStep = (dest->z - source->z) / mUnitSize;

	if(source->y == dest->y)
	{
		if(source->x == dest->x)
		{
			if(source->z == dest->z)
			{
				dest->u = source->u;
				dest->v = source->v;
			}
			else
			{
				dest->u = source->u;
				dest->v = source->v + zStep * mTextureStep;
			}
		}
		else
		{
			if(source->z == dest->z)
			{
				dest->u = source->u + xStep * mTextureStep;
				dest->v = source->v;
			}
			else
			{
				dest->u = source->u + xStep * mTextureStep;
				dest->v = source->v + zStep * mTextureStep;
			}
		}
	}
	else
	{
		if(source->x == dest->x)
		{
			if(source->z == dest->z)
			{
				if(Type == SquareType::CLIFF_NEG_X || Type == SquareType::CLIFF_POS_X)
				{
					dest->u = source->u + yStep * mTextureStep;
					dest->v = source->v;
				}
				else
				{
					dest->u = source->u;
					dest->v = source->v + yStep * mTextureStep;
				}
			}
			else
			{
				dest->u = source->u;
				dest->v = source->v + zStep * mTextureStep;
			}
		}
		else
		{
			if(source->z == dest->z)
			{
				dest->u = source->u + xStep * mTextureStep;
				dest->v = source->v;
			}
			else
			{
				//special case - should only happen once
				dest->u = source->u;
				dest->v = source->v;
			}
		}
	}
}

void GridSquare::reset()
{
	mCurrentIndex = 0;
}

bool GridSquare::hasMoreIndices()
{
	return mCurrentIndex < mNumIndices;
}

int GridSquare::getNextIndex()
{	
	int result = mIndices[mCurrentIndex];
	++mCurrentIndex;
	return result;
}

void GridSquare::moveNextIndex()
{

}

int GridSquare::getCenterX()
{
	return mCenterX;
}

int GridSquare::getCenterY()
{
	return mCenterY;
}

int GridSquare::getCenterZ()
{
	return mCenterZ;
}

float GridSquare::getTextureStep()
{
	return mTextureStep;
}

std::deque<TexturedVertex*> GridSquare::getVertices()
{
	return mVertices;
}

void GridSquare::addVertex(TexturedVertex* vertex)
{
	mVertices.push_back(vertex);
}

TexturedVertex* GridSquare::findVertex(VertexXLocation xloc, VertexYLocation yLoc, VertexZLocation zLoc, CliffFacing cf)
{
	std::deque<TexturedVertex*>::iterator iter;

	for(iter = mVertices.begin(); iter != mVertices.end(); ++iter)
	{
		if((*iter)->XLocation == xloc && (*iter)->YLocation == yLoc && (*iter)->ZLocation == zLoc && (*iter)->CliffFace == cf)
			return (*iter);
	}

	return 0;
}

TexturedVertex* GridSquare::findVertex(VertexXLocation xloc, VertexYLocation yLoc, VertexZLocation zLoc,bool hasTextureCoordinates, CliffFacing cf)
{
	std::deque<TexturedVertex*>::iterator iter;

	for(iter = mVertices.begin(); iter != mVertices.end(); ++iter)
	{

		if((*iter)->XLocation == xloc && (*iter)->YLocation == yLoc && (*iter)->ZLocation == zLoc && (*iter)->CliffFace == cf && (*iter)->gotTextureCoords() == hasTextureCoordinates)
			return (*iter);
	}

	return 0;
}

int GridSquare::findIndex(VertexXLocation xloc, VertexYLocation yLoc, VertexZLocation zLoc, CliffFacing cf)
{
	TexturedVertex* vertex = findVertex(xloc, yLoc, zLoc, cf);

	if(vertex)
	{
		return vertex->index;
	}
	return 0;
}

bool GridSquare::isSloped()
{
	if(	Type == SquareType::SLOPED_NEG_X || 
		Type == SquareType::SLOPED_NEG_Z || 
		Type == SquareType::SLOPED_POS_X || 
		Type == SquareType::SLOPED_POS_Z || 
		Type == SquareType::CORNER_NEG_X_NEG_Z || 
		Type == SquareType::CORNER_NEG_X_POS_Z || 
		Type == SquareType::CORNER_POS_X_NEG_Z || 
		Type == SquareType::CORNER_POS_X_POS_Z ||
		Type == SquareType::INV_CORNER_NEG_X_NEG_Z || 
		Type == SquareType::INV_CORNER_NEG_X_POS_Z || 
		Type == SquareType::INV_CORNER_POS_X_NEG_Z || 
		Type == SquareType::INV_CORNER_POS_X_POS_Z ||
		Type == SquareType::TRANS_CLIFF_SLOPE_NEG_X || 
		Type == SquareType::TRANS_CLIFF_SLOPE_POS_X || 
		Type == SquareType::TRANS_CLIFF_SLOPE_NEG_Z || 
		Type == SquareType::TRANS_CLIFF_SLOPE_POS_Z ||
		Type == SquareType::TRANS_SLOPE_CLIFF_NEG_X || 
		Type == SquareType::TRANS_SLOPE_CLIFF_POS_X || 
		Type == SquareType::TRANS_SLOPE_CLIFF_NEG_Z || 
		Type == SquareType::TRANS_SLOPE_CLIFF_POS_Z)
		return true;

	return false;
}

void GridSquare::setAlphaSplatCoords(TexturedVertex* vertex)
{

	float value = this->isSloped() ? 0.5f : 1.0f;

	switch(mCenterY)
	{
	case 0:
		vertex->EZ = value;
		break;
	case 1:
		vertex->EW = value;
		break;
	case 2:
		vertex->FX = value;
		break;
	case 3:
		vertex->FY = value;
		break;
	case 4:
		vertex->FZ = value;
		break;
	case 5:
		vertex->FW = value;
	}
	
	if(this->isSloped() && mCenterY > 0)
	{

		switch(mCenterY - 1)
		{
		case 0:
			vertex->EZ = value;
			break;
		case 1:
			vertex->EW = value;
			break;
		case 2:
			vertex->FX = value;
			break;
		case 3:
			vertex->FY = value;
			break;
		case 4:
			vertex->FZ = value;
			break;
		case 5:
			vertex->FW = value;
		}
	}
	
}
