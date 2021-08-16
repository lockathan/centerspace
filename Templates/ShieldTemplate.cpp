#include <Templates/ShieldTemplate.h>
#include <string>
#include <hash_map>

ShieldTemplate::ShieldTemplate()
	:
	mMaxShieldHealth(0),
	mEnergyDrainPerTurn(0),
	mRegenRate(0),
	mMaterialName("")
{

}

ShieldTemplate::~ShieldTemplate()
{

}

float ShieldTemplate::getMaxShieldHealth()
{
	return mMaxShieldHealth;
}

void ShieldTemplate::setMaxShieldHealth(const float& health)
{
	mMaxShieldHealth = health;
}
	
float ShieldTemplate::getEnergyDrainPerTurn()
{
	return mEnergyDrainPerTurn;
}

void ShieldTemplate::setEnergyDrainPerTurn(const float& drain)
{
	mEnergyDrainPerTurn = drain;
}
	
float ShieldTemplate::getRegenRate()
{
	return mRegenRate;
}

void ShieldTemplate::setRegenRate(const float& rate)
{
	mRegenRate = rate;
}

std::string ShieldTemplate::getMaterialName()
{
	return mMaterialName;
}

void ShieldTemplate::setMaterialName(const std::string& name)
{
	mMaterialName = name;
}