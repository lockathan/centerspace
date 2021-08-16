#include "DataProvider.h"
#include <Entities/DataSource.h>
#include <Entities/DataSet.h>
#include <GUI/DataBoundElement.h>
#include <GUI/DataProviderElement.h>
#include <Managers/CampaignMissionManager.h>
#include <hash_map>
#include <string>
#include <deque>

const std::string FieldNames::ItemID = "itemid";

const std::string FieldNames::Mission::MissionName = "@MissionName@";
const std::string FieldNames::Mission::Terrain = "@Terrain@";
const std::string FieldNames::Mission::SystemName = "@SystemName@";
const std::string FieldNames::Mission::LongDescription = "@Battlefield@";
const std::string FieldNames::Mission::MissionType = "@Battlefield@";

const std::string FieldNames::Player::FactionName = "@FactionName@";
const std::string FieldNames::Player::PlayerName = "@PlayerName@";
const std::string FieldNames::Player::PlayerNumber = "@PlayerNumber@";
const std::string FieldNames::Player::TeamNumber = "@TeamNumber@";
const std::string FieldNames::Player::Rank = "@Rank@";
const std::string FieldNames::Player::Cash = "@Cash@";

const std::string FieldNames::MCUTemplate::MCUTemplateName = "@MCUTemplateName@";
const std::string FieldNames::MCUTemplate::ArmorName = "@ArmorName@";
const std::string FieldNames::MCUTemplate::BatteryName = "@BatteryName@";
const std::string FieldNames::MCUTemplate::DriveName = "@DriveName@";
const std::string FieldNames::MCUTemplate::ReactorName = "@ReactorName@";
const std::string FieldNames::MCUTemplate::SensorSystemName = "@SensorSystemName@";
const std::string FieldNames::MCUTemplate::ShieldName = "@ShieldName@";
const std::string FieldNames::MCUTemplate::TargetingSystemName = "@TargetingSystemName@";
const std::string FieldNames::MCUTemplate::Cost = "@Cost@";
const std::string FieldNames::MCUTemplate::Description = "@Description@";

const std::string FieldNames::WeaponTemplate::WeaponName = "@WeaponName@";
const std::string FieldNames::WeaponTemplate::Range = "@Range@";
const std::string FieldNames::WeaponTemplate::Type = "@Type@";
const std::string FieldNames::WeaponTemplate::WeaponCost = "@WeaponCost@";
const std::string FieldNames::WeaponTemplate::WeaponWeight = "@WeaponWeight@";
const std::string FieldNames::WeaponTemplate::WeaponDescription = "@WeaponDescription@";
const std::string FieldNames::WeaponTemplate::ShieldDamage = "@ShieldDamage@";
const std::string FieldNames::WeaponTemplate::PiercingDamage = "@PiercingDamage@";
const std::string FieldNames::WeaponTemplate::ArmorDamage = "@ArmorDamage@";
const std::string FieldNames::WeaponTemplate::MaxAmmo = "@MaxAmmo@";
const std::string FieldNames::WeaponTemplate::EnergyCostPerTurn = "@EnergyCostPerTurn@";

const std::string FieldNames::ComponentTemplate::ComponentName = "@ComponentName@";
const std::string FieldNames::ComponentTemplate::ComponentCost = "@ComponentCost@";
const std::string FieldNames::ComponentTemplate::ComponentWeight = "@ComponentWeight@";
const std::string FieldNames::ComponentTemplate::ComponentDescription = "@ComponentDescription@";

const std::string FieldNames::MCU::MCUName = "@MCUName@";
const std::string FieldNames::MCU::MCUTemplateName = "@MCUTemplateName@";
const std::string FieldNames::MCU::ChanceToHitTarget = "@ChanceToHitTarget@";
const std::string FieldNames::MCU::MaxReactorPower = "@MaxReactorPower@";
const std::string FieldNames::MCU::RemainingReactorPower = "@RemainingReactorPower@";
const std::string FieldNames::MCU::MaxBatteryPower = "@MaxBatteryPower@";
const std::string FieldNames::MCU::RemainingBatteryPower = "@RemainingBatteryPower@";
const std::string FieldNames::MCU::MaxShieldPower = "@MaxShieldPower@";
const std::string FieldNames::MCU::RemainingShieldPower = "@RemainingShieldPower@";
const std::string FieldNames::MCU::ArmorName = "@ArmorName@";
const std::string FieldNames::MCU::BatteryName = "@BatteryName@";
const std::string FieldNames::MCU::DriveName = "@DriveName@";
const std::string FieldNames::MCU::ReactorName = "@ReactorName@";
const std::string FieldNames::MCU::SensorSystemName = "@SensorSystemName@";
const std::string FieldNames::MCU::ShieldName = "@ShieldName@";
const std::string FieldNames::MCU::TargetingSystemName = "@TargetingSystemName@";
const std::string FieldNames::MCU::ThumbnailImage = "@ThumbnailImage@";
const std::string FieldNames::MCU::ProfileImage = "@ProfileImage@";
const std::string FieldNames::MCU::Value = "@Value@";
const std::string FieldNames::MCU::RemainingHealthPercent = "@RemainingHealthPercent@";

