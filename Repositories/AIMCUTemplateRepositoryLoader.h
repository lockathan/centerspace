#ifndef AIMCUTemplateRepositoryLoader_h
#define AIMCUTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/AIMCUTemplateRepository.h>

class AIMCUTemplateRepositoryLoader : public RepositoryLoader
{
public:
	AIMCUTemplateRepositoryLoader();
	~AIMCUTemplateRepositoryLoader();
	void load(const std::string& fileName, AIMCUTemplateRepository* r);	
};
#endif