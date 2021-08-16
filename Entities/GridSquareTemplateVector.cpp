#include <Entities/GridSquareTemplateVector.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/Enums.h>
#include <Entities/GridSquare.h>
#include <Entities/GridSquareJoinTemplate.h>
#include <Entities/GridSquareJoinChecker.h>
#include <Entities/GridSquareTemplate.h>
#include <Entities/GridSquareVector.h>
#include <Managers/GameManager.h>
#include <Managers/SQLiteDBManager.h>
#include <Templates/TerrainTemplate.h>
#include <Ogre.h>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace Ogre;

GridSquareTemplateVector::GridSquareTemplateVector()
	:
	mSideLength(0),
	mTerrainTemplate(0)
{
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::FLAT));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_NEG_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_NEG_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_POS_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_POS_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_NEG_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_NEG_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_POS_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_POS_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_NEG_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_NEG_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_POS_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_POS_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::EMPTY));

}

GridSquareTemplateVector::GridSquareTemplateVector(TerrainTemplate* terrain, int size)
{
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::FLAT));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::SLOPED_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_NEG_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_NEG_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_POS_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_POS_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CLIFF_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_NEG_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_NEG_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_POS_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::CORNER_CLIFF_POS_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_NEG_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_NEG_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_POS_X_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::INV_CORNER_POS_X_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_CLIFF_SLOPE_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_NEG_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_NEG_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_POS_X));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::TRANS_SLOPE_CLIFF_POS_Z));
	mGridSquareJoinTemplates.push_back(new GridSquareJoinTemplate(SquareType::EMPTY));

	mTerrainTemplate = terrain;
	mSideLength = size + 1;

	mGridSquares.resize(mSideLength * mSideLength);

	int x(0), z(0);

	for(int i = 0; i < mSideLength * mSideLength; ++i)
	{
		z = i / mSideLength;
		x = i % mSideLength;

		if(z == (mSideLength - 1) || x == (mSideLength - 1))
		{
			GridSquareTemplate* square = new GridSquareTemplate(x, 0, z, SquareType::EMPTY, this);
			mGridSquares[i] = square;
		}
		else
		{
			GridSquareTemplate* square = new GridSquareTemplate(x, 0, z, SquareType::FLAT, this);
			mGridSquares[i] = square;
		}
	}

}

GridSquareTemplateVector::~GridSquareTemplateVector()
{
	std::vector<GridSquareTemplate*>::iterator iter;

	for(iter = mGridSquares.begin(); iter != mGridSquares.end(); ++iter)
	{
		delete (*iter);
	}
}

int GridSquareTemplateVector::getSideLength()
{
	return mSideLength;
}

void GridSquareTemplateVector::setDefaultGridHeight(int y)
{
	std::vector<GridSquareTemplate*>::iterator iter;

	for(iter = mGridSquares.begin(); iter != mGridSquares.end(); ++iter)
	{
		(*iter)->setY(y);
	}
}

GridSquareTemplate* GridSquareTemplateVector::getGridSquare(int index)
{
	return mGridSquares[index];
}

GridSquareTemplate* GridSquareTemplateVector::getGridSquare(int x, int z)
{
	return mGridSquares[(mSideLength * z) + x];
}

void GridSquareTemplateVector::changeSquare(int x, int z, int yDiff, bool createCliff)
{
	GridSquareTemplate* square = mGridSquares[(mSideLength * z) + x];

	if(square)
	{
		TerrainChange change(TerrainChange::TC_NO_CHANGE);
		if(square->getY() < yDiff)
			change = TerrainChange::TC_RAISING;
		else if(square->getY() > yDiff)
			change = TerrainChange::TC_LOWERING;
		square->setY(yDiff);
		
		if(square->getSquareType() != SquareType::FLAT)
			square->setSquareType(SquareType::FLAT);
		
		//equalizeHeight(square, yDiff > 0, createCliff);
		//fixSquareType(square);
		
		
		checkSurroundings(square, true, false, change);
		checkSurroundings(square, false, false, change);
	}
}

void GridSquareTemplateVector::changeSquare(int x, int z, SquareType newType)
{
	GridSquareTemplate* square = mGridSquares[(mSideLength * z) + x];

	if(square)
	{
		square->setSquareType(newType);
		//fixSquareType(square);

		checkSurroundings(square, true);
	}
}

void GridSquareTemplateVector::changeSquare(int x, int z, int yDiff, SquareType newType)
{
	GridSquareTemplate* square = mGridSquares[(mSideLength * z) + x];

	if(square)
	{
		square->setY(yDiff);
		square->setSquareType(newType);
		//fixSquareType(square);

		checkSurroundings(square, true);
	}
}

void GridSquareTemplateVector::createValleyOrRidge(int startX, int startZ, int endX, int endZ, int width, int targetY, bool createCliff)
{
	if(startX < 0)
	{
		startX = 0;
	}
	if(startX >= (mSideLength - 1))
	{
		startX = (mSideLength - 1);
	}
	if(startZ < 0)
	{
		startZ = 0;
	}
	if(startZ >= (mSideLength - 1))
	{
		startZ = (mSideLength - 1);
	}
	if(endX < 0)
	{
		endX = 0;
	}
	if(endX >= (mSideLength - 1))
	{
		endX = (mSideLength - 1);
	}
	if(endZ < 0)
	{
		endZ = 0;
	}
	if(endZ >= (mSideLength - 1))
	{
		endZ = (mSideLength - 1);
	}

	Vector3 origin = Vector3(startX, 0, startZ);
	Vector3 end = Vector3(endX, 0, endZ);
	Vector3 dir = end - origin;
	Real distance = dir.normalise();
	Vector3 perp = dir.crossProduct(Vector3(0,1,0));

	Vector3 originA = origin + (perp * width);
	Vector3 originB = origin - (perp * width);

	Vector3 endA = end + (perp * width);
	Vector3 endB = end - (perp * width);

	std::deque<GridSquareTemplate*> startLine = getSquaresBetweenPoints(originA.x, originA.z, originB.x, originB.z);
	std::deque<GridSquareTemplate*> endLine = getSquaresBetweenPoints(endA.x, endA.z, endB.x, endB.z);
	
	std::deque<GridSquareTemplate*>::iterator startIter = startLine.begin();
	std::deque<GridSquareTemplate*>::iterator endIter = endLine.begin();
	
	std::deque<GridSquareTemplate*> result;

	while(startIter != startLine.end() && endIter != endLine.end())
	{
		std::deque<GridSquareTemplate*> squares = getSquaresBetweenPoints((*startIter)->getX(), (*startIter)->getZ(), (*endIter)->getX(), (*endIter)->getZ());

		result.insert(result.begin(), squares.begin(), squares.end());
		++startIter;
		++endIter;
	}

	std::deque<GridSquareTemplate*>::iterator squareIter;

	for(squareIter = result.begin(); squareIter != result.end(); ++squareIter)
	{
		TerrainChange change(TerrainChange::TC_NO_CHANGE);
		if((*squareIter)->getY() < targetY)
			change = TerrainChange::TC_RAISING;
		else if((*squareIter)->getY() > targetY)
			change = TerrainChange::TC_LOWERING;
		(*squareIter)->setY(targetY);
		(*squareIter)->setSquareType(SquareType::FLAT);
		(*squareIter)->setTerrainChange(change);
		//adjustHeight((*squareIter), true);
		//fixType((*squareIter), true, createCliff, true, false);

		//checkSurroundings((*squareIter), true, createCliff, change, true);
	}
	
	
	for(squareIter = result.begin(); squareIter != result.end(); ++squareIter)
	{
		//adjustHeight((*squareIter), true);
		//fixType((*squareIter), true, createCliff, true, false);
		//adjustHeight((*squareIter), true, createCliff, (*squareIter)->getTerrainChange());
		checkSurroundings((*squareIter), true, createCliff, (*squareIter)->getTerrainChange(), true);
		
	}
	for(squareIter = result.begin(); squareIter != result.end(); ++squareIter)
	{
		//adjustHeight((*squareIter), true);
		//fixType((*squareIter), true, createCliff, true, false);
		//fixType((*squareIter), false, createCliff, (*squareIter)->getTerrainChange());
		checkSurroundings((*squareIter), false, createCliff, (*squareIter)->getTerrainChange());
		
	}
	
	int temp = 0;
	//checkSurroundings(mGridSquares[(1 * mSideLength) + 2],true,mGridSquares[(1 * mSideLength) + 2]);
	//checkSurroundings(mGridSquares[(3 * mSideLength) + 2],true,mGridSquares[(3 * mSideLength) + 2]);
	//checkSurroundings(mGridSquares[(2 * mSideLength) + 1],true,mGridSquares[(2 * mSideLength) + 2]);
	//checkSurroundings(mGridSquares[(2 * mSideLength) + 3],true,mGridSquares[(3 * mSideLength) + 2]);

}

void GridSquareTemplateVector::createPlateau(int startX, int startZ, int minRadius, int maxRadius, int targetY, bool createCliff)
{
	std::deque<GridSquareTemplate*> squares;
	std::deque<GridSquareTemplate*>::iterator iter;
	int currentRadius(maxRadius);
	
	while(currentRadius >= minRadius)
	{
		int x(currentRadius);
		int z = 0;
		int xChange = 1 - (currentRadius << 1);
		int yChange = 0;
		int radiusError = 0;

		int xLoc(0);
		int zLoc(0);
		while(x >= z)
		{
			xLoc = startX + x;
			zLoc = startZ + z;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}
			xLoc = startX + z;
			zLoc = startZ + x;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}
			xLoc = startX - x;
			zLoc = startZ + z;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}
			xLoc = startX - z;
			zLoc = startZ + x;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}
			xLoc = startX + x;
			zLoc = startZ - z;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}
			xLoc = startX + z;
			zLoc = startZ - x;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}
			xLoc = startX - x;
			zLoc = startZ - z;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}
			xLoc = startX - z;
			zLoc = startZ - x;
			if(xLoc >= 0 && xLoc < (mSideLength - 1) && zLoc >= 0 && zLoc < (mSideLength - 1))
			{
				GridSquareTemplate* square = mGridSquares[(mSideLength * zLoc) + xLoc];
				bool found(false);
				for(iter = squares.begin(); iter != squares.end(); ++iter)
				{
					if((*iter) == square)
						found = true;
				}
				if(!found)
					squares.push_back(mGridSquares[(mSideLength * zLoc) + xLoc]);
			}

			++z;
			radiusError += yChange;
			yChange += 2;

			if(((radiusError << 1) + xChange) > 0)
			{
				--x;
				radiusError += xChange;
				xChange += 2;
			}
		}
		--currentRadius;
	}

	for(iter = squares.begin(); iter != squares.end(); ++iter)
	{
		if((*iter)->getX() < 10 || (*iter)->getX() > 40 || (*iter)->getZ() < 10 || (*iter)->getZ() > 40)
		{
			int temp = 0;
		}
		TerrainChange change(TerrainChange::TC_NO_CHANGE);
		if((*iter)->getY() < targetY)
			change = TerrainChange::TC_RAISING;
		else if((*iter)->getY() > targetY)
			change = TerrainChange::TC_LOWERING;
		(*iter)->setY(targetY);
		(*iter)->setSquareType(SquareType::FLAT);
		(*iter)->setTerrainChange(change);
		//adjustHeight((*iter), true);
		//fixType((*iter), true, createCliff, true, true);
		//checkSurroundings((*iter), true, createCliff, change, true, true);
	}
	for(iter = squares.begin(); iter != squares.end(); ++iter)
	{
		//adjustHeight((*iter), true);
		//fixType((*iter), true, createCliff, true, true);
		checkSurroundings((*iter), true, createCliff, (*iter)->getTerrainChange(), true, true);
	}
	for(iter = squares.begin(); iter != squares.end(); ++iter)
	{
		checkSurroundings((*iter), false, createCliff, (*iter)->getTerrainChange(), false, true);
	}
	int temp = 0;
}

