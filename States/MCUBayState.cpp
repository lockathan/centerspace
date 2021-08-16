#include "MCUBayState.h"
#include <States/InBaseState.h>
#include <Ogre.h>
#include <States/CampaignState.h>
#include <States/InventoryState.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/PlayerProfile.h>
#include <Entities/WeaponHardpoint.h>
#include <Repositories/ArmorTemplateRepository.h>
#include <Repositories/BatteryTemplateRepository.h>
#include <Repositories/DriveTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUTemplateRepository.h>
#include <Repositories/ReactorTemplateRepository.h>
#include <Repositories/SensorSystemTemplateRepository.h>
#include <Repositories/ShieldTemplateRepository.h>
#include <Repositories/TargetingSystemTemplateRepository.h>
#include <Repositories/WeaponTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Managers/SQLiteDBManager.h>
#include <GUI/RocketDocuments.h>
using namespace Ogre;

MCUBayState::MCUBayState()
	:
	GameState(),
	mSQLiteDBManager(SQLiteDBManager::getSingletonPtr()),
	mPlayerProfile(0),
	mInventoryState(0)
{

}

MCUBayState::~MCUBayState()
{

}

void MCUBayState::enter() 
{
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	mSelectedMCUTemplate = 0;
	mIsEnabled = true;
	mEditingWeapon = false;
	mWeaponHardpoint = 0;
	mComponentId = -1;
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::PLAYER);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::WEAPON);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCU);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCUTEMPLATE);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::WEAPONTEMPLATE);
	DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->setSubDataSourceType(DataSourceType::WEAPON);
	mRocketRenderer->loadDocument(RocketDocuments::MCUBay::Main);
}

void MCUBayState::exit() 
{
	mInventoryState->save();
	mIsEnabled = false;
}

void MCUBayState::pause() 
{
}

void MCUBayState::resume() 
{

}

void MCUBayState::update(Real timeSinceLastFrame)
{
	if(!mIsEnabled)
		return;
}

PlayerProfile* MCUBayState::getPlayerProfile()
{
	return mPlayerProfile;
}

void MCUBayState::setPlayerProfile(PlayerProfile* player)
{
	mPlayerProfile = player;

}

InventoryState* MCUBayState::getInventoryState()
{
	return mInventoryState;
}

void MCUBayState::setInventoryState(InventoryState* inventory)
{
	mInventoryState = inventory;
}

