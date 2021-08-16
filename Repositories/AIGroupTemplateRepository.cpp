#include <Repositories/AIGroupTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/AIGroupTemplate.h>

AIGroupTemplateRepository::AIGroupTemplateRepository()
{

}

AIGroupTemplateRepository::~AIGroupTemplateRepository()
{

}

std::deque<AIGroupTemplate*> AIGroupTemplateRepository::getAvailableAIGroupTemplates(int factionId, int rank)
{
	std::deque<AIGroupTemplate*> result;
	std::vector<AIGroupTemplate*>::iterator mcuIterator;

	for(mcuIterator = this->begin(); mcuIterator != this->end(); ++mcuIterator)
	{
		if((*mcuIterator))
		{
			if((*mcuIterator)->getFactionTemplate()->getId() == factionId && (*mcuIterator)->getRequiredRank() <= rank && (*mcuIterator)->getRequiredRank() >= (rank - 3))
				result.push_back((*mcuIterator));
		}
	}

	return result;
}

std::deque<AIGroupTemplate*> AIGroupTemplateRepository::getAvailableAIGroupTemplates(int factionId, int rank, AIGroupTemplate::AIGroupType type)
{
	std::deque<AIGroupTemplate*> result;
	std::vector<AIGroupTemplate*>::iterator mcuIterator;

	for(mcuIterator = this->begin(); mcuIterator != this->end(); ++mcuIterator)
	{
		if((*mcuIterator)->getFactionTemplate()->getId() == factionId && (*mcuIterator)->getRequiredRank() == rank && (*mcuIterator)->getType() == type)
			result.push_back((*mcuIterator));
	}

	return result;
}

AIGroupTemplate* AIGroupTemplateRepository::getRandomAIGroupTemplate(int factionId, int rank)
{
	std::deque<AIGroupTemplate*> groups = getAvailableAIGroupTemplates(factionId, rank);
	return groups[rand() % groups.size()];
}

AIGroupTemplateRepository* AIGroupTemplateRepository::mAIGroupTemplateRepository;

AIGroupTemplateRepository* AIGroupTemplateRepository::getSingletonPtr()
{
	if(!mAIGroupTemplateRepository)
	{
		mAIGroupTemplateRepository = new AIGroupTemplateRepository();
	}
	return mAIGroupTemplateRepository;
}