#ifndef ReactorTemplateRepositoryLoader_h
#define ReactorTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/ReactorTemplateRepository.h>

class ReactorTemplateRepositoryLoader : public RepositoryLoader
{
public:
	ReactorTemplateRepositoryLoader();
	~ReactorTemplateRepositoryLoader();
	void load(const std::string& fileName, ReactorTemplateRepository* r);
};
#endif