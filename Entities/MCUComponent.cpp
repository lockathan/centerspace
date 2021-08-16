#include <Entities/MCUComponent.h>
#include <Controllers/MCUController.h>
#include <Managers/GameManager.h>
#include <Managers/SQLiteDBManager.h>
#include <StateChange/MCUComponentStatus.h>
#include <Templates/ArmorTemplate.h>
#include <Templates/MCUComponentTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Ogre.h>

using namespace Ogre;

MCUComponent::MCUComponent()
	:
	mMCUComponentTemplate(0),
	mMCUController(0),
	mHealthRemaining(0),
	mMaxHealth(0),
	mArmorRemaining(0),
	mMaxArmor(0),
	mArmorTemplate(0),
	mComponentType(CT_CHASSIS),
	mMCUComponentStatus(0)
{

}
	
MCUComponent::MCUComponent(MCUController* controller, ComponentType type, const Real& maxHealth, const Real& armor, MCUComponent* armorComponent, ArmorTemplate* armorTemplate)
	:
	mMCUComponentTemplate(0),
	mMCUController(controller),
	mHealthRemaining(maxHealth),
	mMaxHealth(maxHealth),
	mArmorRemaining(armor),
	mMaxArmor(armor),
	mArmorTemplate(armorTemplate),
	mArmorComponent(armorComponent),
	mComponentType(type),
	mWeaponHardpoint(0),
	mMCUComponentStatus(0)
{

}

MCUComponent::MCUComponent(MCUComponentTemplate* component, MCUController* controller, ComponentType type, const Real& armor, MCUComponent* armorComponent, ArmorTemplate* armorTemplate)
	:
	mMCUComponentTemplate(component),
	mMCUController(controller),
	mHealthRemaining(component->getMaxHealth()),
	mMaxHealth(component->getMaxHealth()),
	mArmorRemaining(armor),
	mMaxArmor(armor),
	mArmorTemplate(armorTemplate),
	mArmorComponent(armorComponent),
	mComponentType(type),
	mWeaponHardpoint(0),
	mMCUComponentStatus(0)
{

}
	
MCUComponent::MCUComponent(MCUComponentTemplate* component, MCUController* controller, WeaponHardpoint* weapon, const Real& armor, MCUComponent* armorComponent, ArmorTemplate* armorTemplate)
	:
	mMCUComponentTemplate(component),
	mMCUController(controller),
	mHealthRemaining(component->getMaxHealth()),
	mMaxHealth(component->getMaxHealth()),
	mArmorRemaining(armor),
	mMaxArmor(armor),
	mArmorTemplate(armorTemplate),
	mArmorComponent(armorComponent),
	mWeaponHardpoint(weapon),
	mComponentType(ComponentType::CT_WEAPON),
	mMCUComponentStatus(0)
{

}

MCUComponent::~MCUComponent()
{

}
	
MCUComponentTemplate* MCUComponent::getMCUComponentTemplate()
{
	return mMCUComponentTemplate;
}

void MCUComponent::setMCUComponentTemplate(MCUComponentTemplate* component)
{
	mMCUComponentTemplate = component;
}

MCUController* MCUComponent::getMCUController()
{
	return mMCUController;
}

void MCUComponent::setMCUController(MCUController* mcu)
{
	mMCUController = mcu;
}

MCUComponentStatus* MCUComponent::getMCUComponentStatus()
{
	return mMCUComponentStatus;
}

void MCUComponent::setMCUComponentStatus(MCUComponentStatus* status)
{
	mMCUComponentStatus = status;
}

Real MCUComponent::getHealthRemaining()
{
	return mHealthRemaining;
}

void MCUComponent::setHealthRemaining(const Real& health)
{
	mHealthRemaining = health;
}

Ogre::Real MCUComponent::getMaxHealth()
{
	return mMaxHealth;
}

void MCUComponent::setMaxHealth(const Ogre::Real& health)
{
	mMaxHealth = health;
}


Ogre::Real MCUComponent::getArmorRemaining()
{
	return mArmorRemaining;
}

void MCUComponent::setArmorRemaining(const Ogre::Real& armor)
{
	mArmorRemaining = armor;
}

Ogre::Real MCUComponent::getMaxArmor()
{
	return mMaxArmor;
}

void MCUComponent::setMaxArmor(const Ogre::Real& armor)
{
	mMaxArmor = armor;
}
	
ArmorTemplate* MCUComponent::getArmorTemplate()
{
	return mArmorTemplate;
}

void MCUComponent::setArmorTemplate(ArmorTemplate* armor)
{
	mArmorTemplate = armor;
}


MCUComponent* MCUComponent::getArmorComponent()
{
	return mArmorComponent;
}

void MCUComponent::setArmorComponent(MCUComponent* component)
{
	mArmorComponent = component;
}

ComponentType MCUComponent::getComponentType()
{
	return mComponentType;
}

void MCUComponent::setComponentType(ComponentType type)
{
	mComponentType = type;
}


WeaponHardpoint* MCUComponent::getWeaponHardpoint()
{
	return mWeaponHardpoint;
}

void MCUComponent::setWeaponHardpointNumber(WeaponHardpoint* weapon)
{
	mWeaponHardpoint = weapon;
}

void MCUComponent::applyArmorComponentDamage(const Ogre::Real& damage)
{
	mHealthRemaining -= damage;
	if(mHealthRemaining < 0)
		mHealthRemaining = 0;
}

Real MCUComponent::applyDamage(const Real& damage, DamageType type, MCUController* firedBy)
{
	float finalDamage = mArmorTemplate->getFinalDamageValue(damage, type);

	float armorDamage;
	if(finalDamage > mArmorRemaining)
		armorDamage = mArmorRemaining;
	else
		armorDamage = finalDamage;
	
	GameManager::getSingletonPtr()->luaDebugReport("Armor hit for " + SQLiteDBManager::convertFloatToString(armorDamage) + " damage", 0);
	mArmorComponent->applyArmorComponentDamage(armorDamage);

	float remainingDamage = finalDamage - mArmorRemaining;
	
	if(remainingDamage > 0)
	{
		//report the damage to the armor component for the purposes of record keeping
		float result(0);
		if(remainingDamage > mHealthRemaining)
			result = remainingDamage - mHealthRemaining;

		if(mMCUComponentTemplate)
			GameManager::getSingletonPtr()->luaDebugReport(this->getMCUComponentTemplate()->getName() + " hit for " + SQLiteDBManager::convertFloatToString(std::min(mHealthRemaining, remainingDamage)) + " damage", 0);
		else
			GameManager::getSingletonPtr()->luaDebugReport("Chassis hit for " + SQLiteDBManager::convertFloatToString(std::min(mHealthRemaining, remainingDamage)) + " damage", 0);
		
		mHealthRemaining -= remainingDamage;
		if(mMCUComponentStatus)
			mMCUComponentStatus->setHealthRemaining(std::max<int>(0, mHealthRemaining));
		
		if(mHealthRemaining <= 0)
		{
			mHealthRemaining = 0;
			mMCUController->componentDestroyed(this);
			GameManager::getSingletonPtr()->luaDebugReport("Component destroyed", 0);
			return result;
		}
	}

	return 0;
}

bool MCUComponent::isBroken()
{
	return mHealthRemaining <= 0;
}