void MCUBayState::ProcessEvent(Rocket::Core::Event& ev)
{


	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "loadmcupurchasescreen")
	{
		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCUTEMPLATE);
		std::vector<MCUTemplate*> mcuTemplates = MCUTemplateRepository::getSingletonPtr()->getAvailableMCUTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank());
		std::deque<DataSetItem*> data;
		std::vector<MCUTemplate*>::iterator iter;
		for(iter = mcuTemplates.begin(); iter != mcuTemplates.end(); ++iter)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*iter)->getGuiValues();
			item->mId = (*iter)->getId();
			data.push_back(item);
		}

		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCUTEMPLATE)->getPrimaryDataSet()->setData(data);
		mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUPurchase);
		if(mPlayerProfile && mInventoryState)
		{
			DataProvider::getSingletonPtr()->getDataSource(DataSourceType::PLAYER)->getPrimaryDataSet()->addRecord(mPlayerProfile->getId(), this->getGuiValues());
			DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::PLAYER, "campaigninfodp", mPlayerProfile->getId());
		}
	}
	
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "purchasemcu")
	{
		if(mSelectedMCUTemplate)
		{
			if(mSelectedMCUTemplate->getTotalCost() > mInventoryState->getRemainingCash())
			{
				//cant afford it - message box
				return;
			}

			MechanizedCombatUnit* mcu = mSelectedMCUTemplate->instantiate(true);
			mSelectedMCUTemplate = 0;
			mInventoryState->addMCU(mcu);
			DataProvider::getSingletonPtr()->getDataSource(DataSourceType::PLAYER)->getPrimaryDataSet()->updateData(mPlayerProfile->getId(), this->getGuiValues());
			mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUSelectEdit);
		}
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "loadmcuconfigurescreen")
	{
		
		
		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCU);
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->setSubDataSourceType(DataSourceType::WEAPON);
		std::deque<MechanizedCombatUnit*> mcus = mInventoryState->getMCUs();	
		std::deque<MechanizedCombatUnit*>::iterator mcuIterator;
		std::deque<DataSetItem*> data;

		for(mcuIterator = mcus.begin(); mcuIterator < mcus.end(); ++mcuIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*mcuIterator)->getGuiValues();
			item->mId = (*mcuIterator)->getId();
			data.push_back(item);
			DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->addSubDataSet(item->mId, (*mcuIterator)->getWeaponGuiData());
		}

		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->getPrimaryDataSet()->setData(data);
		mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUSelectEdit);
		if(mPlayerProfile && mInventoryState)
		{
			DataProvider::getSingletonPtr()->getDataSource(DataSourceType::PLAYER)->getPrimaryDataSet()->addRecord(mPlayerProfile->getId(), this->getGuiValues());
			DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::PLAYER, "campaigninfodp", mPlayerProfile->getId());
		}
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "loadmcuditscreen")
	{
		mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUEdit);
		if(mPlayerProfile && mInventoryState)
		{
			DataProvider::getSingletonPtr()->getDataSource(DataSourceType::PLAYER)->getPrimaryDataSet()->addRecord(mPlayerProfile->getId(), this->getGuiValues());
			DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::PLAYER, "campaigninfodp", mPlayerProfile->getId());
		}
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "editarmor")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", true);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPON,"weaponselectdp", -1, false);
		mEditingWeapon = false;
		RequestedComponentType = ComponentType::CT_ARMOR;

		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
		std::vector<ArmorTemplate*> components = ArmorTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank(),mSelectedMCU->getMCUTemplate()->getMaxArmorSize());
		std::vector<ArmorTemplate*>::iterator componentIterator;
		std::deque<DataSetItem*> data;

		for(componentIterator = components.begin();componentIterator < components.end(); ++componentIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*componentIterator)->getGuiValues();
			item->mId = (*componentIterator)->getId();
			data.push_back(item);
		}
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::COMPONENTTEMPLATE)->getPrimaryDataSet()->setData(data);
		mComponentId = mSelectedMCU->getArmor()->getId();
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::COMPONENTTEMPLATE,"componenteditdp",mComponentId, false);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "editbattery")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", true);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPON,"weaponselectdp", -1, false);
		mEditingWeapon = false;
		RequestedComponentType = ComponentType::CT_BATTERY;

		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
		std::vector<BatteryTemplate*> components = BatteryTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank(),mSelectedMCU->getMCUTemplate()->getMaxBatterySize());
		std::vector<BatteryTemplate*>::iterator componentIterator;
		std::deque<DataSetItem*> data;

		for(componentIterator = components.begin();componentIterator < components.end(); ++componentIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*componentIterator)->getGuiValues();
			item->mId = (*componentIterator)->getId();
			data.push_back(item);
		}
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::COMPONENTTEMPLATE)->getPrimaryDataSet()->setData(data);
		mComponentId = mSelectedMCU->getBattery()->getId();
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::COMPONENTTEMPLATE,"componenteditdp",mComponentId, false);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "editdrive")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", true);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPON,"weaponselectdp", -1, false);
		mEditingWeapon = false;
		RequestedComponentType = ComponentType::CT_DRIVE;

		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
		std::vector<DriveTemplate*> components = DriveTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank(),mSelectedMCU->getMCUTemplate()->getMaxDriveSize());
		std::vector<DriveTemplate*>::iterator componentIterator;
		std::deque<DataSetItem*> data;

		for(componentIterator = components.begin();componentIterator < components.end(); ++componentIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*componentIterator)->getGuiValues();
			item->mId = (*componentIterator)->getId();
			data.push_back(item);
		}
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::COMPONENTTEMPLATE)->getPrimaryDataSet()->setData(data);
		mComponentId = mSelectedMCU->getDrive()->getId();
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::COMPONENTTEMPLATE,"componenteditdp",mComponentId, false);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "editreactor")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", true);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPON,"weaponselectdp", -1, false);
		mEditingWeapon = false;
		RequestedComponentType = ComponentType::CT_REACTOR;

		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
		std::vector<ReactorTemplate*> components = ReactorTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank(),mSelectedMCU->getMCUTemplate()->getMaxReactorSize());
		std::vector<ReactorTemplate*>::iterator componentIterator;
		std::deque<DataSetItem*> data;

		for(componentIterator = components.begin();componentIterator < components.end(); ++componentIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*componentIterator)->getGuiValues();
			item->mId = (*componentIterator)->getId();
			data.push_back(item);
		}
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::COMPONENTTEMPLATE)->getPrimaryDataSet()->setData(data);
		mComponentId = mSelectedMCU->getReactor()->getId();
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::COMPONENTTEMPLATE,"componenteditdp",mComponentId, false);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "editsensor")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", true);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPON,"weaponselectdp", -1, false);
		mEditingWeapon = false;
		RequestedComponentType = ComponentType::CT_SENSOR;

		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
		std::vector<SensorSystemTemplate*> components = SensorSystemTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank());
		std::vector<SensorSystemTemplate*>::iterator componentIterator;
		std::deque<DataSetItem*> data;

		for(componentIterator = components.begin();componentIterator < components.end(); ++componentIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*componentIterator)->getGuiValues();
			item->mId = (*componentIterator)->getId();
			data.push_back(item);
		}
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::COMPONENTTEMPLATE)->getPrimaryDataSet()->setData(data);
		mComponentId = mSelectedMCU->getSensorSystem()->getId();
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::COMPONENTTEMPLATE,"componenteditdp",mComponentId, false);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "editshield")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", true);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPON,"weaponselectdp", -1, false);
		mEditingWeapon = false;
		RequestedComponentType = ComponentType::CT_SHIELD;

		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
		std::vector<ShieldTemplate*> components = ShieldTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank(),mSelectedMCU->getMCUTemplate()->getMaxShieldSize());
		std::vector<ShieldTemplate*>::iterator componentIterator;
		std::deque<DataSetItem*> data;

		for(componentIterator = components.begin();componentIterator < components.end(); ++componentIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*componentIterator)->getGuiValues();
			item->mId = (*componentIterator)->getId();
			data.push_back(item);
		}
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::COMPONENTTEMPLATE)->getPrimaryDataSet()->setData(data);
		mComponentId = mSelectedMCU->getShield()->getId();
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::COMPONENTTEMPLATE,"componenteditdp",mComponentId, false);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "edittarget")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", true);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPON,"weaponselectdp", -1, false);

		mEditingWeapon = false;
		RequestedComponentType = ComponentType::CT_TARGET;

		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::COMPONENTTEMPLATE);
		std::vector<TargetingSystemTemplate*> components = TargetingSystemTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(),mInventoryState->getRank());
		std::vector<TargetingSystemTemplate*>::iterator componentIterator;
		std::deque<DataSetItem*> data;

		for(componentIterator = components.begin();componentIterator < components.end(); ++componentIterator)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*componentIterator)->getGuiValues();
			item->mId = (*componentIterator)->getId();
			data.push_back(item);
		}
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::COMPONENTTEMPLATE)->getPrimaryDataSet()->setData(data);
		mComponentId = mSelectedMCU->getTargetingSystem()->getId();
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::COMPONENTTEMPLATE,"componenteditdp",mComponentId, false);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "purchasecomponent")
	{
		if(mComponentId < 0 || RequestedComponentType == ComponentType::CT_NONE)
			return;
		MCUComponentTemplate* component(0);

		switch(RequestedComponentType)
		{
		case CT_ARMOR:
			component = ArmorTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		case CT_BATTERY:
			component = BatteryTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		case CT_DRIVE:
			component = DriveTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		case CT_REACTOR:
			component = ReactorTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		case CT_SENSOR:
			component = SensorSystemTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		case CT_SHIELD:
			component = ShieldTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		case CT_TARGET:
			component = TargetingSystemTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		case CT_WEAPON:
			component = WeaponTemplateRepository::getSingletonPtr()->at(mComponentId);
			break;
		}

		if(component->getCost() > mInventoryState->getRemainingCash())
			return;

		if(RequestedComponentType == CT_WEAPON)
			mInventoryState->changeMCUComponent(RequestedComponentType, mSelectedMCU, component, mWeaponHardpoint->getHardpointNumber());
		else
			mInventoryState->changeMCUComponent(RequestedComponentType, mSelectedMCU, component);

		//mSelectedMCU = mInventoryState->getMCU(mSelectedMCU->getId());
		std::vector<WeaponHardpoint*> weapons = mSelectedMCU->getWeapons();
		std::vector<WeaponHardpoint*>::iterator iter;

		for(iter = weapons.begin(); iter < weapons.end();++iter)
		{
			DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->getSubDataSet(mSelectedMCU->getId())->updateData((*iter)->getHardpointNumber(), (*iter)->getGuiValues());
		}

		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->getPrimaryDataSet()->updateData(mSelectedMCU->getId(), mSelectedMCU->getGuiValues());
		DataProvider::getSingletonPtr()->updateDataSourceListeners(DataSourceType::MCU);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", false);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		mComponentId = -1;
		RequestedComponentType = ComponentType::CT_NONE;
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::PLAYER)->getPrimaryDataSet()->updateData(mPlayerProfile->getId(), this->getGuiValues());
	}
	
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "cancelpurchasecomponent")
	{
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", false);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
		mComponentId = -1;
		RequestedComponentType = ComponentType::CT_NONE;
	}
	
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "backtomcubayscreen")
	{
		mRocketRenderer->loadDocument(RocketDocuments::MCUBay::Main);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "backtomainbasescreen")
	{
		mGameManager->changeState(InBaseState::getSingletonPtr());
	}
}

