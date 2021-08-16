#ifndef RepositoryManager_h
#define RepositoryManager_h

#include "Ogre.h"
#include <hash_map>

class RepositoryLoader;

class AIGroupTemplateRepository;
class AIMCUTemplateRepository;
class ArmorTemplateRepository;
class BatteryTemplateRepository;
class BattlefieldTemplateRepository;
class BeamProjectileTemplateRepository;
class DriveTemplateRepository;
class FactionHubTemplateRepository;
class FactionTemplateRepository;
class HubTemplateRepository;
class InternalComponentTemplateRepository;
class MCUTemplateRepository;
class PhysicalProjectileTemplateRepository;
class RandomMissionTypeTemplateRepository;
class RankTemplateRepository;
class ReactorTemplateRepository;
class SensorSystemTemplateRepository;
class ShieldTemplateRepository;
class SoundTemplateRepository;
class TargetingSystemTemplateRepository;
class TerrainTemplateRepository;
class TerrainTextureTemplateRepository;
class WeaponAnimationTemplateDataLoader;
class WeaponHardpointTemplateRepository;
class WeaponTemplateRepository;

class AIGroupTemplateRepositoryLoader;
class AIMCUTemplateRepositoryLoader;
class AnimationDefinitionTemplateDataLoader;
class ArmorTemplateRepositoryLoader;
class BatteryTemplateRepositoryLoader;
class BattlefieldTemplateRepositoryLoader;
class BeamProjectileTemplateRepositoryLoader;
class DriveTemplateRepositoryLoader;
class FactionHubTemplateRepositoryLoader;
class FactionTemplateRepositoryLoader;
class HubTemplateRepositoryLoader;
class MCUTemplateRepositoryLoader;
class PhysicalProjectileTemplateRepositoryLoader;
class RandomMissionTypeTemplateRepositoryLoader;
class RankTemplateRepositoryLoader;
class ReactorTemplateRepositoryLoader;
class SensorSystemTemplateRepositoryLoader;
class ShieldTemplateRepositoryLoader;
class SoundTemplateRepositoryLoader;
class TargetingSystemTemplateRepositoryLoader;
class TerrainTemplateRepositoryLoader;
class TerrainTextureTemplateRepositoryLoader;
class WeaponAnimationTemplateDataLoader;	
class WeaponHardpointTemplateDataLoader;
class WeaponTemplateRepositoryLoader;

class RepositoryManager
{
public:
	~RepositoryManager();
	static RepositoryManager* getSingletonPtr();
	void initialize();
	void cleanUp();

	void loadData();

	void loadAIMCUs(const std::string& fileName);
	void loadAIGroups(const std::string& fileName);
	void loadAnimations(const std::string& fileName);
	void loadArmors(const std::string& fileName);
	void loadBatteries(const std::string& fileName);
	void loadBattlefields(const std::string& fileName);
	void loadBeamProjectiles(const std::string& fileName);
	void loadDrives(const std::string& fileName);
	void loadFactions(const std::string& fileName);
	void loadFactionHubs(const std::string& fileName);
	void loadHubs(const std::string& fileName);
	void loadMCUs(const std::string& fileName);
	void loadPhysicalProjectiles(const std::string& fileName);
	void loadRanks(const std::string& fileName);
	void loadRandomMissionTypes(const std::string& fileName);
	void loadReactors(const std::string& fileName);
	void loadSensors(const std::string& fileName);
	void loadShields(const std::string& fileName);
	void loadSounds(const std::string& fileName);
	void loadTargetingSystems(const std::string& fileName);
	void loadTerrains(const std::string& fileName);
	void loadTerrainTextures(const std::string& fileName);
	void loadWeaponAnimations(const std::string& fileName);
	void loadWeapons(const std::string& fileName);
	void loadWeaponHardpoints(const std::string& fileName);
private:
	
