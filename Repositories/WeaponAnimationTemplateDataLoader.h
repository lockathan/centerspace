#ifndef WeaponAnimationTemplateDataLoader_h
#define WeaponAnimationTemplateDataLoader_h

#include "Ogre.h"
#include <Repositories/DataLoader.h>

class WeaponAnimationTemplateDataLoader : public DataLoader
{
public:
	WeaponAnimationTemplateDataLoader();
	~WeaponAnimationTemplateDataLoader();
	void load(const std::string& fileName );
};
#endif