void MCUBayState::onSelectedElementChanged(DataSourceType source, int id)
{
	if(source == DataSourceType::MCUTEMPLATE)
	{
		mSelectedMCUTemplate = MCUTemplateRepository::getSingletonPtr()->at(id);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCUTEMPLATE,"mcuinfodp", id, false);
	}
	else if(source == DataSourceType::MCU)
	{
		mSelectedMCU = mInventoryState->getMCU(id);
		mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUEdit);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCU,"mcueditdp", id, false);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", false);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", false);
	}
	else if(source == DataSourceType::WEAPON)
	{
		mEditingWeapon = true;
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::COMPONENTTEMPLATE, "componenteditdp", false);
		DataProvider::getSingletonPtr()->setDataProviderElementVisible(DataSourceType::WEAPONTEMPLATE, "weaponeditdp", true);
		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::WEAPONTEMPLATE);

		mWeaponHardpoint = mSelectedMCU->getWeaponHardpoint(id);
		RequestedComponentType = ComponentType::CT_WEAPON;
		std::vector<WeaponTemplate*> weapons = WeaponTemplateRepository::getSingletonPtr()->getAvailableTemplates(mInventoryState->getFactionTemplate()->getId(), mInventoryState->getRank(), mWeaponHardpoint->getWeaponHardpointTemplate()->getMaxWeaponSize(), mWeaponHardpoint->getWeaponHardpointTemplate()->getHardpointType());
		std::vector<WeaponTemplate*>::iterator iter;
		std::deque<DataSetItem*> data;

		for(iter = weapons.begin();iter < weapons.end(); ++iter)
		{
			DataSetItem* item = new DataSetItem();
			item->mData = (*iter)->getGuiValues();
			item->mId = (*iter)->getId();
			data.push_back(item);
		}

		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::WEAPONTEMPLATE)->getPrimaryDataSet()->setData(data);
		DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::WEAPONTEMPLATE,"weaponeditdp", mWeaponHardpoint->getWeapon()->getId(), false);
	}
	else if(source == DataSourceType::COMPONENTTEMPLATE)
	{
		mComponentId = id;
	}
	else if(source == DataSourceType::WEAPONTEMPLATE)
	{
		mComponentId = id;
	}
}