bool GridSquareTemplateVector::contains(std::vector<SquareType> types, SquareType type)
{
	std::vector<SquareType>::iterator iter;

	for(iter = types.begin(); iter != types.end(); ++iter)
	{
		if((*iter) == type)
			return true;
	}
	return false;
}

void GridSquareTemplateVector::checkSurroundings(GridSquareTemplate* square, bool forceOnce, bool createCliff, TerrainChange change, bool changeTransitions, bool isCircle)
{
	if(square->getX() == 2 && square->getZ() == 2)
		int temp = 0;
	
	GridSquareTemplate* current;

	std::deque<GridSquareTemplate*> neighbours;
	std::deque<GridSquareTemplate*> squares;
	std::deque<GridSquareTemplate*> visited;
	squares.push_back(square);
	
	while(!squares.empty())
	{
		current = squares.front();
		int numSameHeight(0);
		int numCardinalNeighbours(0);
		int numFlat(0);
		GridSquareTemplate* negX(0);
		GridSquareTemplate* posX(0);
		GridSquareTemplate* negZ(0);
		GridSquareTemplate* posZ(0);

		GridSquareTemplate* negXnegZ(0);
		GridSquareTemplate* negXposZ(0);
		GridSquareTemplate* posXnegZ(0);
		GridSquareTemplate* posXposZ(0);
		
		bool changeMade(false);
		neighbours.clear();

		if(current->getX() > 0)
		{
			negX = mGridSquares[(mSideLength * current->getZ()) + current->getX() - 1];
			neighbours.push_back(negX);
			if(negX->getY() == current->getY())
				++numSameHeight;
			if(negX->getSquareType() == SquareType::FLAT)
				++numFlat;
			++numCardinalNeighbours;
		}

		if(current->getZ() > 0)
		{
			negZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX()];
			neighbours.push_back(negZ);
			if(negZ->getY() == current->getY())
				++numSameHeight;
			if(negZ->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}

		if(current->getX() < (mSideLength - 1))
		{
			posX = mGridSquares[(mSideLength * current->getZ()) + current->getX() + 1];
			neighbours.push_back(posX);
			if(posX->getY() == current->getY())
				++numSameHeight;
			if(posX->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}

		if(current->getZ() < (mSideLength - 1))
		{
			posZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX()];
			neighbours.push_back(posZ);
			if(posZ->getY() == current->getY())
				++numSameHeight;
			if(posZ->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}
		if(current->getX() > 0 && current->getZ() > 0)
		{
			negXnegZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX() - 1];
			neighbours.push_back(negXnegZ);
		}

		if(current->getX() < (mSideLength - 1) && current->getZ() > 0)
		{
			posXnegZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX() + 1];
			neighbours.push_back(posXnegZ);
		}

		if(current->getX() > 0 && current->getZ() < (mSideLength - 1))
		{
			negXposZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX() - 1];
			neighbours.push_back(negXposZ);
		}

		if(current->getX() < (mSideLength - 1) && current->getZ() < (mSideLength - 1))
		{
			posXposZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX() + 1];
			neighbours.push_back(posXposZ);
		}
		if(current->getX() == 27 && current->getZ() == 10 && current->getSquareType() == SquareType::FLAT && current->getY() == 1)
			int temp = 0;
		if(current->getX() == 26 && current->getZ() == 10 && current->getSquareType() == SquareType::FLAT && current->getY() == 0 && posX->getY() == 1 && posX->getSquareType() == SquareType::FLAT)
			int temp = 0;

		if(current->getX() == (mSideLength - 1) || current->getZ() == (mSideLength - 1))
		{
			current->setSquareType(SquareType::EMPTY);
		}

		if(current->getSquareType() == SquareType::EMPTY)
		{
			visited.push_back(current);
			squares.pop_front();
			if(current->getX() == (mSideLength - 1) && negX)
			{
				current->setY(negX->getY());
			}
			if(current->getZ() == (mSideLength - 1) && negZ)
			{
				current->setY(negZ->getY());
			}
			continue;
		}
		std::deque<GridSquareTemplate*>::iterator iter;

		for(iter = neighbours.begin(); iter != neighbours.end(); ++iter)
		{
			if((*iter)->getY() < current->getY())
			{
				if((*iter)->getY() != (current->getY() - 1))
				{
					(*iter)->setY(current->getY() - 1);
					(*iter)->setSquareType(SquareType::FLAT);
					changeMade = true;
				}
			}
			else if((*iter)->getY() > current->getY())
			{
				if((*iter)->getY() != (current->getY() + 1))
				{
					(*iter)->setY(current->getY() + 1);
					(*iter)->setSquareType(SquareType::FLAT);
					changeMade = true;
				}
			}
		}
		
		
		if(negX && posX)
		{
			if(negX->getY() < current ->getY() && posX->getY() < current->getY() && change != TerrainChange::TC_LOWERING)
			{
				negX->setY(current->getY());
				posX->setY(current->getY());
				current->setSquareType(SquareType::FLAT);
				negX->setSquareType(SquareType::FLAT);
				posX->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() > current ->getY() && posX->getY() > current->getY() && change == TerrainChange::TC_LOWERING &&
				!(negX->getSquareType() == SquareType::SLOPED_POS_X || negX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negX->getSquareType() == SquareType::CORNER_POS_X_POS_Z) &&
				!(posX->getSquareType() == SquareType::SLOPED_NEG_X || posX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || posX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z) )
			{
				//current->setY(negX->getY());
				current->setSquareType(SquareType::FLAT);
				negX->setSquareType(SquareType::SLOPED_POS_X);
				posX->setSquareType(SquareType::SLOPED_NEG_X);
				changeMade = true;
			}
			if(negX->getY() > current ->getY() && posX->getY() > current->getY() && change != TerrainChange::TC_LOWERING && isCircle && false)
			{
				current->setY(negX->getY());
				current->setSquareType(SquareType::FLAT);
				negX->setSquareType(SquareType::FLAT);
				posX->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() < current ->getY() && posX->getY() < current->getY() && change == TerrainChange::TC_LOWERING)
			{
				current->setY(negX->getY());
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() == current->getY() && posX->getY() == current->getY() && negX->getSquareType() == SquareType::FLAT && posX->getSquareType() == SquareType::FLAT && current->getSquareType() != SquareType::FLAT)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}

			if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) && 
				(posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::SLOPED_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_NEG_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) && (posX->getSquareType() == SquareType::CLIFF_NEG_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::CLIFF_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(posX->getSquareType() == SquareType::CLIFF_NEG_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_NEG_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) &&
				(posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::SLOPED_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_POS_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z) && (posX->getSquareType() == SquareType::CLIFF_POS_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::CLIFF_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(posX->getSquareType() == SquareType::CLIFF_POS_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_POS_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z) &&
				(posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_POS_Z);
				changeMade = true;
			}
		}
		if(negZ && posZ)
		{
			if(negZ->getY() < current ->getY() && posZ->getY() < current->getY() && change != TerrainChange::TC_LOWERING)
			{
				negZ->setY(current->getY());
				posZ->setY(current->getY());
				current->setSquareType(SquareType::FLAT);
				negZ->setSquareType(SquareType::FLAT);
				posZ->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() > current ->getY() && posZ->getY() > current->getY() && change == TerrainChange::TC_LOWERING &&
				!(negZ->getSquareType() == SquareType::SLOPED_POS_Z || negZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z) &&
				!(posZ->getSquareType() == SquareType::SLOPED_NEG_Z || posZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || posZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z) )
			{
				//current->setY(negZ->getY());
				current->setSquareType(SquareType::FLAT);
				negZ->setSquareType(SquareType::SLOPED_POS_Z);
				posZ->setSquareType(SquareType::SLOPED_NEG_Z);
				changeMade = true;
			}
			if(negZ->getY() > current ->getY() && posZ->getY() > current->getY() && change != TerrainChange::TC_LOWERING && isCircle && false)
			{
				current->setY(negZ->getY());
				current->setSquareType(SquareType::FLAT);
				negZ->setSquareType(SquareType::FLAT);
				posZ->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() < current ->getY() && posZ->getY() < current->getY() && change == TerrainChange::TC_LOWERING)
			{
				current->setY(negZ->getY());
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() == current->getY() && posZ->getY() == current->getY() && negZ->getSquareType() == SquareType::FLAT && posZ->getSquareType() == SquareType::FLAT && current->getSquareType() != SquareType::FLAT)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if((negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				negZ->getY() == current->getY() && posZ->getY() == current->getY())
			{
				current->setSquareType(SquareType::SLOPED_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_NEG_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) && (posZ->getSquareType() == SquareType::CLIFF_NEG_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z))
			{
				current->setSquareType(SquareType::CLIFF_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::CLIFF_NEG_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_NEG_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				negZ->getY() == current->getY() && posZ->getY() == current->getY())
			{
				current->setSquareType(SquareType::SLOPED_POS_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_POS_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) && (posZ->getSquareType() == SquareType::CLIFF_POS_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::CLIFF_POS_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::CLIFF_POS_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_POS_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_POS_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_POS_X);
				changeMade = true;
			}
		}
		

		if(negX && posX && negZ && posZ)
		{
			if(negX->getY() == current->getY() && current->getY() == negZ->getY() && negZ->getY() == posZ->getY() && posZ->getY() == posX->getY() && current->getSquareType() != SquareType::FLAT)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
		}

		/*
		if(current->getSquareType() != SquareType::FLAT)
		{
			if(negX && negZ && posX && posZ)
			{
				if(numSameHeight >= (numCardinalNeighbours - 1) && numFlat >= 2 && negX->getSquareType() == SquareType::SLOPED_POS_X)
				{
					current->setSquareType(SquareType::FLAT);
					changeMade = true;
				}
			}
		}
		*/
		if(numSameHeight >= (numCardinalNeighbours - 2) && current->getSquareType() != SquareType::FLAT && !changeMade && changeTransitions)
		{
			current->setSquareType(SquareType::FLAT);
			changeMade = true;
		}

		bool test = true;
		
		if(negX)
		{
			if(current->getSquareType() == SquareType::SLOPED_NEG_X && negX->getSquareType() == SquareType::FLAT && negX->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_NEG_X);
				else	
					current->setSquareType(SquareType::CLIFF_NEG_X);
				changeMade = true;
			}
			if(negX->getY() == current->getY() && changeTransitions && current->getSquareType() == SquareType::SLOPED_NEG_X && negX->getSquareType() != SquareType::SLOPED_POS_X)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(!createCliff && test && posX && negZ && posZ)
			{
				if(negZ->getY() == current->getY() && posZ->getY() == current->getY())
				{
					if(negX->getY() < current->getY() && posX->getY() == current->getY())
					{
						if(negX->getSquareType() == SquareType::FLAT && 
							(posX->getSquareType() == SquareType::FLAT || posX->getSquareType() == SquareType::SLOPED_POS_X))
						{
							current->setSquareType(SquareType::SLOPED_NEG_X);
							changeMade = true;
						}
					}
					else if(negX->getY() == current->getY() && posX->getY() == current->getY())
					{
						if((negX->getSquareType() == SquareType::SLOPED_POS_X) && 
							(posX->getSquareType() == SquareType::FLAT || posX->getSquareType() == SquareType::SLOPED_POS_X))
						{
							current->setSquareType(SquareType::SLOPED_NEG_X);
							changeMade = true;
						}
					}
					else if(negX->getY() == current->getY() && posX->getY() < current->getY())
					{
						if((negX->getSquareType() == SquareType::FLAT || negX->getSquareType() == SquareType::SLOPED_NEG_X ) && 
							(posX->getSquareType() == SquareType::FLAT ))
						{
							current->setSquareType(SquareType::SLOPED_POS_X);
							changeMade = true;
						}
					}
				}
			}
		}
		if(posX)
		{
			if(current->getSquareType() == SquareType::SLOPED_POS_X && posX->getSquareType() == SquareType::FLAT && posX->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(posX->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_POS_X);
				else	
					current->setSquareType(SquareType::CLIFF_POS_X);
				changeMade = true;
			}
			if(posX->getY() == current->getY() && changeTransitions && current->getSquareType() == SquareType::SLOPED_POS_X && posX->getSquareType() != SquareType::SLOPED_NEG_X)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(!createCliff && test && negX && negZ && posZ)
			{
				if(negZ->getY() == current->getY() && posZ->getY() == current->getY())
				{
					if(negX->getY() < current->getY() && posX->getY() == current->getY())
					{
						if(negX->getSquareType() == SquareType::FLAT && 
							(posX->getSquareType() == SquareType::FLAT || posX->getSquareType() == SquareType::SLOPED_POS_X))
						{
							current->setSquareType(SquareType::SLOPED_NEG_X);
							changeMade = true;
						}
					}
					else if(negX->getY() == current->getY() && posX->getY() == current->getY())
					{
						if((negX->getSquareType() == SquareType::SLOPED_POS_X) && 
							(posX->getSquareType() == SquareType::FLAT || posX->getSquareType() == SquareType::SLOPED_POS_X))
						{
							current->setSquareType(SquareType::SLOPED_NEG_X);
							changeMade = true;
						}
					}
					else if(negX->getY() == current->getY() && posX->getY() < current->getY())
					{
						if((negX->getSquareType() == SquareType::FLAT || negX->getSquareType() == SquareType::SLOPED_NEG_X ) && 
							(posX->getSquareType() == SquareType::FLAT ))
						{
							current->setSquareType(SquareType::SLOPED_POS_X);
							changeMade = true;
						}
					}
				}
			}
		}
		if(negZ)
		{
			if(current->getSquareType() == SquareType::SLOPED_NEG_Z && negZ->getSquareType() == SquareType::FLAT && negZ->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_NEG_Z);
				else	
					current->setSquareType(SquareType::CLIFF_NEG_Z);
				changeMade = true;
			}
			if(negZ->getY() == current->getY() && changeTransitions && current->getSquareType() == SquareType::SLOPED_NEG_Z && negZ->getSquareType() != SquareType::SLOPED_POS_Z)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(!createCliff && test && posZ && negX && posX)
			{
				if(negX->getY() == current->getY() && posX->getY() == current->getY())
				{
					if(negZ->getY() < current->getY() && posZ->getY() == current->getY())
					{
						if(negZ->getSquareType() == SquareType::FLAT && 
							(posZ->getSquareType() == SquareType::FLAT || posZ->getSquareType() == SquareType::SLOPED_POS_Z))
						{
							current->setSquareType(SquareType::SLOPED_NEG_Z);
							changeMade = true;
						}
					}
					else if(negZ->getY() == current->getY() && posZ->getY() == current->getY())
					{
						if((negZ->getSquareType() == SquareType::SLOPED_POS_Z) && 
							(posZ->getSquareType() == SquareType::FLAT || posZ->getSquareType() == SquareType::SLOPED_POS_Z))
						{
							current->setSquareType(SquareType::SLOPED_NEG_Z);
							changeMade = true;
						}
					}
					else if(negZ->getY() == current->getY() && posZ->getY() < current->getY())
					{
						if((negZ->getSquareType() == SquareType::FLAT || negZ->getSquareType() == SquareType::SLOPED_NEG_Z ) && 
							(posZ->getSquareType() == SquareType::FLAT ))
						{
							current->setSquareType(SquareType::SLOPED_POS_Z);
							changeMade = true;
						}
					}
				}
			}
		}
		if(posZ)
		{
			if(current->getSquareType() == SquareType::SLOPED_POS_Z && posZ->getSquareType() == SquareType::FLAT && posZ->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(posZ->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_POS_Z);
				else	
					current->setSquareType(SquareType::CLIFF_POS_Z);
				changeMade = true;
			}
			if(posZ->getY() == current->getY() && changeTransitions && current->getSquareType() == SquareType::SLOPED_POS_Z && posZ->getSquareType() != SquareType::SLOPED_NEG_Z)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(!createCliff && test && negZ && negX && posX)
			{
				if(negX->getY() == current->getY() && posX->getY() == current->getY())
				{
					if(negZ->getY() < current->getY() && posZ->getY() == current->getY())
					{
						if(negZ->getSquareType() == SquareType::FLAT && 
							(posZ->getSquareType() == SquareType::FLAT || posZ->getSquareType() == SquareType::SLOPED_POS_Z))
						{
							current->setSquareType(SquareType::SLOPED_NEG_Z);
							changeMade = true;
						}
					}
					else if(negZ->getY() == current->getY() && posZ->getY() == current->getY())
					{
						if((negZ->getSquareType() == SquareType::SLOPED_POS_Z) && 
							(posZ->getSquareType() == SquareType::FLAT || posZ->getSquareType() == SquareType::SLOPED_POS_Z))
						{
							current->setSquareType(SquareType::SLOPED_NEG_Z);
							changeMade = true;
						}
					}
					else if(negZ->getY() == current->getY() && posZ->getY() < current->getY())
					{
						if((negZ->getSquareType() == SquareType::FLAT || negZ->getSquareType() == SquareType::SLOPED_NEG_Z ) && 
							(posZ->getSquareType() == SquareType::FLAT ))
						{
							current->setSquareType(SquareType::SLOPED_POS_Z);
							changeMade = true;
						}
					}
				}
			}
		}
		
		if(negX && negZ)
		{

			if(negX->getY() < current->getY() && negX->getY() == negZ->getY() && changeTransitions)
			{
				if((!posX || (posX && posX->getY() == current->getY() && posX->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z// && posX->getSquareType() != SquareType::CORNER_NEG_X_POS_Z && posX->getSquareType() != SquareType::CORNER_POS_X_POS_Z
					)) &&
					(!posZ || (posZ && posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z// && posZ->getSquareType() != SquareType::CORNER_POS_X_NEG_Z && posZ->getSquareType() != SquareType::CORNER_POS_X_POS_Z
					)))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_NEG_X_NEG_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_NEG_Z);
				
					changeMade = true;
				}
			}
			if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				negX->getY() == negZ->getY() && current->getY() == negX->getY() &&
				(!posX || (posX->getY() != current->getY()) || (posX->getY() == current->getY() && posX->getSquareType() != SquareType::SLOPED_NEG_Z && posX->getSquareType() != SquareType::INV_CORNER_NEG_X_NEG_Z)) &&
				(!posZ || (posZ->getY() != current->getY()) || (posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::SLOPED_NEG_X && posZ->getSquareType() != SquareType::INV_CORNER_NEG_X_NEG_Z)))
			{
				current->setSquareType(SquareType::INV_CORNER_NEG_X_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				negX->getY() == negZ->getY() &&	current->getY() == negX->getY() &&
				(!posX || (posX->getY() != current->getY()) || (posX->getY() == current->getY() && posX->getSquareType() != SquareType::SLOPED_POS_Z && posX->getSquareType() != SquareType::CORNER_POS_X_POS_Z)) &&
				(!posZ || (posZ->getY() != current->getY()) || (posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::SLOPED_POS_X && posZ->getSquareType() != SquareType::CORNER_POS_X_POS_Z)))
			{
				current->setSquareType(SquareType::CORNER_POS_X_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_POS_X || negX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) &&
				(negZ->getSquareType() == SquareType::CLIFF_POS_Z || negZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z) &&
				negX->getY() == negZ->getY() &&	current->getY() == negX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_POS_X_POS_Z);
				changeMade = true;
			}
		}
		if(negX && posZ)
		{
			if(negX->getY() < current->getY() && negX->getY() == posZ->getY() && changeTransitions)
			{
				if((!posX || (posX && posX->getY() == current->getY() && posX->getSquareType() != SquareType::CORNER_NEG_X_POS_Z
					//&& posX->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z
					// && posX->getSquareType() != SquareType::CORNER_POS_X_NEG_Z
					)) &&
					(!negZ || (negZ && negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::CORNER_NEG_X_POS_Z
					// && negZ->getSquareType() != SquareType::CORNER_POS_X_POS_Z 
					//&& negZ->getSquareType() != SquareType::CORNER_POS_X_NEG_Z
					)))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_NEG_X_POS_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_POS_Z);
				
					changeMade = true;
				}
			}
			if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				negX->getY() == posZ->getY() && current->getY() == negX->getY() &&
				(!posX || (posX->getY() != current->getY()) || (posX->getY() == current->getY() && posX->getSquareType() != SquareType::SLOPED_POS_Z && posX->getSquareType() != SquareType::INV_CORNER_NEG_X_POS_Z)) &&
				(!negZ || (negZ->getY() != current->getY()) || (negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::SLOPED_NEG_X && negZ->getSquareType() != SquareType::INV_CORNER_NEG_X_POS_Z)))
			{
				current->setSquareType(SquareType::INV_CORNER_NEG_X_POS_Z);
				changeMade = true;
				
			}
			else if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				negX->getY() == posZ->getY() && current->getY() == negX->getY() &&
				(!posX || (posX->getY() != current->getY()) || (posX->getY() == current->getY() && posX->getSquareType() != SquareType::SLOPED_NEG_Z && posX->getSquareType() != SquareType::CORNER_POS_X_NEG_Z)) &&
				(!negZ || (negZ->getY() != current->getY()) || (negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::SLOPED_POS_X && negZ->getSquareType() != SquareType::CORNER_POS_X_NEG_Z)))
			{
				current->setSquareType(SquareType::CORNER_POS_X_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_NEG_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::CLIFF_POS_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z) &&
				negX->getY() == posZ->getY() && current->getY() == negX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_POS_X_NEG_Z);
				changeMade = true;
			}
		}
		if(posX && negZ)
		{
			if(posX->getY() < current->getY() && posX->getY() == negZ->getY() && changeTransitions)
			{
				if((!negX || (negX && negX->getY() == current->getY() && negX->getSquareType() != SquareType::CORNER_POS_X_NEG_Z// && negX->getSquareType() != SquareType::CORNER_POS_X_POS_Z && negX->getSquareType() != SquareType::CORNER_NEG_X_POS_Z
					)) &&
					(!posZ || (posZ && posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::CORNER_POS_X_NEG_Z// && posZ->getSquareType() != SquareType::CORNER_POS_X_NEG_Z && posZ->getSquareType() != SquareType::CORNER_NEG_X_POS_Z
					)))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_POS_X_NEG_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_POS_X_NEG_Z);
				
					changeMade = true;
				}
			}
			if((posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				posX->getY() == negZ->getY() && current->getY() == posX->getY() &&
				(!negX || (negX->getY() != current->getY()) || (negX->getY() == current->getY() && negX->getSquareType() != SquareType::SLOPED_NEG_Z && negX->getSquareType() != SquareType::INV_CORNER_POS_X_NEG_Z)) &&
				(!posZ || (posZ->getY() != current->getY()) || (posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::SLOPED_POS_X && posZ->getSquareType() != SquareType::INV_CORNER_POS_X_NEG_Z)))
			{
				current->setSquareType(SquareType::INV_CORNER_POS_X_NEG_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				posX->getY() == negZ->getY() && current->getY() == posX->getY() &&
				(!negX || (negX->getY() != current->getY()) || (negX->getY() == current->getY() && negX->getSquareType() != SquareType::SLOPED_POS_Z && negX->getSquareType() != SquareType::CORNER_NEG_X_POS_Z)) &&
				(!posZ || (posZ->getY() != current->getY()) || (posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::SLOPED_NEG_X && posZ->getSquareType() != SquareType::CORNER_NEG_X_POS_Z)))
			{
				current->setSquareType(SquareType::CORNER_NEG_X_POS_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::CLIFF_POS_Z || posX->getSquareType() == SquareType::CLIFF_POS_Z) &&
				(negZ->getSquareType() == SquareType::CLIFF_NEG_X || negZ->getSquareType() == SquareType::CLIFF_NEG_X) &&
				posX->getY() == negZ->getY() && current->getY() == posX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_POS_Z);
				changeMade = true;
			}
		}
		if(posX && posZ)
		{
			if(posX->getY() < current->getY() && posX->getY() == posZ->getY() && changeTransitions)
			{
				if((!negX || (negX && negX->getY() == current->getY() && negX->getSquareType() != SquareType::CORNER_POS_X_POS_Z// && negX->getSquareType() != SquareType::CORNER_POS_X_NEG_Z
					)) &&
					(!negZ || (negZ && negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::CORNER_POS_X_POS_Z// && negZ->getSquareType() != SquareType::CORNER_NEG_X_POS_Z
					)))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_POS_X_POS_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_POS_X_POS_Z);
				
					changeMade = true;
				}
			}
			if((posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				posX->getY() == posZ->getY() && current->getY() == posX->getY() &&
				(!negX || (negX->getY() != current->getY()) || (negX->getY() == current->getY() && negX->getSquareType() != SquareType::SLOPED_POS_Z && negX->getSquareType() != SquareType::INV_CORNER_POS_X_POS_Z)) &&
				(!negZ || (negZ->getY() != current->getY()) || (negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::SLOPED_POS_X && negZ->getSquareType() != SquareType::INV_CORNER_POS_X_POS_Z)))
			{
				current->setSquareType(SquareType::INV_CORNER_POS_X_POS_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				posX->getY() == posZ->getY() && current->getY() == posX->getY()	 &&
				(!negX || (negX->getY() != current->getY()) || (negX->getY() == current->getY() && negX->getSquareType() != SquareType::SLOPED_NEG_Z && negX->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z)) &&
				(!negZ || (negZ->getY() != current->getY()) || (negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::SLOPED_NEG_X && negZ->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z)))
			{
				current->setSquareType(SquareType::CORNER_NEG_X_NEG_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z) &&
				posX->getY() == posZ->getY() && current->getY() == posX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_NEG_Z);
				changeMade = true;
			}
		}
		
		if(current->getX() == 5 && current->getZ() == 2 && current->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z)
			int temp = 0;


		bool corrected = current->correctType(negX, posX, negZ, posZ);

		if(!corrected)
		{
			if(negX && !current->isNegXJoinValid(negX))
			{
				if(current->isSloped() && negX->isSloped())
				{
					current->setSquareType(SquareType::FLAT);
					negX->setSquareType(SquareType::FLAT);
					
					if(current->getY() == negX->getY())
					{
						if(current->getY() > 0)
						{
							current->setY(current->getY() - 1);
							negX->setY(negX->getY() - 1);
						}
					}
					else if(current->getY() < negX->getY())
					{
						negX->setY(current->getY());
					}
					else
					{
						current->setY(negX->getY());
					}
				}
			}
			if(posX && !current->isPosXJoinValid(posX))
			{
				if(current->isSloped() && posX->isSloped())
				{
					current->setSquareType(SquareType::FLAT);
					posX->setSquareType(SquareType::FLAT);
					
					if(current->getY() == posX->getY())
					{
						if(current->getY() > 0)
						{
							current->setY(current->getY() - 1);
							posX->setY(posX->getY() - 1);
						}
					}
					else if(current->getY() < posX->getY())
					{
						posX->setY(current->getY());
					}
					else
					{
						current->setY(posX->getY());
					}
				}
			}
			if(negZ && !current->isNegZJoinValid(negZ))
			{
				if(current->isSloped() && negZ->isSloped())
				{
					current->setSquareType(SquareType::FLAT);
					negZ->setSquareType(SquareType::FLAT);
					
					if(current->getY() == negZ->getY())
					{
						if(current->getY() > 0)
						{
							current->setY(current->getY() - 1);
							negZ->setY(negZ->getY() - 1);
						}
					}
					else if(current->getY() < negZ->getY())
					{
						negZ->setY(current->getY());
					}
					else
					{
						current->setY(negZ->getY());
					}
				}
			}
			if(posZ && !current->isPosZJoinValid(posZ))
			{
				if(current->isSloped() && posZ->isSloped())
				{
					current->setSquareType(SquareType::FLAT);
					posZ->setSquareType(SquareType::FLAT);
					
					if(current->getY() == posZ->getY())
					{
						if(current->getY() > 0)
						{
							current->setY(current->getY() - 1);
							posZ->setY(posZ->getY() - 1);
						}
					}
					else if(current->getY() < posZ->getY())
					{
						posZ->setY(current->getY());
					}
					else
					{
						current->setY(posZ->getY());
					}
				}
			}
		}

		visited.push_back(current);
		squares.pop_front();

		if(changeMade || forceOnce)
		{
			forceOnce = false;
			if(isCircle)
			{
				switch(change)
				{
				case TerrainChange::TC_LOWERING:
					change = TerrainChange::TC_RAISING;
					break;
				case TerrainChange::TC_NO_CHANGE:
					change = TerrainChange::TC_LOWERING;
					break;
				case TerrainChange::TC_RAISING:
					change = TerrainChange::TC_NO_CHANGE;
					break;
				}
			}

			bool foundNegX(false), foundPosX(false), foundNegZ(false), foundPosZ(false);
			bool visitedNegX(false), visitedPosX(false), visitedNegZ(false), visitedPosZ(false);

			for(iter = squares.begin(); iter != squares.end(); ++iter)
			{
				if((*iter) == negX)
					foundNegX = true;

				if((*iter) == posX)
					foundPosX = true;

				if((*iter) == negZ)
					foundNegZ = true;
				
				if((*iter) == posZ)
					foundPosZ = true;
			}

			for(iter = visited.begin(); iter != visited.end(); ++iter)
			{
				if((*iter) == negX)
					visitedNegX = true;

				if((*iter) == posX)
					visitedPosX = true;

				if((*iter) == negZ)
					visitedNegZ = true;
				
				if((*iter) == posZ)
					visitedPosZ = true;
			}
			if(!foundNegX && !visitedNegX && negX)
				squares.push_back(negX);
			if(!foundPosX && !visitedPosX && posX)
				squares.push_back(posX);
			if(!foundNegZ && !visitedNegZ && negZ)
				squares.push_back(negZ);
			if(!foundPosZ && !visitedPosZ && posZ)
				squares.push_back(posZ);
		}
	}
	/*
	std::deque<GridSquareTemplate*>::iterator vIter;
	for(vIter = visited.begin(); vIter != visited.end(); ++vIter)
	{
		fixSquareType((*vIter));
	}
	*/
}

