#ifndef FactionHubTemplateRepositoryLoader_h
#define FactionHubTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/FactionHubTemplateRepository.h>

class FactionHubTemplateRepositoryLoader : public RepositoryLoader
{
public:
	FactionHubTemplateRepositoryLoader();
	~FactionHubTemplateRepositoryLoader();
	void load(const std::string& fileName, FactionHubTemplateRepository* r);	
};
#endif