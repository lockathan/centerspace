#include "InBaseState.h"
#include <Ogre.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include <Entities/PlayerProfile.h>
#include <GUI/RocketDocuments.h>
#include <Managers/CampaignMissionManager.h>
#include <Managers/LuaStateManager.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/FactionTemplateRepository.h>
#include <StateChange/BattleResult.h>
#include <StateChange/MCUComponentStatus.h>
#include <StateChange/MCUStatus.h>
#include <StateChange/PlayerPreBattleState.h>
#include <StateChange/PlayerResult.h>
#include <States/CampaignState.h>
#include <States/InBattleState.h>
#include <States/InventoryState.h>
#include <States/MainMenuState.h>
#include <States/MCUBayState.h>
#include <Templates/FactionTemplate.h>
#include <Templates/RankTemplate.h>


using namespace Ogre;

InBaseState::InBaseState()
{
	mSQLiteDBManager = SQLiteDBManager::getSingletonPtr();
	mCampaignMissionManager = CampaignMissionManager::getSingletonPtr();
}

void InBaseState::enter() 
{
	mIsEnabled = true;
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	if(mCampaignState->displayBattleResults())
	{
		
		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCU_STATUS);
		DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::BATTLE_RESULT);

		std::hash_map<std::string,std::string> battleresult = mCampaignState->getBattleResult()->getGuiValues(mCampaignState->getPlayerProfile());
		
		if(mCampaignState->getPlayerResult()->gainedPromotion())
		{
			battleresult[FieldNames::BattleResults::Promotion] = "You have been promoted to " + mCampaignState->getRankTemplate()->getName();
		}
		else
		{
			battleresult[FieldNames::BattleResults::Promotion] = "";
		}

		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::BATTLE_RESULT)->getPrimaryDataSet()->addRecord(0, battleresult);
		
		std::deque<MCUStatus*>::iterator mcu;
		std::deque<MCUStatus*>& mcus = mCampaignState->getPlayerResult()->getMCUStatuses();
		int id(0);
		for(mcu = mcus.begin(); mcu != mcus.end(); ++mcu)
		{
			DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU_STATUS)->getPrimaryDataSet()->addRecord(id, (*mcu)->getGuiValues());
			++id;
		}
		mRocketRenderer->loadDocument(RocketDocuments::InBase::PostBattle);
		mCampaignState->setDisplayBattleResults(false);
		InBattleState::getSingletonPtr()->cleanUp();
	}
	else
	{
		this->loadMainBaseScreen();
	}
}

void InBaseState::exit() 
{
	mIsEnabled = false;
}

void InBaseState::pause() 
{
}

void InBaseState::resume() 
{

}

void InBaseState::update(Real timeSinceLastFrame)
{
	if(!mIsEnabled)
		return;
}

FactionTemplate* InBaseState::getFactionTemplate()
{
	return mFactionTemplate;
}

PlayerProfile* InBaseState::getPlayerProfile()
{
	return mPlayerProfile;
}

CampaignState* InBaseState::getCampaignState()
{
	return mCampaignState;
}

void InBaseState::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

void InBaseState::setPlayerProfile(PlayerProfile* player)
{
	mPlayerProfile = player;
}

void InBaseState::setCampaignState(CampaignState* campaign)
{
	mCampaignState = campaign;
}

void InBaseState::loadCampaignState(long playerProfileId, int factionId)
{
	if(playerProfileId <= 0 || factionId <= 0)
		return;

	
	mPlayerProfile = mSQLiteDBManager->getPlayerProfile(playerProfileId);
	mFactionTemplate = FactionTemplateRepository::getSingletonPtr()->at(factionId);
	mCampaignState = mSQLiteDBManager->getCampaignState(playerProfileId, factionId);
	
	if(!mCampaignState)
	{
		mCampaignState = new CampaignState();
		mCampaignState->setFactionTemplate(mFactionTemplate);
		mCampaignState->setPlayerProfile(mPlayerProfile);
		mCampaignState->setRank(3);
		LuaStateManager::getSingletonPtr()->loadScript(this, mCampaignState->getFactionTemplate()->getStartScript());
		LuaStateManager::getSingletonPtr()->executeCampaignStartScript(this,mCampaignState->getFactionTemplate()->getStartEntrypoint(), mCampaignState);
		mSQLiteDBManager->addCampaignState(mCampaignState);
	}
}

void InBaseState::loadMainBaseScreen()
{
	mRocketRenderer->loadDocument(RocketDocuments::InBase::Main);
}

void InBaseState::startMission()
{
	switch(mSelectedMissionType)
	{
	case MissionType::RANDOM_MISSION:
		mCampaignMissionManager->loadRandomMission(mSelectedMissionID, mCampaignState->getPlayerProfile());
		break;
	}
}

void InBaseState::ProcessEvent(Rocket::Core::Event& ev)
{
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "backtomainbasescreen")
	{
		this->loadMainBaseScreen();
	}

	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "startmission")
	{
		mCampaignState->getInventoryState()->save();
		this->startMission();
	}

	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "loadmcubay")
	{
		MCUBayState* mcuBayState = MCUBayState::getSingletonPtr();
		mcuBayState->setPlayerProfile(mPlayerProfile);
		mcuBayState->setInventoryState(mCampaignState->getInventoryState());
		mGameManager->changeState(mcuBayState);
	}

	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "loadmissionselect")
	{
		
		mCampaignMissionManager->setCampaignState(mCampaignState);
		mCampaignMissionManager->setFactionTemplate(mCampaignState->getFactionTemplate());

		DataProvider* mDataProvider = DataProvider::getSingletonPtr();
		
		mDataProvider->clearDataSource(DataSourceType::RANDOM_MISSIONS);

		mDataProvider->getDataSource(DataSourceType::RANDOM_MISSIONS)->setPrimaryDataSet(mCampaignMissionManager->getRandomMissionDataSet());

		mRocketRenderer->loadDocument(RocketDocuments::InBase::MissionSelect);
	}

	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "backtocampaignscreen")
	{
		mGameManager->changeState(MainMenuState::getSingletonPtr());
	}
}

void InBaseState::createScene()
{

}

void InBaseState::destroyScene()
{

}

void InBaseState::onSelectedElementChanged(DataSourceType source, int id)
{
	switch(source)
	{
	case DataSourceType::RANDOM_MISSIONS:
		mSelectedMissionType = MissionType::RANDOM_MISSION;
		break;
	case DataSourceType::STORY_MISSIONS:
		mSelectedMissionType = MissionType::STORY_MISSION;
		break;
	}
	mSelectedMissionID = id;

}

InBaseState* InBaseState::mInBaseState;
InBaseState* InBaseState::getSingletonPtr() {
	if( !mInBaseState ) {
		mInBaseState = new InBaseState();
	}

	return mInBaseState;
}