void GridSquareTemplateVector::equalizeHeight(GridSquareTemplate* square, bool raised, bool createCliff)
{
	if(square->getX() == 2 && square->getZ() == 2)
		int temp = 0;

	if(square->getX() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * square->getZ()) + square->getX() - 1];
		
		if(neighbour)
		{
			if(neighbour->getX() == 2 && neighbour->getZ() == 2)
				int temp = 0;

			if(raised && neighbour->getY() == square->getY() && !isValid(square, neighbour))
			{
				neighbour->setSquareType(SquareType::FLAT);
				//fixSquareType(neighbour);
			}
			else if(raised && neighbour->getY() < square->getY())
			{
				neighbour->setY(square->getY());
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_NEG_X);
				else
					neighbour->setSquareType(SquareType::CLIFF_NEG_X);
			}
			else if(!raised && neighbour->getY() > square->getY())
			{
				neighbour->setY(square->getY() + 1);
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_POS_X);
				else
					neighbour->setSquareType(SquareType::CLIFF_POS_X);
			}
			//checkSurroundings(neighbour, true, neighbour);
		}
	}

	if(square->getZ() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() - 1)) + square->getX()];
		
		if(neighbour)
		{
			if(neighbour->getX() == 2 && neighbour->getZ() == 2)
				int temp = 0;

			if(raised && neighbour->getY() == square->getY() && !isValid(square, neighbour))
			{
				neighbour->setSquareType(SquareType::FLAT);
				//fixSquareType(neighbour);
			}
			else if(raised && neighbour->getY() < square->getY())
			{
				neighbour->setY(square->getY());
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_NEG_Z);
				else
					neighbour->setSquareType(SquareType::CLIFF_NEG_Z);
			}
			else if(!raised && neighbour->getY() > square->getY())
			{
				neighbour->setY(square->getY() + 1);
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_POS_Z);
				else
					neighbour->setSquareType(SquareType::CLIFF_POS_Z);
			}
			//checkSurroundings(neighbour, true, neighbour);
		}
	}

	if(square->getX() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * square->getZ()) + square->getX() + 1];
		
		if(neighbour)
		{
			if(neighbour->getX() == 2 && neighbour->getZ() == 2)
				int temp = 0;

			if(raised && neighbour->getY() == square->getY() && !isValid(square, neighbour))
			{
				neighbour->setSquareType(SquareType::FLAT);
				//fixSquareType(neighbour);
			}
			else if(raised && neighbour->getY() < square->getY())
			{
				neighbour->setY(square->getY());
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_POS_X);
				else
					neighbour->setSquareType(SquareType::CLIFF_POS_X);
			}
			else if(!raised && neighbour->getY() > square->getY())
			{
				neighbour->setY(square->getY() + 1);
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_NEG_X);
				else
					neighbour->setSquareType(SquareType::CLIFF_NEG_X);
			}
			//checkSurroundings(neighbour, true, neighbour);
		}
	}

	if(square->getZ() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() + 1)) + square->getX()];
		
		if(neighbour)
		{
			if(neighbour->getX() == 2 && neighbour->getZ() == 2)
				int temp = 0;

			if(raised && neighbour->getY() == square->getY() && !isValid(square, neighbour))
			{
				neighbour->setSquareType(SquareType::FLAT);
				//fixSquareType(neighbour);
			}
			else if(raised && neighbour->getY() < square->getY())
			{
				neighbour->setY(square->getY());
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_POS_Z);
				else
					neighbour->setSquareType(SquareType::CLIFF_POS_Z);
			}
			else if(!raised && neighbour->getY() > square->getY())
			{
				neighbour->setY(square->getY() + 1);
				if(!createCliff)
					neighbour->setSquareType(SquareType::SLOPED_NEG_Z);
				else
					neighbour->setSquareType(SquareType::CLIFF_NEG_Z);
			}
			//checkSurroundings(neighbour, true, neighbour);
		}
	}
}