const std::string FieldNames::Weapon::Accuracy = "@Accuracy@";
const std::string FieldNames::Weapon::EnergyCost = "@EnergyCost@";
const std::string FieldNames::Weapon::Range = "@Range@";
const std::string FieldNames::Weapon::RemainingAmmo = "@RemainingAmmo@";
const std::string FieldNames::Weapon::WeaponName = "@WeaponName@";
const std::string FieldNames::Weapon::CanFire = "@CanFire@";
const std::string FieldNames::Weapon::ChanceToHitTarget = "@ChanceToHitTarget@";
const std::string FieldNames::Weapon::ErrorText = "@ErrorText@";

const std::string FieldNames::BattleResults::MoneyEarned = "@MoneyEarned@";
const std::string FieldNames::BattleResults::TotalRepairCosts = "@TotalRepairCosts@";
const std::string FieldNames::BattleResults::NetProfit = "@NetProfit@";
const std::string FieldNames::BattleResults::ExperienceEarned = "@ExperienceEarned@";
const std::string FieldNames::BattleResults::Promotion = "@Promotion@";

const std::string FieldNames::MCUStatus::MCUName = "@MCUName@";
const std::string FieldNames::MCUStatus::RepairCost = "@RepairCost@";
const std::string FieldNames::MCUStatus::Status = "@Status@";


DataProvider::DataProvider()
{
	mBusyUpdating = false;
	mBufferContainsElements = false;
}

DataProvider::~DataProvider()
{
	mDataProviderElements.clear();
	mListenerBuffer.clear();
	mListeners.clear();
}

DataSource* DataProvider::getDataSource(DataSourceType source)
{
	return mData[source];
}

void DataProvider::changeSelectedPrimaryElement(DataSourceType source, std::string dataSourceID, int id, bool propagate)
{
	std::deque<DataProviderElement*>::iterator iter;
	
	for(iter = mDataProviderElements.begin(); iter != mDataProviderElements.end(); ++iter)
	{
		if(*iter)
		{
			DataProviderElement* provider = (*iter)->getDataProviderElement(source, dataSourceID);
			if(provider)
			{
				provider->changeSelectedElement(id, propagate);
			}

		}
	}

}

void DataProvider::registerForUpdates(DataProviderElement* listener)
{
	mDataProviderElements.push_front(listener);
}

void DataProvider::registerForUpdates(DataSourceUpdateListener* listener, DataSourceType source)
{
	mData[source]->registerForUpdates(listener);
}

void DataProvider::deregister(DataProviderElement* listener)
{
	std::deque<DataProviderElement*>::iterator eraser;

	if(mDataProviderElements.size() > 0)
	{
		for(eraser = mDataProviderElements.begin(); eraser != mDataProviderElements.end(); ++eraser)
		{
			if(listener == (*eraser))
			{
				mDataProviderElements.erase(eraser);
				return;
			}
		}
	}
}

void DataProvider::deregister(DataSourceUpdateListener* listener, DataSourceType source)
{
	if(source == DataSourceType::COULD_NOT_FIND)
		return;
	mData[source]->deregister(listener);
}

void DataProvider::updateDataSourceListeners(DataSourceType source)
{
	mData[source]->updateListeners();
}

void DataProvider::clearDataSource(DataSourceType source, bool preserveListeners)
{
	if(mData[source])
	{
		
		std::deque<DataProviderElement*>::iterator iter;
	
		for(iter = mDataProviderElements.begin(); iter != mDataProviderElements.end(); ++iter)
		{
			if(*iter)
			{
				if((*iter)->getDataSourceType() == source)
				{
					(*iter)->clearDataSource();
				}
			}
		}

		if(!preserveListeners)
		{
			delete mData[source];
			mData[source] = 0;
			mData[source] = new DataSource(source);
		}
		else
		{
			std::deque<DataSourceUpdateListener*> listeners = mData[source]->getListeners();
			delete mData[source];
			mData[source] = 0;
			mData[source] = new DataSource(source);
			mData[source]->setListeners(listeners);
		}
		
	}
	else
	{
		mData[source] = new DataSource(source);
	}
}

