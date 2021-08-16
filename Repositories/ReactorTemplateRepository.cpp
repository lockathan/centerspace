#include <Repositories/ReactorTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/ReactorTemplate.h>

ReactorTemplateRepository::ReactorTemplateRepository()
{

}

ReactorTemplateRepository::~ReactorTemplateRepository()
{

}

std::vector<ReactorTemplate*> ReactorTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<ReactorTemplate*> result;
	std::vector<ReactorTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			if(((*componentIterator)->getFactionTemplate()->isGenericFaction() || (*componentIterator)->getFactionTemplate()->getId() == factionId) && (*componentIterator)->getRequiredRank() <= rank && (*componentIterator)->getSize() == size)
				result.push_back((*componentIterator));
		}
	}

	return result;
}


ReactorTemplateRepository* ReactorTemplateRepository::mReactorTemplateRepository;

ReactorTemplateRepository* ReactorTemplateRepository::getSingletonPtr()
{
	if(!mReactorTemplateRepository)
	{
		mReactorTemplateRepository = new ReactorTemplateRepository();
	}
	return mReactorTemplateRepository;
}