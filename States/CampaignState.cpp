#include "CampaignState.h"
#include <States/InventoryState.h>
#include <Ogre.h>
#include <Entities/PlayerProfile.h>
#include <Managers/DbNames.h>
#include <Managers/LuaStateManager.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/FactionHubTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/HubTemplateRepository.h>
#include <Repositories/RankTemplateRepository.h>
#include <StateChange/BattleResult.h>
#include <StateChange/MCUComponentStatus.h>
#include <StateChange/MCUStatus.h>
#include <StateChange/PlayerResult.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/HubTemplate.h>
#include <Templates/RankTemplate.h>
#include <hash_map>

using namespace Ogre;

CampaignState::CampaignState(bool createInventoryState)
	:
	mRank(1),
	mDifficulty(1),
	mFactionHubTemplate(0),
	mBattleResult(0),
	mDisplayBattleResults(false),
	mExperience(0),
	mRankTemplate(0)
{
	if(createInventoryState)
	{
		mInventoryState = new InventoryState();
		mInventoryState->setRemainingCash(1);
	}
}

CampaignState::~CampaignState()
{

}

void CampaignState::enter() 
{

}

void CampaignState::exit() 
{

}

void CampaignState::load() 
{

}

void CampaignState::save() 
{
	SQLiteDBManager::getSingletonPtr()->saveCampaignState(this);
}


void CampaignState::reset()
{

}

PlayerProfile* CampaignState::getPlayerProfile()
{
	return mPlayerProfile;
}

void CampaignState::setPlayerProfile(PlayerProfile* profile)
{
	mPlayerProfile = profile;
}

RankTemplate* CampaignState::getRankTemplate()
{
	return mRankTemplate;
}

void CampaignState::setRankTemplate(RankTemplate* rank)
{
	mRankTemplate = rank;
}

int CampaignState::getRank()
{
	return mRank;
}

void CampaignState::setRank(int rank)
{
	mRank = rank;
	mInventoryState->setRank(rank);
}

InventoryState* CampaignState::getInventoryState()
{
	return mInventoryState;
}

void CampaignState::setInventoryState(InventoryState* state)
{
	if(state)
		mInventoryState = state;
}

FactionTemplate* CampaignState::getFactionTemplate()
{
	return mFactionTemplate;
}

void CampaignState::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
	mInventoryState->setFactionTemplate(faction);

	if(!mFactionHubTemplate)
	{
		mFactionHubTemplate = FactionHubTemplateRepository::getSingletonPtr()->getFactionHubTemplate(mFactionTemplate->getId(),1);
	}
	mRankTemplate = RankTemplateRepository::getSingletonPtr()->getRank(mRank, mFactionTemplate);
}

int CampaignState::getDifficulty()
{
	return mDifficulty;
}

void CampaignState::setDifficulty(int difficulty)
{
	mDifficulty = difficulty;
}

int CampaignState::getExperience()
{
	return mExperience;
}

void CampaignState::setExperience(int exp)
{
	mExperience = exp;
}

void CampaignState::addExperience(int exp)
{
	mExperience+= exp;
}

FactionHubTemplate* CampaignState::getFactionHubTemplate()
{
	return mFactionHubTemplate;
}

void CampaignState::setFactionHubTemplate(FactionHubTemplate* hub)
{
	mFactionHubTemplate = hub;
}

std::hash_map<std::string,DbValue> CampaignState::getValues()
{
	std::hash_map<std::string,DbValue> values;

	values[DbNames::CampaignState::ID].StringValue = SQLiteDBManager::convertLongToString(mId);
	values[DbNames::CampaignState::ID].IsNumber = true;

	values[DbNames::CampaignState::FACTION_ID].StringValue = SQLiteDBManager::convertIntToString(mFactionTemplate->getId());
	values[DbNames::CampaignState::FACTION_ID].IsNumber = true;

	values[DbNames::CampaignState::RANK].StringValue = SQLiteDBManager::convertIntToString(mRank);
	values[DbNames::CampaignState::RANK].IsNumber = true;

	values[DbNames::CampaignState::EXPERIENCE].StringValue = SQLiteDBManager::convertIntToString(mExperience);
	values[DbNames::CampaignState::EXPERIENCE].IsNumber = true;

	values[DbNames::CampaignState::HUB_ID].StringValue = SQLiteDBManager::convertIntToString(mFactionHubTemplate->getHubTemplate()->getId());
	values[DbNames::CampaignState::HUB_ID].IsNumber = true;

	values[DbNames::CampaignState::DIFFICULTY].StringValue = SQLiteDBManager::convertIntToString(mDifficulty);
	values[DbNames::CampaignState::DIFFICULTY].IsNumber = true;

	values[DbNames::CampaignState::INVENTORY_ID].StringValue = SQLiteDBManager::convertLongToString(mInventoryState->getId());
	values[DbNames::CampaignState::INVENTORY_ID].IsNumber = true;

	values[DbNames::CampaignState::PlayerProfile_ID].StringValue = SQLiteDBManager::convertLongToString(mPlayerProfile->getId());
	values[DbNames::CampaignState::PlayerProfile_ID].IsNumber = true;
	return values;
}

