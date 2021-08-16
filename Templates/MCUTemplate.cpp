#include <Templates/MCUTemplate.h>
#include <Entities/DataProvider.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/WeaponHardpoint.h>
#include <Managers/SQLiteDBManager.h>
#include <Templates/AnimationDefinitionTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include "Ogre.h"
#include <hash_map>
#include <string>

MCUTemplate::MCUTemplate()
	:
	mWeaponHardpointTemplates(10),
	mCurrentHardpoint(0),
	mMaxArmorSize(0),
	mMaxBatterySize(0),
	mMaxReactorSize(0),
	mMaxDriveSize(0),
	mMaxShieldSize(0),
	mMaxComponentSlots(0),
	mMCUType(MCUType::MT_LIGHT),
	mMaterialName(""),
	mMeshName(""),
	mThumbnailFile(""),
	mMovementSpeed(10.f),
	mWalkingAnimationSpeed(1.f),
	mScale(1.f),
	mAnimationDefinitionTemplate(0),
	mDefaultArmor(0),
	mDefaultBattery(0),
	mDefaultDrive(0),
	mDefaultReactor(0),
	mDefaultSensorSystem(0),
	mDefaultShield(0),
	mTargetingSystemTemplate(0),
	mProfileImageFile(""),
	mWalkSoundName("")
{

}

MCUTemplate::~MCUTemplate()
{

}

MechanizedCombatUnit* MCUTemplate::instantiate(bool useDefaults)
{
	MechanizedCombatUnit* result = new MechanizedCombatUnit();
	result->setArmor(mDefaultArmor);
	result->setBattery(mDefaultBattery);
	result->setDrive(mDefaultDrive);
	result->setReactor(mDefaultReactor);
	result->setSensorSystem(mDefaultSensorSystem);
	result->setShield(mDefaultShield);
	result->setTargetingSystem(mTargetingSystemTemplate);
	result->setMCUTemplate(this);
	result->setName(mName);
	
	std::vector<WeaponHardpointTemplate*>::iterator weaponIterator;

	for(weaponIterator = mWeaponHardpointTemplates.begin();weaponIterator != mWeaponHardpointTemplates.end(); ++weaponIterator)
	{
		if((*weaponIterator))
		{
			result->setWeapon((*weaponIterator)->getHardpointNumber(),(*weaponIterator)->getDefaultWeapon());
			result->getWeaponHardpoint((*weaponIterator)->getHardpointNumber())->setWeaponHardpointTemplate((*weaponIterator));
		}
	}

	return result;
}

ArmorTemplate* MCUTemplate::getDefaultArmor()
{
	return mDefaultArmor;
}

void MCUTemplate::setDefaultArmor(ArmorTemplate* armor)
{
	mDefaultArmor = armor;
}

BatteryTemplate* MCUTemplate::getDefaultBattery()
{
	return mDefaultBattery;
}

void MCUTemplate::setDefaultBattery(BatteryTemplate* battery)
{
	mDefaultBattery = battery;
}

DriveTemplate* MCUTemplate::getDefaultDrive()
{
	return mDefaultDrive;
}

void MCUTemplate::setDefaultDrive(DriveTemplate* drive)
{
	mDefaultDrive = drive;
}

ReactorTemplate* MCUTemplate::getDefaultReactor()
{
	return mDefaultReactor;
}

SensorSystemTemplate* MCUTemplate::getDefaultSensorSystem()
{
	return mDefaultSensorSystem;
}

void MCUTemplate::setDefaultSensorSystem(SensorSystemTemplate* sensor)
{
	mDefaultSensorSystem = sensor;
}

ShieldTemplate* MCUTemplate::getDefaultShield()
{
	return mDefaultShield;
}

void MCUTemplate::setDefaultShield(ShieldTemplate* shield)
{
	mDefaultShield = shield;
}

void MCUTemplate::setDefaultReactor(ReactorTemplate* reactor)
{
	mDefaultReactor = reactor;
}

TargetingSystemTemplate* MCUTemplate::getDefaultTargetingSystemTemplate()
{
	return mTargetingSystemTemplate;
}

void MCUTemplate::setDefaultTargetingSystemTemplate(TargetingSystemTemplate* target)
{
	mTargetingSystemTemplate = target;
}

int MCUTemplate::getMaxArmorSize()
{
	return mMaxArmorSize;
}

void MCUTemplate::setMaxArmorSize(int max)
{
	mMaxArmorSize = max;
}

int MCUTemplate::getMaxBatterySize()
{
	return mMaxBatterySize;
}

void MCUTemplate::setMaxBatterySize(int maxBatterySize)
{
	mMaxBatterySize = maxBatterySize;
}

int MCUTemplate::getMaxDriveSize()
{
	return mMaxDriveSize;
}

void MCUTemplate::setMaxDriveSize(int maxDriveSize)
{
	mMaxDriveSize = maxDriveSize;
}

int MCUTemplate::getMaxReactorSize()
{
	return mMaxReactorSize;
}

void MCUTemplate::setMaxReactorSize(int maxReactorSize)
{
	mMaxReactorSize = maxReactorSize;
}

int MCUTemplate::getMaxShieldSize()
{
	return mMaxShieldSize;
}

void MCUTemplate::setMaxShieldSize(int maxShieldSize)
{
	mMaxShieldSize = maxShieldSize;
}

int MCUTemplate::getMaxComponentSlots()
{
	return mMaxComponentSlots;
}

void MCUTemplate::setMaxComponentSlots(int maxComponentSlots)
{
	mMaxComponentSlots = maxComponentSlots;
}

