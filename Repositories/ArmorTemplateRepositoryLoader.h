#ifndef ArmorTemplateRepositoryLoader_h
#define ArmorTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/ArmorTemplateRepository.h>

class ArmorTemplateRepositoryLoader : public RepositoryLoader
{
public:
	ArmorTemplateRepositoryLoader();
	~ArmorTemplateRepositoryLoader();
	void load(const std::string& fileName, ArmorTemplateRepository* r);
};
#endif