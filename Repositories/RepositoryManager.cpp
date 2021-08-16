#include <Repositories/RepositoryManager.h>
#include <Repositories/Repository.h>
#include <Repositories/AIGroupTemplateRepository.h>
#include <Repositories/AIGroupTemplateRepositoryLoader.h>
#include <Repositories/AIMCUTemplateRepository.h>
#include <Repositories/AIMCUTemplateRepositoryLoader.h>
#include <Repositories/ArmorTemplateRepository.h>
#include <Repositories/ArmorTemplateRepositoryLoader.h>
#include <Repositories/AnimationDefinitionTemplateDataLoader.h>
#include <Repositories/BatteryTemplateRepository.h>
#include <Repositories/BatteryTemplateRepositoryLoader.h>
#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/BattlefieldTemplateRepositoryLoader.h>
#include <Repositories/BeamProjectileTemplateRepository.h>
#include <Repositories/BeamProjectileTemplateRepositoryLoader.h>
#include <Repositories/DriveTemplateRepository.h>
#include <Repositories/DriveTemplateRepositoryLoader.h>
#include <Repositories/FactionHubTemplateRepository.h>
#include <Repositories/FactionHubTemplateRepositoryLoader.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/FactionTemplateRepositoryLoader.h>
#include <Repositories/HubTemplateRepository.h>
#include <Repositories/HubTemplateRepositoryLoader.h>
#include <Repositories/MCUTemplateRepository.h>
#include <Repositories/MCUTemplateRepositoryLoader.h>
#include <Repositories/PhysicalProjectileTemplateRepository.h>
#include <Repositories/PhysicalProjectileTemplateRepositoryLoader.h>
#include <Repositories/RandomMissionTypeTemplateRepository.h>
#include <Repositories/RandomMissionTypeTemplateRepositoryLoader.h>
#include <Repositories/RankTemplateRepository.h>
#include <Repositories/RankTemplateRepositoryLoader.h>
#include <Repositories/ReactorTemplateRepository.h>
#include <Repositories/ReactorTemplateRepositoryLoader.h>
#include <Repositories/SensorSystemTemplateRepository.h>
#include <Repositories/SensorSystemTemplateRepositoryLoader.h>
#include <Repositories/ShieldTemplateRepository.h>
#include <Repositories/ShieldTemplateRepositoryLoader.h>
#include <Repositories/SoundTemplateRepository.h>
#include <Repositories/SoundTemplateRepositoryLoader.h>
#include <Repositories/TargetingSystemTemplateRepository.h>
#include <Repositories/TargetingSystemTemplateRepositoryLoader.h>
#include <Repositories/TerrainTemplateRepository.h>
#include <Repositories/TerrainTemplateRepositoryLoader.h>
#include <Repositories/TerrainTextureTemplateRepository.h>
#include <Repositories/TerrainTextureTemplateRepositoryLoader.h>
#include <Repositories/WeaponAnimationTemplateDataLoader.h>
#include <Repositories/WeaponHardpointTemplateDataLoader.h>
#include <Repositories/WeaponTemplateRepository.h>
#include <Repositories/WeaponTemplateRepositoryLoader.h>
#include <hash_map>
#include <string>

