#include <Repositories/TargetingSystemTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/TargetingSystemTemplate.h>

TargetingSystemTemplateRepository::TargetingSystemTemplateRepository()
{

}

TargetingSystemTemplateRepository::~TargetingSystemTemplateRepository()
{

}

std::vector<TargetingSystemTemplate*> TargetingSystemTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<TargetingSystemTemplate*> result;
	std::vector<TargetingSystemTemplate*>::iterator componentIterator;

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

TargetingSystemTemplateRepository* TargetingSystemTemplateRepository::mTargetingSystemTemplateRepository;

TargetingSystemTemplateRepository* TargetingSystemTemplateRepository::getSingletonPtr()
{
	if(!mTargetingSystemTemplateRepository)
	{
		mTargetingSystemTemplateRepository = new TargetingSystemTemplateRepository();
	}
	return mTargetingSystemTemplateRepository;
}