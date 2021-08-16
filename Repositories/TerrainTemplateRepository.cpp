#include <Repositories/TerrainTemplateRepository.h>
#include <Templates/HubTemplate.h>

TerrainTemplateRepository::TerrainTemplateRepository()
{

}

TerrainTemplateRepository::~TerrainTemplateRepository()
{

}

std::deque<TerrainTemplate*> TerrainTemplateRepository::getAvailableTemplates(int hubId)
{
	std::deque<TerrainTemplate*> result;
	std::vector<TerrainTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		
		if((*componentIterator))
		{
			if((*componentIterator)->getHubTemplate()->getId() == hubId)
				result.push_back((*componentIterator));
		}
	}

	return result;
}

TerrainTemplateRepository* TerrainTemplateRepository::mTerrainTemplateRepository;

TerrainTemplateRepository* TerrainTemplateRepository::getSingletonPtr()
{
	if(!mTerrainTemplateRepository)
	{
		mTerrainTemplateRepository = new TerrainTemplateRepository();
	}
	return mTerrainTemplateRepository;
}