RepositoryManager::RepositoryManager()
	:
	mAIGroupTemplateRepositoryLoader(0),
	mAIMCUTemplateRepositoryLoader(0),
	mAnimationDefinitionTemplateDataLoader(0),
	mArmorTemplateRepositoryLoader(0),
	mBatteryTemplateRepositoryLoader(0),
	mBattlefieldTemplateRepositoryLoader(0),
	mBeamProjectileTemplateRepositoryLoader(0),
	mDriveTemplateRepositoryLoader(0),
	mFactionHubTemplateRepositoryLoader(0),
	mFactionTemplateRepositoryLoader(0),
	mHubTemplateRepositoryLoader(0),
	mMCUTemplateRepositoryLoader(0),
	mPhysicalProjectileTemplateRepositoryLoader(0),
	mRandomMissionTypeTemplateRepositoryLoader(0),
	mRankTemplateRepositoryLoader(0),
	mReactorTemplateRepositoryLoader(0),
	mSensorSystemTemplateRepositoryLoader(0),
	mShieldTemplateRepositoryLoader(0),
	mTargetingSystemTemplateRepositoryLoader(0),
	mTerrainTemplateRepositoryLoader(0),
	mWeaponAnimationTemplateDataLoader(0),
	mWeaponHardpointTemplateDataLoader(0),
	mWeaponTemplateRepositoryLoader(0)
{
	mAIGroupTemplateRepository = AIGroupTemplateRepository::getSingletonPtr();
	mAIMCUTemplateRepository = AIMCUTemplateRepository::getSingletonPtr();
	mArmorTemplateRepository = ArmorTemplateRepository::getSingletonPtr();
	mBatteryTemplateRepository = BatteryTemplateRepository::getSingletonPtr();
	mBattlefieldTemplateRepository = BattlefieldTemplateRepository::getSingletonPtr();
	mBeamProjectileTemplateRepository = BeamProjectileTemplateRepository::getSingletonPtr();
	mDriveTemplateRepository = DriveTemplateRepository::getSingletonPtr();
	mFactionHubTemplateRepository = FactionHubTemplateRepository::getSingletonPtr();
	mFactionTemplateRepository = FactionTemplateRepository::getSingletonPtr();
	mHubTemplateRepository = HubTemplateRepository::getSingletonPtr();
	mMCUTemplateRepository = MCUTemplateRepository::getSingletonPtr();
	mPhysicalProjectileTemplateRepository = PhysicalProjectileTemplateRepository::getSingletonPtr();
	mRandomMissionTypeTemplateRepository = RandomMissionTypeTemplateRepository::getSingletonPtr();
	mRankTemplateRepository = RankTemplateRepository::getSingletonPtr();
	mReactorTemplateRepository = ReactorTemplateRepository::getSingletonPtr();
	mSensorSystemTemplateRepository = SensorSystemTemplateRepository::getSingletonPtr();
	mShieldTemplateRepository = ShieldTemplateRepository::getSingletonPtr();
	mSoundTemplateRepository = SoundTemplateRepository::getSingletonPtr();
	mTargetingSystemTemplateRepository = TargetingSystemTemplateRepository::getSingletonPtr();
	mTerrainTemplateRepository = TerrainTemplateRepository::getSingletonPtr();
	mTerrainTextureTemplateRepository = TerrainTextureTemplateRepository::getSingletonPtr();
	mWeaponTemplateRepository = WeaponTemplateRepository::getSingletonPtr();
}

RepositoryManager::~RepositoryManager()
{
	delete mAIGroupTemplateRepository;
	delete mAIMCUTemplateRepository;
	delete mArmorTemplateRepository;
	delete mBatteryTemplateRepository;
	delete mBattlefieldTemplateRepository;
	delete mBeamProjectileTemplateRepository;
	delete mDriveTemplateRepository;
	delete mFactionHubTemplateRepository;
	delete mFactionTemplateRepository;
	delete mHubTemplateRepository;
	delete mMCUTemplateRepository;
	delete mPhysicalProjectileTemplateRepository;
	delete mRandomMissionTypeTemplateRepository;
	delete mRankTemplateRepository;
	delete mReactorTemplateRepository;
	delete mSensorSystemTemplateRepository;
	delete mShieldTemplateRepository;
	delete mSoundTemplateRepository;
	delete mTargetingSystemTemplateRepository;
	delete mTerrainTemplateRepository;
	delete mTerrainTextureTemplateRepository;
	delete mWeaponTemplateRepository;
}

