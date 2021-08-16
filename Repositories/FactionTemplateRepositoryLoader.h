#ifndef FactionTemplateRepositoryLoader_h
#define FactionTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/FactionTemplateRepository.h>

class FactionTemplateRepositoryLoader : public RepositoryLoader
{
public:
	FactionTemplateRepositoryLoader();
	~FactionTemplateRepositoryLoader();
	void load(const std::string& fileName, FactionTemplateRepository* r);
};
#endif