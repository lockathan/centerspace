#include "MCUComponentStatus.h"
#include <Entities/MCUComponent.h>
#include <StateChange/MCUStatus.h>
#include <Templates/MCUComponentTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Ogre.h>
#include <deque>

using namespace Ogre;

MCUComponentStatus::MCUComponentStatus()
	:
	mMCUComponentTemplate(0),
	mComponentType(ComponentType::CT_CHASSIS),
	mHealthRemaining(0),
	mMaxHealth(0),
	mMCUStatus(0),
	mMCUTemplate(0)
{

}

MCUComponentStatus::MCUComponentStatus(MCUComponent* component,ComponentType type)
	:
	mMCUComponentTemplate(component->getMCUComponentTemplate()),
	mComponentType(type),
	mHealthRemaining(mMCUComponentTemplate->getMaxHealth()),
	mMaxHealth(mMCUComponentTemplate->getMaxHealth()),
	mMCUStatus(0),
	mMCUTemplate(0)
{
	component->setMCUComponentStatus(this);
}
	
MCUComponentStatus::MCUComponentStatus(ComponentType type, float maxHealth, MCUTemplate* mcu)
	:
	mMCUComponentTemplate(0),
	mComponentType(type),
	mMaxHealth(maxHealth),
	mHealthRemaining(maxHealth),
	mMCUStatus(0),
	mMCUTemplate(mcu)
{

}

MCUComponentStatus::~MCUComponentStatus()
{

}

MCUComponentTemplate* MCUComponentStatus::getMCUComponentTemplate()
{
	return mMCUComponentTemplate;
}

void MCUComponentStatus::setMCUComponentTemplate(MCUComponentTemplate* component)
{
	mMCUComponentTemplate = component;
}

float MCUComponentStatus::getHealthRemaining()
{
	return mHealthRemaining;
}

void MCUComponentStatus::setHealthRemaining(float health)
{
	mHealthRemaining = health;
	if(health < mMaxHealth)
	{
		mMCUStatus->setMCUCondition(MCUCondition::MC_DAMAGED);
	}
}

ComponentType MCUComponentStatus::getComponentType()
{
	return mComponentType;
}

void MCUComponentStatus::setComponentType(ComponentType type)
{
	mComponentType = type;
}

MCUStatus* MCUComponentStatus::getMCUStatus()
{
	return mMCUStatus;
}

void MCUComponentStatus::setMCUStatus(MCUStatus* mcu)
{
	mMCUStatus = mcu;
}

float MCUComponentStatus::getDamagePercent()
{
	return (mHealthRemaining / mMCUComponentTemplate->getMaxHealth()) * 100;
}

float MCUComponentStatus::getRepairCosts()
{
	if(mMCUComponentTemplate)
		return mMCUComponentTemplate->getCost() * ((mMaxHealth - mHealthRemaining) / mMaxHealth) * 0.5;
	else
		return mMCUTemplate->getCost() * ((mMaxHealth - mHealthRemaining) / mMaxHealth) * 0.5;
}