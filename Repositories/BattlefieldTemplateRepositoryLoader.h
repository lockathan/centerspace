#ifndef BattlefieldTemplateRepositoryLoader_h
#define BattlefieldTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/BattlefieldTemplateRepository.h>

class BattlefieldTemplateRepositoryLoader : public RepositoryLoader
{
public:
	BattlefieldTemplateRepositoryLoader();
	~BattlefieldTemplateRepositoryLoader();
	void load(const std::string& fileName, BattlefieldTemplateRepository* r);	
};
#endif