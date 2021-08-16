#ifndef MCUTemplateRepositoryLoader_h
#define MCUTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/MCUTemplateRepository.h>

class MCUTemplateRepositoryLoader : public RepositoryLoader
{
public:
	MCUTemplateRepositoryLoader();
	~MCUTemplateRepositoryLoader();
	void load(const std::string& fileName, MCUTemplateRepository* r);	
};
#endif