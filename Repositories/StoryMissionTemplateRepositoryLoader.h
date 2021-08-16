#ifndef StoryMissionTemplateRepositoryLoader_h
#define StoryMissionTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/StoryMissionTemplateRepository.h>

class StoryMissionTemplateRepositoryLoader : public RepositoryLoader
{
public:
	StoryMissionTemplateRepositoryLoader();
	~StoryMissionTemplateRepositoryLoader();
	void load(const std::string& fileName, StoryMissionTemplateRepository* r);	
};
#endif