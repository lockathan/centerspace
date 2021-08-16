#include "PlayerResult.h"
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/PlayerProfile.h>
#include <StateChange/MCUComponentStatus.h>
#include <StateChange/MCUStatus.h>
#include <Templates/MCUTemplate.h>
#include <Ogre.h>
#include <deque>

using namespace Ogre;

PlayerResult::PlayerResult()
	:
	mPlayerOutcome(PlayerOutcome::PO_PLAYING),
	mPlayerProfile(0),
	mTeam(0),
	mKills(0),
	mGainedPromotion(false)
{

}

PlayerResult::PlayerResult(PlayerProfile* profile, int team)
	:
	mPlayerOutcome(PlayerOutcome::PO_PLAYING),
	mPlayerProfile(profile),
	mTeam(team),
	mKills(0),
	mGainedPromotion(false)
{

}


PlayerResult::~PlayerResult()
{
	std::deque<MCUStatus*>::iterator iter;

	for(iter = mMCUStatuses.begin(); iter != mMCUStatuses.end(); ++iter)
	{
		delete (*iter);
	}
}

PlayerOutcome PlayerResult::getPlayerOutcome()
{
	return mPlayerOutcome;
}

void PlayerResult::setPlayerOutcome(PlayerOutcome outcome)
{
	mPlayerOutcome = outcome;
}

PlayerProfile* PlayerResult::getPlayerProfile()
{
	return mPlayerProfile;
}

void PlayerResult::setPlayerProfile(PlayerProfile* player)
{
	mPlayerProfile = player;
}

std::deque<MCUStatus*>& PlayerResult::getMCUStatuses()
{
	return mMCUStatuses;
}

void PlayerResult::setMCUStatuses(std::deque<MCUStatus*>& statuses)
{
	mMCUStatuses = statuses;
}

void PlayerResult::addMCUStatus(MCUStatus* status)
{
	status->setPlayerResult(this);
	mMCUStatuses.push_back(status);
}

MCUStatus* PlayerResult::getMCUStatus(MechanizedCombatUnit* mcu)
{
	std::deque<MCUStatus*>::iterator iter;

	for(iter = mMCUStatuses.begin(); iter != mMCUStatuses.end(); ++iter)
	{
		if((*iter)->getMechanizedCombatUnit() == mcu)
			return (*iter);
	}
	return 0;
}

int PlayerResult::getTeam()
{
	return mTeam;
}

void PlayerResult::setTeam(int t)
{
	mTeam = t;
}

int PlayerResult::getKills()
{
	return mKills;
}

void PlayerResult::setKills(int kills)
{
	mKills = kills;
}

void PlayerResult::addKill()
{
	++mKills;
}

bool PlayerResult::gainedPromotion()
{
	return mGainedPromotion;
}

void PlayerResult::setGainedPromotion(bool value)
{
	mGainedPromotion = value;
}

float PlayerResult::getTotalRepairCosts()
{
	float repairCost(0);
	std::deque<MCUStatus*>::iterator iter;

	for(iter = mMCUStatuses.begin(); iter != mMCUStatuses.end(); ++iter)
	{
		repairCost += (*iter)->getTotalRepairCosts();
	}

	return repairCost;
}
