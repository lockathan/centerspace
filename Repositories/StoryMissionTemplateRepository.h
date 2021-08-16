#ifndef StoryMissionTemplateRepository_h
#define StoryMissionTemplateRepository_h

#include "Ogre.h"
#include <Templates/StoryMissionTemplate.h>
#include <Repositories/Repository.h>

#include <vector>

class StoryMissionTemplateRepository : public Repository<StoryMissionTemplate>
{
public:
	static StoryMissionTemplateRepository* getSingletonPtr();
	~StoryMissionTemplateRepository();

	std::vector<StoryMissionTemplate*> getAvailableTemplates(int factionHubId, int rank);
private:
	StoryMissionTemplateRepository();
	static StoryMissionTemplateRepository* mStoryMissionTemplateRepository;
};
#endif