void CampaignState::setValues(std::hash_map<std::string,std::string> values)
{
	mId = atol(values[DbNames::CampaignState::ID].c_str());
	mFactionTemplate = FactionTemplateRepository::getSingletonPtr()->at( atoi(values[DbNames::CampaignState::FACTION_ID].c_str()));
	mRank = atoi(values[DbNames::CampaignState::RANK].c_str());
	mExperience = atoi(values[DbNames::CampaignState::EXPERIENCE].c_str());
	mFactionHubTemplate = FactionHubTemplateRepository::getSingletonPtr()->getFactionHubTemplate(mFactionTemplate->getId(),atoi(values[DbNames::CampaignState::HUB_ID].c_str()));
	mFactionTemplate = mFactionHubTemplate->getPlayerFactionTemplate();
	mRankTemplate = RankTemplateRepository::getSingletonPtr()->getRank(mRank, mFactionTemplate);
	mDifficulty = atoi(values[DbNames::CampaignState::DIFFICULTY].c_str());
}

bool CampaignState::displayBattleResults()
{
	return mDisplayBattleResults;
}

void CampaignState::setDisplayBattleResults(bool value)
{
	mDisplayBattleResults = value;
}

BattleResult* CampaignState::getBattleResult()
{
	return mBattleResult;
}


PlayerResult* CampaignState::getPlayerResult()
{
	if(mBattleResult)
	{
		return mBattleResult->getPlayerResult(mPlayerProfile);
	}

	return 0;
}

void CampaignState::interpretResult(BattleResult* result)
{
	mBattleResult = result;
	mDisplayBattleResults = true;
	PlayerResult* pResult = result->getPlayerResult(mPlayerProfile);

	if(pResult)
	{
		std::deque<PlayerResult*> players = result->getPlayerResults();
		std::deque<PlayerResult*>::iterator iter;
		std::deque<MCUStatus*>::iterator mcu;

		RankTemplate* currentRank(mRankTemplate);
		mExperience+= pResult->getKills();
		mRankTemplate = RankTemplateRepository::getSingletonPtr()->getRankByExperience(mExperience, mFactionTemplate);
		if(mRankTemplate != currentRank)
		{
			pResult->setGainedPromotion(true);
			mRank = mRankTemplate->getRank();
			mInventoryState->setRank(mRank);
		}

		std::deque<MCUStatus*>& mcus = pResult->getMCUStatuses();
		for(mcu = mcus.begin(); mcu != mcus.end(); ++mcu)
		{
			if((*mcu)->getMCUCondition() == MCUCondition::MC_DESTROYED)
			{
				mInventoryState->removeMCU((*mcu)->getMechanizedCombatUnit());
			}
		}
		float remainingCash(mInventoryState->getRemainingCash());
		if(pResult->getPlayerOutcome() == PlayerOutcome::PO_VICTORY)
		{
			remainingCash+= mBattleResult->getBaseMissionPay();
			remainingCash+= mBattleResult->getBounty() * pResult->getKills();
		}
		mInventoryState->setRemainingCash(remainingCash - pResult->getTotalRepairCosts());
	}

	save();
}

std::hash_map<std::string,std::string> CampaignState::getGuiValues()
{
	std::hash_map<std::string,std::string> result;

	result[FieldNames::Player::PlayerName] = mPlayerProfile->getName();
	result[FieldNames::Player::Rank] = mRankTemplate->getName();
	result[FieldNames::Player::Cash] =  SQLiteDBManager::convertFloatToString( mInventoryState->getRemainingCash());
	result[FieldNames::Player::FactionName] =  mFactionTemplate->getName();
	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString( this->getId() );


	return result;
}