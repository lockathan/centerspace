#include <Templates/WeaponHardpointTemplate.h>
#include <Entities/WeaponHardpoint.h>
#include <Templates/Enums.h>
#include <Ogre.h>

using namespace Ogre;

WeaponHardpointTemplate::WeaponHardpointTemplate()
	:
	mHardpointType(HardpointType::BALLISTIC_ONLY),
	mMaxWeaponSize(0),
	mHardpointNumber(0),
	mBarrelOffset(Vector3::ZERO),
	mDefaultWeapon(0),
	mFiringBoneName("")
{

}

WeaponHardpointTemplate::~WeaponHardpointTemplate()
{

}
	
int WeaponHardpointTemplate::getHardpointNumber()
{
	return mHardpointNumber;
}

void WeaponHardpointTemplate::setHardpointNumber(int hNumber)
{
	mHardpointNumber = hNumber;
}

HardpointType WeaponHardpointTemplate::getHardpointType()
{
	return mHardpointType;
}

void WeaponHardpointTemplate::setHardpointType(HardpointType type)
{
	mHardpointType = type;
}

void WeaponHardpointTemplate::setHardpointType(int i)
{
	switch(i)
	{
	case 0:
		mHardpointType = HardpointType::BALLISTIC_ONLY;
		break;
	case 1:
		mHardpointType = HardpointType::ENERGY_ONLY;
		break;
	case 2:
		mHardpointType = HardpointType::ENERGY_OR_BALLISTIC;
		break;
	case 3:
		mHardpointType = HardpointType::MISSILE_ONLY;
		break;
	default:
		break;
	}
}
	
int WeaponHardpointTemplate::getMaxWeaponSize()
{
	return mMaxWeaponSize;
}

void WeaponHardpointTemplate::setMaxWeaponSize(int maxSize)
{
	mMaxWeaponSize = maxSize;
}

WeaponTemplate* WeaponHardpointTemplate::getDefaultWeapon()
{
	return mDefaultWeapon;
}

void WeaponHardpointTemplate::setDefaultWeapon(WeaponTemplate* defaultWeapon)
{
	mDefaultWeapon = defaultWeapon;
}

Vector3 WeaponHardpointTemplate::getBarrelOffset()
{
	return mBarrelOffset;
}

void WeaponHardpointTemplate::setBarrelOffset(float x, float y, float z)
{
	mBarrelOffset = Vector3(x,y,z);
}

void WeaponHardpointTemplate::setBarrelOffset(Vector3 location)
{
	mBarrelOffset = location;
}

std::string WeaponHardpointTemplate::getFiringBoneName()
{
	return mFiringBoneName;
}

void WeaponHardpointTemplate::setFiringBoneName(std::string bone)
{
	mFiringBoneName = bone;
}

WeaponHardpoint* WeaponHardpointTemplate::instantiate(bool useDefaults)
{
	WeaponHardpoint* weapon = new WeaponHardpoint();
	
	return weapon;
}