void RepositoryManager::initialize()
{
	mAIGroupTemplateRepositoryLoader =  new AIGroupTemplateRepositoryLoader();
	mAIMCUTemplateRepositoryLoader = new AIMCUTemplateRepositoryLoader();
	mAnimationDefinitionTemplateDataLoader = new AnimationDefinitionTemplateDataLoader();
	mArmorTemplateRepositoryLoader = new ArmorTemplateRepositoryLoader();
	mBatteryTemplateRepositoryLoader = new BatteryTemplateRepositoryLoader();
	mBattlefieldTemplateRepositoryLoader = new BattlefieldTemplateRepositoryLoader();
	mBeamProjectileTemplateRepositoryLoader = new BeamProjectileTemplateRepositoryLoader();
	mDriveTemplateRepositoryLoader = new DriveTemplateRepositoryLoader();
	mFactionHubTemplateRepositoryLoader = new FactionHubTemplateRepositoryLoader();
	mFactionTemplateRepositoryLoader = new FactionTemplateRepositoryLoader();
	mHubTemplateRepositoryLoader = new HubTemplateRepositoryLoader();
	mMCUTemplateRepositoryLoader = new MCUTemplateRepositoryLoader();
	mPhysicalProjectileTemplateRepositoryLoader = new PhysicalProjectileTemplateRepositoryLoader();
	mRandomMissionTypeTemplateRepositoryLoader = new RandomMissionTypeTemplateRepositoryLoader();
	mRankTemplateRepositoryLoader = new RankTemplateRepositoryLoader();
	mReactorTemplateRepositoryLoader = new ReactorTemplateRepositoryLoader();
	mSensorSystemTemplateRepositoryLoader = new SensorSystemTemplateRepositoryLoader();
	mShieldTemplateRepositoryLoader = new ShieldTemplateRepositoryLoader();
	mSoundTemplateRepositoryLoader = new SoundTemplateRepositoryLoader();
	mTargetingSystemTemplateRepositoryLoader = new TargetingSystemTemplateRepositoryLoader();
	mTerrainTemplateRepositoryLoader = new TerrainTemplateRepositoryLoader();
	mTerrainTextureTemplateRepositoryLoader = new TerrainTextureTemplateRepositoryLoader();
	mWeaponAnimationTemplateDataLoader = new WeaponAnimationTemplateDataLoader();	
	mWeaponHardpointTemplateDataLoader = new WeaponHardpointTemplateDataLoader();
	mWeaponTemplateRepositoryLoader = new WeaponTemplateRepositoryLoader();
}

void RepositoryManager::loadAIMCUs(const std::string& fileName)
{
	mAIMCUTemplateRepositoryLoader->load(fileName, mAIMCUTemplateRepository);
}

void RepositoryManager::loadAIGroups(const std::string& fileName)
{
	mAIGroupTemplateRepositoryLoader->load(fileName, mAIGroupTemplateRepository);
}

void RepositoryManager::loadAnimations(const std::string& fileName)
{
	mAnimationDefinitionTemplateDataLoader->load(fileName);
}

void RepositoryManager::loadArmors(const std::string& fileName)
{
	mArmorTemplateRepositoryLoader->load(fileName, mArmorTemplateRepository);
}

void RepositoryManager::loadBatteries(const std::string& fileName)
{
	mBatteryTemplateRepositoryLoader->load(fileName, mBatteryTemplateRepository);
}

void RepositoryManager::loadBattlefields(const std::string& fileName)
{
	mBattlefieldTemplateRepositoryLoader->load(fileName, mBattlefieldTemplateRepository);
}

void RepositoryManager::loadBeamProjectiles(const std::string& fileName)
{
	mBeamProjectileTemplateRepositoryLoader->load(fileName, mBeamProjectileTemplateRepository);
}

void RepositoryManager::loadDrives(const std::string& fileName)
{
	mDriveTemplateRepositoryLoader->load(fileName, mDriveTemplateRepository);
}

void RepositoryManager::loadFactions(const std::string& fileName)
{
	mFactionTemplateRepositoryLoader->load(fileName, mFactionTemplateRepository);
}

void RepositoryManager::loadFactionHubs(const std::string& fileName)
{
	mFactionHubTemplateRepositoryLoader->load(fileName, mFactionHubTemplateRepository);
}

void RepositoryManager::loadHubs(const std::string& fileName)
{
	mHubTemplateRepositoryLoader->load(fileName, mHubTemplateRepository);
}

void RepositoryManager::loadMCUs(const std::string& fileName)
{
	mMCUTemplateRepositoryLoader->load(fileName, mMCUTemplateRepository);
}

void RepositoryManager::loadPhysicalProjectiles(const std::string& fileName)
{
	mPhysicalProjectileTemplateRepositoryLoader->load(fileName, mPhysicalProjectileTemplateRepository);
}

void RepositoryManager::loadRandomMissionTypes(const std::string& fileName)
{
	mRandomMissionTypeTemplateRepositoryLoader->load(fileName, mRandomMissionTypeTemplateRepository);
}

void RepositoryManager::loadRanks(const std::string& fileName)
{
	mRankTemplateRepositoryLoader->load(fileName, mRankTemplateRepository);
}

void RepositoryManager::loadReactors(const std::string& fileName)
{
	mReactorTemplateRepositoryLoader->load(fileName, mReactorTemplateRepository);
}

void RepositoryManager::loadSensors(const std::string& fileName)
{
	mSensorSystemTemplateRepositoryLoader->load(fileName, mSensorSystemTemplateRepository);
}

