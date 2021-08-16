#include <Entities/WeaponHardpoint.h>
#include <Controllers/MCUController.h>
#include <Entities/DataProvider.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Managers/DbNames.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/WeaponTemplateRepository.h>
#include <Templates/MCUTemplate.h>
#include <Templates/ProjectileTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <hash_map>

WeaponHardpoint::WeaponHardpoint()
{
	mMCU = 0;
	mHardpointNumber = 0;
	mWeaponHardpointTemplate = 0;
	mWeaponTemplate = 0;
	mAmmoRemaining = 0;
	mRoundsRemainingThisTurn = 0;
	
}

WeaponHardpoint::~WeaponHardpoint()
{

}

MechanizedCombatUnit* WeaponHardpoint::getMCU()
{
	return mMCU;
}

void WeaponHardpoint::setMCU(MechanizedCombatUnit* mcu)
{
	mMCU = mcu;
}

void WeaponHardpoint::setWeapon(WeaponTemplate* weapon)
{
	mWeaponTemplate = weapon;
	mRoundsRemainingThisTurn = weapon->getRoundsPerTurn();
	mAmmoRemaining = weapon->getMaxAmmo();
}

WeaponHardpointTemplate* WeaponHardpoint::getWeaponHardpointTemplate()
{
	return mWeaponHardpointTemplate;
}

void WeaponHardpoint::setWeaponHardpointTemplate(WeaponHardpointTemplate* t)
{
	mWeaponHardpointTemplate = t;
	setWeapon(t->getDefaultWeapon());
	setHardpointNumber(t->getHardpointNumber());
}

WeaponTemplate* WeaponHardpoint::getWeapon()
{
	return mWeaponTemplate;
}

bool WeaponHardpoint::canFire()
{
	if(mWeaponTemplate->getMaxAmmo() != 0)
	{
		if(mAmmoRemaining > 0 && mRoundsRemainingThisTurn > 0)
			return true;
		else
			return false;
	}
	else
	{
		if(mRoundsRemainingThisTurn > 0)
			return true;
		else
			return false;
	}
	return false;
}

bool WeaponHardpoint::decrementAmmo()
{
	if(mWeaponTemplate->getMaxAmmo() != 0)
	{
		if(mAmmoRemaining > 0 && mRoundsRemainingThisTurn > 0)
		{
			mAmmoRemaining--;
			mRoundsRemainingThisTurn--;
			return true;
		}
		else
			return false;
	}
	else
	{
		if(mRoundsRemainingThisTurn > 0)
		{
			mRoundsRemainingThisTurn--;
			return true;
		}
		
		return false;
	}
}

void WeaponHardpoint::resetAmmoRemaining()
{
	mAmmoRemaining = mWeaponTemplate->getMaxAmmo();
	mRoundsRemainingThisTurn = mWeaponTemplate->getRoundsPerTurn();
}

void WeaponHardpoint::endTurn()
{
	mRoundsRemainingThisTurn = mWeaponTemplate->getRoundsPerTurn();
}

void WeaponHardpoint::setHardpointNumber(int HardpointNumber)
{
	mHardpointNumber = HardpointNumber;
}

int WeaponHardpoint::getHardpointNumber()
{
	return mHardpointNumber;
}

float WeaponHardpoint::getCurrentChanceToHit()
{
	return mCurrentChanceToHit;
}

void WeaponHardpoint::setCurrentChanceToHit(const float& chance)
{
	mCurrentChanceToHit = chance;
}

void WeaponHardpoint::fire(MCUController* mcu, const FireInstruction& instruction)
{
	decrementAmmo();

	mWeaponTemplate->getProjectileTemplate()->instantiate(mcu, instruction);
}

std::hash_map<std::string,DbValue> WeaponHardpoint::getValues()
{
	std::hash_map<std::string,DbValue> values;
	values[DbNames::MCUWeapon::ID].StringValue = SQLiteDBManager::convertLongToString(mId);
	values[DbNames::MCUWeapon::ID].IsNumber = true;

	values[DbNames::MCUWeapon::MCU_ID].StringValue = SQLiteDBManager::convertLongToString(mMCU->getId());
	values[DbNames::MCUWeapon::MCU_ID].IsNumber = true;

	values[DbNames::MCUWeapon::WEAPONTEMPLATE_ID].StringValue = SQLiteDBManager::convertIntToString(mWeaponTemplate->getId());
	values[DbNames::MCUWeapon::WEAPONTEMPLATE_ID].IsNumber = true;

	values[DbNames::MCUWeapon::HARDPOINT_NUMBER].StringValue = SQLiteDBManager::convertIntToString(mHardpointNumber);
	values[DbNames::MCUWeapon::HARDPOINT_NUMBER].IsNumber = true;

	return values;
}

void WeaponHardpoint::setValues(std::hash_map<std::string,std::string> values)
{
	setId(atoi(values[DbNames::MCUWeapon::ID].c_str()));
	setHardpointNumber(atoi(values[DbNames::MCUWeapon::HARDPOINT_NUMBER].c_str()));
	setWeapon(WeaponTemplateRepository::getSingletonPtr()->at(atoi(values[DbNames::MCUWeapon::WEAPONTEMPLATE_ID].c_str())));
	mWeaponHardpointTemplate = mMCU->getMCUTemplate()->getHardpoint(mHardpointNumber);
	setName(mWeaponTemplate->getName());
}

std::hash_map<std::string, std::string> WeaponHardpoint::getGuiValues()
{
	std::hash_map<std::string, std::string> result;

	result[FieldNames::Weapon::Accuracy] = "";
	result[FieldNames::Weapon::EnergyCost] = SQLiteDBManager::convertFloatToString( mWeaponTemplate->getEnergyDrain());
	result[FieldNames::Weapon::Range] = SQLiteDBManager::convertFloatToString(mWeaponTemplate->getRange());
	result[FieldNames::Weapon::RemainingAmmo] = SQLiteDBManager::convertIntToString(mAmmoRemaining);
	result[FieldNames::Weapon::WeaponName] = mWeaponTemplate->getName();
	result[FieldNames::Weapon::CanFire] = this->canFire() ? "true" : "false";
	result[FieldNames::Weapon::ErrorText] = "";
	result[FieldNames::Weapon::ChanceToHitTarget] = SQLiteDBManager::convertFloatToString((int)(mCurrentChanceToHit * 100));
	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString(mHardpointNumber);

	return result;
}
