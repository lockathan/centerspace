#ifndef MechanizedCombatUnit_h
#define MechanizedCombatUnit_h

#include "Ogre.h"
#include <Entities/PersistentEntity.h>
#include <Templates/ArmorTemplate.h>
#include <Templates/BatteryTemplate.h>
#include <Templates/DriveTemplate.h>
#include <Templates/ReactorTemplate.h>
#include <Templates/SensorSystemTemplate.h>
#include <Templates/ShieldTemplate.h>
#include <Templates/TargetingSystemTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <hash_map>
#include <vector>

class DataSet;
class Projectile;
class WeaponHardpoint;
class MCUTemplate;
class InventoryState;
class DbValue;

class MechanizedCombatUnit : public PersistentEntity
{
public:
	MechanizedCombatUnit();
	~MechanizedCombatUnit();
	
	ArmorTemplate* getArmor();
	void setArmor(ArmorTemplate* armor);

	BatteryTemplate* getBattery();
	void setBattery(BatteryTemplate* battery);

	DriveTemplate* getDrive();
	void setDrive(DriveTemplate* drive);
	
	InventoryState* getInventoryState();
	void setInventoryState(InventoryState* state);

	MCUTemplate* getMCUTemplate();
	void setMCUTemplate(MCUTemplate* t);

	ReactorTemplate* getReactor();
	void setReactor(ReactorTemplate* reactor);

	SensorSystemTemplate* getSensorSystem();
	void setSensorSystem(SensorSystemTemplate* sensor);

	ShieldTemplate* getShield();
	void setShield(ShieldTemplate* shield);

	TargetingSystemTemplate* getTargetingSystem();
	void setTargetingSystem(TargetingSystemTemplate* target);

	WeaponHardpoint* getWeaponHardpoint(unsigned int slot);
	std::vector<WeaponHardpoint*> getWeapons();

	void setWeapon(int slot, WeaponTemplate* weapon);
	void addWeapon(WeaponHardpoint* hardpoint);
	void setWeapons(const std::vector<WeaponHardpoint*>& weapons);
	void removeWeapon(int slot);

	float getTotalWeight();
	float getMovementCostPerSquare();

	int getSequence();
	void setSequence(int sequence);

	std::hash_map<std::string,DbValue> getValues();
	void setValues(std::hash_map<std::string,std::string> values);
	
	void applyWarhead(WeaponTemplate* weapon, const float& shieldDamageNotAbsorbed, bool shieldWasActive);

	std::hash_map<std::string,std::string> getGuiValues();
	DataSet* getWeaponGuiData();

protected:

	InventoryState* mInventoryState;

	void recalculateWeight();
	std::vector<WeaponHardpoint*> mWeaponHardpoints;

	ArmorTemplate* mArmor;
	BatteryTemplate* mBattery;
	DriveTemplate* mDrive;
	ReactorTemplate* mReactor;
	SensorSystemTemplate* mSensorSystem;
	ShieldTemplate* mShield;
	TargetingSystemTemplate* mTargetingSystem;

	float mTotalWeight;

	int mSequence;

	MCUTemplate* mMCUTemplate;

};
#endif