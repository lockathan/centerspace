#ifndef SoundTemplateRepositoryLoader_h
#define SoundTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/SoundTemplateRepository.h>

class SoundTemplateRepositoryLoader : public RepositoryLoader
{
public:
	SoundTemplateRepositoryLoader();
	~SoundTemplateRepositoryLoader();
	void load(const std::string& fileName, SoundTemplateRepository* r);	
};
#endif