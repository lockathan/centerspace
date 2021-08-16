#ifndef ShieldTemplate_h
#define ShieldTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>

class ShieldTemplate : public MCUComponentTemplate
{
public:
	ShieldTemplate();
	~ShieldTemplate();

	float getMaxShieldHealth();
	void setMaxShieldHealth(const float& health);
	
	float getEnergyDrainPerTurn();
	void setEnergyDrainPerTurn(const float& drain);
	
	float getRegenRate();
	void setRegenRate(const float& rate);
	
	std::string getMaterialName();
	void setMaterialName(const std::string& name);

	
protected:
	float mMaxShieldHealth;
	float mEnergyDrainPerTurn;
	float mRegenRate;
	std::string mMaterialName;
};
#endif