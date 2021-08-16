#ifndef TargetingSystemTemplateRepositoryLoader_h
#define TargetingSystemTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/TargetingSystemTemplateRepository.h>

class TargetingSystemTemplateRepositoryLoader : public RepositoryLoader
{
public:
	TargetingSystemTemplateRepositoryLoader();
	~TargetingSystemTemplateRepositoryLoader();
	void load(const std::string& fileName, TargetingSystemTemplateRepository* r);
	
};
#endif