bool GridSquareTemplateVector::isValid(GridSquareTemplate* square1, GridSquareTemplate* square2)
{
	if(square2->getX() == 2 && square2->getZ() == 2 && square2->getY() == 1 && square2->getSquareType() == SquareType::FLAT)
				int temp = 0;

	return GridSquareJoinChecker::getSingletonPtr()->checkValidity(square1, square2);
}

bool GridSquareTemplateVector::isValid2(GridSquareTemplate* square, GridSquareTemplate* originator)
{
	bool result(true);


	std::deque<GridSquareTemplate*> neighbours;
	
	int numNeighbours(0);

	if(square->getX() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * square->getZ()) + square->getX() - 1];
		if(neighbour != originator)
		{
			neighbours.push_back(neighbour);
			++numNeighbours;
		}
	}

	if(square->getZ() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() - 1)) + square->getX()];
		if(neighbour != originator)
		{
			neighbours.push_back(neighbour);
			++numNeighbours;
		}
	}

	if(square->getX() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * square->getZ()) + square->getX() + 1];
		if(neighbour != originator)
		{
			neighbours.push_back(neighbour);
			++numNeighbours;
		}
	}

	if(square->getZ() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() + 1)) + square->getX()];
		if(neighbour != originator)
		{
			neighbours.push_back(neighbour);
			++numNeighbours;
		}
	}

	std::deque<GridSquareTemplate*>::iterator neighbourIterator;
	std::deque<SquareType>::iterator iter;
	for(neighbourIterator = neighbours.begin(); neighbourIterator != neighbours.end(); ++neighbourIterator)
	{
		result = result && isValid(square, (*neighbourIterator));
	}

	return result;
}

