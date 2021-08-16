#include <Entities/MechanizedCombatUnit.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/Projectile.h>
#include <Entities/WeaponHardpoint.h>
#include <Managers/DbNames.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/ArmorTemplateRepository.h>
#include <Repositories/BatteryTemplateRepository.h>
#include <Repositories/DriveTemplateRepository.h>
#include <Repositories/MCUTemplateRepository.h>
#include <Repositories/ReactorTemplateRepository.h>
#include <Repositories/SensorSystemTemplateRepository.h>
#include <Repositories/ShieldTemplateRepository.h>
#include <Repositories/TargetingSystemTemplateRepository.h>
#include <States/InventoryState.h>
#include <Templates/ArmorTemplate.h>
#include <Templates/BatteryTemplate.h>
#include <Templates/DriveTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/ReactorTemplate.h>
#include <Templates/SensorSystemTemplate.h>
#include <Templates/ShieldTemplate.h>
#include <Templates/TargetingSystemTemplate.h>
#include "Ogre.h"
#include <hash_map>
#include <vector>

MechanizedCombatUnit::MechanizedCombatUnit()
	:
	
	mArmor(0),
	mBattery(0),
	mDrive(0),
	mReactor(0),
	mSensorSystem(0),
	mShield(0),
	mTargetingSystem(0),
	mTotalWeight(0),
	mSequence(1),
	mMCUTemplate(0),
	mWeaponHardpoints(10)
{
	std::vector<WeaponHardpoint*>::iterator weaponIterator;

	for(weaponIterator = mWeaponHardpoints.begin(); weaponIterator != mWeaponHardpoints.end(); ++weaponIterator)
	{
		(*weaponIterator) = 0;
	}
}

MechanizedCombatUnit::~MechanizedCombatUnit()
{
	std::vector<WeaponHardpoint*>::iterator weaponIterator;

	for(weaponIterator = mWeaponHardpoints.begin(); weaponIterator != mWeaponHardpoints.end(); ++weaponIterator)
	{
		if((*weaponIterator))
			delete (*weaponIterator);

		weaponIterator = mWeaponHardpoints.erase(weaponIterator);
	}
}

ArmorTemplate* MechanizedCombatUnit::getArmor()
{
	return mArmor;
}

void MechanizedCombatUnit::setArmor(ArmorTemplate* armor)
{
	mArmor = armor;
}

BatteryTemplate* MechanizedCombatUnit::getBattery()
{
	return mBattery;
}

void MechanizedCombatUnit::setBattery(BatteryTemplate* battery)
{
	mBattery = battery;
	recalculateWeight();
}

DriveTemplate* MechanizedCombatUnit::getDrive()
{
	return mDrive;
}

void MechanizedCombatUnit::setDrive(DriveTemplate* drive)
{
	mDrive = drive;
}

InventoryState* MechanizedCombatUnit::getInventoryState()
{
	return mInventoryState;
}

void MechanizedCombatUnit::setInventoryState(InventoryState* state)
{
	mInventoryState = state;
}

MCUTemplate* MechanizedCombatUnit::getMCUTemplate()
{
	return mMCUTemplate;
}

void MechanizedCombatUnit::setMCUTemplate(MCUTemplate* t)
{
	mMCUTemplate = t;
}

ReactorTemplate* MechanizedCombatUnit::getReactor()
{
	return mReactor;
}

void MechanizedCombatUnit::setReactor(ReactorTemplate* reactor)
{
	mReactor = reactor;
	recalculateWeight();
}

SensorSystemTemplate* MechanizedCombatUnit::getSensorSystem()
{
	return mSensorSystem;
}

void MechanizedCombatUnit::setSensorSystem(SensorSystemTemplate* sensor)
{
	mSensorSystem = sensor;
}

ShieldTemplate* MechanizedCombatUnit::getShield()
{
	return mShield;
}

void MechanizedCombatUnit::setShield(ShieldTemplate* shield)
{
	mShield = shield;
}

TargetingSystemTemplate* MechanizedCombatUnit::getTargetingSystem()
{
	return mTargetingSystem;
}

