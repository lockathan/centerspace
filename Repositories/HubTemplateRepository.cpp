#include <Repositories/HubTemplateRepository.h>
#include <Templates/HubTemplate.h>
#include <Templates/FactionTemplate.h>

HubTemplateRepository::HubTemplateRepository()
{

}

HubTemplateRepository::~HubTemplateRepository()
{

}

std::vector<HubTemplate*> HubTemplateRepository::getAvailableTemplates(int rank)
{
	std::vector<HubTemplate*> result;
	std::vector<HubTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			if((*componentIterator)->getRequiredRank() <= rank)
				result.push_back((*componentIterator));
		}
	}

	return result;
}

HubTemplateRepository* HubTemplateRepository::mHubTemplateRepository;

HubTemplateRepository* HubTemplateRepository::getSingletonPtr()
{
	if(!mHubTemplateRepository)
	{
		mHubTemplateRepository = new HubTemplateRepository();
	}
	return mHubTemplateRepository;
}