bool GridSquareTemplateVector::fixSquareType(GridSquareTemplate* square)
{	
	bool fixed = false;

	if(square->getX() == 2 && square->getZ() == 2)
		int temp = 0;
	
	int solutions [GridSquare::NumSquareTypes];

	for(int i = 0; i < GridSquare::NumSquareTypes; ++i)
	{
		solutions[i] = 0;
	}

	std::deque<GridSquareTemplate*> neighbours;
	
	int numNeighbours(0);

	if(square->getX() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * square->getZ()) + square->getX() - 1];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}

	if(square->getZ() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() - 1)) + square->getX()];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}

	if(square->getX() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * square->getZ()) + square->getX() + 1];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}

	if(square->getZ() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() + 1)) + square->getX()];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}
	/*
	if(square->getX() > 0 && square->getZ() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() - 1)) + square->getX() - 1];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}

	if(square->getX() < (mSideLength - 1) && square->getZ() > 0)
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() - 1)) + square->getX() + 1];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}

	if(square->getX() > 0 && square->getZ() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() + 1)) + square->getX() - 1];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}

	if(square->getX() < (mSideLength - 1) && square->getZ() < (mSideLength - 1))
	{
		GridSquareTemplate* neighbour = mGridSquares[(mSideLength * (square->getZ() + 1)) + square->getX() + 1];
		neighbours.push_back(neighbour);
		++numNeighbours;
	}
	*/
	std::deque<GridSquareTemplate*>::iterator neighbourIterator;
	std::deque<SquareType> allowed;
	std::deque<SquareType>::iterator iter;
	for(neighbourIterator = neighbours.begin(); neighbourIterator != neighbours.end(); ++neighbourIterator)
	{
		allowed = GridSquareJoinChecker::getSingletonPtr()->getAllowedTypes(square, (*neighbourIterator), square->getX() > (*neighbourIterator)->getX() || square->getZ() > (*neighbourIterator)->getZ());
		
		for(iter = allowed.begin(); iter != allowed.end(); ++iter)
		{
			++solutions[(int)(*iter)];
		}
	}

	for(int i = 0; i < GridSquare::NumSquareTypes; ++i)
	{
		if(solutions[i] >= (numNeighbours / 2) + 1)
		{
			if(square->getSquareType() != (SquareType)i)
				fixed = true;
			square->setSquareType((SquareType)i);
			return fixed;
		}
	}
	
	//if we cant find a suitable match, adjust its height upwards by one unit and try again.
	square->setY(square->getY() + 1);

	for(int i = 0; i < GridSquare::NumSquareTypes; ++i)
	{
		solutions[i] = 0;
	}
	
	for(neighbourIterator = neighbours.begin(); neighbourIterator != neighbours.end(); ++neighbourIterator)
	{
		allowed = GridSquareJoinChecker::getSingletonPtr()->getAllowedTypes(square, (*neighbourIterator), square->getX() > (*neighbourIterator)->getX() || square->getZ() > (*neighbourIterator)->getZ());
		
		for(iter = allowed.begin(); iter != allowed.end(); ++iter)
		{
			++solutions[(int)(*iter)];
		}
	}

	for(int i = 0; i < GridSquare::NumSquareTypes; ++i)
	{
		if(solutions[i] >= (numNeighbours / 2) + 1)
		{
			if(square->getSquareType() != (SquareType)i)
				fixed = true;
			square->setSquareType((SquareType)i);
			return fixed;
		}
	}

	//if we still cant find a match, adjust the height downwards by 2 units (ie original minus 1) and try again
	
	square->setY(square->getY() - 2);
	for(int i = 0; i < GridSquare::NumSquareTypes; ++i)
	{
		solutions[i] = 0;
	}

	for(neighbourIterator = neighbours.begin(); neighbourIterator != neighbours.end(); ++neighbourIterator)
	{
		allowed = GridSquareJoinChecker::getSingletonPtr()->getAllowedTypes(square, (*neighbourIterator), square->getX() > (*neighbourIterator)->getX() || square->getZ() > (*neighbourIterator)->getZ());
		
		for(iter = allowed.begin(); iter != allowed.end(); ++iter)
		{
			++solutions[(int)(*iter)];
		}
	}

	for(int i = 0; i < GridSquare::NumSquareTypes; ++i)
	{
		if(solutions[i] >= (numNeighbours / 2) + 1)
		{
			if(square->getSquareType() != (SquareType)i)
				fixed = true;
			square->setSquareType((SquareType)i);
			return fixed;
		}
	}
	square->setY(square->getY() + 1);
	return false;
}

std::deque<GridSquareTemplate*> GridSquareTemplateVector::getSquaresBetweenPoints(int startX, int startZ, int endX, int endZ)
{
	std::deque<GridSquareTemplate*> result;
	int dx = abs( endX - startX );

	int dz = abs( endZ - startZ );
	int sx(0), sz(0);

	if(startX < endX)
		sx = 1;
	else if (startX > endX)
		sx = -1;

	if(startZ < endZ)
		sz = 1;
	else if (startZ > endZ)
		sz = -1;

	if(sx == 0 && sz == 0)
	{
		return result;
	}

	int cX(0), cZ(0), e2(0);
	cX = startX;
	cZ = startZ;

	if(sx == 0)
	{
		cZ+=sz;
		while(cZ != endZ && cZ >= 0 && cZ < mSideLength)
		{
			result.push_back(mGridSquares[(mSideLength * cZ) + cX]);
			cZ +=sz;
		}
		return result;
	}

	if(sz == 0)
	{
		cX+=sx;
		while(cX != endX && cX >= 0 && cX < mSideLength)
		{
			result.push_back(mGridSquares[(mSideLength * cZ) + cX]);
			cX +=sx;
		}
		return result;
	}
		
	int err = dx - dz;
	while(cX != endX && cZ != endZ && cX >= 0 && cZ >= 0)
	{
		e2 = 2 * err;
		if(e2 > -dz)
		{
			err-=dz;
			cX+=sx;
		}
		if(e2 < dx)
		{
			err+=dx;
			cZ+=sz;
		}

		result.push_back(mGridSquares[(mSideLength * cZ) + cX]);
	}

	return result;
}

