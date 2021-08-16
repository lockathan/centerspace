#include "InventoryState.h"
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/WeaponHardpoint.h>
#include <Managers/DbNames.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/BatteryTemplateRepository.h>
#include <Repositories/DriveTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/ReactorTemplateRepository.h>
#include <Repositories/WeaponTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Ogre.h>
#include <vector>
#include <hash_map>
#include <deque>

using namespace Ogre;

InventoryState::InventoryState()
{
	mRank = 1;
	mRemainingCash = 0;
	mSQLiteDBManager = SQLiteDBManager::getSingletonPtr();
}

InventoryState::~InventoryState()
{

}

void InventoryState::enter() 
{

}

void InventoryState::exit() 
{

}

void InventoryState::load() 
{

}

void InventoryState::save() 
{
	mSQLiteDBManager->saveInventoryState(this);
}

void InventoryState::addMCU(MechanizedCombatUnit* mcu)
{
	mcu->setInventoryState(this);
	mMCUs.push_back(mcu);

	if(mMCUSequenceNumbers[mcu->getMCUTemplate()->getName()] <= 0)
		mMCUSequenceNumbers[mcu->getMCUTemplate()->getName()] = mcu->getSequence();
	else
	{
		mcu->setSequence(mMCUSequenceNumbers[mcu->getMCUTemplate()->getName()] + 1);
		mMCUSequenceNumbers[mcu->getMCUTemplate()->getName()] = mcu->getSequence();
	}
	save();
}

void InventoryState::removeMCU(MechanizedCombatUnit* mcu)
{
	std::deque<MechanizedCombatUnit*>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter) == mcu)
		{
			mMCUs.erase(iter);
		}
	}
	delete mcu;
	save();
}

void InventoryState::setMCUs(std::deque<MechanizedCombatUnit*> mcus)
{
	mMCUs = mcus;
}

std::deque<MechanizedCombatUnit*> InventoryState::getMCUs()
{
	return mMCUs;
}


MechanizedCombatUnit* InventoryState::getMCU(long id)
{
	std::deque<MechanizedCombatUnit*>::iterator mcu;

	for(mcu = mMCUs.begin(); mcu != mMCUs.end(); ++mcu)
	{
		if((*mcu)->getId() == id)
			return (*mcu);
	}

	return 0;
}
int InventoryState::getRank()
{
	return mRank;
}

void InventoryState::setRank(int rank)
{
	mRank = rank;
}

float InventoryState::getRemainingCash()
{
	return mRemainingCash;
}

void InventoryState::setRemainingCash(float cash)
{
	mRemainingCash = cash;
}

FactionTemplate* InventoryState::getFactionTemplate()
{
	return mFactionTemplate;
}

void InventoryState::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

void InventoryState::changeMCUComponent(MCUBayState::ComponentType componentType,long mcuId, int componentId, int hardpoint)
{
	if(componentType == MCUBayState::ComponentType::CT_BATTERY)
	{
		BatteryTemplate* battery = BatteryTemplateRepository::getSingletonPtr()->at(componentId);
		mRemainingCash-=battery->getCost();
		mMCUs[mcuId]->setBattery(battery);
		return;
	}
	if(componentType ==  MCUBayState::ComponentType::CT_DRIVE)
	{
		DriveTemplate* drive = DriveTemplateRepository::getSingletonPtr()->at(componentId);
		mRemainingCash-=drive->getCost();
		mMCUs[mcuId]->setDrive(drive);
		return;
	}
	if(componentType ==  MCUBayState::ComponentType::CT_REACTOR)
	{
		ReactorTemplate* reactor = ReactorTemplateRepository::getSingletonPtr()->at(componentId);
		mRemainingCash-=reactor->getCost();
		mMCUs[mcuId]->setReactor(reactor);
		return;
	}

	if(componentType ==  MCUBayState::ComponentType::CT_WEAPON)
	{ 
		WeaponTemplate* weapon = WeaponTemplateRepository::getSingletonPtr()->at(componentId);
		mRemainingCash-=weapon->getCost();
		mMCUs[mcuId]->setWeapon(hardpoint, weapon);
		return;
	}
}

void InventoryState::changeMCUComponent(MCUBayState::ComponentType componentType, MechanizedCombatUnit* mcu, MCUComponentTemplate* component, int hardpoint)
{
	mRemainingCash-=component->getCost();

	switch(componentType)
		{
		case MCUBayState::ComponentType::CT_ARMOR:
			mRemainingCash +=mcu->getArmor()->getCost();
			mcu->setArmor((ArmorTemplate*)component);
			break;
		case MCUBayState::ComponentType::CT_BATTERY:
			mRemainingCash +=mcu->getBattery()->getCost();
			mcu->setBattery((BatteryTemplate*)component);
			break;
		case MCUBayState::ComponentType::CT_DRIVE:
			mRemainingCash +=mcu->getDrive()->getCost();
			mcu->setDrive((DriveTemplate*)component);
			break;
		case MCUBayState::ComponentType::CT_REACTOR:
			mRemainingCash +=mcu->getReactor()->getCost();
			mcu->setReactor((ReactorTemplate*)component);
			break;
		case MCUBayState::ComponentType::CT_SENSOR:
			mRemainingCash +=mcu->getSensorSystem()->getCost();
			mcu->setSensorSystem((SensorSystemTemplate*)component);
			break;
		case MCUBayState::ComponentType::CT_SHIELD:
			mRemainingCash +=mcu->getShield()->getCost();
			mcu->setShield((ShieldTemplate*)component);
			break;
		case MCUBayState::ComponentType::CT_TARGET:
			mRemainingCash +=mcu->getTargetingSystem()->getCost();
			mcu->setTargetingSystem((TargetingSystemTemplate*)component);
			break;
		case MCUBayState::ComponentType::CT_WEAPON:
			mRemainingCash += mcu->getWeaponHardpoint(hardpoint)->getWeapon()->getCost();
			mcu->setWeapon(hardpoint, (WeaponTemplate*) component);
			break;
		}
}

std::hash_map<std::string,DbValue> InventoryState::getValues()
{
	std::hash_map<std::string,DbValue> values;
	values[DbNames::InventoryState::ID].StringValue = SQLiteDBManager::convertLongToString(mId);
	values[DbNames::InventoryState::ID].IsNumber = true;

	values[DbNames::InventoryState::FACTION_ID].StringValue = SQLiteDBManager::convertIntToString(mFactionTemplate->getId());
	values[DbNames::InventoryState::FACTION_ID].IsNumber = true;

	values[DbNames::InventoryState::RANK].StringValue = SQLiteDBManager::convertIntToString(mRank);
	values[DbNames::InventoryState::RANK].IsNumber = true;

	values[DbNames::InventoryState::REMAININGCASH].StringValue = SQLiteDBManager::convertFloatToString(mRemainingCash);
	values[DbNames::InventoryState::REMAININGCASH].IsNumber = true;

	return values;
}

void InventoryState::setValues(std::hash_map<std::string,std::string> values)
{
	mId = atol(values[DbNames::InventoryState::ID].c_str());
	mFactionTemplate = FactionTemplateRepository::getSingletonPtr()->at( atoi(values[DbNames::InventoryState::FACTION_ID].c_str()));
	mRank = atoi(values[DbNames::InventoryState::RANK].c_str());
	mRemainingCash = atof(values[DbNames::InventoryState::REMAININGCASH].c_str());
}