void RepositoryManager::loadShields(const std::string& fileName)
{
	mShieldTemplateRepositoryLoader->load(fileName, mShieldTemplateRepository);
}

void RepositoryManager::loadSounds(const std::string& fileName)
{
	mSoundTemplateRepositoryLoader->load(fileName, mSoundTemplateRepository);
}

void RepositoryManager::loadTargetingSystems(const std::string& fileName)
{
	mTargetingSystemTemplateRepositoryLoader->load(fileName, mTargetingSystemTemplateRepository);
}

void RepositoryManager::loadTerrains(const std::string& fileName)
{
	mTerrainTemplateRepositoryLoader->load(fileName, mTerrainTemplateRepository);
}

void RepositoryManager::loadTerrainTextures(const std::string& fileName)
{
	mTerrainTextureTemplateRepositoryLoader->load(fileName, mTerrainTextureTemplateRepository);
}

void RepositoryManager::loadWeaponAnimations(const std::string& fileName)
{
	mWeaponAnimationTemplateDataLoader->load(fileName);
}

void RepositoryManager::loadWeapons(const std::string& fileName)
{
	mWeaponTemplateRepositoryLoader->load(fileName, mWeaponTemplateRepository);
}

void RepositoryManager::loadWeaponHardpoints(const std::string& fileName)
{
	mWeaponHardpointTemplateDataLoader->load(fileName);
}

void RepositoryManager::cleanUp()
{
	delete mAIGroupTemplateRepositoryLoader;
	mAIGroupTemplateRepositoryLoader = 0;
	delete mAIMCUTemplateRepositoryLoader;
	mAIMCUTemplateRepositoryLoader = 0;
	delete mAnimationDefinitionTemplateDataLoader;
	mAnimationDefinitionTemplateDataLoader = 0;
	delete mArmorTemplateRepositoryLoader;
	mArmorTemplateRepositoryLoader = 0;
	delete mBatteryTemplateRepositoryLoader;
	mBatteryTemplateRepositoryLoader = 0;
	delete mBattlefieldTemplateRepositoryLoader;
	mBattlefieldTemplateRepositoryLoader = 0;
	delete mBeamProjectileTemplateRepositoryLoader;
	mBeamProjectileTemplateRepositoryLoader = 0;
	delete mDriveTemplateRepositoryLoader;
	mDriveTemplateRepositoryLoader = 0;
	delete mFactionHubTemplateRepositoryLoader;
	mFactionHubTemplateRepositoryLoader = 0;
	delete mFactionTemplateRepositoryLoader;
	mFactionTemplateRepositoryLoader = 0;
	delete mHubTemplateRepositoryLoader;
	mHubTemplateRepositoryLoader = 0;
	delete mMCUTemplateRepositoryLoader;
	mMCUTemplateRepositoryLoader = 0;
	delete mPhysicalProjectileTemplateRepositoryLoader;
	mPhysicalProjectileTemplateRepositoryLoader = 0;
	delete mRandomMissionTypeTemplateRepositoryLoader;
	mRandomMissionTypeTemplateRepositoryLoader = 0;
	delete mReactorTemplateRepositoryLoader;
	mReactorTemplateRepositoryLoader = 0;
	delete mSensorSystemTemplateRepositoryLoader;
	mSensorSystemTemplateRepositoryLoader = 0;
	delete mShieldTemplateRepositoryLoader;
	mShieldTemplateRepositoryLoader = 0;
	delete mSoundTemplateRepositoryLoader;
	mSoundTemplateRepositoryLoader = 0;
	delete mTargetingSystemTemplateRepositoryLoader;
	mTargetingSystemTemplateRepositoryLoader = 0;
	delete mTerrainTemplateRepositoryLoader;
	mTerrainTemplateRepositoryLoader = 0;
	delete mWeaponAnimationTemplateDataLoader;
	mWeaponAnimationTemplateDataLoader = 0;
	delete mWeaponHardpointTemplateDataLoader;
	mWeaponHardpointTemplateDataLoader = 0;
	delete mWeaponTemplateRepositoryLoader;
	mWeaponTemplateRepositoryLoader = 0;
}

void RepositoryManager::loadData()
{

}

RepositoryManager* RepositoryManager::mRepositoryManager;

RepositoryManager* RepositoryManager::getSingletonPtr()
{
	if(! mRepositoryManager)
	{
		mRepositoryManager = new RepositoryManager();
	}
	return mRepositoryManager;
}