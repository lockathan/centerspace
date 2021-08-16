#include <Templates/WeaponTemplate.h>
#include <Ogre.h>
#include <Entities/DataProvider.h>
#include <Managers/SQLiteDBManager.h>
#include <Templates/Enums.h>
#include <Templates/FactionTemplate.h>
#include <Templates/ProjectileTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <string>
#include <hash_map>

WeaponTemplate::WeaponTemplate()
	:
	mWeaponType(WeaponType::NONE),
	mShieldDamage(0),
	mArmorDamage(0),
	mPiercingDamage(0),
	mRange(0),
	mRoundsPerTurn(0),
	mMaxAmmo(0),
	mEnergyDrain(0),
	mIndirectFireCapable(false),
	mProjectileTemplate(0)
{

}

WeaponTemplate::~WeaponTemplate()
{

}

WeaponType WeaponTemplate::getWeaponType()
{
	return mWeaponType;
}

void WeaponTemplate::setWeaponType(WeaponType type)
{
	mWeaponType = type;
}

void WeaponTemplate::setWeaponType(int i)
{
	switch(i)
	{
	case 0:
		mWeaponType = WeaponType::BALLISTIC;
		break;
	case 1:
		mWeaponType = WeaponType::ENERGY;
		break;
	case 2:
		mWeaponType = WeaponType::MISSILE;
		break;
	case 3:
		mWeaponType = WeaponType::NONE;
	default:
		break;
	}
}

float WeaponTemplate::getShieldDamage()
{
	return mShieldDamage;
}

void WeaponTemplate::setShieldDamage(float damage)
{
	mShieldDamage = damage;
}

float WeaponTemplate::getArmorDamage()
{
	return mArmorDamage;
}

void WeaponTemplate::setArmorDamage(const float& damage)
{
	mArmorDamage = damage;
}

float WeaponTemplate::getPiercingDamage()
{
	return mPiercingDamage;
}

void WeaponTemplate::setPiercingDamage(const float& damage)
{
	mPiercingDamage = damage;
}

float WeaponTemplate::getRange()
{
	return mRange;
}

void WeaponTemplate::setRange(float range)
{
	mRange = range;
}

int WeaponTemplate::getRoundsPerTurn()
{
	return mRoundsPerTurn;
}

void WeaponTemplate::setRoundsPerTurn(int rounds)
{
	mRoundsPerTurn = rounds;
}

int WeaponTemplate::getMaxAmmo()
{
	return mMaxAmmo;
}

void WeaponTemplate::setMaxAmmo(int maxAmmo)
{
	mMaxAmmo = maxAmmo;
}

float WeaponTemplate::getEnergyDrain()
{
	return mEnergyDrain;
}

void WeaponTemplate::setEnergyDrain(float drain)
{
	mEnergyDrain = drain;
}


bool WeaponTemplate::canFit(HardpointType hType)
{
	switch(mWeaponType)
	{
	case WeaponType::ENERGY:
		if(hType == HardpointType::ENERGY_ONLY || hType == HardpointType::ENERGY_OR_BALLISTIC)
			return true;
		return false;
	case WeaponType::BALLISTIC:
		if(hType == HardpointType::BALLISTIC_ONLY || hType == HardpointType::ENERGY_OR_BALLISTIC)
			return true;
		return false;
	case WeaponType::MISSILE:
		if(hType == HardpointType::MISSILE_ONLY)
			return true;
		return false;
	case WeaponType::NONE:
		return true;
	}
	return false;
}

bool WeaponTemplate::isIndirectFireCapable()
{
	return mIndirectFireCapable;
}

void WeaponTemplate::setIndirectFireCapable(bool value)
{
	mIndirectFireCapable = value;
}

ProjectileTemplate* WeaponTemplate::getProjectileTemplate()
{
	return mProjectileTemplate;
}

void WeaponTemplate::setProjectileTemplate(ProjectileTemplate* t)
{

	mProjectileTemplate = t;
}

DamageType WeaponTemplate::getDamageType()
{
	return mDamageType;
}

void WeaponTemplate::setDamageType(DamageType type)
{
	mDamageType = type;
}

std::hash_map<std::string, std::string> WeaponTemplate::getGuiValues()
{
	std::hash_map<std::string, std::string> result;

	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString( this->getId() );
	result[FieldNames::WeaponTemplate::WeaponCost] = SQLiteDBManager::convertFloatToString( mCost);
	result[FieldNames::WeaponTemplate::WeaponDescription] = mDescription;
	result[FieldNames::WeaponTemplate::WeaponName] = mName;
	result[FieldNames::WeaponTemplate::WeaponWeight] = SQLiteDBManager::convertFloatToString( mWeight );
	result[FieldNames::WeaponTemplate::ArmorDamage] = SQLiteDBManager::convertFloatToString( mArmorDamage );
	result[FieldNames::WeaponTemplate::EnergyCostPerTurn] = SQLiteDBManager::convertFloatToString( mEnergyDrain );
	result[FieldNames::WeaponTemplate::MaxAmmo] = SQLiteDBManager::convertFloatToString( mMaxAmmo );
	result[FieldNames::WeaponTemplate::PiercingDamage] = SQLiteDBManager::convertFloatToString( mPiercingDamage );
	result[FieldNames::WeaponTemplate::Range] = SQLiteDBManager::convertFloatToString( mRange );
	result[FieldNames::WeaponTemplate::ShieldDamage] = SQLiteDBManager::convertFloatToString( mShieldDamage );
	
	return result;
}
