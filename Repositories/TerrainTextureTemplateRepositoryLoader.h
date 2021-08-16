#ifndef TerrainTextureTemplateRepositoryLoader_h
#define TerrainTextureTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/TerrainTextureTemplateRepository.h>

class TerrainTextureTemplateRepositoryLoader : public RepositoryLoader
{
public:
	TerrainTextureTemplateRepositoryLoader();
	~TerrainTextureTemplateRepositoryLoader();
	void load(const std::string& fileName, TerrainTextureTemplateRepository* r);	
};
#endif