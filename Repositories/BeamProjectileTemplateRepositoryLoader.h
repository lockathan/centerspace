#ifndef BeamProjectileTemplateRepositoryLoader_h
#define BeamProjectileTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>

class BeamProjectileTemplateRepository;

class BeamProjectileTemplateRepositoryLoader : public RepositoryLoader
{
public:
	BeamProjectileTemplateRepositoryLoader();
	~BeamProjectileTemplateRepositoryLoader();
	void load(const std::string& fileName, BeamProjectileTemplateRepository* r);
};
#endif