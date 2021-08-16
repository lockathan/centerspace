#ifndef DriveTemplateRepositoryLoader_h
#define DriveTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/DriveTemplateRepository.h>

class DriveTemplateRepositoryLoader : public RepositoryLoader
{
public:
	DriveTemplateRepositoryLoader();
	~DriveTemplateRepositoryLoader();
	void load(const std::string& fileName, DriveTemplateRepository* r);
	
};
#endif