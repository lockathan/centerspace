#ifndef HubTemplateRepositoryLoader_h
#define HubTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/HubTemplateRepository.h>

class HubTemplateRepositoryLoader : public RepositoryLoader
{
public:
	HubTemplateRepositoryLoader();
	~HubTemplateRepositoryLoader();
	void load(const std::string& fileName, HubTemplateRepository* r);	
};
#endif