	AIGroupTemplateRepository* mAIGroupTemplateRepository;
	AIMCUTemplateRepository* mAIMCUTemplateRepository;
	ArmorTemplateRepository* mArmorTemplateRepository;
	BatteryTemplateRepository* mBatteryTemplateRepository;
	BattlefieldTemplateRepository* mBattlefieldTemplateRepository;
	BeamProjectileTemplateRepository* mBeamProjectileTemplateRepository;
	DriveTemplateRepository* mDriveTemplateRepository;
	FactionHubTemplateRepository* mFactionHubTemplateRepository;
	FactionTemplateRepository* mFactionTemplateRepository;
	HubTemplateRepository* mHubTemplateRepository;
	InternalComponentTemplateRepository* mInternalComponentTemplateRepository;
	MCUTemplateRepository* mMCUTemplateRepository;
	PhysicalProjectileTemplateRepository* mPhysicalProjectileTemplateRepository;
	RandomMissionTypeTemplateRepository* mRandomMissionTypeTemplateRepository;
	RankTemplateRepository* mRankTemplateRepository;
	ReactorTemplateRepository* mReactorTemplateRepository;
	SensorSystemTemplateRepository* mSensorSystemTemplateRepository;
	ShieldTemplateRepository* mShieldTemplateRepository;
	SoundTemplateRepository* mSoundTemplateRepository;
	TargetingSystemTemplateRepository* mTargetingSystemTemplateRepository;
	TerrainTemplateRepository* mTerrainTemplateRepository;
	TerrainTextureTemplateRepository* mTerrainTextureTemplateRepository;
	WeaponTemplateRepository* mWeaponTemplateRepository;

	AIGroupTemplateRepositoryLoader* mAIGroupTemplateRepositoryLoader;
	AIMCUTemplateRepositoryLoader* mAIMCUTemplateRepositoryLoader;
	AnimationDefinitionTemplateDataLoader* mAnimationDefinitionTemplateDataLoader;
	ArmorTemplateRepositoryLoader* mArmorTemplateRepositoryLoader;
	BatteryTemplateRepositoryLoader* mBatteryTemplateRepositoryLoader;
	BattlefieldTemplateRepositoryLoader* mBattlefieldTemplateRepositoryLoader;
	BeamProjectileTemplateRepositoryLoader* mBeamProjectileTemplateRepositoryLoader;
	DriveTemplateRepositoryLoader* mDriveTemplateRepositoryLoader;
	FactionHubTemplateRepositoryLoader* mFactionHubTemplateRepositoryLoader;
	FactionTemplateRepositoryLoader* mFactionTemplateRepositoryLoader;
	HubTemplateRepositoryLoader* mHubTemplateRepositoryLoader;
	MCUTemplateRepositoryLoader* mMCUTemplateRepositoryLoader;
	PhysicalProjectileTemplateRepositoryLoader* mPhysicalProjectileTemplateRepositoryLoader;
	RandomMissionTypeTemplateRepositoryLoader* mRandomMissionTypeTemplateRepositoryLoader;
	RankTemplateRepositoryLoader* mRankTemplateRepositoryLoader;
	ReactorTemplateRepositoryLoader* mReactorTemplateRepositoryLoader;
	SensorSystemTemplateRepositoryLoader* mSensorSystemTemplateRepositoryLoader;
	ShieldTemplateRepositoryLoader* mShieldTemplateRepositoryLoader;
	SoundTemplateRepositoryLoader* mSoundTemplateRepositoryLoader;
	TargetingSystemTemplateRepositoryLoader* mTargetingSystemTemplateRepositoryLoader;
	TerrainTemplateRepositoryLoader* mTerrainTemplateRepositoryLoader;
	TerrainTextureTemplateRepositoryLoader* mTerrainTextureTemplateRepositoryLoader;
	WeaponAnimationTemplateDataLoader* mWeaponAnimationTemplateDataLoader;	
	WeaponHardpointTemplateDataLoader* mWeaponHardpointTemplateDataLoader;
	WeaponTemplateRepositoryLoader* mWeaponTemplateRepositoryLoader;

	static RepositoryManager* mRepositoryManager;

	RepositoryManager();
	
};
#endif