bool GridSquareTemplateVector::checkEntireGrid()
{
	int passes(0);
	bool gridIsValid(false);
	

	while(!gridIsValid)
	{
		
		bool changeMade(false);

		for(int i = 0; i < mSideLength * mSideLength; ++i)
		{
			GridSquareTemplate* current = mGridSquares[i];
			
			GridSquareTemplate* negX(0);
			GridSquareTemplate* posX(0);
			GridSquareTemplate* negZ(0);
			GridSquareTemplate* posZ(0);


			if(current->getX() > 0)
			{
				negX = mGridSquares[(mSideLength * current->getZ()) + current->getX() - 1];
			}

			if(current->getZ() > 0)
			{
				negZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX()];
			}

			if(current->getX() < (mSideLength - 1))
			{
				posX = mGridSquares[(mSideLength * current->getZ()) + current->getX() + 1];
			}

			if(current->getZ() < (mSideLength - 1))
			{
				posZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX()];
			}

			if(current->getX() == 19 && current->getZ() == 8)
			{
				int temp = 0;
			}
			if(current->getX() == 20 && current->getZ() == 9)
			{
				int temp = 0;
			}
			
			checkSurroundings(mGridSquares[i],false,false, TerrainChange::TC_NO_CHANGE, false);
		}
		if(!changeMade)
		{
			gridIsValid = true;
		}
		++passes;

		if(passes > 1000)
		{
			return false;
		}
	}

	for(int i = 0; i < mSideLength * mSideLength; ++i)
	{
		if(!mGridSquares[i]->allJoinsValid())
			return false;
	}
	return true;
}

void GridSquareTemplateVector::adjustHeight(GridSquareTemplate* square, bool forceOnce, bool createCliff, TerrainChange change, bool changeTransitions, bool isCircle)
{
	GridSquareTemplate* current;

	std::deque<GridSquareTemplate*> neighbours;
	std::deque<GridSquareTemplate*> squares;
	std::deque<GridSquareTemplate*> visited;
	squares.push_back(square);
	while(!squares.empty())
	{
		current = squares.front();
		
		int numSameHeight(0);
		int numCardinalNeighbours(0);
		int numFlat(0);
		GridSquareTemplate* negX(0);
		GridSquareTemplate* posX(0);
		GridSquareTemplate* negZ(0);
		GridSquareTemplate* posZ(0);

		GridSquareTemplate* negXnegZ(0);
		GridSquareTemplate* negXposZ(0);
		GridSquareTemplate* posXnegZ(0);
		GridSquareTemplate* posXposZ(0);
		
		bool changeMade(false);
		neighbours.clear();

		if(current->getX() > 0)
		{
			negX = mGridSquares[(mSideLength * current->getZ()) + current->getX() - 1];
			neighbours.push_back(negX);
			if(negX->getY() == current->getY())
				++numSameHeight;
			if(negX->getSquareType() == SquareType::FLAT)
				++numFlat;
			++numCardinalNeighbours;
		}

		if(current->getZ() > 0)
		{
			negZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX()];
			neighbours.push_back(negZ);
			if(negZ->getY() == current->getY())
				++numSameHeight;
			if(negZ->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}

		if(current->getX() < (mSideLength - 1))
		{
			posX = mGridSquares[(mSideLength * current->getZ()) + current->getX() + 1];
			neighbours.push_back(posX);
			if(posX->getY() == current->getY())
				++numSameHeight;
			if(posX->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}

		if(current->getZ() < (mSideLength - 1))
		{
			posZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX()];
			neighbours.push_back(posZ);
			if(posZ->getY() == current->getY())
				++numSameHeight;
			if(posZ->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}
		if(current->getX() > 0 && current->getZ() > 0)
		{
			negXnegZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX() - 1];
			neighbours.push_back(negXnegZ);
		}

		if(current->getX() < (mSideLength - 1) && current->getZ() > 0)
		{
			posXnegZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX() + 1];
			neighbours.push_back(posXnegZ);
		}

		if(current->getX() > 0 && current->getZ() < (mSideLength - 1))
		{
			negXposZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX() - 1];
			neighbours.push_back(negXposZ);
		}

		if(current->getX() < (mSideLength - 1) && current->getZ() < (mSideLength - 1))
		{
			posXposZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX() + 1];
			neighbours.push_back(posXposZ);
		}
		if(current->getX() == (mSideLength - 1) || current->getZ() == (mSideLength - 1))
		{
			current->setSquareType(SquareType::EMPTY);
		}

		if(current->getSquareType() == SquareType::EMPTY)
		{
			visited.push_back(current);
			squares.pop_front();
			if(current->getX() == (mSideLength - 1) && negX)
			{
				current->setY(negX->getY());
			}
			if(current->getZ() == (mSideLength - 1) && negZ)
			{
				current->setY(negZ->getY());
			}
			continue;
		}
		std::deque<GridSquareTemplate*>::iterator iter;

		for(iter = neighbours.begin(); iter != neighbours.end(); ++iter)
		{
			if((*iter)->getY() < current->getY())
			{
				if((*iter)->getY() != (current->getY() - 1))
				{
					(*iter)->setY(current->getY() - 1);
					(*iter)->setSquareType(SquareType::FLAT);
					changeMade = true;
				}
			}
			else if((*iter)->getY() > current->getY())
			{
				if((*iter)->getY() != (current->getY() + 1))
				{
					(*iter)->setY(current->getY() + 1);
					(*iter)->setSquareType(SquareType::FLAT);
					changeMade = true;
				}
			}
		}

		
		if(negX && posX)
		{
			if(negX->getY() < current ->getY() && posX->getY() < current->getY() && change != TerrainChange::TC_LOWERING)
			{
				negX->setY(current->getY());
				posX->setY(current->getY());
				current->setSquareType(SquareType::FLAT);
				negX->setSquareType(SquareType::FLAT);
				posX->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() > current ->getY() && posX->getY() > current->getY() && change == TerrainChange::TC_LOWERING)
			{
				//current->setY(negX->getY());
				current->setSquareType(SquareType::FLAT);
				negX->setSquareType(SquareType::SLOPED_POS_X);
				posX->setSquareType(SquareType::SLOPED_NEG_X);
				changeMade = true;
			}
			if(negX->getY() > current ->getY() && posX->getY() > current->getY() && change != TerrainChange::TC_LOWERING && isCircle)
			{
				current->setY(negX->getY());
				current->setSquareType(SquareType::FLAT);
				negX->setSquareType(SquareType::FLAT);
				posX->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() < current ->getY() && posX->getY() < current->getY() && change == TerrainChange::TC_LOWERING)
			{
				current->setY(negX->getY());
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() == current->getY() && posX->getY() == current->getY() && negX->getSquareType() == SquareType::FLAT && posX->getSquareType() == SquareType::FLAT && current->getSquareType() != SquareType::FLAT)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}

		}
		if(negZ && posZ)
		{
			if(negZ->getY() < current ->getY() && posZ->getY() < current->getY() && change != TerrainChange::TC_LOWERING)
			{
				negZ->setY(current->getY());
				posZ->setY(current->getY());
				current->setSquareType(SquareType::FLAT);
				negZ->setSquareType(SquareType::FLAT);
				posZ->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() > current ->getY() && posZ->getY() > current->getY() && change == TerrainChange::TC_LOWERING)
			{
				//current->setY(negZ->getY());
				current->setSquareType(SquareType::FLAT);
				negZ->setSquareType(SquareType::SLOPED_POS_Z);
				posZ->setSquareType(SquareType::SLOPED_NEG_Z);
				changeMade = true;
			}
			if(negZ->getY() > current ->getY() && posZ->getY() > current->getY() && change != TerrainChange::TC_LOWERING && isCircle)
			{
				current->setY(negZ->getY());
				current->setSquareType(SquareType::FLAT);
				negZ->setSquareType(SquareType::FLAT);
				posZ->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() < current ->getY() && posZ->getY() < current->getY() && change == TerrainChange::TC_LOWERING)
			{
				current->setY(negZ->getY());
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() == current->getY() && posZ->getY() == current->getY() && negZ->getSquareType() == SquareType::FLAT && posZ->getSquareType() == SquareType::FLAT && current->getSquareType() != SquareType::FLAT)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
		}

		visited.push_back(current);
		squares.pop_front();

		if(changeMade || forceOnce)
		{
			forceOnce = false;
			
			bool foundNegX(false), foundPosX(false), foundNegZ(false), foundPosZ(false);
			bool visitedNegX(false), visitedPosX(false), visitedNegZ(false), visitedPosZ(false);

			for(iter = squares.begin(); iter != squares.end(); ++iter)
			{
				if((*iter) == negX)
					foundNegX = true;

				if((*iter) == posX)
					foundPosX = true;

				if((*iter) == negZ)
					foundNegZ = true;
				
				if((*iter) == posZ)
					foundPosZ = true;
			}

			for(iter = visited.begin(); iter != visited.end(); ++iter)
			{
				if((*iter) == negX)
					visitedNegX = true;

				if((*iter) == posX)
					visitedPosX = true;

				if((*iter) == negZ)
					visitedNegZ = true;
				
				if((*iter) == posZ)
					visitedPosZ = true;
			}
			if(!foundNegX && !visitedNegX && negX)
				squares.push_back(negX);
			if(!foundPosX && !visitedPosX && posX)
				squares.push_back(posX);
			if(!foundNegZ && !visitedNegZ && negZ)
				squares.push_back(negZ);
			if(!foundPosZ && !visitedPosZ && posZ)
				squares.push_back(posZ);
		}
	}
}

