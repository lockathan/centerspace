#ifndef BatteryTemplateRepositoryLoader_h
#define BatteryTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/BatteryTemplateRepository.h>

class BatteryTemplateRepositoryLoader : public RepositoryLoader
{
public:
	BatteryTemplateRepositoryLoader();
	~BatteryTemplateRepositoryLoader();
	void load(const std::string& fileName, BatteryTemplateRepository* r);
	
};
#endif