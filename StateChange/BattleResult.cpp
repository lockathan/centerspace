#include "BattleResult.h"
#include <Entities/DataProvider.h>
#include <Entities/PlayerProfile.h>
#include <Managers/SQLiteDBManager.h>
#include <StateChange/PlayerResult.h>
#include <Ogre.h>
#include <deque>

using namespace Ogre;

BattleResult::BattleResult()
	:
	mBaseMissionPay(0),
	mBounty(0)
{

}

BattleResult::BattleResult(float basePay, float bounty)
	:
	mBaseMissionPay(basePay),
	mBounty(bounty)
{

}

BattleResult::~BattleResult()
{
	std::deque<PlayerResult*>::iterator iter;

	for(iter = mPlayerResults.begin(); iter != mPlayerResults.end(); ++iter)
	{
		delete (*iter);
	}
}

std::deque<PlayerResult*>& BattleResult::getPlayerResults()
{
	return mPlayerResults;
}

void BattleResult::setPlayerResults(std::deque<PlayerResult*>& results)
{
	mPlayerResults = results;
}

void BattleResult::addPlayerResult(PlayerResult* result)
{
	mPlayerResults.push_back(result);
}
	
PlayerResult* BattleResult::getPlayerResult(PlayerProfile* player)
{
	std::deque<PlayerResult*>::iterator iter;

	for(iter = mPlayerResults.begin(); iter != mPlayerResults.end(); ++iter)
	{
		if((*iter)->getPlayerProfile() == player)
			return (*iter);
	}
	return 0;
}

float BattleResult::getBaseMissionPay()
{
	return mBaseMissionPay;
}

void BattleResult::setBaseMissionPay(float value)
{
	mBaseMissionPay = value;
}

float BattleResult::getBounty()
{
	return mBounty;
}

void BattleResult::setBounty(float value)
{
	mBounty = value;
}


std::hash_map<std::string,std::string> BattleResult::getGuiValues(PlayerProfile* profile)
{
	PlayerResult* player = getPlayerResult(profile);
	std::hash_map<std::string,std::string> result;
	float moneyEarned = mBaseMissionPay + (mBounty * player->getKills());
	result[FieldNames::BattleResults::ExperienceEarned] = SQLiteDBManager::convertIntToString(player->getKills());
	result[FieldNames::BattleResults::MoneyEarned] = SQLiteDBManager::convertFloatToString(moneyEarned);
	result[FieldNames::BattleResults::TotalRepairCosts] = SQLiteDBManager::convertFloatToString(player->getTotalRepairCosts());
	result[FieldNames::BattleResults::NetProfit] = SQLiteDBManager::convertFloatToString(moneyEarned - player->getTotalRepairCosts());

	return result;
}
