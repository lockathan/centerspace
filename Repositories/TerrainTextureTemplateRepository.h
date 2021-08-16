#ifndef TerrainTextureTemplateRepository_h
#define TerrainTextureTemplateRepository_h

#include "Ogre.h"
#include <Templates/TerrainTextureTemplate.h>
#include <Repositories/Repository.h>

class TerrainTextureTemplateRepository : public Repository<TerrainTextureTemplate>
{
public:

	static TerrainTextureTemplateRepository* getSingletonPtr();
	~TerrainTextureTemplateRepository();
private:
	TerrainTextureTemplateRepository();
	static TerrainTextureTemplateRepository* mTerrainTextureTemplateRepository;
};
#endif