void GridSquareTemplateVector::fixType(GridSquareTemplate* square, bool forceOnce, bool createCliff, TerrainChange change, bool changeTransitions, bool isCircle)
{
	GridSquareTemplate* current;

	std::deque<GridSquareTemplate*> neighbours;
	std::deque<GridSquareTemplate*> squares;
	std::deque<GridSquareTemplate*> visited;
	squares.push_back(square);

	while(!squares.empty())
	{
		current = squares.front();
		int numSameHeight(0);
		int numCardinalNeighbours(0);
		int numFlat(0);
		GridSquareTemplate* negX(0);
		GridSquareTemplate* posX(0);
		GridSquareTemplate* negZ(0);
		GridSquareTemplate* posZ(0);

		GridSquareTemplate* negXnegZ(0);
		GridSquareTemplate* negXposZ(0);
		GridSquareTemplate* posXnegZ(0);
		GridSquareTemplate* posXposZ(0);
		
		bool changeMade(false);
		neighbours.clear();

		if(current->getX() > 0)
		{
			negX = mGridSquares[(mSideLength * current->getZ()) + current->getX() - 1];
			neighbours.push_back(negX);
			if(negX->getY() == current->getY())
				++numSameHeight;
			if(negX->getSquareType() == SquareType::FLAT)
				++numFlat;
			++numCardinalNeighbours;
		}

		if(current->getZ() > 0)
		{
			negZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX()];
			neighbours.push_back(negZ);
			if(negZ->getY() == current->getY())
				++numSameHeight;
			if(negZ->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}

		if(current->getX() < (mSideLength - 1))
		{
			posX = mGridSquares[(mSideLength * current->getZ()) + current->getX() + 1];
			neighbours.push_back(posX);
			if(posX->getY() == current->getY())
				++numSameHeight;
			if(posX->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}

		if(current->getZ() < (mSideLength - 1))
		{
			posZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX()];
			neighbours.push_back(posZ);
			if(posZ->getY() == current->getY())
				++numSameHeight;
			if(posZ->getSquareType() == SquareType::FLAT)
				++numFlat;

			++numCardinalNeighbours;
		}
		if(current->getX() > 0 && current->getZ() > 0)
		{
			negXnegZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX() - 1];
			neighbours.push_back(negXnegZ);
		}

		if(current->getX() < (mSideLength - 1) && current->getZ() > 0)
		{
			posXnegZ = mGridSquares[(mSideLength * (current->getZ() - 1)) + current->getX() + 1];
			neighbours.push_back(posXnegZ);
		}

		if(current->getX() > 0 && current->getZ() < (mSideLength - 1))
		{
			negXposZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX() - 1];
			neighbours.push_back(negXposZ);
		}

		if(current->getX() < (mSideLength - 1) && current->getZ() < (mSideLength - 1))
		{
			posXposZ = mGridSquares[(mSideLength * (current->getZ() + 1)) + current->getX() + 1];
			neighbours.push_back(posXposZ);
		}
		if(current->getX() == (mSideLength - 1) || current->getZ() == (mSideLength - 1))
		{
			current->setSquareType(SquareType::EMPTY);
		}

		std::deque<GridSquareTemplate*>::iterator iter;

		if(current->getX() == 41 && current->getZ() == 49 && current->getSquareType() != SquareType::FLAT && negZ->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z)
			int temp = 0;

		
		if(negX && posX && negZ && posZ)
		{
			if(negX->getY() == current->getY() && current->getY() == negZ->getY() && negZ->getY() == posZ->getY() && posZ->getY() == posX->getY() && current->getSquareType() != SquareType::FLAT)
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
		}

		if(negX)
		{
			if(current->getSquareType() == SquareType::SLOPED_NEG_X && negX->getSquareType() == SquareType::FLAT && negX->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negX->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_NEG_X);
				else	
					current->setSquareType(SquareType::CLIFF_NEG_X);
				changeMade = true;
			}
			if(posX && posX->getY() == current->getY() && negX->getY() == current->getY() && !createCliff && false)
			{
				if((negX->getSquareType() == SquareType::SLOPED_POS_X || negX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negX->getSquareType() == SquareType::CORNER_POS_X_POS_Z) &&
					(posX->getSquareType() == SquareType::SLOPED_NEG_X || negX->getSquareType() == SquareType::FLAT))
				{
					current->setSquareType(SquareType::SLOPED_POS_X);
					changeMade = true;
				}
			}
		}
		if(posX)
		{
			if(current->getSquareType() == SquareType::SLOPED_POS_X && posX->getSquareType() == SquareType::FLAT && posX->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(posX->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_POS_X);
				else	
					current->setSquareType(SquareType::CLIFF_POS_X);
				changeMade = true;
			}
			if(negX && negX->getY() == current->getY() && posX->getY() == current->getY() && !createCliff && false)
			{
				if((negX->getSquareType() == SquareType::SLOPED_POS_X || negX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negX->getSquareType() == SquareType::CORNER_POS_X_POS_Z) &&
					(posX->getSquareType() == SquareType::SLOPED_NEG_X || negX->getSquareType() == SquareType::FLAT))
				{
					current->setSquareType(SquareType::SLOPED_POS_X);
					changeMade = true;
				}
			}	
		}
		if(negZ)
		{
			if(current->getSquareType() == SquareType::SLOPED_NEG_Z && negZ->getSquareType() == SquareType::FLAT && negZ->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(negZ->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_NEG_Z);
				else	
					current->setSquareType(SquareType::CLIFF_NEG_Z);
				changeMade = true;
			}
			if(posZ && posZ->getY() == current->getY() && negZ->getY() == current->getY() && !createCliff && false)
			{
				if((negZ->getSquareType() == SquareType::SLOPED_POS_Z || negZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z) &&
					(posZ->getSquareType() == SquareType::SLOPED_NEG_Z || posZ->getSquareType() == SquareType::FLAT))
				{
					current->setSquareType(SquareType::SLOPED_POS_Z);
					changeMade = true;
				}
				if((negZ->getSquareType() == SquareType::SLOPED_NEG_Z || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z) &&
					(posZ->getSquareType() == SquareType::SLOPED_POS_Z || posZ->getSquareType() == SquareType::FLAT))
				{
					current->setSquareType(SquareType::SLOPED_NEG_Z);
					changeMade = true;
				}
			}
		}
		if(posZ)
		{
			if(current->getSquareType() == SquareType::SLOPED_POS_Z && posZ->getSquareType() == SquareType::FLAT && posZ->getY() == current->getY())
			{
				current->setSquareType(SquareType::FLAT);
				changeMade = true;
			}
			if(posZ->getY() < current->getY() && changeTransitions)
			{
				if(!createCliff)
					current->setSquareType(SquareType::SLOPED_POS_Z);
				else	
					current->setSquareType(SquareType::CLIFF_POS_Z);
				changeMade = true;
			}
			if(negZ && negZ->getY() == current->getY() && posZ->getY() == current->getY() && !createCliff && false)
			{
				if((negZ->getSquareType() == SquareType::SLOPED_POS_Z || negZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z) &&
					(posZ->getSquareType() == SquareType::SLOPED_NEG_Z || posZ->getSquareType() == SquareType::FLAT))
				{
					current->setSquareType(SquareType::SLOPED_POS_Z);
					changeMade = true;
				}
				if((negZ->getSquareType() == SquareType::SLOPED_NEG_Z || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z) &&
					(posZ->getSquareType() == SquareType::SLOPED_POS_Z || posZ->getSquareType() == SquareType::FLAT))
				{
					current->setSquareType(SquareType::SLOPED_NEG_Z);
					changeMade = true;
				}
			}
		}

		if(current->getSquareType() != SquareType::FLAT)
		{
			if(negX && negZ && posX && posZ)
			{
				if(numSameHeight >= (numCardinalNeighbours - 1) && numFlat >= 2 && negX->getSquareType() == SquareType::SLOPED_POS_X)
				{
					current->setSquareType(SquareType::FLAT);
					changeMade = true;
				}
			}
		}
		/*
		if(numSameHeight >= (numCardinalNeighbours - 1) && current->getSquareType() != SquareType::FLAT && !changeMade)
		{
			current->setSquareType(SquareType::FLAT);
			changeMade = true;
		}
		*/
		if(negX && posX)
		{
			

			if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) && 
				(posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::SLOPED_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_NEG_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) && (posX->getSquareType() == SquareType::CLIFF_NEG_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::CLIFF_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(posX->getSquareType() == SquareType::CLIFF_NEG_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_NEG_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) &&
				(posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::SLOPED_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_POS_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z) && (posX->getSquareType() == SquareType::CLIFF_POS_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::CLIFF_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(posX->getSquareType() == SquareType::CLIFF_POS_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_POS_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z) &&
				(posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_POS_Z);
				changeMade = true;
			}
		}
		if(negZ && posZ)
		{
			
			if((negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z))
			{
				current->setSquareType(SquareType::SLOPED_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_NEG_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) && (posZ->getSquareType() == SquareType::CLIFF_NEG_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z))
			{
				current->setSquareType(SquareType::CLIFF_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::CLIFF_NEG_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_NEG_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_NEG_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::SLOPED_POS_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_POS_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) && (posZ->getSquareType() == SquareType::CLIFF_POS_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::CLIFF_POS_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::CLIFF_POS_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z))
			{
				current->setSquareType(SquareType::TRANS_SLOPE_CLIFF_POS_X);
				changeMade = true;
			}
			else if((negZ->getSquareType() == SquareType::CLIFF_POS_X || negZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z))
			{
				current->setSquareType(SquareType::TRANS_CLIFF_SLOPE_POS_X);
				changeMade = true;
			}
		}


		if(negX && negZ)
		{

			if(negX->getY() < current->getY() && negX->getY() == negZ->getY() && changeTransitions)
			{
				if((!posX || (posX && posX->getY() == current->getY())) && (!posZ || (posZ && posZ->getY() == current->getY())))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_NEG_X_NEG_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_NEG_Z);
				
					changeMade = true;
				}
			}
			if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				negX->getY() == negZ->getY() && current->getY() == negX->getY() &&
				(!posX || (posX && posX->getSquareType() != SquareType::SLOPED_NEG_Z)))
			{
				current->setSquareType(SquareType::INV_CORNER_NEG_X_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				negX->getY() == negZ->getY() &&	current->getY() == negX->getY() &&
				(!posX || (posX->getY() != current->getY()) || (posX->getY() == current->getY() && posX->getSquareType() != SquareType::CORNER_POS_X_POS_Z)) &&
				(!posZ || (posZ->getY() != current->getY()) || (posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::CORNER_POS_X_POS_Z)))
			{
				current->setSquareType(SquareType::CORNER_POS_X_POS_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_POS_X || negX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) &&
				(negZ->getSquareType() == SquareType::CLIFF_POS_Z || negZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z) &&
				negX->getY() == negZ->getY() &&	current->getY() == negX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_POS_X_POS_Z);
				changeMade = true;
			}
		}
		if(negX && posZ)
		{
			if(negX->getY() < current->getY() && negX->getY() == posZ->getY() && changeTransitions)
			{
				if((!posX || (posX && posX->getY() == current->getY())) && (!negZ || (negZ && negZ->getY() == current->getY())))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_NEG_X_POS_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_POS_Z);
				
					changeMade = true;
				}
			}
			if((negX->getSquareType() == SquareType::SLOPED_POS_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				negX->getY() == posZ->getY() && current->getY() == negX->getY())
			{
				current->setSquareType(SquareType::INV_CORNER_NEG_X_POS_Z);
				changeMade = true;
				
			}
			else if((negX->getSquareType() == SquareType::SLOPED_NEG_Z || negX->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negX->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				negX->getY() == posZ->getY() && current->getY() == negX->getY() &&
				(!negZ || (negZ->getY() != current->getY()) || (negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::CORNER_POS_X_NEG_Z)) &&
				(!posX || (posX->getY() != current->getY()) || (posX->getY() == current->getY() && posX->getSquareType() != SquareType::CORNER_POS_X_NEG_Z)))
			{
				current->setSquareType(SquareType::CORNER_POS_X_NEG_Z);
				changeMade = true;
			}
			else if((negX->getSquareType() == SquareType::CLIFF_NEG_Z || negX->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::CLIFF_POS_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_POS_X_POS_Z) &&
				negX->getY() == posZ->getY() && current->getY() == negX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_POS_X_NEG_Z);
				changeMade = true;
			}
		}
		if(posX && negZ)
		{
			if(posX->getY() < current->getY() && posX->getY() == negZ->getY() && changeTransitions)
			{
				if((!negX || (negX && negX->getY() == current->getY())) && (!posZ || (posZ && posZ->getY() == current->getY())))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_POS_X_NEG_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_POS_X_NEG_Z);
				
					changeMade = true;
				}
			}
			if((posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_POS_X || negZ->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_POS_X_POS_Z) &&
				posX->getY() == negZ->getY() && current->getY() == posX->getY())
			{
				current->setSquareType(SquareType::INV_CORNER_POS_X_NEG_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(negZ->getSquareType() == SquareType::SLOPED_NEG_X || negZ->getSquareType() == SquareType::CORNER_NEG_X_NEG_Z || negZ->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				posX->getY() == negZ->getY() && current->getY() == posX->getY() &&
				(!negX || (negX->getY() != current->getY()) || (negX->getY() == current->getY() && negX->getSquareType() != SquareType::CORNER_NEG_X_POS_Z)) &&
				(!posZ || (posZ->getY() != current->getY()) || (posZ->getY() == current->getY() && posZ->getSquareType() != SquareType::CORNER_NEG_X_POS_Z)))
			{
				current->setSquareType(SquareType::CORNER_NEG_X_POS_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::CLIFF_POS_Z || posX->getSquareType() == SquareType::CLIFF_POS_Z) &&
				(negZ->getSquareType() == SquareType::CLIFF_NEG_X || negZ->getSquareType() == SquareType::CLIFF_NEG_X) &&
				posX->getY() == negZ->getY() && current->getY() == posX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_POS_Z);
				changeMade = true;
			}
		}
		if(posX && posZ)
		{
			if(posX->getY() < current->getY() && posX->getY() == posZ->getY() && changeTransitions)
			{
				if((!negX || (negX && negX->getY() == current->getY())) && (!negZ || (negZ && negZ->getY() == current->getY())))
				{
					if(!createCliff)
						current->setSquareType(SquareType::CORNER_POS_X_POS_Z);
					else
						current->setSquareType(SquareType::CORNER_CLIFF_POS_X_POS_Z);
				
					changeMade = true;
				}
			}
			if((posX->getSquareType() == SquareType::SLOPED_POS_Z || posX->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_POS_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_POS_X || posZ->getSquareType() == SquareType::CORNER_POS_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_POS_X_NEG_Z) &&
				posX->getY() == posZ->getY() && current->getY() == posX->getY() &&
				(!negX || (negX && negX->getSquareType() != SquareType::SLOPED_POS_Z)))
			{
				current->setSquareType(SquareType::INV_CORNER_POS_X_POS_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_POS_X_NEG_Z || posX->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_NEG_X_POS_Z || posZ->getSquareType() == SquareType::INV_CORNER_NEG_X_NEG_Z) &&
				posX->getY() == posZ->getY() && current->getY() == posX->getY() 
				&&
				(!negX || (negX->getY() != current->getY()) || (negX->getY() == current->getY() && negX->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z)) &&
				(!negZ || (negZ->getY() != current->getY()) || (negZ->getY() == current->getY() && negZ->getSquareType() != SquareType::CORNER_NEG_X_NEG_Z))
				)
			{
				current->setSquareType(SquareType::CORNER_NEG_X_NEG_Z);
				changeMade = true;
			}
			else if((posX->getSquareType() == SquareType::SLOPED_NEG_Z || posX->getSquareType() == SquareType::CORNER_CLIFF_POS_X_NEG_Z) &&
				(posZ->getSquareType() == SquareType::SLOPED_NEG_X || posZ->getSquareType() == SquareType::CORNER_CLIFF_NEG_X_POS_Z) &&
				posX->getY() == posZ->getY() && current->getY() == posX->getY())
			{
				current->setSquareType(SquareType::CORNER_CLIFF_NEG_X_NEG_Z);
				changeMade = true;
			}
		}


		visited.push_back(current);
		squares.pop_front();

		if(changeMade || forceOnce)
		{
			forceOnce = false;
			if(isCircle)
			{
				switch(change)
				{
				case TerrainChange::TC_LOWERING:
					change = TerrainChange::TC_RAISING;
					break;
				case TerrainChange::TC_NO_CHANGE:
					change = TerrainChange::TC_LOWERING;
					break;
				case TerrainChange::TC_RAISING:
					change = TerrainChange::TC_NO_CHANGE;
					break;
				}
			}

			bool foundNegX(false), foundPosX(false), foundNegZ(false), foundPosZ(false);
			bool visitedNegX(false), visitedPosX(false), visitedNegZ(false), visitedPosZ(false);

			for(iter = squares.begin(); iter != squares.end(); ++iter)
			{
				if((*iter) == negX)
					foundNegX = true;

				if((*iter) == posX)
					foundPosX = true;

				if((*iter) == negZ)
					foundNegZ = true;
				
				if((*iter) == posZ)
					foundPosZ = true;
			}

			for(iter = visited.begin(); iter != visited.end(); ++iter)
			{
				if((*iter) == negX)
					visitedNegX = true;

				if((*iter) == posX)
					visitedPosX = true;

				if((*iter) == negZ)
					visitedNegZ = true;
				
				if((*iter) == posZ)
					visitedPosZ = true;
			}
			if(!foundNegX && !visitedNegX && negX)
				squares.push_back(negX);
			if(!foundPosX && !visitedPosX && posX)
				squares.push_back(posX);
			if(!foundNegZ && !visitedNegZ && negZ)
				squares.push_back(negZ);
			if(!foundPosZ && !visitedPosZ && posZ)
				squares.push_back(posZ);
		}
	}
	/*
	std::deque<GridSquareTemplate*>::iterator vIter;
	for(vIter = visited.begin(); vIter != visited.end(); ++vIter)
	{
		fixSquareType((*vIter));
	}
	*/
}

