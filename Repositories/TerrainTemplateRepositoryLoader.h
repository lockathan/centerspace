#ifndef TerrainTemplateRepositoryLoader_h
#define TerrainTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/TerrainTemplateRepository.h>

class TerrainTemplateRepositoryLoader : public RepositoryLoader
{
public:
	TerrainTemplateRepositoryLoader();
	~TerrainTemplateRepositoryLoader();
	void load(const std::string& fileName, TerrainTemplateRepository* r);	
};
#endif