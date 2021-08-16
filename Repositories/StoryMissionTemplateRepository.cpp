#include <Repositories/StoryMissionTemplateRepository.h>
#include <Templates/StoryMissionTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/HubTemplate.h>

StoryMissionTemplateRepository::StoryMissionTemplateRepository()
{

}

StoryMissionTemplateRepository::~StoryMissionTemplateRepository()
{

}

std::vector<StoryMissionTemplate*> StoryMissionTemplateRepository::getAvailableTemplates(int factionHubId, int rank)
{
	std::vector<StoryMissionTemplate*> result;
	std::vector<StoryMissionTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			if((*componentIterator)->getFactionHubTemplate()->getId() == factionHubId && (*componentIterator)->getRequiredRank() <= rank )
				result.push_back((*componentIterator));
		}
	}

	return result;
}

StoryMissionTemplateRepository* StoryMissionTemplateRepository::mStoryMissionTemplateRepository;

StoryMissionTemplateRepository* StoryMissionTemplateRepository::getSingletonPtr()
{
	if(!mStoryMissionTemplateRepository)
	{
		mStoryMissionTemplateRepository = new StoryMissionTemplateRepository();
	}
	return mStoryMissionTemplateRepository;
}