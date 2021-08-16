#ifndef WeaponTemplateRepository_h
#define WeaponTemplateRepository_h

#include "Ogre.h"
#include <Templates/Enums.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Repositories/Repository.h>

class WeaponTemplateRepository : public Repository<WeaponTemplate>
{
public:
	static WeaponTemplateRepository* getSingletonPtr();
	~WeaponTemplateRepository();
	
	std::vector<WeaponTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);
	std::vector<WeaponTemplate*> getAvailableTemplates(int factionId, int rank, int size, HardpointType hardpointType );

private:
	WeaponTemplateRepository();
	static WeaponTemplateRepository* mWeaponTemplateRepository;
};
#endif