void GridSquareTemplateVector::startRecursiveSubdivision()
{
	recursivelySubdivide(mSideLength, mSideLength, 0, mSideLength, 0, 0, mSideLength, 0);
	//checkEntireGrid();
}

void GridSquareTemplateVector::writeToFile(const std::string& fileName)
{
	std::vector<GridSquareTemplate*>::iterator iter;
	
	std::ofstream outputFile;
	outputFile.open(fileName,std::ios_base::out);
	outputFile << SQLiteDBManager::convertIntToString(mSideLength) << std::endl;
	for(iter = mGridSquares.begin(); iter != mGridSquares.end(); ++iter)
	{

		outputFile << SQLiteDBManager::convertIntToString((*iter)->getY()) << "|" << SQLiteDBManager::convertIntToString((*iter)->getSquareType()) << std::endl;
	}
	
	outputFile.close();
}

void GridSquareTemplateVector::loadFromFile(const std::string& fileName)
{
	std::vector<GridSquareTemplate*>::iterator iter;
	for(iter = mGridSquares.begin(); iter != mGridSquares.end(); ++iter)
	{
		delete (*iter);
	}
	std::ifstream inputFile;
	std::string line;
	inputFile.open(fileName, std::ios_base::in);
	
	std::getline(inputFile, line);
	mSideLength = std::atoi(line.c_str());
	
	mGridSquares.resize(mSideLength * mSideLength);
	int x(0), z(0);

	for(int i = 0; i < mSideLength * mSideLength; ++i)
	{
		z = i / mSideLength;
		x = i % mSideLength;

		std::getline(inputFile, line);
		int y = std::atoi(line.substr(0, line.find('|')).c_str());
		int type = std::atoi(line.substr(line.find('|') + 1).c_str());
		GridSquareTemplate* square = new GridSquareTemplate(x, y, z, (SquareType)type, this);
		mGridSquares[i] = square;
		
	}
}

void GridSquareTemplateVector::recursivelySubdivide(int topLeftX, int topLeftZ, int topRightX, int topRightZ, int bottomRightX, int bottomRightZ, int bottomLeftX, int bottomLeftZ)
{
	int length = topLeftX - topRightX;
	if(length <= 1)
		return;
	else
	{
		int half = (int)(length / 2);
		int midX = bottomRightX + half;
		int midZ = bottomRightZ + half;
		int height = rand() % 2;
		changeSquare(midX, midZ, height,false);

		recursivelySubdivide(topLeftX, topLeftZ, midX, topRightZ, midX, midZ, bottomLeftX, midZ);
		recursivelySubdivide(midX, topLeftZ, topRightX, topRightZ, topRightX, midZ, midX, midZ);
		recursivelySubdivide(midX, midZ, topRightX, midZ, bottomRightX, bottomRightZ, midX, bottomRightZ);
		recursivelySubdivide(bottomLeftX, midZ, midX, midZ, midX, bottomRightZ, bottomLeftX, bottomLeftZ);
	}
}


std::deque<GridSquareJoinTemplate*>& GridSquareTemplateVector::getGridSquareJoinTemplates()
{
	return mGridSquareJoinTemplates;
}

GridSquareJoinTemplate* GridSquareTemplateVector::getGridSquareJoinTemplate(SquareType type)
{
	std::deque<GridSquareJoinTemplate*>::iterator iter;

	for(iter = mGridSquareJoinTemplates.begin(); iter != mGridSquareJoinTemplates.end(); ++iter)
	{
		if((*iter)->getSquareType() == type)
			return (*iter);
	}
	return 0;
}
