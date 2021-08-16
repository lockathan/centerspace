#ifndef WeaponHardpointTemplateRepositoryLoader_h
#define WeaponHardpointTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/DataLoader.h>

class WeaponHardpointTemplateDataLoader : public DataLoader
{
public:
	WeaponHardpointTemplateDataLoader();
	~WeaponHardpointTemplateDataLoader();
	void load(const std::string& fileName );	
};
#endif