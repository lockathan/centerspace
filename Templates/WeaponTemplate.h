#ifndef WeaponTemplate_h
#define WeaponTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>
#include <Templates/Enums.h>

class ProjectileTemplate;

enum DamageType
{
	DT_LASER = 0,
	DT_PLASMA = 1,
	DT_CANNON = 2,
	DT_MISSILE = 3,
	DT_PIERCING = 4
};

class WeaponTemplate : public MCUComponentTemplate
{
public:

	WeaponTemplate();
	~WeaponTemplate();

	WeaponType getWeaponType();
	void setWeaponType(WeaponType type);
	void setWeaponType(int i);
	
	float getShieldDamage();
	void setShieldDamage(float damage);
	
	float getArmorDamage();
	void setArmorDamage(const float& damage);

	float getPiercingDamage();
	void setPiercingDamage(const float& damage);

	float getRange();
	void setRange(float range);
	
	int getRoundsPerTurn();
	void setRoundsPerTurn(int rounds);
	
	int getMaxAmmo();
	void setMaxAmmo(int maxAmmo);
	
	float getEnergyDrain();
	void setEnergyDrain(float drain);
	
	

	bool canFit(HardpointType hType);
	
	bool isIndirectFireCapable();
	void setIndirectFireCapable(bool value);

	ProjectileTemplate* getProjectileTemplate();
	void setProjectileTemplate(ProjectileTemplate* t);

	DamageType getDamageType();
	void setDamageType(DamageType type);

	std::hash_map<std::string, std::string> getGuiValues();
protected:
	WeaponType mWeaponType;
	float mShieldDamage;
	float mArmorDamage;
	float mPiercingDamage;

	float mRange;
	int mRoundsPerTurn;
	int mMaxAmmo;
	float mEnergyDrain;
	bool mIndirectFireCapable;
	ProjectileTemplate* mProjectileTemplate;
	DamageType mDamageType;
};


struct Warhead
{
public:
	Warhead(int projId, WeaponTemplate* weapon) : ProjectileID(projId), TotalShieldDamage(weapon->getShieldDamage()),RemainingShieldDamage(weapon->getShieldDamage()),TotalArmorDamage(weapon->getArmorDamage()),RemainingArmorDamage(weapon->getArmorDamage()), TotalPiercingDamage(weapon->getPiercingDamage()),RemainingPiercingDamage(weapon->getPiercingDamage()), ShieldWasActive(false), DamageType(weapon->getDamageType()) {}
	int ProjectileID;
	float TotalShieldDamage;
	float RemainingShieldDamage;
	float TotalArmorDamage;
	float RemainingArmorDamage;
	float TotalPiercingDamage;
	float RemainingPiercingDamage;
	bool ShieldWasActive;
	DamageType DamageType;
};

#endif