void MCUBayState::loadMCUPurchaseSelectScreen()
{
	
	mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUPurchase);
}

void MCUBayState::loadMCUEditSelectScreen()
{
	mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUSelectEdit);
}

void MCUBayState::loadMCUEditScreen(long mcuID)
{
	
	mRocketRenderer->loadDocument(RocketDocuments::MCUBay::MCUEdit);
}

void MCUBayState::changeSelectedComponent(int componentId)
{
	if(RequestedComponentType == ComponentType::CT_WEAPON)
		mInventoryState->changeMCUComponent(RequestedComponentType,CurrentMCUID,componentId,RequestedWeaponHardpoint->getHardpointNumber());
	else
		mInventoryState->changeMCUComponent(RequestedComponentType,CurrentMCUID,componentId);
}


void MCUBayState::createScene()
{

}
void MCUBayState::destroyScene()
{

}


std::hash_map<std::string,std::string> MCUBayState::getGuiValues()
{
	std::hash_map<std::string,std::string> result;

	result[FieldNames::Player::PlayerName] = mPlayerProfile->getName();
	result[FieldNames::Player::Cash] =  SQLiteDBManager::convertFloatToString( mInventoryState->getRemainingCash());
	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString( mPlayerProfile->getId() );


	return result;
}

MCUBayState* MCUBayState::mMCUBayState;
MCUBayState* MCUBayState::getSingletonPtr() 
{
	if( !mMCUBayState ) 
	{
		mMCUBayState = new MCUBayState();
	}

	return mMCUBayState;
}