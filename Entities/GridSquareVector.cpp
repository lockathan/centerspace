#include <Entities/GridSquareVector.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <Entities/GridSquare.h>
#include <Entities/GridSquareTemplate.h>
#include <Entities/GridSquareTemplateVector.h>
#include <Managers/GameManager.h>
#include <Templates/TerrainTemplate.h>
#include <Templates/TerrainTextureTemplate.h>

#include <Ogre.h>
using namespace Ogre;

GridSquareVector::GridSquareVector(BattlefieldGrid* grid)
	:
	mBattlefieldGrid(grid),
	mCurrentVertex(0),
	mCurrentIndex(0),
	mNumVertices(0),
	mSideLength(grid->getSideLength() + 1),
	mTotalLength((grid->getSideLength() + 1) * (grid->getSideLength() + 1)),
	mGridSquareTemplateVector(grid->getGridSquareTemplateVector())
{
	mGridSquareTemplateVector->writeToFile("terrain.txt");
	mGridSquares.resize(mTotalLength);
	mVertexCache.resize(mTotalLength * 4);
	
	float maxHeight = 0;

	for(int i = 0; i < mSideLength * mSideLength; ++i)
	{
		this->addSquare(mGridSquareTemplateVector->getGridSquare(i));
		if(mGridSquareTemplateVector->getGridSquare(i)->getY() * BattlefieldGrid::UnitSize > maxHeight)
			maxHeight = mGridSquareTemplateVector->getGridSquare(i)->getY() * BattlefieldGrid::UnitSize;
	}

	mBattlefield = GameManager::getSingletonPtr()->getSceneManager()->createManualObject("Layers");
	this->initialize();

	
	MaterialPtr terrainTemplate = Ogre::MaterialManager::getSingleton().load("diffuse_template2","InGame");
	if(MaterialManager::getSingleton().resourceExists("TerrainMaterial"))
	{
		MaterialManager::getSingleton().remove("TerrainMaterial");
	}
	MaterialPtr terrainMaterial = terrainTemplate->clone("TerrainMaterial");
	Ogre::AliasTextureNamePairList textureList;
	TerrainTemplate* terrain = mBattlefieldGrid->getTerrainTemplate();

	if(terrain->getTerrainTextureTemplate(0))
		textureList["diffuse_tex1"] = terrain->getTerrainTextureTemplate(0)->getTextureName();
	if(terrain->getTerrainTextureTemplate(1))
		textureList["diffuse_tex2"] = terrain->getTerrainTextureTemplate(1)->getTextureName();
	if(terrain->getTerrainTextureTemplate(2))
		textureList["diffuse_tex3"] = terrain->getTerrainTextureTemplate(2)->getTextureName();
	if(terrain->getTerrainTextureTemplate(3))
		textureList["diffuse_tex4"] = terrain->getTerrainTextureTemplate(3)->getTextureName();
	if(terrain->getTerrainTextureTemplate(4))
		textureList["diffuse_tex5"] = terrain->getTerrainTextureTemplate(4)->getTextureName();
	if(terrain->getTerrainTextureTemplate(5))
		textureList["diffuse_tex6"] = terrain->getTerrainTextureTemplate(5)->getTextureName();
	if(terrain->getTerrainTextureTemplate(6))
		textureList["diffuse_tex7"] = terrain->getTerrainTextureTemplate(6)->getTextureName();
	if(terrain->getTerrainTextureTemplate(7))
		textureList["grid_texture"] = terrain->getTerrainTextureTemplate(7)->getTextureName();

	terrainMaterial->applyTextureAliases(textureList);
	mBattlefield->begin("TerrainMaterial", RenderOperation::OT_TRIANGLE_LIST);
	
	
	//mBattlefield->begin(mBattlefieldGrid->getTerrainTemplate()->getMaterialName(), RenderOperation::OT_TRIANGLE_LIST);

	
	std::vector<TexturedVertex*> vertices;
	std::deque<float> mFXs;

	while(this->hasMoreVertices())
	{
		TexturedVertex* vertex = this->getNextVertex();
		vertices.push_back(vertex);
		mBattlefield->position(vertex->getVertexXYZ());
		mBattlefield->normal(vertex->Normal);
		mBattlefield->textureCoord(getFogTextureCoord(vertex->x), getFogTextureCoord(vertex->z));
		mBattlefield->textureCoord(vertex->u,vertex->v);
		if(vertex->FX != 0 || vertex->FX < 0 || vertex->FX > 0)
		{
			mFXs.push_back(vertex->FX);
		}
		mBattlefield->textureCoord(vertex->EX, vertex->EY, vertex->EZ, vertex->EW);
		mBattlefield->textureCoord(vertex->FX, vertex->FY, vertex->FZ, vertex->FW);
		
	}

	int idx0 = 0, idx1 = 0, idx2 = 0;


	std::vector<Vector3> indices;

	while(this->hasMoreIndices())
	{

		idx0 = this->getNextIndex();
		idx1 = this->getNextIndex();
		idx2 = this->getNextIndex();

		indices.push_back(Vector3(idx0, idx1, idx2));

		mBattlefield->triangle(idx0,idx1,idx2);
	}
	

	mBattlefield->end();
	
	Ogre::EdgeData::EdgeGroupList::iterator itShadow, itEndShadow;
	for( itShadow=mBattlefield->getEdgeList()->edgeGroups.begin(), itEndShadow=mBattlefield->getEdgeList()->edgeGroups.end(); itShadow!=itEndShadow; itShadow++ )
	   const_cast<Ogre::VertexData*>((*itShadow).vertexData)->prepareForShadowVolume();

	mBattlefieldGrid->setMaxVertexHeight(maxHeight);
	setGridCells();
}

