#ifndef TerrainTemplateRepository_h
#define TerrainTemplateRepository_h

#include "Ogre.h"
#include <Templates/TerrainTemplate.h>
#include <Repositories/Repository.h>

class TerrainTemplateRepository : public Repository<TerrainTemplate>
{
public:
	std::deque<TerrainTemplate*> getAvailableTemplates(int hubId);

	static TerrainTemplateRepository* getSingletonPtr();
	~TerrainTemplateRepository();
private:
	TerrainTemplateRepository();
	static TerrainTemplateRepository* mTerrainTemplateRepository;
};
#endif