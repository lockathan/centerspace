#include <Templates/ArmorTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <string>
#include <hash_map>

ArmorTemplate::ArmorTemplate()
	:
	mLaserDamageTaken(0),
	mPlasmaDamageTaken(0),
	mCannonDamageTaken(0),
	mMissileDamageTaken(0),
	mPiercingDamageTaken(0)
{

}

ArmorTemplate::~ArmorTemplate()
{

}

float ArmorTemplate::getLaserDamageTaken()
{
	return mLaserDamageTaken;
}

void ArmorTemplate::setLaserDamageTaken(const float& damage)
{
	mLaserDamageTaken = damage;
}
	
float ArmorTemplate::getPlasmaDamageTaken()
{
	return mPlasmaDamageTaken;
}

void ArmorTemplate::setPlasmaDamageTaken(const float& damage)
{
	mPlasmaDamageTaken = damage;
}
	
float ArmorTemplate::getCannonDamageTaken()
{
	return mCannonDamageTaken;
}

void ArmorTemplate::setCannonDamageTaken(const float& damage)
{
	mCannonDamageTaken = damage;
}
	
float ArmorTemplate::getMissileDamageTaken()
{
	return mMissileDamageTaken;
}

void ArmorTemplate::setMissileDamageTaken(const float& damage)
{
	mMissileDamageTaken = damage;
}
	
float ArmorTemplate::getPiercingDamageTaken()
{
	return mPiercingDamageTaken;
}

void ArmorTemplate::setPiercingDamageTaken(const float& damage)
{
	mPiercingDamageTaken = damage;
}


float ArmorTemplate::getFinalDamageValue(const float& damageIn, DamageType damageType)
{
	switch(damageType)
	{
	case DamageType::DT_LASER:
		return damageIn * mLaserDamageTaken;
		break;
	case DamageType::DT_PLASMA:
		return damageIn * mPlasmaDamageTaken;
		break;
	case DamageType::DT_CANNON:
		return damageIn * mCannonDamageTaken;
		break;
	case DamageType::DT_MISSILE:
		return damageIn * mMissileDamageTaken;
		break;
	case DamageType::DT_PIERCING:
		return damageIn * mPiercingDamageTaken;
		break;
	}
}
