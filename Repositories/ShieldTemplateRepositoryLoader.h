#ifndef ShieldTemplateRepositoryLoader_h
#define ShieldTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/ShieldTemplateRepository.h>

class ShieldTemplateRepositoryLoader : public RepositoryLoader
{
public:
	ShieldTemplateRepositoryLoader();
	~ShieldTemplateRepositoryLoader();
	void load(const std::string& fileName, ShieldTemplateRepository* r);
};
#endif