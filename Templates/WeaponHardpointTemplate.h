#ifndef WeaponHardpointTemplate_h
#define WeaponHardpointTemplate_h

#include "Ogre.h"
#include <Templates/Enums.h>
#include <Templates/WeaponTemplate.h>
#include <Entities/PersistentEntity.h>

class WeaponHardpoint;
class WeaponTemplate;

class WeaponHardpointTemplate : public Template
{
public:

	WeaponHardpointTemplate();
	~WeaponHardpointTemplate();

	HardpointType getHardpointType();
	void setHardpointType(HardpointType type);
	void setHardpointType(int i);

	int getMaxWeaponSize();
	void setMaxWeaponSize(int maxSize);

	WeaponTemplate* getDefaultWeapon();
	void setDefaultWeapon(WeaponTemplate* defaultWeapon);

	int getHardpointNumber();
	void setHardpointNumber(int hNumber);

	Ogre::Vector3 getBarrelOffset();
	void setBarrelOffset(float x, float y, float z);
	void setBarrelOffset(Ogre::Vector3 location);

	std::string getFiringBoneName();
	void setFiringBoneName(std::string bone);

	WeaponHardpoint* instantiate(bool useDefaults);
	WeaponHardpointTemplate* getNextWeapon();
protected:
	HardpointType mHardpointType;
	int mMaxWeaponSize;
	int mHardpointNumber;
	Ogre::Vector3 mBarrelOffset;
	WeaponTemplate* mDefaultWeapon;
	std::string mFiringBoneName;
};
#endif