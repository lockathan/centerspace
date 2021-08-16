#include <Templates/AIMCUTemplate.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/WeaponHardpoint.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include "Ogre.h"
#include <hash_map>
#include <string>

AIMCUTemplate::AIMCUTemplate()
	:
	mWeaponTemplates(10),
	
	mFactionTemplate(0),

	mDefaultBattery(0),
	mDefaultDrive(0),
	mDefaultReactor(0),
	mDefaultSensorSystem(0),
	mDefaultShield(0),
	mDefaultTargetingSystem(0),

	mMCUTemplate(0)
{

}

AIMCUTemplate::~AIMCUTemplate()
{

}

MechanizedCombatUnit* AIMCUTemplate::instantiate(bool useDefaults)
{
	MechanizedCombatUnit* result = mMCUTemplate->instantiate(true);
	result->setArmor(mDefaultArmor);
	result->setBattery(mDefaultBattery);
	result->setDrive(mDefaultDrive);
	result->setReactor(mDefaultReactor);
	result->setSensorSystem(mDefaultSensorSystem);
	result->setShield(mDefaultShield);
	result->setTargetingSystem(mDefaultTargetingSystem);

	result->setName(mName);

	for(unsigned int i = 0; i < mWeaponTemplates.size();++i)
	{
		if(mWeaponTemplates[i])
		{
			result->setWeapon(i,mWeaponTemplates[i]);
		}
	}

	return result;
}

ArmorTemplate* AIMCUTemplate::getDefaultArmor()
{
	return mDefaultArmor;
}

void AIMCUTemplate::setDefaultArmor(ArmorTemplate* armor)
{
	mDefaultArmor = armor;
}

BatteryTemplate* AIMCUTemplate::getDefaultBattery()
{
	return mDefaultBattery;
}

void AIMCUTemplate::setDefaultBattery(BatteryTemplate* battery)
{
	mDefaultBattery = battery;
}

DriveTemplate* AIMCUTemplate::getDefaultDrive()
{
	return mDefaultDrive;
}

void AIMCUTemplate::setDefaultDrive(DriveTemplate* drive)
{
	mDefaultDrive = drive;
}

ReactorTemplate* AIMCUTemplate::getDefaultReactor()
{
	return mDefaultReactor;
}

void AIMCUTemplate::setDefaultReactor(ReactorTemplate* reactor)
{
	mDefaultReactor = reactor;
}

SensorSystemTemplate* AIMCUTemplate::getDefaultSensorSystem()
{
	return mDefaultSensorSystem;
}

void AIMCUTemplate::setDefaultSensorSystem(SensorSystemTemplate* sensor)
{
	mDefaultSensorSystem = sensor;
}

ShieldTemplate* AIMCUTemplate::getDefaultShield()
{
	return mDefaultShield;
}

void AIMCUTemplate::setDefaultShield(ShieldTemplate* shield)
{
	mDefaultShield = shield;
}

TargetingSystemTemplate* AIMCUTemplate::getDefaultTargetingSystem()
{
	return mDefaultTargetingSystem;
}

void AIMCUTemplate::setDefaultTargetingSystem(TargetingSystemTemplate* target)
{
	mDefaultTargetingSystem = target;
}

FactionTemplate* AIMCUTemplate::getFactionTemplate()
{
	return mFactionTemplate;
}

void AIMCUTemplate::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

MCUTemplate* AIMCUTemplate::getMCUTemplate()
{
	return mMCUTemplate;
}

void AIMCUTemplate::setMCUTemplate(MCUTemplate* mcu)
{
	mMCUTemplate = mcu;
}

void AIMCUTemplate::setWeapon(int slot, WeaponTemplate* weapon)
{
	mWeaponTemplates[slot] = weapon;
}

