#ifndef PhysicalProjectileTemplateRepositoryLoader_h
#define PhysicalProjectileTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>

class PhysicalProjectileTemplateRepository;

class PhysicalProjectileTemplateRepositoryLoader : public RepositoryLoader
{
public:
	PhysicalProjectileTemplateRepositoryLoader();
	~PhysicalProjectileTemplateRepositoryLoader();
	void load(const std::string& fileName, PhysicalProjectileTemplateRepository* r);
};
#endif