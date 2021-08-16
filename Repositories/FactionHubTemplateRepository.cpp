#include <Repositories/FactionHubTemplateRepository.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/HubTemplate.h>
#include <deque>

FactionHubTemplateRepository::FactionHubTemplateRepository()
{

}

FactionHubTemplateRepository::~FactionHubTemplateRepository()
{

}

FactionHubTemplate* FactionHubTemplateRepository::getFactionHubTemplate(int factionId, int rank)
{
	std::vector<FactionHubTemplate*>::iterator componentIterator;
	int highestRank(0);
	std::deque<FactionHubTemplate*> fHubTemplates;
	std::deque<FactionHubTemplate*>::iterator hubIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			if((*componentIterator)->getHubTemplate()->getRequiredRank() <= rank && (*componentIterator)->getPlayerFactionTemplate()->getId() == factionId)
			{
				if((*componentIterator)->getHubTemplate()->getRequiredRank() > highestRank)
					highestRank = (*componentIterator)->getHubTemplate()->getRequiredRank();
				fHubTemplates.push_back((*componentIterator));
			}
		}
	}

	for(hubIterator = fHubTemplates.begin(); hubIterator != fHubTemplates.end(); ++hubIterator)
	{
		if((*hubIterator)->getHubTemplate()->getRequiredRank() == highestRank)
			return (*hubIterator);
	}

	return 0;
}

FactionHubTemplateRepository* FactionHubTemplateRepository::mFactionHubTemplateRepository;

FactionHubTemplateRepository* FactionHubTemplateRepository::getSingletonPtr()
{
	if(!mFactionHubTemplateRepository)
	{
		mFactionHubTemplateRepository = new FactionHubTemplateRepository();
	}
	return mFactionHubTemplateRepository;
}