GridSquareVector::~GridSquareVector()
{
	std::vector<GridSquare*>::iterator iter;

	for(iter = mGridSquares.begin(); iter != mGridSquares.end(); ++iter)
	{
		delete (*iter);
	}
}

void GridSquareVector::setGridCells()
{
	std::vector<GridSquare*>::iterator iter;

	for(iter = mGridSquares.begin(); iter != mGridSquares.end(); ++iter)
	{
		(*iter)->reset();
	}

	int x = 0, z = 0;
	float maxRoughness(0.f);

	for(iter = mGridSquares.begin(); iter != mGridSquares.end(); ++iter)
	{
		if((*iter)->Type != SquareType::EMPTY)
		{
			float roughness =  mBattlefieldGrid->getTerrainTemplate()->getTerrainTextureTemplate((*iter)->getCenterY())->getRoughness() ;
			if( roughness > maxRoughness)
				maxRoughness =  roughness;

			BattlefieldGridCell* cell = new BattlefieldGridCell((*iter), roughness);

			std::deque<TexturedVertex*> vertices = (*iter)->getVertices();

			if(vertices.size() > 0)
			{
				TexturedVertex* vertex = 0;

				vertex = (*iter)->findVertex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NA);
				if(vertex)
					cell->setNegXNegZVertex(vertex->getVertexXYZ());
				else
				{
					vertex = (*iter)->findVertex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NA);
					cell->setNegXNegZVertex(vertex->getVertexXYZ());
				}

				vertex = (*iter)->findVertex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NA);
				if(vertex)
					cell->setPosXNegZVertex(vertex->getVertexXYZ());
				else
				{
					vertex = (*iter)->findVertex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_NEG_Z, CliffFacing::CF_NA);
					cell->setPosXNegZVertex(vertex->getVertexXYZ());
				}

				vertex = (*iter)->findVertex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NA);
				if(vertex)
					cell->setNegXPosZVertex(vertex->getVertexXYZ());
				else
				{
					vertex = (*iter)->findVertex(VertexXLocation::VL_NEG_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NA);
					cell->setNegXPosZVertex(vertex->getVertexXYZ());
				}

				vertex = (*iter)->findVertex(VertexXLocation::VL_POS_X, VertexYLocation::VL_POS_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NA);
				if(vertex)
					cell->setPosXPosZVertex(vertex->getVertexXYZ());
				else
				{
					vertex = (*iter)->findVertex(VertexXLocation::VL_POS_X, VertexYLocation::VL_NEG_Y, VertexZLocation::VL_POS_Z, CliffFacing::CF_NA);
					cell->setPosXPosZVertex(vertex->getVertexXYZ());
				}
				mBattlefieldGrid->setGridCell((cell->getZ() * (mSideLength - 1)) + cell->getX(), cell);
			}
		}
	}
	mBattlefieldGrid->setMaxRoughness(maxRoughness);

}

