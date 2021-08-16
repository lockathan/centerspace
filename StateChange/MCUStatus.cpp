#include "MCUStatus.h"
#include <StateChange/MCUComponentStatus.h>
#include <StateChange/PlayerResult.h>
#include <Entities/DataProvider.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/PlayerProfile.h>
#include <Managers/SQLiteDBManager.h>
#include <Templates/MCUComponentTemplate.h>
#include <Templates/MCUTemplate.h>
#include <deque>

#include <hash_map>
#include <Ogre.h>

using namespace Ogre;

MCUStatus::MCUStatus()
	:
	mMechanizedCombatUnit(0),
	mMCUCondition(MCUCondition::MC_INTACT),
	mPlayerResult(0)
{

}
	
MCUStatus::MCUStatus(MechanizedCombatUnit* mcu)
	:
	mMechanizedCombatUnit(mcu),
	mMCUCondition(MCUCondition::MC_INTACT)
{

}

MCUStatus::~MCUStatus()
{
	std::deque<MCUComponentStatus*>::iterator iter;

	for(iter = mMCUComponentStatuses.begin(); iter != mMCUComponentStatuses.end(); ++iter)
	{
		delete (*iter);
	}

	if(mPlayerResult->getPlayerProfile()->isAI())
	{
		delete mMechanizedCombatUnit;
	}
}

MCUCondition MCUStatus::getMCUCondition()
{
	return mMCUCondition;
}

void MCUStatus::setMCUCondition(MCUCondition condition)
{
	mMCUCondition= condition;
}

MechanizedCombatUnit* MCUStatus::getMechanizedCombatUnit()
{
	return mMechanizedCombatUnit;
}

void MCUStatus::setMechanizedCombatUnit(MechanizedCombatUnit* mcu)
{
	mMechanizedCombatUnit = mcu;
}


std::deque<MCUComponentStatus*>& MCUStatus::getComponentStatuses()
{
	return mMCUComponentStatuses;
}

void MCUStatus::setComponentStatuses(std::deque<MCUComponentStatus*>& statuses)
{
	mMCUComponentStatuses = statuses;
}

void MCUStatus::addComponentStatus(MCUComponentStatus* status)
{
	status->setMCUStatus(this);
	mMCUComponentStatuses.push_back(status);
}
	
PlayerResult* MCUStatus::getPlayerResult()
{
	return mPlayerResult;
}

void MCUStatus::setPlayerResult(PlayerResult* player)
{
	mPlayerResult = player;
}

float MCUStatus::getTotalRepairCosts()
{
	if(mMCUCondition == MCUCondition::MC_DESTROYED)
		return 0;
	float repairCost(0);
	std::deque<MCUComponentStatus*>::iterator iter;

	for(iter = mMCUComponentStatuses.begin(); iter != mMCUComponentStatuses.end(); ++iter)
	{
		repairCost+=(*iter)->getRepairCosts();
	}
	return repairCost;
}

std::hash_map<std::string,std::string> MCUStatus::getGuiValues()
{
	std::hash_map<std::string,std::string> result;
	result[FieldNames::MCUStatus::MCUName] = mMechanizedCombatUnit->getName();
	result[FieldNames::MCUStatus::RepairCost] = SQLiteDBManager::convertFloatToString(getTotalRepairCosts());
	if(mMCUCondition == MCUCondition::MC_INTACT)
	{
		result[FieldNames::MCUStatus::Status] = "Intact";
	}
	else if(mMCUCondition == MCUCondition::MC_DAMAGED)
	{
		result[FieldNames::MCUStatus::Status] = "Damaged";
	}
	else
	{
		result[FieldNames::MCUStatus::Status] = "Destroyed";
	}
	return result;
}
