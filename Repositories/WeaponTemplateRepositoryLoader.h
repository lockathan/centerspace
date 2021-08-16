#ifndef WeaponTemplateRepositoryLoader_h
#define WeaponTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/WeaponTemplateRepository.h>

class WeaponTemplateRepositoryLoader : public RepositoryLoader
{
public:
	WeaponTemplateRepositoryLoader();
	~WeaponTemplateRepositoryLoader();
	void load(const std::string& fileName, WeaponTemplateRepository* r);
};
#endif