void DataProvider::registerListener(DataBoundElement* listener)
{
	if(!mBusyUpdating)
		mListeners.push_back(listener);
	else
	{
		mListenerBuffer.push_back(listener);
		mBufferContainsElements = true;
	}
}

void DataProvider::initializeAllListenersImpl()
{
	if(!mBusyUpdating)
	{
		mBusyUpdating = true;
		std::deque<DataBoundElement*>::iterator listenerIterator;

		for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end();++listenerIterator)
		{
			(*listenerIterator)->initialize(true);
		}
		mListeners.clear();
		mBusyUpdating = false;
	}
}


void DataProvider::initializeAllListeners()
{
	initializeAllListenersImpl();
	while(mBufferContainsElements)
	{
		mListeners = std::deque<DataBoundElement*>(mListenerBuffer);
		mBufferContainsElements = false;
		initializeAllListenersImpl();
		
		if(!mBufferContainsElements)
			mListenerBuffer.clear();
	}
}

DataSourceType DataProvider::getDataSource(std::string source)
{
	if(source == "randommissions")
		return DataSourceType::RANDOM_MISSIONS;

	if(source == "storymissions")
		return DataSourceType::STORY_MISSIONS;

	if(source == "mcus")
		return DataSourceType::MCU;

	if(source == "weapons")
		return DataSourceType::WEAPON;

	if(source == "singleplayerprofiles")
		return DataSourceType::PLAYER;

	if(source == "mcutemplates")
		return DataSourceType::MCUTEMPLATE;
	
	if(source == "weapontemplates")
		return DataSourceType::WEAPONTEMPLATE;
	
	if(source == "componenttemplates")
		return DataSourceType::COMPONENTTEMPLATE;
	
	if(source == "enemymcus")
		return DataSourceType::ENEMY_MCU;
	
	if(source == "battleresult")
		return DataSourceType::BATTLE_RESULT;
	
	if(source == "mcustatus")
		return DataSourceType::MCU_STATUS;

	if(source == "none" || source == "")
		return DataSourceType::NO_DATA_SOURCE;
	
	return DataSourceType::COULD_NOT_FIND;
}

std::string DataProvider::getTypeString(DataSourceType type)
{
	switch(type)
	{
		
	case DataSourceType::RANDOM_MISSIONS:
		return "random";

		case DataSourceType::STORY_MISSIONS:
		return "story";

		case DataSourceType::MCU:
		return "MCU";

		case DataSourceType::WEAPON:
		return "WEAPON";

		case DataSourceType::PLAYER:
		return "PLAYER";

		case DataSourceType::MCUTEMPLATE:
		return "MCUTEMPLATE";
	
		case DataSourceType::WEAPONTEMPLATE:
		return "WEAPONTEMPLATE";
	
		case DataSourceType::COMPONENTTEMPLATE:
		return "COMPONENTTEMPLATE";
	
		case DataSourceType::ENEMY_MCU:
		return "enemy mcu";
	
		case DataSourceType::BATTLE_RESULT:
		return "battle result";
	
		case DataSourceType::MCU_STATUS:
		return "mcu status";

		case DataSourceType::NO_DATA_SOURCE:
		return "no data source";
	}
	return "";
}

void DataProvider::setDataProviderElementVisible(DataSourceType source, std::string id, bool visible)
{
	
	std::deque<DataProviderElement*>::iterator iter;
	
	for(iter = mDataProviderElements.begin(); iter != mDataProviderElements.end(); ++iter)
	{
		if(*iter)
		{
			if((*iter)->getDataSourceID() == id && (*iter)->getDataSourceType() == source)
			{
				//(*iter)->setItemID(id);
				(*iter)->setVisible(visible);
				//return;
			}
		}
	}
}

void DataProvider::registerLuaScriptElement(LuaScriptElement* element)
{
	std::deque<DataProviderElement*>::iterator iter;
	
	for(iter = mDataProviderElements.begin(); iter != mDataProviderElements.end(); ++iter)
	{
		(*iter)->registerLuaScriptElement(element);
	}
}

void DataProvider::deregisterLuaScriptElement(LuaScriptElement* element)
{
	std::deque<DataProviderElement*>::iterator iter;
	
	for(iter = mDataProviderElements.begin(); iter != mDataProviderElements.end(); ++iter)
	{
		(*iter)->deregisterLuaScriptElement(element);
	}
}

DataProvider* DataProvider::mDataProvider;

DataProvider* DataProvider::getSingletonPtr()
{
	if(!mDataProvider)
	{
		mDataProvider = new DataProvider();
	}
	return mDataProvider;
}