void MechanizedCombatUnit::setTargetingSystem(TargetingSystemTemplate* target)
{
	mTargetingSystem = target;
}

WeaponHardpoint* MechanizedCombatUnit::getWeaponHardpoint(unsigned int slot)
{	
	if(slot >= mWeaponHardpoints.size())
		return 0;

	return mWeaponHardpoints[slot];
}

std::vector<WeaponHardpoint*> MechanizedCombatUnit::getWeapons()
{
	return mWeaponHardpoints;
}

void MechanizedCombatUnit::setWeapon(int slot, WeaponTemplate* weapon)
{
	if(mWeaponHardpoints[slot])
	{
		mWeaponHardpoints[slot]->setWeapon(weapon);
	}
	else
	{
		WeaponHardpoint* hardpoint = new WeaponHardpoint();
		hardpoint->setHardpointNumber(slot);
		hardpoint->setWeapon(weapon);
		hardpoint->setMCU(this);
		mWeaponHardpoints[slot] = hardpoint;
	}
	recalculateWeight();
}

void MechanizedCombatUnit::addWeapon(WeaponHardpoint* weapon)
{
	weapon->setMCU(this);
	mWeaponHardpoints[weapon->getHardpointNumber()] = weapon;
	recalculateWeight();
}

void MechanizedCombatUnit::setWeapons(const std::vector<WeaponHardpoint*>& weapons)
{
	mWeaponHardpoints.clear();
	mWeaponHardpoints = weapons;
	std::vector<WeaponHardpoint*>::iterator weaponIterator;

	for(weaponIterator = mWeaponHardpoints.begin(); weaponIterator < mWeaponHardpoints.end(); ++weaponIterator)
	{
		if((*weaponIterator))
			(*weaponIterator)->setMCU(this);
	}

	recalculateWeight();
}



void MechanizedCombatUnit::removeWeapon(int slot)
{
	
}

void MechanizedCombatUnit::recalculateWeight()
{
	mTotalWeight = 0;
	if(mBattery)
		mTotalWeight+= mBattery->getWeight();
	if(mDrive)
		mTotalWeight+= mDrive->getWeight();
	if(mReactor)
		mTotalWeight+= mReactor->getWeight();

	std::vector< WeaponHardpoint*>::iterator weaponIterator;

	for(weaponIterator = mWeaponHardpoints.begin(); weaponIterator != mWeaponHardpoints.end(); ++weaponIterator)
	{
		if((*weaponIterator))
			mTotalWeight+=(*weaponIterator)->getWeapon()->getWeight();
	}
}

float MechanizedCombatUnit::getTotalWeight()
{
	return mTotalWeight;
}

float MechanizedCombatUnit::getMovementCostPerSquare()
{
	if(mDrive->getEfficiency() != 0)
	{
		return (mTotalWeight / mDrive->getEfficiency()) / 1000.f;
	}
	return 10000;
	
}

int MechanizedCombatUnit::getSequence()
{
	return mSequence;
}

void MechanizedCombatUnit::setSequence(int sequence)
{
	mSequence = sequence;
	mName = mMCUTemplate->getName() + " " + SQLiteDBManager::convertIntToString(sequence);
}


void MechanizedCombatUnit::applyWarhead(WeaponTemplate* weapon, const float& shieldDamageNotAbsorbed, bool shieldWasActive)
{

}

