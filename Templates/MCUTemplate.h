#ifndef MCUTemplate_h
#define MCUTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <hash_map>
#include <string>

class AnimationDefinitionTemplate;
class ArmorTemplate;
class BatteryTemplate;
class DriveTemplate;
class ReactorTemplate;
class SensorSystemTemplate;
class ShieldTemplate;
class TargetingSystemTemplate;
class MechanizedCombatUnit;

enum MCUType
{
	MT_LIGHT = 0,
	MT_MEDIUM,
	MT_HEAVY
};

class MCUTemplate : public MCUComponentTemplate
{
public:
	MCUTemplate();
	~MCUTemplate();

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

	TargetingSystemTemplate* getDefaultTargetingSystemTemplate();
	void setDefaultTargetingSystemTemplate(TargetingSystemTemplate* target);

	WeaponHardpointTemplate* getHardpoint(int hardpoint);
	void addWeaponHardpointTemplate(WeaponHardpointTemplate* weapon);
	
	int getMaxArmorSize();
	void setMaxArmorSize(int max);

	int getMaxBatterySize();
	void setMaxBatterySize(int maxBatterySize);
	
	int getMaxDriveSize();
	void setMaxDriveSize(int maxDriveSize);
	
	int getMaxReactorSize();
	void setMaxReactorSize(int maxReactorSize);
	
	int getMaxShieldSize();
	void setMaxShieldSize(int maxShieldSize);

	int getMaxComponentSlots();
	void setMaxComponentSlots(int maxComponentSlots);

	MCUType getMCUType();
	void setMCUType(MCUType type);
	void setMCUType(std::string type);

	std::string getMaterialName();
	void setMaterialName(std::string name);
	
	std::string getMeshName();
	void setMeshName(std::string name);
	
	std::string getThumbnailFile();
	void setThumbnailFile(std::string name);
	
	std::string getProfileImageFile();
	void setProfileImageFile(const std::string& name);

	AnimationDefinitionTemplate* getAnimationDefinitionTemplate();
	void setAnimationDefinitionTemplate(AnimationDefinitionTemplate* anim);

	float getMovementSpeed();
	void setMovementSpeed(float speed);

	float getWalkingAnimationSpeed();
	void setWalkingAnimationSpeed(float speed);

	float getScale();
	void setScale(float scale);

	std::string getWeaponsDescription();

	float getTotalCost();

	std::string getWalkSoundName();
	void setWalkSoundName(const std::string& walkSound);

	std::hash_map<std::string, std::string> getGuiValues();

	MechanizedCombatUnit* instantiate(bool useDefaults);
protected:
	std::vector<WeaponHardpointTemplate*> mWeaponHardpointTemplates;
	int mCurrentHardpoint;

	int mMaxArmorSize;
	int mMaxBatterySize;
	int mMaxReactorSize;
	int mMaxDriveSize;
	int mMaxShieldSize;
	int mMaxComponentSlots;
	
	MCUType mMCUType;

	std::string mMaterialName;
	std::string mMeshName;
	std::string mThumbnailFile;
	std::string mProfileImageFile;

	float mMovementSpeed;
	float mWalkingAnimationSpeed;
	float mScale;

	AnimationDefinitionTemplate* mAnimationDefinitionTemplate;
	ArmorTemplate* mDefaultArmor;
	BatteryTemplate* mDefaultBattery;
	DriveTemplate* mDefaultDrive;
	ReactorTemplate* mDefaultReactor;
	SensorSystemTemplate* mDefaultSensorSystem;
	ShieldTemplate* mDefaultShield;
	TargetingSystemTemplate* mTargetingSystemTemplate;

	std::string mWalkSoundName;

};
#endif