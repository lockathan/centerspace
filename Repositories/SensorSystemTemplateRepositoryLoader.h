#ifndef SensorSystemTemplateRepositoryLoader_h
#define SensorSystemTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/SensorSystemTemplateRepository.h>

class SensorSystemTemplateRepositoryLoader : public RepositoryLoader
{
public:
	SensorSystemTemplateRepositoryLoader();
	~SensorSystemTemplateRepositoryLoader();
	void load(const std::string& fileName, SensorSystemTemplateRepository* r);
	
};
#endif