void GridSquareVector::initialize()
{
	mCurrentVertex = 0;
	mCurrentIndex = 0;
	mVertexCache.shrink_to_fit();
	computeIndices();
}

GridSquare* GridSquareVector::getGridSquare(int x, int z)
{

	return mGridSquares[(mSideLength * z) + x];
}

void GridSquareVector::addSquare(GridSquareTemplate* squareTemplate)
{
	GridSquare* square = new GridSquare(this, squareTemplate);
	mGridSquares[(mSideLength * squareTemplate->getZ()) + squareTemplate->getX()] = square;
}

void GridSquareVector::computeIndices()
{

	for(int i = 0;i < mTotalLength; ++i)
	{
		mGridSquares[i]->computeIndices();
	}

	return ;
}

void GridSquareVector::resetGridSquares()
{
	mCurrentIndex = 0;
	mCurrentVertex = 0;
	for(int i = 0;i < mTotalLength; ++i)
	{
		mGridSquares[i]->reset();
	}
}

bool GridSquareVector::hasMoreVertices()
{
	return mCurrentVertex < mNumVertices;
}

TexturedVertex* GridSquareVector::getNextVertex()
{
	TexturedVertex* result = mVertexCache[mCurrentVertex];
	++mCurrentVertex;
	return result;
}

bool GridSquareVector::hasMoreIndices()
{
	while(mCurrentIndex < mTotalLength && !mGridSquares[mCurrentIndex]->hasMoreIndices())
	{
		++mCurrentIndex;
	}
	
	if(mCurrentIndex == mTotalLength - 1)
		return mGridSquares[mCurrentIndex]->hasMoreIndices();

	if(mCurrentIndex >= mTotalLength)
		return false;

	return true;
}

int GridSquareVector::getNextIndex()
{
	if(mGridSquares[mCurrentIndex]->hasMoreIndices())
		return mGridSquares[mCurrentIndex]->getNextIndex();
	else
	{
		++mCurrentIndex;
		while(!mGridSquares[mCurrentIndex] && mCurrentIndex < mTotalLength)
		{
			++mCurrentIndex;
		}
		return mGridSquares[mCurrentIndex]->getNextIndex();
	}
}


void GridSquareVector::insertVertex(TexturedVertex* vertex, GridSquare* owner)
{
	insertVertex(mCurrentVertex, vertex, owner);
	++mCurrentVertex;
}

