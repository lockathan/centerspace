#ifndef ArmorTemplate_h
#define ArmorTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>
#include <Templates/WeaponTemplate.h>

class ArmorTemplate : public MCUComponentTemplate
{
public:
	ArmorTemplate();
	~ArmorTemplate();

	float getLaserDamageTaken();
	void setLaserDamageTaken(const float& damage);
	
	float getPlasmaDamageTaken();
	void setPlasmaDamageTaken(const float& damage);
	
	float getCannonDamageTaken();
	void setCannonDamageTaken(const float& damage);
	
	float getMissileDamageTaken();
	void setMissileDamageTaken(const float& damage);
	
	float getPiercingDamageTaken();
	void setPiercingDamageTaken(const float& damage);

	float getFinalDamageValue(const float& damageIn, DamageType damageType);
	std::string getPropertiesDescription();
private:
	float mLaserDamageTaken;
	float mPlasmaDamageTaken;
	float mCannonDamageTaken;
	float mMissileDamageTaken;
	float mPiercingDamageTaken;
};
#endif