std::hash_map<std::string,DbValue> MechanizedCombatUnit::getValues()
{
	std::hash_map<std::string,DbValue> values;
	values[DbNames::MCU::ID].StringValue = SQLiteDBManager::convertLongToString(mId);
	values[DbNames::MCU::ID].IsNumber = true;

	values[DbNames::MCU::ARMOR_ID].StringValue = SQLiteDBManager::convertIntToString(mArmor->getId());
	values[DbNames::MCU::ARMOR_ID].IsNumber = true;
	values[DbNames::MCU::BATTERY_ID].StringValue = SQLiteDBManager::convertIntToString(mBattery->getId());
	values[DbNames::MCU::BATTERY_ID].IsNumber = true;
	values[DbNames::MCU::DRIVE_ID].StringValue = SQLiteDBManager::convertIntToString(mDrive->getId());
	values[DbNames::MCU::DRIVE_ID].IsNumber = true;
	values[DbNames::MCU::REACTOR_ID].StringValue = SQLiteDBManager::convertIntToString(mReactor->getId());
	values[DbNames::MCU::REACTOR_ID].IsNumber = true;
	values[DbNames::MCU::TEMPLATE_ID].StringValue = SQLiteDBManager::convertIntToString(mMCUTemplate->getId());
	values[DbNames::MCU::TEMPLATE_ID].IsNumber = true;
	values[DbNames::MCU::INVENTORY_ID].StringValue = SQLiteDBManager::convertLongToString(mInventoryState->getId());
	values[DbNames::MCU::INVENTORY_ID].IsNumber = true;
	values[DbNames::MCU::TARGETING_SYS_ID].StringValue = SQLiteDBManager::convertLongToString(mTargetingSystem->getId());
	values[DbNames::MCU::TARGETING_SYS_ID].IsNumber = true;
	values[DbNames::MCU::SHIELD_ID].StringValue = SQLiteDBManager::convertLongToString(mShield->getId());
	values[DbNames::MCU::SHIELD_ID].IsNumber = true;
	values[DbNames::MCU::SENSOR_ID].StringValue = SQLiteDBManager::convertLongToString(mSensorSystem->getId());
	values[DbNames::MCU::SENSOR_ID].IsNumber = true;

	values[DbNames::MCU::NAME].StringValue = mName;
	values[DbNames::MCU::NAME].IsNumber = false;

	return values;
}

void MechanizedCombatUnit::setValues(std::hash_map<std::string,std::string> values)
{
	setId(atol(values[DbNames::MCU::ID].c_str()));
	setArmor(ArmorTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::ARMOR_ID].c_str())));
	setBattery(BatteryTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::BATTERY_ID].c_str())));
	setDrive(DriveTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::DRIVE_ID].c_str())));
	setReactor(ReactorTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::REACTOR_ID].c_str())));
	setMCUTemplate(MCUTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::TEMPLATE_ID].c_str())));
	setSensorSystem(SensorSystemTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::SENSOR_ID].c_str())));
	setShield(ShieldTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::SHIELD_ID].c_str())));
	setTargetingSystem(TargetingSystemTemplateRepository::getSingletonPtr()->at(atol(values[DbNames::MCU::TARGETING_SYS_ID].c_str())));
	setName(values[DbNames::MCU::NAME]);
	recalculateWeight();
}


std::hash_map<std::string,std::string> MechanizedCombatUnit::getGuiValues()
{
	std::hash_map<std::string,std::string> result;

	result[FieldNames::MCU::MCUName] = this->getName();
	result[FieldNames::MCU::MCUTemplateName] = this->getMCUTemplate()->getName();
	result[FieldNames::MCU::ArmorName] = mArmor->getName();
	result[FieldNames::MCU::BatteryName] = mBattery->getName();
	result[FieldNames::MCU::DriveName] = mDrive->getName();
	result[FieldNames::MCU::ReactorName] = mReactor->getName();
	result[FieldNames::MCU::SensorSystemName] = mSensorSystem->getName();
	result[FieldNames::MCU::ShieldName] = mShield->getName();
	result[FieldNames::MCU::TargetingSystemName] = mTargetingSystem->getName();

	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString( this->getId() );

	return result;
}

DataSet* MechanizedCombatUnit::getWeaponGuiData()
{
	DataSet* result = new DataSet(DataSourceType::WEAPON);

	std::vector<WeaponHardpoint*>::iterator iter;

	for(iter = mWeaponHardpoints.begin(); iter != mWeaponHardpoints.end(); ++iter)
	{
		if((*iter))
			result->addRecord((*iter)->getHardpointNumber(), (*iter)->getGuiValues());
	}

	return result;
}
