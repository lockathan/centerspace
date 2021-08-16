#ifndef AIMCUTemplate_h
#define AIMCUTemplate_h

#include "Ogre.h"
#include <Templates/RequiredRankTemplate.h>
#include <hash_map>
#include <string>

class ArmorTemplate;
class BatteryTemplate;
class DriveTemplate;
class ReactorTemplate;
class SensorSystemTemplate;
class ShieldTemplate;
class TargetingSystemTemplate;
class WeaponTemplate;
class MechanizedCombatUnit;
class MCUTemplate;
class FactionTemplate;

class AIMCUTemplate : public RequiredRankTemplate
{
public:
	AIMCUTemplate();
	~AIMCUTemplate();

	ArmorTemplate* getDefaultArmor();
	void setDefaultArmor(ArmorTemplate* armor);

	BatteryTemplate* getDefaultBattery();
	void setDefaultBattery(BatteryTemplate* battery);
	
	DriveTemplate* getDefaultDrive();
	void setDefaultDrive(DriveTemplate* drive);
	
	ReactorTemplate* getDefaultReactor();
	void setDefaultReactor(ReactorTemplate* reactor);
	
	SensorSystemTemplate* getDefaultSensorSystem();
	void setDefaultSensorSystem(SensorSystemTemplate* sensor);

	ShieldTemplate* getDefaultShield();
	void setDefaultShield(ShieldTemplate* shield);

	TargetingSystemTemplate* getDefaultTargetingSystem();
	void setDefaultTargetingSystem(TargetingSystemTemplate* target);
	
	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);

	MCUTemplate* getMCUTemplate();
	void setMCUTemplate(MCUTemplate* mcu);

	void setWeapon(int slot, WeaponTemplate* weapon);	

	MechanizedCombatUnit* instantiate(bool useDefaults);
protected:

	std::vector<WeaponTemplate*> mWeaponTemplates;
	
	FactionTemplate* mFactionTemplate;

	ArmorTemplate* mDefaultArmor;
	BatteryTemplate* mDefaultBattery;
	DriveTemplate* mDefaultDrive;
	ReactorTemplate* mDefaultReactor;
	SensorSystemTemplate* mDefaultSensorSystem;
	ShieldTemplate* mDefaultShield;
	TargetingSystemTemplate* mDefaultTargetingSystem;

	MCUTemplate* mMCUTemplate;
};
#endif