MCUType MCUTemplate::getMCUType()
{
	return mMCUType;
}

void MCUTemplate::setMCUType(MCUType type)
{
	mMCUType = type;
}

void MCUTemplate::setMCUType(std::string type)
{
	if(type == "light" || type == "LIGHT")
	{
		mMCUType = MCUType::MT_LIGHT;
		return;
	}

	if(type == "medium" || type == "MEDIUM")
	{
		mMCUType = MCUType::MT_MEDIUM;
		return;
	}

	if(type == "heavy" || type == "HEAVY")
	{
		mMCUType = MCUType::MT_HEAVY;
		return;
	}

	mMCUType = MCUType::MT_LIGHT;
}

AnimationDefinitionTemplate* MCUTemplate::getAnimationDefinitionTemplate()
{
	return mAnimationDefinitionTemplate;
}

void MCUTemplate::setAnimationDefinitionTemplate(AnimationDefinitionTemplate* anim)
{
	mAnimationDefinitionTemplate = anim;
}

std::string MCUTemplate::getWeaponsDescription()
{
	std::string weapons = "";

	std::vector<WeaponHardpointTemplate*>::iterator weaponIterator;

	for(weaponIterator = mWeaponHardpointTemplates.begin(); weaponIterator != mWeaponHardpointTemplates.end(); ++weaponIterator)
	{
		if((*weaponIterator))
			weapons += (*weaponIterator)->getDefaultWeapon()->getName();
	}

	return weapons;
}

WeaponHardpointTemplate* MCUTemplate::getHardpoint(int hardpoint)
{
	return mWeaponHardpointTemplates[hardpoint];
}
	
void MCUTemplate::addWeaponHardpointTemplate(WeaponHardpointTemplate* weapon)
{
	if(mWeaponHardpointTemplates[mCurrentHardpoint])
	{
		delete mWeaponHardpointTemplates[mCurrentHardpoint];
		mWeaponHardpointTemplates[mCurrentHardpoint] = 0;
	}
	mWeaponHardpointTemplates[mCurrentHardpoint] = weapon;
	weapon->setHardpointNumber(mCurrentHardpoint);
	++mCurrentHardpoint;
}



std::string MCUTemplate::getMaterialName()
{
	return mMaterialName;
}

void MCUTemplate::setMaterialName(std::string name)
{
	mMaterialName = name;
}


std::string MCUTemplate::getMeshName()
{
	return mMeshName;
}

void MCUTemplate::setMeshName(std::string name)
{
	mMeshName = name;
}

std::string MCUTemplate::getThumbnailFile()
{
	return mThumbnailFile;
}

void MCUTemplate::setThumbnailFile(std::string name)
{
	mThumbnailFile = name;
}

std::string MCUTemplate::getProfileImageFile()
{
	return mProfileImageFile;
}

void MCUTemplate::setProfileImageFile(const std::string& name)
{
	mProfileImageFile = name;
}

float MCUTemplate::getMovementSpeed()
{
	return mMovementSpeed;
}

void MCUTemplate::setMovementSpeed(float speed)
{
	mMovementSpeed = speed;
}

float MCUTemplate::getWalkingAnimationSpeed()
{
	return mWalkingAnimationSpeed;
}

void MCUTemplate::setWalkingAnimationSpeed(float speed)
{
	mWalkingAnimationSpeed = speed;
}


float MCUTemplate::getScale()
{
	return mScale;
}

void MCUTemplate::setScale(float scale)
{
	mScale = scale;
}

float MCUTemplate::getTotalCost()
{
	float cost(this->getCost());
	cost+= mDefaultArmor->getCost();
	cost+= mDefaultBattery->getCost();
	cost+= mDefaultDrive->getCost();
	cost+= mDefaultReactor->getCost();
	cost+= mDefaultSensorSystem->getCost();
	cost+= mDefaultShield->getCost();
	cost+= mTargetingSystemTemplate->getCost();

	std::vector<WeaponHardpointTemplate*>::iterator weaponIterator;

	for(weaponIterator = mWeaponHardpointTemplates.begin(); weaponIterator != mWeaponHardpointTemplates.end(); ++weaponIterator)
	{
		if((*weaponIterator))
		{
			cost+= (*weaponIterator)->getDefaultWeapon()->getCost();
		}
	}

	return cost;
}

std::string MCUTemplate::getWalkSoundName()
{
	return mWalkSoundName;
}

void MCUTemplate::setWalkSoundName(const std::string& walkSound)
{
	mWalkSoundName = walkSound;
}

std::hash_map<std::string, std::string> MCUTemplate::getGuiValues()
{
	
	std::hash_map<std::string, std::string> result;

	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString( this->getId() );
	result[FieldNames::MCUTemplate::ArmorName] = mDefaultArmor->getName();
	result[FieldNames::MCUTemplate::BatteryName] = mDefaultBattery->getName();
	result[FieldNames::MCUTemplate::DriveName] = mDefaultDrive->getName();
	result[FieldNames::MCUTemplate::Cost] = SQLiteDBManager::convertFloatToString(this->getTotalCost());
	result[FieldNames::MCUTemplate::MCUTemplateName] = this->getName();
	result[FieldNames::MCUTemplate::ReactorName] = mDefaultReactor->getName();
	result[FieldNames::MCUTemplate::SensorSystemName] = mDefaultSensorSystem->getName();
	result[FieldNames::MCUTemplate::ShieldName] = mDefaultShield->getName();
	result[FieldNames::MCUTemplate::TargetingSystemName] = mTargetingSystemTemplate->getName();
	result[FieldNames::MCUTemplate::Description] = this->getDescription();
	
	return result;
}
