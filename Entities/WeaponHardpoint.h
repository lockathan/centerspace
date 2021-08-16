#ifndef WeaponHardpoint_h
#define WeaponHardpoint_h

#include "Ogre.h"
#include <Controllers/MCUController.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <Entities/PersistentEntity.h>

class MechanizedCombatUnit;
class DbValue;

class WeaponHardpoint : public PersistentEntity
{
public:
	WeaponHardpoint();
	~WeaponHardpoint();

	MechanizedCombatUnit* getMCU();
	void setMCU(MechanizedCombatUnit* mcu);
	void setWeapon(WeaponTemplate* weapon);
	WeaponTemplate* getWeapon();
	void setHardpointNumber(int);

	bool canFire();
	bool decrementAmmo();
	void resetAmmoRemaining();
	void endTurn();

	int getHardpointNumber();

	void fire(MCUController* mcu, const FireInstruction& instruction);

	std::hash_map<std::string,DbValue> getValues();
	void setValues(std::hash_map<std::string,std::string> values);

	WeaponHardpointTemplate* getWeaponHardpointTemplate();
	void setWeaponHardpointTemplate(WeaponHardpointTemplate* t);

	float getCurrentChanceToHit();
	void setCurrentChanceToHit(const float& chance);

	std::hash_map<std::string, std::string> getGuiValues();
protected:
	MechanizedCombatUnit* mMCU;
	WeaponTemplate* mWeaponTemplate;
	float mCurrentChanceToHit;
	int mAmmoRemaining;
	int mRoundsRemainingThisTurn;
	int mHardpointNumber;
	WeaponHardpointTemplate* mWeaponHardpointTemplate;
};
#endif