void GridSquareVector::insertVertex(unsigned int i, TexturedVertex* vertex, GridSquare* owner)
{
	vertex->index = i;
	if(i > mVertexCache.size())
		mVertexCache.resize(i + 100);


	mVertexCache[i] = vertex;
	++mNumVertices;


	if(owner->getCenterX() > 0 && vertex->XLocation == VertexXLocation::VL_NEG_X)
	{
		GridSquare* neighbour = getGridSquare(owner->getCenterX() - 1, owner->getCenterZ());

		if(neighbour)
		{
			bool canAdd = true;
			TexturedVertex* nv = new TexturedVertex(vertex);
			if(nv->XLocation == VertexXLocation::VL_NEG_X)
				nv->XLocation = VertexXLocation::VL_POS_X;
			else
				nv->XLocation = VertexXLocation::VL_NEG_X;
			
			if(owner->getCenterY() == neighbour->getCenterY() + 1)
			{
				if(nv->YLocation == VertexYLocation::VL_NEG_Y)
					nv->YLocation = VertexYLocation::VL_POS_Y;
				else
					nv->YLocation = VertexYLocation::VL_UPPER_Y;
			}
			else if(owner->getCenterY() + 1 == neighbour->getCenterY())
			{
				if(nv->YLocation == VertexYLocation::VL_NEG_Y)
					canAdd = false;
				else
					nv->YLocation = VertexYLocation::VL_NEG_Y;
			}
			
			if(canAdd)
				neighbour->addVertex(nv);
			
		}
	}

	if(owner->getCenterZ() > 0 && vertex->ZLocation == VertexZLocation::VL_NEG_Z)
	{
		GridSquare* neighbour = getGridSquare(owner->getCenterX(), owner->getCenterZ() - 1);

		if(neighbour)
		{
			TexturedVertex* nv = new TexturedVertex(vertex);

			bool canAdd = true;

			if(owner->getCenterY() == neighbour->getCenterY() + 1)
			{
				if(nv->YLocation == VertexYLocation::VL_NEG_Y)
					nv->YLocation = VertexYLocation::VL_POS_Y;
				else
					nv->YLocation = VertexYLocation::VL_UPPER_Y;
			}
			else if(owner->getCenterY() + 1 == neighbour->getCenterY())
			{
				if(nv->YLocation == VertexYLocation::VL_NEG_Y)
					canAdd = false;
				else
					nv->YLocation = VertexYLocation::VL_NEG_Y;
			}

			if(canAdd)
			{
				nv->ZLocation = VertexZLocation::VL_POS_Z;
				neighbour->addVertex(nv);
			}
			else
			{
				canAdd = false;
			}
		}
	}
	if(owner->getCenterX() > 0 && owner->getCenterZ() > 0 && vertex->XLocation == VertexXLocation::VL_NEG_X && vertex->ZLocation == VertexZLocation::VL_NEG_Z)
	{
		GridSquare* neighbour = getGridSquare(owner->getCenterX() - 1, owner->getCenterZ() - 1);

		if(neighbour)
		{
			TexturedVertex* nv = new TexturedVertex(vertex);
			if(nv->XLocation == VertexXLocation::VL_NEG_X)
				nv->XLocation = VertexXLocation::VL_POS_X;
			else
				nv->XLocation = VertexXLocation::VL_NEG_X;

			bool canAdd = true;

			if(owner->getCenterY() == neighbour->getCenterY() + 1)
			{
				if(nv->YLocation == VertexYLocation::VL_NEG_Y)
					nv->YLocation = VertexYLocation::VL_POS_Y;
				else
					nv->YLocation = VertexYLocation::VL_UPPER_Y;
			}
			else if(owner->getCenterY() + 1 == neighbour->getCenterY())
			{
				if(nv->YLocation == VertexYLocation::VL_NEG_Y)
					canAdd = false;
				else
					nv->YLocation = VertexYLocation::VL_NEG_Y;
			}

			if(canAdd)
			{
				nv->ZLocation = VertexZLocation::VL_POS_Z;
				neighbour->addVertex(nv);
			}
			else
			{
				delete nv;
			}
		}
	}
}

void GridSquareVector::updateTextureCoordinates(TexturedVertex* vertex)
{
	if(!vertex->gotTextureCoords())
		return;
	
	TexturedVertex* update = getVertex(vertex->index);

	if(update && update != vertex)
	{
		if(vertex->setUCoord)
		{
			update->u = vertex->u;
			update->setUCoord = true;
		}
		if(vertex->setVCoord)
		{
			update->v = vertex->v;
			update->setVCoord = true;
		}

		update->EX = vertex->EX;
		update->EY = vertex->EY;
		update->EZ = vertex->EZ;
		update->EW = vertex->EW;

		update->FX = vertex->FX;
		update->FY = vertex->FY;
		update->FZ = vertex->FZ;
		update->FW = vertex->FW;

	}
}


TexturedVertex* GridSquareVector::getVertex(unsigned int i)
{
	if(i < mVertexCache.size())
		return mVertexCache[i];

	return 0;
}


Ogre::ManualObject* GridSquareVector::getBattlefield()
{
	return mBattlefield;
}

BattlefieldGrid* GridSquareVector::getBattlefieldGrid()
{
	return mBattlefieldGrid;
}

GridSquareTemplateVector* GridSquareVector::getGridSquareTemplateVector()
{
	return mGridSquareTemplateVector;
}

Ogre::Real GridSquareVector::getFogTextureCoord(const Ogre::Real& inCoord)
{
	Real result = inCoord / (BattlefieldGrid::UnitSize * mSideLength);
	return result;
}