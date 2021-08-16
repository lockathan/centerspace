#ifndef RandomMissionTypeTemplateRepositoryLoader_h
#define RandomMissionTypeTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/RandomMissionTypeTemplateRepository.h>

class RandomMissionTypeTemplateRepositoryLoader : public RepositoryLoader
{
public:
	RandomMissionTypeTemplateRepositoryLoader();
	~RandomMissionTypeTemplateRepositoryLoader();
	void load(const std::string& fileName, RandomMissionTypeTemplateRepository* r);	
};
#endif