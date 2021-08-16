#include <Controllers/MCUController.h>
#include <Controllers/KinematicCharacterController.h>
#include <Controllers/MCUAnimationController.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include <Entities/FogSquare.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MCUComponent.h>
#include <Entities/PathFindingNode.h>
#include <Entities/Projectile.h>
#include <Entities/Shield.h>
#include <Entities/WeaponHardpoint.h>
#include <Managers/GameManager.h>
#include <Managers/LuaStateManager.h>
#include <Managers/PhysicsManager.h>
#include <Managers/SQLiteDBManager.h>
#include <AI/AIGroup.h>
#include <AI/GroupAttackOrder.h>
#include <AI/GroupOrder.h>
#include <Repositories/SoundTemplateRepository.h>
#include <StateChange/BattleResult.h>
#include <StateChange/MCUComponentStatus.h>
#include <StateChange/MCUStatus.h>
#include <StateChange/PlayerResult.h>
#include <States/InBattleState.h>
#include <States/PlayerBattleState.h>
#include <Templates/MCUComponentTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/SoundTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <Templates/WeaponTemplate.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <Ogre.h>
#include <math.h>
#include <list>
#include <queue>
#include <OgreAL.h>
using namespace Ogre;

MCUController::MCUController()
	:
	MovableEntity()
{
	initialize();
}
MCUController::MCUController(Ogre::SceneNode* node,Ogre::Entity* entity,MechanizedCombatUnit* mcu, PlayerBattleState* player,BattlefieldGridCell* location, BattlefieldGrid* grid, int battlefieldId)
	:
	MovableEntity()
{
	initialize();

	
	mBattlefieldID = battlefieldId;
	mEntity = entity;
	mNode = node;
	setMechanizedCombatUnit(mcu);
	float scale = mMechanizedCombatUnit->getMCUTemplate()->getScale();
	mNode->scale(scale, scale, scale);

	mHeight = mEntity->getBoundingBox().getSize().y;
	
	mPlayerBattleState = player;
	InBattleState::getSingletonPtr()->getBattleResult()->getPlayerResult( mPlayerBattleState->getPlayerProfile())->addMCUStatus(mMCUStatus);
	mCurrentLocation = location;
	mBattlefieldGrid = grid;
	mMovementSpeed = mMechanizedCombatUnit->getMCUTemplate()->getMovementSpeed();
	
	Vector3 start = mCurrentLocation->getMiddleFloor();
	Matrix4 worldTransform;
	btTransform btWorldTransform;
	btWorldTransform.setIdentity();
	start.y+= mEntity->getBoundingBox().getSize().y / 2.f;
	btWorldTransform.setOrigin(BtOgre::Convert::toBullet(start));
	btWorldTransform.setRotation(BtOgre::Convert::toBullet(Quaternion::IDENTITY));

	worldTransform.makeTransform(start, Vector3::UNIT_SCALE, Quaternion::IDENTITY);

	BtOgre::AnimatedMeshToShapeConverter converter(mEntity,worldTransform);
					
	btCylinderShape* shape = new btCylinderShape(btVector3(mEntity->getBoundingBox().getSize().x / 2.f, mEntity->getBoundingBox().getSize().y / 2.f,  mEntity->getBoundingBox().getSize().z / 2.f));
	//btCapsuleShape* shape = new btCapsuleShape(mEntity->getBoundingBox().getSize().x,mEntity->getBoundingBox().getSize().y);

	//btBoxShape* shape = converter.createBox();
	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
	ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	ghost->setWorldTransform(btWorldTransform);
	ghost->setCollisionShape(shape);
					
					
	mKinematicCharacterController = new KinematicCharacterController(ghost,shape,0.5f,1);
					
	mKinematicCharacterController->setFallSpeed(10);
	mKinematicCharacterController->setJumpSpeed(20);
	mKinematicCharacterController->setMaxJumpHeight(20);

	PhysicsManager::getSingletonPtr()->addCharacterController(mKinematicCharacterController,ghost);
	mMCUAnimationController = new MCUAnimationController(mNode, mEntity, mMechanizedCombatUnit->getMCUTemplate()->getAnimationDefinitionTemplate(), this);

	mShield = new Shield(mMechanizedCombatUnit->getShield(),this,mNode,btWorldTransform);
	PhysicsManager::getSingletonPtr()->registerCollisionObject(ghost,mShield->getGhostObject(),mEntity,mShield->getEntity(), this, "MCU" + SQLiteDBManager::convertIntToString(mBattlefieldID));
	
	
	mRadarBillBoard = GameManager::getSingletonPtr()->getSceneManager()->createBillboardChain("radar" + SQLiteDBManager::convertIntToString(mBattlefieldID));
	mRadarBillBoard->setFaceCamera(true);
	mRadarBillBoard->setMaxChainElements(4);
	mRadarBillBoard->setNumberOfChains(2);
	mRadarBillBoard->setMaterialName("radar");

	float quarter = BattlefieldGrid::UnitSize / 4.f;
	Vector3 loc = Vector3(-quarter, BattlefieldGrid::UnitSize, 0);

	BillboardChain::Element* elem = new BillboardChain::Element(loc, BattlefieldGrid::UnitSize / 2.f,0.f,ColourValue(0.5f,0.5f,0.5f),Quaternion::IDENTITY);
	mRadarBillBoard->addChainElement(1, *elem);

	loc = Vector3(quarter, BattlefieldGrid::UnitSize, 0);
	elem = new BillboardChain::Element(loc, BattlefieldGrid::UnitSize / 2.f,0.f,ColourValue(0.5f,0.5f,0.5f),Quaternion::IDENTITY);
	mRadarBillBoard->addChainElement(1, *elem);
	mNode->attachObject(mRadarBillBoard);
	mRadarBillBoard->setVisible(false);

	updateFogOfWar();
}

MCUController::MCUController(Ogre::SceneNode* node,Ogre::Entity* entity, MCUAnimationController* anim,KinematicCharacterController* physics,MechanizedCombatUnit* mcu, PlayerBattleState* player,BattlefieldGridCell* location, BattlefieldGrid* grid, Ogre::Real height)
	:
	MovableEntity()
{
	initialize();

	mEntity = entity;
	mMCUAnimationController = anim;
	mKinematicCharacterController = physics;

	setMechanizedCombatUnit(mcu);

	mPlayerBattleState = player;
	InBattleState::getSingletonPtr()->getBattleResult()->getPlayerResult( mPlayerBattleState->getPlayerProfile())->addMCUStatus(mMCUStatus);
	mCurrentLocation = location;
	mBattlefieldGrid = grid;
	mMovementSpeed = mMechanizedCombatUnit->getMCUTemplate()->getMovementSpeed();
	mNode = node;
	mHeight = height;

	updateFogOfWar();
}

void MCUController::initialize()
{
	mActive = true;
	mIsFiring = false;
	mIsJumping = false;
	mJumped = false;
	mLanding = false;
	mStartedTurning = false;
	mYawAmount = 0;
	mInitializedFogOfWar = false;
	mNeedsFogUpdate = true;
	mNeedsFogRender = false;
	mIsPlayerMCU = false;
	mSensorActive = true;
	mFogSquareGrid = 0;
	Sequence = 0;
	mBattlefieldID = 0;
	mCurrentTarget = 0;
	mMCUStatus = 0;
	mDirection = Vector3::ZERO;
	mPosition = Vector3::ZERO;
	mTarget = Vector3::ZERO;
	mDistanceRemaining = 0;
	mShield = 0;
	mHasPath = false;
	mMoving = false;
	mOrderedToStop = false;
	mReachedDestination = false;
	mTurning = false;
	mTurnToFire = false;
	mRotationToTarget = Quaternion();
	mCurrentFireInstruction = FireInstruction();
	mTargetedEnemyComponent = 0;
	mEntity = 0;
	mMCUAnimationController = 0;
	mKinematicCharacterController = 0;
	mMechanizedCombatUnit = 0;
	mBattlefieldGrid = 0;
	mMovementSpeed = 10.f;
	mNode = 0;
	mHeight = 0;
	mAISpeedBoost = 1;

	mChassisComponent = 0;
	mArmorComponent = 0;
	mBatteryComponent = 0;
	mDriveComponent = 0;
	mReactorComponent = 0;
	mSensorSystemComponent = 0;
	mShieldComponent = 0;
	mTargetingSystemComponent = 0;

	mRemainingBatteryPower = 0;
	mRemainingReactorPower = 0;

	mAIGroup = 0;
	mLastFiredUponBy = 0;

	mWalkSound = 0;
	mJumpSound = 0;
	mLandSound = 0;
	mTurnSound = 0;

	mFogRayTestDirections.push_back(Vector3(1,0,0).normalisedCopy());
	mFogRayTestDirections.push_back(Vector3(1,0,1).normalisedCopy());
	mFogRayTestDirections.push_back(Vector3(0,0,1).normalisedCopy());
	mFogRayTestDirections.push_back(Vector3(-1,0,1).normalisedCopy());
	mFogRayTestDirections.push_back(Vector3(-1,0,0).normalisedCopy());
	mFogRayTestDirections.push_back(Vector3(-1,0,-1).normalisedCopy());
	mFogRayTestDirections.push_back(Vector3(0,0,-1).normalisedCopy());
	mFogRayTestDirections.push_back(Vector3(1,0,-1).normalisedCopy());

	mLastDamagedBy = 0;
}
	
MCUController::~MCUController()
{
	mCurrentLocation->occupyCell(0);
	SceneManager* sceneManager = GameManager::getSingletonPtr()->getSceneManager();

	if(mShield)
	{
		delete mShield;
		mShield = 0;
	}
	
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();

	if(mWalkSound)
	{
		soundManager->destroySound(mWalkSound);
	}
	if(mJumpSound)
	{
		soundManager->destroySound(mJumpSound);
	}
	if(mLandSound)
	{
		soundManager->destroySound(mLandSound);
	}
	if(mTurnSound)
	{
		soundManager->destroySound(mTurnSound);
	}
	if(mChassisComponent)
		delete mChassisComponent;

	if(mArmorComponent)
		delete mArmorComponent;
	
	if(mBatteryComponent)
		delete mBatteryComponent;

	if(mDriveComponent)
		delete mDriveComponent;
	
	if(mReactorComponent)
		delete mReactorComponent;
	
	if(mSensorSystemComponent)
		delete mSensorSystemComponent;

	if(mShieldComponent)
		delete mShieldComponent;

	if(mTargetingSystemComponent)
		delete mTargetingSystemComponent;

	std::deque<MCUComponent*>::iterator hardpoint;

	for(hardpoint = mWeaponHardpointComponents.begin(); hardpoint != mWeaponHardpointComponents.end();++hardpoint )
	{
		if((*hardpoint))
			delete (*hardpoint);
	}

	if(mEntity)
	{
		sceneManager->destroyEntity(mEntity);
		mEntity = 0;
	}

	if(mNode)
	{
		sceneManager->destroySceneNode(mNode);
		mNode = 0;
	}


	if(mMCUAnimationController)
	{
		delete mMCUAnimationController;
		mMCUAnimationController = 0;
	}

	if(mKinematicCharacterController)
	{
		delete mKinematicCharacterController;
		mKinematicCharacterController = 0;
	}
}

void MCUController::startTurn()
{
	std::vector<WeaponHardpoint*> weapons = mMechanizedCombatUnit->getWeapons();
	std::vector<WeaponHardpoint*>::iterator weaponIterator;

	for(weaponIterator = weapons.begin(); weaponIterator != weapons.end(); ++weaponIterator)
	{
		if((*weaponIterator))
		{
			(*weaponIterator)->endTurn();
		}
	}	
}

void MCUController::endTurn()
{
	mIsPlayerMCU = false;
	if(!mBatteryComponent->isBroken())
	{
		mRemainingBatteryPower+= mRemainingReactorPower;
		mRemainingReactorPower = mMechanizedCombatUnit->getReactor()->getMaxEnergy();
	
		if(mRemainingBatteryPower > mMechanizedCombatUnit->getBattery()->getMaxEnergy())
			mRemainingBatteryPower = mMechanizedCombatUnit->getBattery()->getMaxEnergy();
		else
		{
			float difference = mMechanizedCombatUnit->getBattery()->getMaxEnergy() - mRemainingBatteryPower;
			mRemainingReactorPower-= difference;
			mRemainingBatteryPower+=difference;
		}

		if(mRemainingBatteryPower > mShield->getShieldTemplate()->getEnergyDrainPerTurn() && !mShieldComponent->isBroken())
		{
			mRemainingBatteryPower -= mShield->getShieldTemplate()->getEnergyDrainPerTurn();
			mShield->regenerate();
		}
		else
		{
			mShield->setCurrentHealth(0);
		}
		if(mRemainingBatteryPower > mMechanizedCombatUnit->getSensorSystem()->getEnergyDrainPerTurn() && !mSensorSystemComponent->isBroken())
		{
			mRemainingBatteryPower -= mMechanizedCombatUnit->getSensorSystem()->getEnergyDrainPerTurn();
			mSensorActive = true;
		}
		else
		{
			mSensorActive = false;
		}
	}
	else
	{
		mRemainingReactorPower = mMechanizedCombatUnit->getReactor()->getMaxEnergy();
		if(mRemainingReactorPower > mShield->getShieldTemplate()->getEnergyDrainPerTurn() && !mShieldComponent->isBroken())
		{
			mRemainingReactorPower -= mShield->getShieldTemplate()->getEnergyDrainPerTurn();
			mShield->regenerate();
		}
		else
		{
			mShield->setCurrentHealth(0);
		}
		if(mRemainingReactorPower > mMechanizedCombatUnit->getSensorSystem()->getEnergyDrainPerTurn() && !mSensorSystemComponent->isBroken())
		{
			mRemainingReactorPower -= mMechanizedCombatUnit->getSensorSystem()->getEnergyDrainPerTurn();
			mSensorActive = true;
		}
		else
		{
			mSensorActive = false;
		}
	}
	std::deque<MCUComponent*>::iterator weapon;

	for(weapon = mWeaponHardpointComponents.begin(); weapon != mWeaponHardpointComponents.end(); ++weapon)
	{
		(*weapon)->getWeaponHardpoint()->endTurn();
	}

	mNeedsFogUpdate = true;
	updateFogOfWar();
}

Ogre::Entity* MCUController::getEntity()
{
	return mEntity;
}

void MCUController::setEntity(Ogre::Entity* entity)
{
	mEntity = entity;
}

MCUAnimationController* MCUController::getMCUAnimationController()
{
	return mMCUAnimationController;
}

void MCUController::setMCUAnimationController(MCUAnimationController* anim)
{
	mMCUAnimationController = anim;
}

KinematicCharacterController* MCUController::getKinematicCharacterController()
{
	return mKinematicCharacterController;
}

void MCUController::setKinematicCharacterController(KinematicCharacterController* physics)
{
	mKinematicCharacterController = physics;
}

MechanizedCombatUnit* MCUController::getMechanizedCombatUnit()
{
	return mMechanizedCombatUnit;
}

void MCUController::setMechanizedCombatUnit(MechanizedCombatUnit* mcu)
{
	mMechanizedCombatUnit = mcu;
	mMovementSpeed = mMechanizedCombatUnit->getMCUTemplate()->getMovementSpeed();
	mMCUStatus = new MCUStatus(mMechanizedCombatUnit);
	mAllComponents.clear();
	
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();

	SoundTemplate* walkSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName(mMechanizedCombatUnit->getMCUTemplate()->getWalkSoundName());
	if(walkSoundTemplate)
	{
		mWalkSound = soundManager->createSound(walkSoundTemplate->getName() + SQLiteDBManager::convertIntToString(mBattlefieldID), walkSoundTemplate->getFileName(), walkSoundTemplate->loop());
		mNode->attachObject(mWalkSound);
	}
	SoundTemplate* jumpSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName("jump");
	if(jumpSoundTemplate)
	{
		mJumpSound = soundManager->createSound(jumpSoundTemplate->getName() + SQLiteDBManager::convertIntToString(mBattlefieldID), jumpSoundTemplate->getFileName(), jumpSoundTemplate->loop());
		mNode->attachObject(mJumpSound);
	}
	SoundTemplate* landSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName("land");
	if(landSoundTemplate)
	{
		mLandSound = soundManager->createSound(landSoundTemplate->getName() + SQLiteDBManager::convertIntToString(mBattlefieldID), landSoundTemplate->getFileName(), landSoundTemplate->loop());
		mNode->attachObject(mLandSound);
	}
	SoundTemplate* turnSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName("turn");
	if(turnSoundTemplate)
	{
		mTurnSound = soundManager->createSound(turnSoundTemplate->getName() + SQLiteDBManager::convertIntToString(mBattlefieldID), turnSoundTemplate->getFileName(), turnSoundTemplate->loop());
		mNode->attachObject(mTurnSound);
	}
	if(mChassisComponent)
		delete mChassisComponent;

	if(mArmorComponent)
		delete mArmorComponent;

	if(mBatteryComponent)
		delete mBatteryComponent;

	if(mDriveComponent)
		delete mDriveComponent;
	
	if(mReactorComponent)
		delete mReactorComponent;

	if(mSensorSystemComponent)
		delete mSensorSystemComponent;

	if(mShieldComponent)
		delete mShieldComponent;

	if(mTargetingSystemComponent)
		delete mTargetingSystemComponent;
	
	std::deque<MCUComponent*>::iterator iter;

	for(iter = mWeaponHardpointComponents.begin(); iter != mWeaponHardpointComponents.end(); )
	{
		if((*iter))
			delete (*iter);
		iter = mWeaponHardpointComponents.erase(iter);
	}

	mArmorComponent = new MCUComponent(mMechanizedCombatUnit->getArmor(),this, ComponentType::CT_ARMOR,mMechanizedCombatUnit->getArmor()->getMaxHealth(), 0, mMechanizedCombatUnit->getArmor());
	mMCUStatus->addComponentStatus(new MCUComponentStatus(mArmorComponent, ComponentType::CT_ARMOR));
	Real armorRemaining(mArmorComponent->getMaxArmor());

	Real armorGiven(mArmorComponent->getMaxArmor() / 5.f);
	mChassisComponent = new MCUComponent(this, ComponentType::CT_CHASSIS, this->getMechanizedCombatUnit()->getMCUTemplate()->getMaxHealth(),armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
	mChassisComponent->setMCUComponentStatus(new MCUComponentStatus(ComponentType::CT_CHASSIS, this->getMechanizedCombatUnit()->getMCUTemplate()->getMaxHealth(), mMechanizedCombatUnit->getMCUTemplate()));
	mMCUStatus->addComponentStatus(mChassisComponent->getMCUComponentStatus());
	armorRemaining-=armorGiven;

	armorGiven = mArmorComponent->getMaxArmor() / 10.f;
	mBatteryComponent = new MCUComponent(mMechanizedCombatUnit->getBattery(), this, ComponentType::CT_BATTERY,armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
	mMCUStatus->addComponentStatus(new MCUComponentStatus(mBatteryComponent, ComponentType::CT_BATTERY));
	armorRemaining-=armorGiven;

	armorGiven = mArmorComponent->getMaxArmor() / 10.f;
	mDriveComponent = new MCUComponent(mMechanizedCombatUnit->getDrive(),this, ComponentType::CT_DRIVE,armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
	mMCUStatus->addComponentStatus(new MCUComponentStatus(mDriveComponent, ComponentType::CT_ARMOR));
	armorRemaining-=armorGiven;

	armorGiven = mArmorComponent->getMaxArmor() / 10.f;
	mReactorComponent = new MCUComponent(mMechanizedCombatUnit->getReactor(),this, ComponentType::CT_REACTOR,armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
	mMCUStatus->addComponentStatus(new MCUComponentStatus(mReactorComponent, ComponentType::CT_REACTOR));
	armorRemaining-=armorGiven;

	armorGiven = mArmorComponent->getMaxArmor() / 10.f;
	mSensorSystemComponent = new MCUComponent(mMechanizedCombatUnit->getSensorSystem(), this, ComponentType::CT_SENSOR_SYSTEM,armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
	mMCUStatus->addComponentStatus(new MCUComponentStatus(mSensorSystemComponent, ComponentType::CT_SENSOR_SYSTEM));
	armorRemaining-=armorGiven;

	armorGiven = mArmorComponent->getMaxArmor() / 10.f;
	mShieldComponent = new MCUComponent(mMechanizedCombatUnit->getShield(),this, ComponentType::CT_SHIELD,armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
	mMCUStatus->addComponentStatus(new MCUComponentStatus(mShieldComponent, ComponentType::CT_SHIELD));
	armorRemaining-=armorGiven;

	armorGiven = mArmorComponent->getMaxArmor() / 10.f;
	mTargetingSystemComponent = new MCUComponent(mMechanizedCombatUnit->getTargetingSystem(),this, ComponentType::CT_TARGETING_SYSTEM,armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
	mMCUStatus->addComponentStatus(new MCUComponentStatus(mTargetingSystemComponent, ComponentType::CT_TARGETING_SYSTEM));
	armorRemaining-=armorGiven;
	
	mAllComponents.push_back(mChassisComponent);
	mAllComponents.push_back(mArmorComponent);
	mAllComponents.push_back(mBatteryComponent);
	mAllComponents.push_back(mDriveComponent);
	mAllComponents.push_back(mReactorComponent);
	mAllComponents.push_back(mSensorSystemComponent);
	mAllComponents.push_back(mShieldComponent);
	mAllComponents.push_back(mTargetingSystemComponent);

	std::vector<WeaponHardpoint*> weapons = mMechanizedCombatUnit->getWeapons();
	std::vector<WeaponHardpoint*>::iterator weaponIterator;
	armorGiven = armorRemaining / (float)(weapons.size());
	for(weaponIterator = weapons.begin(); weaponIterator != weapons.end(); ++weaponIterator)
	{
		if((*weaponIterator))
		{
			(*weaponIterator)->resetAmmoRemaining();
			MCUComponent* weapon = new MCUComponent((*weaponIterator)->getWeapon(), this, (*weaponIterator),armorGiven, mArmorComponent, mMechanizedCombatUnit->getArmor());
			mMCUStatus->addComponentStatus(new MCUComponentStatus(weapon, ComponentType::CT_WEAPON));
			mWeaponHardpointComponents.push_back(weapon);
			mAllComponents.push_back(weapon);
		}
	}
	
	mRemainingBatteryPower = mMechanizedCombatUnit->getBattery()->getMaxEnergy();
	mRemainingReactorPower = mMechanizedCombatUnit->getReactor()->getMaxEnergy();

}

PlayerBattleState* MCUController::getPlayerBattleState()
{
	return mPlayerBattleState;
}

void MCUController::setPlayerBattleState(PlayerBattleState* player)
{
	mPlayerBattleState = player;
}

BattlefieldGridCell* MCUController::getCurrentLocation()
{
	return mCurrentLocation;
}

void MCUController::setCurrentLocation(BattlefieldGridCell* location)
{
	mCurrentLocation = location;
}

Shield* MCUController::getShield()
{
	return mShield;
}

void MCUController::setShield(Shield* shield)
{
	mShield = shield;
}

std::deque<BattlefieldGridCell*>& MCUController::getCurrentPath()
{
	return mCurrentPath;
}

BattlefieldGridCell* MCUController::getCurrentGoal()
{
	if(mCurrentPath.size() == 0)
		return 0;

	return mCurrentPath.back();
}

void MCUController::moveAlongPath()
{
	mMoving = true;
	mOrderedToStop = false;
	mReachedDestination = false;
	mDistanceRemaining = 0;

	
	while(mCurrentPath.size() > 0 && mCurrentPath.front() == mCurrentLocation)
	{
		mCurrentPath.pop_front();
	}
	if(mCurrentPath.size() == 0)
	{
		mMoving = false;
		mReachedDestination = true;
		if(mAIGroup)
		{
			mAIGroup->notifyOfMovementChange(this);
		}
	}
}

bool MCUController::isMoving()
{
	return mMoving;
}

void MCUController::stopMoving(bool blocked, MCUController* occupant, bool notify)
{
	mMoving = false;
	mKinematicCharacterController->setWalkDirection(btVector3(0,0,0));
	mMCUAnimationController->animWalkEnd();

	if(mCurrentPath.size() > 0)
	{
		InBattleState::getSingletonPtr()->renderPath(this, mCurrentPath);
	}
	mOrderedToStop = false;
	if(mAIGroup && notify)
	{
		if(blocked)
			mAIGroup->notifyOfMovementFailureDueToBlockage(this, occupant, mCurrentLocation);
		else
			mAIGroup->notifyOfMovementChange(this);
	}
	if(mWalkSound)
	{
		mWalkSound->stop();
	}
}

void MCUController::giveStopOrder()
{
	mOrderedToStop = true;
}

bool MCUController::lowMovementEnergy()
{
	if(mRemainingReactorPower < mMechanizedCombatUnit->getReactor()->getMaxEnergy() * 0.1f)
		return true;
	return false;
}

void MCUController::setCurrentPath(PathFindingNode* goal)
{
	if(!mActive)
		return;

	if(mDriveComponent->isBroken())
		return;

	if(mMoving)
		return;

	mCurrentPath.clear();

	PathFindingNode* current = goal;
	PathFindingNode* old = 0;
	while(current)
	{
		mCurrentPath.push_front (mBattlefieldGrid->getGridCell((current->getZ() * mBattlefieldGrid->getSideLength()) + current->getX()));

		old = current;
		current = current->getParent();
		delete old;
	}

	mHasPath = true;
}

bool MCUController::hasPath()
{
	return mHasPath;
}

BattlefieldGrid* MCUController::getGrid()
{
	return mBattlefieldGrid;
}

void MCUController::setBattlefieldGrid(BattlefieldGrid* grid)
{
	mBattlefieldGrid = grid;
}

bool MCUController::canFire(int hardpoint)
{
	return canFire(mMechanizedCombatUnit->getWeaponHardpoint(hardpoint));
}

bool MCUController::canFire(WeaponHardpoint* weapon)
{
	if(!mActive)
		return false;

	if(mIsFiring)
		return false;

	std::deque<MCUComponent*>::iterator iter;

	for(iter = mWeaponHardpointComponents.begin(); iter != mWeaponHardpointComponents.end();++iter)
	{
		if((*iter)->getWeaponHardpoint() == weapon && (*iter)->isBroken())
			return false;
	}

	if(weapon->getWeapon()->getEnergyDrain() > this->getTotalRemainingPower())
		return false;

	return weapon->canFire();
}

bool MCUController::canFireAtLeastOneWeapon()
{
	bool atLeastOne = false;

	std::deque<MCUComponent*>::iterator iter;

	for(iter = mWeaponHardpointComponents.begin(); iter != mWeaponHardpointComponents.end();++iter)
	{
		if(!(*iter)->isBroken() && (*iter)->getWeaponHardpoint()->canFire())
		{
			atLeastOne = true;
		}
	}

	return atLeastOne;
}

bool MCUController::canFireUpon(MCUController* target)
{
	if(target->readyToBeDeleted())
		return false;

	float range = mCurrentLocation->getMiddle().distance(target->getCurrentLocation()->getMiddle()) / BattlefieldGrid::UnitSize;

	if(range > getLongestWeaponRange())
	{
		return false;
	}

	std::deque<MCUComponent*>::iterator iter;

	for(iter = mWeaponHardpointComponents.begin(); iter != mWeaponHardpointComponents.end();++iter)
	{
		WeaponTemplate* weapon = (*iter)->getWeaponHardpoint()->getWeapon();
		float weaponRange = (*iter)->getWeaponHardpoint()->getWeapon()->getRange();
		if(!(*iter)->isBroken() &&
			(*iter)->getWeaponHardpoint()->canFire() &&
			range < (weaponRange) &&
			weapon->getEnergyDrain() < this->getTotalRemainingPower() &&
			(weapon->getPiercingDamage() > 0 || !target->shieldsUp() && weapon->getArmorDamage() > 0 || target->shieldsUp() && weapon->getShieldDamage() > 0))
		{
			return true;
		}
	}
	return false;
}

bool MCUController::shieldsUp()
{
	return mShield->isActive();
}

bool MCUController::reachedDestination()
{
	return mReachedDestination;
}

void MCUController::beginFiringWeapon(WeaponHardpoint* weapon, const Ogre::Vector3& source, const Ogre::Vector3& dest, bool returningFire)
{
	if(!mActive)
		return;
	
	mIsFiring = true;

	if((!mCurrentFireInstruction.Initialized && !mCurrentFireInstruction.Executed) || (mCurrentFireInstruction.Initialized && mCurrentFireInstruction.Executed))
	{
		mCurrentFireInstruction = FireInstruction(source, dest, this, weapon, returningFire);
		mRotationToTarget = mNode->getOrientation().zAxis().getRotationTo(dest - source);
	}
}

WeaponHardpoint* MCUController::getBestAntiShieldWeapon()
{
	WeaponHardpoint* result(0);
	float range = rangeToTarget();

	if(range > getLongestWeaponRange())
	{
		return result;
	}

	std::deque<MCUComponent*>::iterator iter;

	for(iter = mWeaponHardpointComponents.begin(); iter != mWeaponHardpointComponents.end();++iter)
	{
		WeaponTemplate* weapon = (*iter)->getWeaponHardpoint()->getWeapon();
		float weaponRange = weapon->getRange();
		if(!(*iter)->isBroken() &&
			(*iter)->getWeaponHardpoint()->canFire() &&
			range < (weaponRange * weaponRange) &&
			weapon->getEnergyDrain() < this->getTotalRemainingPower() &&
			weapon->getShieldDamage() >= weapon->getArmorDamage())
		{
			if(result)
			{
				if(result->getWeapon()->getRange() < weapon->getRange())
					result = (*iter)->getWeaponHardpoint();
				else if(result->getWeapon()->getRange() == weapon->getRange() && result->getWeapon()->getShieldDamage() < weapon->getShieldDamage())
					result = (*iter)->getWeaponHardpoint();
			}
			else
			{
				result = (*iter)->getWeaponHardpoint();
			}
		}
	}
	return result;
}

WeaponHardpoint* MCUController::getBestAntiArmorWeapon()
{
	WeaponHardpoint* result(0);
	float range = rangeToTarget();

	if(range > getLongestWeaponRange())
	{
		return result;
	}

	std::deque<MCUComponent*>::iterator iter;

	for(iter = mWeaponHardpointComponents.begin(); iter != mWeaponHardpointComponents.end();++iter)
	{
		WeaponTemplate* weapon = (*iter)->getWeaponHardpoint()->getWeapon();
		float weaponRange = weapon->getRange();
		if(!(*iter)->isBroken() &&
			(*iter)->getWeaponHardpoint()->canFire() &&
			range < (weaponRange * weaponRange) &&
			weapon->getEnergyDrain() < this->getTotalRemainingPower() &&
			weapon->getShieldDamage() <= weapon->getArmorDamage())
		{
			if(result)
			{
				if(result->getWeapon()->getRange() < weapon->getRange())
					result = (*iter)->getWeaponHardpoint();
				else if(result->getWeapon()->getRange() == weapon->getRange() && result->getWeapon()->getArmorDamage() < weapon->getArmorDamage())
					result = (*iter)->getWeaponHardpoint();
			}
			else
			{
				result = (*iter)->getWeaponHardpoint();
			}
		}
	}
	return result;
}
	
bool MCUController::isFiring()
{
	return mIsFiring;
}

void MCUController::weaponFiringComplete()
{
	mIsFiring = false;
	if(mAIGroup && mAIGroup->hasAttackOrder())
	{
		mAIGroup->getGroupAttackOrder()->setOrderStatus(this, OrderStatus::OS_STARTED);
		mAIGroup->executeGroupAttackOrder();
	}
}

void MCUController::executeCurrentFiringInstruction()
{
	mMCUAnimationController->startFiringAnimation(mCurrentFireInstruction.WeaponHardpoint->getHardpointNumber());
}

void MCUController::fireWeapon(int weaponHardpoint)
{
	if(mCurrentFireInstruction.WeaponHardpoint->getHardpointNumber() == weaponHardpoint)
	{
		mCurrentFireInstruction.WeaponHardpoint->fire(this, mCurrentFireInstruction);
		updateGuiData("fire weapon");
		mCurrentFireInstruction.Executed = true;
	}
}

void MCUController::applyWarhead(int projId, const FireInstruction& instruction)
{
	if(!mActive)
		return;

	
	Warhead warhead(projId, instruction.WeaponHardpoint->getWeapon());
	WeaponTemplate* weapon = instruction.WeaponHardpoint->getWeapon();
	

	mShield->applyWarhead(warhead);
	mLastDamagedBy = instruction.FiredBy;
	if(!instruction.ReturningFire)
	{
		setCurrentTarget(mLastDamagedBy);
		if(mAIGroup)
		{
			GameManager::getSingletonPtr()->luaDebugReport("AI returning fire",0);
		}
		else
		{
			GameManager::getSingletonPtr()->luaDebugReport("Player returning fire",0);
		}
		
		LuaStateManager::getSingletonPtr()->returnFire(InBattleState::getSingletonPtr(), this);
	}

	if(warhead.RemainingShieldDamage < warhead.TotalShieldDamage && warhead.TotalShieldDamage > 0)
	{
		warhead.RemainingArmorDamage -= warhead.TotalArmorDamage * ((warhead.TotalShieldDamage - warhead.RemainingShieldDamage) / warhead.TotalShieldDamage);
	}

	if(warhead.ShieldWasActive && warhead.RemainingPiercingDamage > 0)
	{
		MCUComponent* target(0);
		
		float remainingDamage(warhead.RemainingPiercingDamage);
		while(remainingDamage > 0 && !mChassisComponent->isBroken() && !mReactorComponent->isBroken())
		{
			target = mAllComponents[rand() % mAllComponents.size()];
			if(target->isBroken() || target->getComponentType() == ComponentType::CT_ARMOR)
				continue;
			remainingDamage = target->applyDamage(remainingDamage, warhead.DamageType, instruction.FiredBy);
			
		}
		mLastDamagedBy = instruction.FiredBy;
	}
	else if(!warhead.ShieldWasActive && warhead.RemainingArmorDamage > 0)
	{
		
		MCUComponent* target = mAllComponents[rand() % mAllComponents.size()];
		
		float remainingDamage(warhead.RemainingArmorDamage);
		while(remainingDamage > 0 && !mChassisComponent->isBroken() && !mReactorComponent->isBroken())
		{
			if(target->getComponentType() == ComponentType::CT_ARMOR || target->isBroken())
			{
				int attempts = 0;
				
				while((target->getComponentType() == ComponentType::CT_ARMOR || target->isBroken()) && attempts < mAllComponents.size())
				{
					target = mAllComponents[rand() % mAllComponents.size()];
					++attempts;
				}

				if(target->getComponentType() == ComponentType::CT_ARMOR || target->isBroken())
				{
					break;
				}
			}

			remainingDamage = target->applyDamage(remainingDamage, warhead.DamageType, instruction.FiredBy);
			target = mAllComponents[rand() % mAllComponents.size()];
		}
		
		
	}
	if(mChassisComponent->isBroken() || mReactorComponent->isBroken())
	{
		this->startDestructionSequence();
	}
}

void MCUController::update(const Real& timeSinceLastFrame)
{
	mMCUAnimationController->update(timeSinceLastFrame);
	if(!mActive)
		return;
	
	btTransform transform = mKinematicCharacterController->getGhostObject()->getWorldTransform();
	mShield->move(transform);
	btQuaternion rot = transform.getRotation();
	mPosition = BtOgre::Convert::toOgre(transform.getOrigin());
	
	Vector3 tmp = Vector3(mPosition);
	tmp.y -= (mHeight / 2.f);
	//tmp.y += 10.f;
	//mNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	mNode->setPosition(tmp);

	if(mCurrentFireInstruction.Initialized && !mCurrentFireInstruction.Executed)
	{
		mRotationToTarget = mNode->getOrientation().zAxis().getRotationTo(mCurrentFireInstruction.Destination - mCurrentFireInstruction.Source);
		Real yawToGoal = mRotationToTarget.getYaw().valueDegrees();
		
		if(Math::Abs(yawToGoal) < 2)
			mTurnToFire = false;
		else
			mTurnToFire = true;

		if(mTurnToFire)
		{
			// this is how much the character CAN turn this frame
			Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * timeSinceLastFrame * 500.f;

			// turn as much as we can, but not more than we need to
			if(Math::Abs(yawToGoal) > 175)
			{
				if (yawToGoal < 0)
					yawToGoal = -45;
				else if (yawToGoal > 0)
					yawToGoal = 45;
			}
			else
			{
				if (yawToGoal < 0)
					yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
				else if (yawToGoal > 0)
					yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));
			}

			mNode->yaw(Degree(yawToGoal));
		}
		else
		{
			executeCurrentFiringInstruction();
		}
		return;
	}

	if(!mMoving)
		return;

	
	if(mDriveComponent->isBroken())
	{
		stopMoving();
		return;
	}
	
	BattlefieldGridCell* destination(0);
	bool requiresJump(false);

	if(mIsJumping)
	{
		if(mMCUAnimationController->isTakingOff())
		{
			return;
		}
	}
	if(mLanding)
	{
		if(!mMCUAnimationController->isLanding())
		{
			mIsJumping = false;
			mJumped = false;
			mLanding = false;
			mKinematicCharacterController->warp(BtOgre::Convert::toBullet(mTarget));
			transform = mKinematicCharacterController->getGhostObject()->getWorldTransform();
			mShield->move(transform);
			rot = transform.getRotation();
			mPosition = BtOgre::Convert::toOgre(transform.getOrigin());
	
			tmp = Vector3(mPosition);
			tmp.y -= (mHeight / 2.f);
			//tmp.y += 10.f;
			//mNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
			mNode->setPosition(tmp);
			mDistanceRemaining = 0;
		}
	}

	if(mDistanceRemaining < 0.001f)
	{
		bool changeMade(false);
		if(mPosition.x < 0)
		{
			mPosition.x = BattlefieldGrid::UnitSize / 2;
			changeMade = true;
		}
		if(mPosition.x > mBattlefieldGrid->getSideLength() * BattlefieldGrid::UnitSize)
		{
			mPosition.x = (mBattlefieldGrid->getSideLength() * BattlefieldGrid::UnitSize) - (BattlefieldGrid::UnitSize / 2);
			changeMade = true;
		}
		if(mPosition.z < 0)
		{
			mPosition.z = BattlefieldGrid::UnitSize / 2;
			changeMade = true;
		}
		if(mPosition.z > mBattlefieldGrid->getSideLength() * BattlefieldGrid::UnitSize)
		{
			mPosition.z = (mBattlefieldGrid->getSideLength() * BattlefieldGrid::UnitSize) - (BattlefieldGrid::UnitSize / 2);
			changeMade = true;
		}
		
		BattlefieldGridCell* nextCell = mBattlefieldGrid->getGridCell(mPosition.x, mPosition.z);
		if(changeMade)
		{
			mKinematicCharacterController->warp(BtOgre::Convert::toBullet(nextCell->getMiddle()));
		}

		if(mIsJumping && mMCUAnimationController->isInAir() && mJumped && ((nextCell->getMiddleFloor().y - mNode->getPosition().y) < (BattlefieldGrid::UnitSize / 5)))
		{
			mMCUAnimationController->animJumpEnd();
			mLanding = true;
			mIsJumping = false;
			return;
		}

		InBattleState::getSingletonPtr()->updateMCUVisibility();
		updateGuiData("movement");

		if(!mMoving)
		{
			//we might have stopped because we have seen an enemy.
			return;
		}

		if(nextCell != mCurrentLocation)
		{
			if(nextCell->getOccupant() && nextCell->getOccupant() != this)
			{
				mReachedDestination = false;
				
				stopMoving(true, nextCell->getOccupant());
				
				return;

			}
			mCurrentLocation->leaveCell();
			mCurrentLocation = nextCell;
			mCurrentLocation->occupyCell(this);
		}

		mNeedsFogUpdate = true;
		updateFogOfWar();

		if(mOrderedToStop)
		{
			mReachedDestination = false;
			stopMoving();
			return;
		}
		//GameManager::getSingletonPtr()->updateDebugText("Current X: " + SQLiteDBManager::convertIntToString(mCurrentLocation->getX()) + " Z: " + SQLiteDBManager::convertIntToString(mCurrentLocation->getZ()));

		float thisSquare(0);
		if(mCurrentPath.size() > 0)
		{
			if(mCurrentPath.front()->isOccupied() && mCurrentPath.front()->getOccupant() != this)
			{
				stopMoving();
				
				mReachedDestination = false;
				if(mAIGroup)
				{
					//mAIGroup->notifyOfMovementChange(this);
					mAIGroup->notifyOfMovementFailureDueToBlockage(this, nextCell->getOccupant(), mCurrentPath.back());
				}
				return;
			}

			
			thisSquare = this->getMechanizedCombatUnit()->getMovementCostPerSquare() * mCurrentPath.front()->getMovementCost() * InBattleState::getSingletonPtr()->getBattlefieldGrid()->getTerrainTemplate()->getGravity();
			
			if(mCurrentLocation->getX() != mCurrentPath.front()->getX() && mCurrentLocation->getZ() != mCurrentPath.front()->getZ())
			{
				//diagonal movement
				thisSquare*=1.414f;
			}
			if(mCurrentLocation->requiresJump(mCurrentPath.front()))
			{
				mIsJumping = true;
				mJumped = false;
				mLanding = false;
				if(mCurrentLocation->getY() > mCurrentPath.front()->getY())
				{
					mIsJumpingDown = true;
				}
				else
				{
					mIsJumpingDown = false;
				}
				mKinematicCharacterController->setWalkDirection(btVector3(0,0,0));
				thisSquare*=mMechanizedCombatUnit->getDrive()->getJumpingEfficiency();
			}
		}
		
		if((mCurrentPath.size() == 0) || mCurrentPath.begin() == mCurrentPath.end() || thisSquare > this->getRemainingReactorPower())
		{
			if(mCurrentPath.size() > 0)
			{
				if(mCurrentPath.back() == mCurrentLocation)
					mReachedDestination = true;
				else
					mReachedDestination = false;
			}
			else if(mCurrentPath.size() == 0)
			{
				mReachedDestination = true;
			}

			stopMoving();
			
			
			return;
		}
		
		mTarget = mCurrentPath.front()->getMiddle();
		if(mIsJumping)
		{
			if(mIsJumpingDown)
				mTarget.y += BattlefieldGrid::UnitSize;
			else
				mTarget.y -= BattlefieldGrid::UnitSize;
		}
		destination = mCurrentPath.front();

		//mTarget.y -= (mHeight / 2.f);
		mCurrentPath.pop_front();
		this->drainEnergyFromReactor(thisSquare);
		mDirection = mTarget - mPosition;
		mDistanceRemaining = mDirection.normalise();
		mMCUAnimationController->animWalkStart();
		if(this->isVisible() && mWalkSound)
		{
			mWalkSound->play();
		}
	}
	
	Quaternion toGoal = mNode->getOrientation().zAxis().getRotationTo(mDirection);
	
	
	// calculate how much the character has to turn to face goal direction
	Real yawToGoal = toGoal.getYaw().valueDegrees();
		
	if(Math::Abs( yawToGoal) < 2)
	{
		mTurning = false;
		mStartedTurning = false;
	}
	else
	{
		mTurning = true;
	}
	
	if(mTurning)
	{
		mKinematicCharacterController->setWalkDirection(btVector3(0,0,0));
		// this is how much the character CAN turn this frame
		Real yawAtSpeed(0);
		if(!mIsJumping)
			yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * timeSinceLastFrame * 600.f;
		else
			yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * timeSinceLastFrame * 800;

		// turn as much as we can, but not more than we need to
		
		if(Math::Abs(yawToGoal) > 170)
		{
			if (yawToGoal < 0)
				yawToGoal = -45;
			else if (yawToGoal > 0)
				yawToGoal = 45;
		}
		else
		{
			if (yawToGoal < 0)
				yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
			else if (yawToGoal > 0)
				yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));
		}
		mNode->yaw(Degree(yawToGoal));
		toGoal = mNode->getOrientation().zAxis().getRotationTo(mDirection);
		Real yawToGoal2 = toGoal.getYaw().valueDegrees();
		if(abs(yawToGoal - yawToGoal2) < 0.01)
		{
			int temp = 0;
		}
	}
	
	if(!mTurning)
	{
		if(mIsJumping)
		{
			if(!mMCUAnimationController->isTakingOff() && !mMCUAnimationController->isInAir() && !mJumped)
			{
				if(mIsJumping)
				{
					if(mIsJumpingDown)
						mTarget.y -= BattlefieldGrid::UnitSize;
					else
						mTarget.y += BattlefieldGrid::UnitSize;
				}
				mDirection = mTarget - mPosition;
				mDistanceRemaining = mDirection.normalise();

				mMCUAnimationController->animJumpStart();
				mIsJumping = true;
				mKinematicCharacterController->setWalkDirection(btVector3(0,0,0));
				if(mIsJumpingDown)
				{
					mKinematicCharacterController->setJumpSpeed(10);
					mKinematicCharacterController->setFallSpeed(125);
				}
				else
				{
					mKinematicCharacterController->setJumpSpeed(22);
					mKinematicCharacterController->setFallSpeed(80);
				}
				//mKinematicCharacterController->setJumpSpeed(mMovementSpeed * 0.65);
			}
			else if(mMCUAnimationController->isInAir())
			{
				mDistanceRemaining-= timeSinceLastFrame * mMovementSpeed * 0.3;
				
				if(!mJumped)
				{
					mKinematicCharacterController->jump();
					if(mIsJumpingDown)
					{
						mKinematicCharacterController->setWalkDirection(BtOgre::Convert::toBullet((mDirection) * timeSinceLastFrame * mMovementSpeed * 0.28));
					}
					else
					{
						mKinematicCharacterController->setWalkDirection(BtOgre::Convert::toBullet((mDirection) * timeSinceLastFrame * mMovementSpeed * 0.35));
					}
					mJumped = true;
				}
			}
			
		}
		else
		{
			mDistanceRemaining-= timeSinceLastFrame * mMovementSpeed * mAISpeedBoost;
			Sequence+=timeSinceLastFrame;
			mKinematicCharacterController->updateSequence(timeSinceLastFrame);
			mKinematicCharacterController->setWalkDirection(BtOgre::Convert::toBullet(mDirection * timeSinceLastFrame * mMovementSpeed * mAISpeedBoost));
		}
	}

	
}

void MCUController::updateFogOfWar()
{
	if(mNeedsFogUpdate)
	{
		std::deque<FogSquare*> squares;

		std::deque<Vector3>::iterator iter;
		PhysicsManager* pm = PhysicsManager::getSingletonPtr();
		int currentX = this->getMiddle().x / BattlefieldGrid::UnitSize;
		int currentZ = this->getMiddle().z / BattlefieldGrid::UnitSize;

		for(iter = mFogRayTestDirections.begin(); iter != mFogRayTestDirections.end(); ++iter)
		{
			Vector3 endpoint;
			if(mSensorActive)
				endpoint = this->getMiddle() + (*iter) * this->getMechanizedCombatUnit()->getSensorSystem()->getSightRange() * BattlefieldGrid::UnitSize;
			else
				endpoint = this->getMiddle() + (*iter) * BattlefieldGrid::UnitSize;

			TerrainRayTestResultCallback callback = pm->terrainRayTest(BtOgre::Convert::toBullet(this->getMiddle()), BtOgre::Convert::toBullet(endpoint));
		
			if(callback.hasHit() && callback.hitTerrain())
			{
				endpoint = BtOgre::Convert::toOgre(callback.m_hitPointWorld);
				endpoint*= 0.975f;
			}

			int x = endpoint.x / BattlefieldGrid::UnitSize;
			int z = endpoint.z / BattlefieldGrid::UnitSize;

			if(x < 0) {x = 0;}
			if(z < 0) {z = 0;}
		
			FogSquare* square = new FogSquare();
			square->X = x;
			square->Z = z;
			square->State = VisibilityState::VS_COMPLETE;
			squares.push_back(square);
		}

		std::deque<FogSquare*> boundary;
		std::deque<FogSquare*>::iterator current;
		std::deque<FogSquare*>::iterator next;

		//clone the first element and insert it at the end to make the loop below consider all possible combinations
		//the clone is eliminated later on.
		squares.insert(squares.end(), squares.front());
		current = squares.begin();
		next = squares.begin();
		++next;

		while(next != squares.end())
		{
			int dx = abs( (*next)->X - (*current)->X );
			int dz = abs( (*next)->Z - (*current)->Z );
			int sx(0), sz(0);

			if((*current)->X < (*next)->X)
				sx = 1;
			else if ((*current)->X > (*next)->X)
				sx = -1;

			if((*current)->Z < (*next)->Z)
				sz = 1;
			else if ((*current)->Z > (*next)->Z)
				sz = -1;

			if(sx == 0 && sz == 0)
			{
				++current;
				++next;
				continue;
			}

			int cX(0), cZ(0), e2(0);
			cX = (*current)->X;
			cZ = (*current)->Z;

			if(sx == 0)
			{
				cZ+=sz;
				while(cZ != (*next)->Z)
				{
					FogSquare* square =  new FogSquare();
					square->X = cX;
					square->Z = cZ;
					square->State = VisibilityState::VS_COMPLETE;
					boundary.push_back(square);
					cZ +=sz;
				}
			
				++current;
				++next;
				continue;
			}

			if(sz == 0)
			{
				cX+=sx;
				while(cX != (*next)->X)
				{
					FogSquare* square =  new FogSquare();
					square->X = cX;
					square->Z = cZ;
					square->State = VisibilityState::VS_COMPLETE;
					boundary.push_back(square);
					cX +=sx;
				}
				++current;
				++next;
				continue;
			}
		
			int err = dx - dz;
			while(cX != (*next)->X && cZ != (*next)->Z && cX >= 0 && cZ >= 0)
			{
				e2 = 2 * err;
				if(e2 > -dz)
				{
					err-=dz;
					cX+=sx;
				}
				if(e2 < dx)
				{
					err+=dx;
					cZ+=sz;
				}

				FogSquare* square =  new FogSquare();
				square->X = cX;
				square->Z = cZ;
				square->State = VisibilityState::VS_COMPLETE;
				boundary.push_back(square);
			}
			++current;
			++next;
		}

		squares.insert(squares.begin(), boundary.begin(), boundary.end());

		if(mFogSquareGrid)
		{
			delete mFogSquareGrid;
			mFogSquareGrid = 0;
		}
		mFogSquareGrid = new FogSquareGrid();
		mFogSquareGrid->insert(squares);
		mFogSquareGrid->fillGaps();

		mNeedsFogUpdate = false;
		mNeedsFogRender = true;
	}
}

void MCUController::renderFogOfWar()
{
	if(!mNeedsFogRender)
		return;

	if(!mInitializedFogOfWar)
	{
		SceneManager* sm = GameManager::getSingletonPtr()->getFogSceneManager();
		mFogSceneNode = sm->createSceneNode("MCU" + SQLiteDBManager::convertIntToString(mBattlefieldID) + "FogSceneNode");
		mFogManualObject = sm->createManualObject("MCU" + SQLiteDBManager::convertIntToString(mBattlefieldID) + "FogManualObject");
		mFogManualObject->setDynamic(true);
	
		mFogManualObject->begin("fogsquare", RenderOperation::OT_TRIANGLE_LIST);
	}
	else
	{
		mFogManualObject->beginUpdate(0);
	}

	int numSquares = 0;
	while(mFogSquareGrid->hasMoreFogSquares())
	{
		FogSquare* square = mFogSquareGrid->getNext();
		
		mFogManualObject->position(square->X * BattlefieldGrid::UnitSize, 0, square->Z * BattlefieldGrid::UnitSize);
		mFogManualObject->colour(1,1,1);

		mFogManualObject->position((square->X + 1) * BattlefieldGrid::UnitSize, 0, square->Z * BattlefieldGrid::UnitSize);
		mFogManualObject->colour(1,1,1);

		mFogManualObject->position(square->X * BattlefieldGrid::UnitSize, 0, (square->Z + 1) * BattlefieldGrid::UnitSize);
		mFogManualObject->colour(1,1,1);

		mFogManualObject->position((square->X + 1) * BattlefieldGrid::UnitSize, 0, (square->Z + 1) * BattlefieldGrid::UnitSize);
		mFogManualObject->colour(1,1,1);
		++numSquares;

	}

	for(int i = 0; i < numSquares;++i)
	{
		int pos = i * 4;
		mFogManualObject->triangle(pos, pos + 2, pos + 1);
		mFogManualObject->triangle(pos + 2, pos + 3, pos + 1);
	}

	mFogManualObject->end();
	mFogSquareGrid->reset();
	mNeedsFogRender = false;

	if(!mInitializedFogOfWar)
	{
		SceneManager* sm = GameManager::getSingletonPtr()->getFogSceneManager();
		mFogSceneNode->attachObject(mFogManualObject);
		sm->getRootSceneNode()->addChild(mFogSceneNode);
		mInitializedFogOfWar = true;
	}
}

bool MCUController::canSee(MCUController* target)
{
	return mFogSquareGrid->isInside(target->getCurrentLocation()->getX(), target->getCurrentLocation()->getZ());
}

float MCUController::getShortestWeaponRange()
{
	float shortestRange(1000.0f);

	std::vector<WeaponHardpoint*> weapons = mMechanizedCombatUnit->getWeapons();
	std::vector<WeaponHardpoint*>::iterator iter;

	for(iter = weapons.begin(); iter != weapons.end(); ++iter)
	{
		if((*iter))
		{
			if((*iter)->getWeapon()->getRange() < shortestRange)
				shortestRange = (*iter)->getWeapon()->getRange();
		}
	}

	return shortestRange;
}

float MCUController::getLongestWeaponRange()
{
	float longestRange(0.0f);

	std::vector<WeaponHardpoint*> weapons = mMechanizedCombatUnit->getWeapons();
	std::vector<WeaponHardpoint*>::iterator iter;

	for(iter = weapons.begin(); iter != weapons.end(); ++iter)
	{
		if((*iter))
		{
			if((*iter)->getWeapon()->getRange() > longestRange)
				longestRange = (*iter)->getWeapon()->getRange();
		}
	}

	return longestRange;
}

float MCUController::remainingHealthPercent()
{
	float maxHealth(0);
	float currentHealth(0);

	std::deque<MCUComponent*>::iterator iter;
	for(iter = mAllComponents.begin(); iter != mAllComponents.end(); ++iter)
	{
		maxHealth += (*iter)->getMaxHealth();
		currentHealth += (*iter)->getHealthRemaining();
	}
	if(maxHealth != 0)
	{
		return (currentHealth / maxHealth) * 100;
	}
	return 0;
}

bool MCUController::sensorActive()
{
	return mSensorActive;
}

bool MCUController::areEqual(Ogre::Quaternion q1, Ogre::Quaternion q2, float marginOfError)
{
	 return (abs(q1.w - q2.w) < marginOfError && abs(q1.x - q2.x) < marginOfError && abs(q1.y - q2.y) < marginOfError && abs(q1.z - q2.z) < marginOfError);
}

float MCUController::calculateAccuracy(WeaponHardpoint* weapon)
{
	if(!weapon->canFire())
		return 0.f;

	if(!mCurrentTarget)
		return 0.f;

	float range = this->rangeToTarget();
	
	if(range > weapon->getWeapon()->getRange())
		return 0.f;

	Vector3 origin = this->getMiddle();
	
	float coverage = calculateCoverage(origin);
	
	TargetingSystemTemplate* system = this->getMechanizedCombatUnit()->getTargetingSystem();

	float rangeTerm = (range / weapon->getWeapon()->getRange());
	//Ranged accuracy decrease represents the decrease in accuracy as range decreases. Min = 0.5 Max = 1.0. High values are better, meaning smaller values for rangeTerm are better.
	float rangedAccuracyDecrease = (((1 - rangeTerm) / 2) + 0.5f);

	if(coverage <= 0.15f)
	{
		if(weapon->getWeapon()->isIndirectFireCapable())
		{
			return std::min(1.f,system->getBaseAccuracy() * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
		}
		else
		{
			return 0.f;
		}
	}
	else if(coverage > 0.15f && coverage < 0.5f)
	{
		if(weapon->getWeapon()->isIndirectFireCapable())
		{
			return std::min (1.f,system->getBaseAccuracy() * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
		}
		else
		{
			return std::min(1.f, system->getBaseAccuracy() * coverage * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
		}
	}
	else
	{
		return std::min(1.f, system->getBaseAccuracy() * coverage * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
	}
}

void MCUController::playJumpSound()
{

	if(this->isVisible() && mJumpSound)
	{
		mJumpSound->play();
	}
}

void MCUController::playLandSound()
{
	
	if(this->isVisible() && mLandSound)
	{
		mLandSound->play();
	}
}

float MCUController::calculateCoverage(const Ogre::Vector3& origin)
{
	Vector3 dest = mCurrentTarget->getMiddle();

	int numCanSee(0), maxCanSee(0);
	
	/*
	if(mCoverageDebugger)
		mSceneManager->destroyManualObject(mCoverageDebugger);

	mCoverageDebugger = mSceneManager->createManualObject("coveragedebugger");
	mCoverageDebugger->begin("BaseWhiteNoMaterial",Ogre::RenderOperation::OT_LINE_LIST);
	*/

	Skeleton::BoneIterator iter = mCurrentTarget->getEntity()->getSkeleton()->getBoneIterator();

	while(iter.hasMoreElements())
	{
		Bone* bone = iter.getNext();
		
		dest = mCurrentTarget->getNode()->convertLocalToWorldPosition( bone->getPosition() );
		dest.y+= mCurrentTarget->getEntity()->getBoundingBox().getSize().y / 2.f;
		//mCoverageDebugger->position(origin);
		//mCoverageDebugger->position(dest);
		numCanSee+= canSee(origin, dest) ? 1 : 0;
		++maxCanSee;

	}

	//mCoverageDebugger->end();
	//mSceneManager->getRootSceneNode()->attachObject(mCoverageDebugger);

	return numCanSee / (maxCanSee * 1.0f);
}

bool MCUController::canSee(const Ogre::Vector3& origin, const Ogre::Vector3& dest)
{
	CollisionObjectMapping* mapping = PhysicsManager::getSingletonPtr()->closestNotMeRayTestCallback(this->getKinematicCharacterController()->getGhostObject(),BtOgre::Convert::toBullet(origin), BtOgre::Convert::toBullet(dest)).getCollisionObjectMapping();

	if(mapping)
	{
		if(mapping->MainMovableObject == mCurrentTarget->getEntity() || mapping->ShieldMovableObject == mCurrentTarget->getShield()->getEntity())
		{
			
			return true;
		}
		
		
		return false;
	}

	return false;
}

void MCUController::enableAnimations()
{
	mMCUAnimationController->setEnabled(true);
}

bool MCUController::hasTarget()
{
	return mCurrentTarget != 0;
}

float MCUController::rangeToTarget()
{
	if(mCurrentTarget)
	{
		return mCurrentLocation->getMiddle().distance(mCurrentTarget->getCurrentLocation()->getMiddle()) / BattlefieldGrid::UnitSize;
	}

	return 0.f;
}

MCUController* MCUController::getCurrentTarget()
{
	return mCurrentTarget;
}

void MCUController::setCurrentTarget(MCUController* target)
{
	if(mCurrentTarget)
		mCurrentTarget->noLongerTargetedBy(this);

	mCurrentTarget = target;
	mCurrentTarget->targetedBy(this);
	updateGuiData("set current target");
}

void MCUController::targetedBy(MCUController* source)
{
	mTargetedBy.push_back(source);
}

void MCUController::noLongerTargetedBy(MCUController* source)
{
	std::deque<MCUController*>::iterator iter;
	for(iter = mTargetedBy.begin(); iter != mTargetedBy.end(); ++iter)
	{
		if((*iter) == source)
		{
			iter = mTargetedBy.erase(iter);
			return;
		}
	}
}

void MCUController::targetDestroyed()
{
	if(InBattleState::getSingletonPtr()->mcuHasTargetBillboard(mCurrentTarget))
	{
		InBattleState::getSingletonPtr()->hideTargetSelectBillboard();
	}

	if(mAIGroup)
	{
		mAIGroup->finishedGroupAttackOrder();
	}
	mCurrentTarget = 0;

	
}

void MCUController::reportKill()
{
	if(mPlayerBattleState)
	{
		PlayerResult* result = InBattleState::getSingletonPtr()->getBattleResult()->getPlayerResult(mPlayerBattleState->getPlayerProfile());

		if(result)
		{
			result->addKill();
		}
	}
	
}

AIGroup* MCUController::getAIGroup()
{
	return mAIGroup;
}

void MCUController::setAIGroup(AIGroup* group)
{
	mAIGroup = group;
	if(mAIGroup)
		mAISpeedBoost = 2;
}

int MCUController::getBattlefieldID()
{
	return mBattlefieldID;
}

void MCUController::setBattlefieldID(int id)
{
	mBattlefieldID = id;
}

bool MCUController::isActive()
{
	return mActive;
}

void MCUController::setActive(bool value)
{
	mActive = value;
}

bool MCUController::isVisible()
{
	return mEntity->getVisible();
}

void MCUController::setVisible(bool value)
{
	mEntity->setVisible(value);
	
	if(value)
	{
		if(InBattleState::getSingletonPtr()->mcuHasTargetBillboard(this))
		{
			InBattleState::getSingletonPtr()->showTargetSelectBillboard();
		}
		mRadarBillBoard->setVisible(!value);
	}
	else
	{
		if(InBattleState::getSingletonPtr()->mcuHasTargetBillboard(this))
		{
			InBattleState::getSingletonPtr()->hideTargetSelectBillboard();
		}
	}
}

bool MCUController::isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance)
{
	Vector3 loc = mNode->convertLocalToWorldPosition(mEntity->getBoundingBox().getCenter());
	return point.squaredDistance(loc) < squaredDistance;
}

bool MCUController::isRadarVisible()
{
	return mRadarBillBoard->isVisible();
}

void MCUController::setRadarVisible(bool value)
{
	mRadarBillBoard->setVisible(value);

	if(value)
		mEntity->setVisible(!value);
}

bool MCUController::isPlayerMCU()
{
	return mIsPlayerMCU;
}

void MCUController::setPlayerMCU(bool value)
{
	mIsPlayerMCU = value;
}

Ogre::Bone* MCUController::getFiringBone(WeaponHardpoint* weapon)
{
	return mEntity->getSkeleton()->getBone(weapon->getWeaponHardpointTemplate()->getFiringBoneName());
}

Ogre::Vector3 MCUController::getFiringSourcePosition(WeaponHardpoint* weapon)
{
	return mNode->convertLocalToWorldPosition(weapon->getWeaponHardpointTemplate()->getBarrelOffset());	
}

Vector3 MCUController::getPosition()
{
	return mNode->getPosition();
}

Ogre::Vector3 MCUController::getMiddle()
{
	return mNode->convertLocalToWorldPosition(mEntity->getBoundingBox().getCenter());
	//return mEntity->getWorldBoundingBox().getCenter();
}

Ogre::SceneNode* MCUController::getNode()
{
	return mNode;
}


float MCUController::getRemainingReactorPower()
{
	return mRemainingReactorPower;
}


float MCUController::getRemainingBatteryPower()
{
	return mRemainingBatteryPower;
}

float MCUController::getTotalRemainingPower()
{
	return mRemainingBatteryPower + mRemainingReactorPower;
}

void MCUController::drainEnergyFromBattery(float amount)
{
	mRemainingBatteryPower-=amount;
}

void MCUController::drainEnergyFromReactor(float amount)
{
	mRemainingReactorPower-=amount;

	if(mRemainingReactorPower < 0 && mRemainingBatteryPower > (-1 * mRemainingReactorPower))
	{
		mRemainingBatteryPower+=mRemainingReactorPower;
		mRemainingReactorPower = 0;
	}
}

void MCUController::drainWeaponEnergy(float amount)
{
	mRemainingBatteryPower-=amount;

	if(mRemainingBatteryPower < 0 && mRemainingReactorPower > (-1 * mRemainingBatteryPower))
	{
		mRemainingReactorPower+=mRemainingBatteryPower;
		mRemainingBatteryPower = 0;
	}
}

void MCUController::componentDestroyed(MCUComponent* component)
{
	ComponentType destroyed = component->getComponentType();

	switch(destroyed)
	{
	case ComponentType::CT_CHASSIS:
		startDestructionSequence();
		break;
	case ComponentType::CT_ARMOR:
		break;
	case ComponentType::CT_BATTERY:
		mRemainingBatteryPower = 0;
		break;
	case ComponentType::CT_DRIVE:

		break;
	case ComponentType::CT_REACTOR:
		startDestructionSequence();
		break;
	case ComponentType::CT_SENSOR_SYSTEM:
		mSensorActive = false;
		mNeedsFogUpdate = true;
		updateFogOfWar();
		break;

	case ComponentType::CT_SHIELD:
		mShield->setCurrentHealth(0);
		break;
	case ComponentType::CT_TARGETING_SYSTEM:
		break;
	case ComponentType::CT_WEAPON:
		break;
	}
}

void MCUController::startDestructionSequence()
{
	mMCUAnimationController->animDie();
}

void MCUController::destroy()
{
	mActive = false;
	if(mLastDamagedBy)
	{
		mLastDamagedBy->reportKill();
	}

	std::deque<MCUController*>::iterator iter;
	for(iter = mTargetedBy.begin(); iter != mTargetedBy.end(); ++iter)
	{
		(*iter)->targetDestroyed();
	}

	if(mCurrentTarget)
	{
		mCurrentTarget->noLongerTargetedBy(this);
		mCurrentTarget = 0;
	}
	//InBattleState::getSingletonPtr()->removeMCU(this);
	if(mAIGroup)
	{
		mAIGroup->removeMCU(this);
		mAIGroup = 0;
	}
	InBattleState::getSingletonPtr()->getBattleResult()->getPlayerResult(mPlayerBattleState->getPlayerProfile())->getMCUStatus(mMechanizedCombatUnit)->setMCUCondition(MCUCondition::MC_DESTROYED);
	//mReadyToBeRemoved = true;
	mReadyToBeDeleted = true;
	
	mPlayerBattleState->removeMCU(this);
}

std::deque<MCUComponent*>& MCUController::getAllComponents()
{
	return mAllComponents;
}

std::hash_map<std::string,std::string> MCUController::getGuiValues()
{
	std::hash_map<std::string,std::string> result;

	result[FieldNames::MCU::MCUName] = mMechanizedCombatUnit->getName();
	result[FieldNames::MCU::MCUTemplateName] = mMechanizedCombatUnit->getMCUTemplate()->getName();
	result[FieldNames::MCU::MaxBatteryPower] =  SQLiteDBManager::convertFloatToString( this->getMechanizedCombatUnit()->getBattery()->getMaxEnergy() );
	result[FieldNames::MCU::RemainingBatteryPower] =  SQLiteDBManager::convertFloatToString( this->getRemainingBatteryPower() );
	result[FieldNames::MCU::MaxReactorPower] =  SQLiteDBManager::convertFloatToString( this->getMechanizedCombatUnit()->getReactor()->getMaxEnergy() );
	result[FieldNames::MCU::RemainingReactorPower] = SQLiteDBManager::convertFloatToString( this->getRemainingReactorPower() );
	result[FieldNames::MCU::MaxShieldPower] =  SQLiteDBManager::convertFloatToString( this->getMechanizedCombatUnit()->getShield()->getMaxShieldHealth() );
	result[FieldNames::MCU::RemainingShieldPower] = SQLiteDBManager::convertIntToString( (int)(this->getShield()->getCurrentHealth()) );
	result[FieldNames::MCU::RemainingHealthPercent] = SQLiteDBManager::convertIntToString( (int)(this->remainingHealthPercent()) );

	result[FieldNames::MCU::ThumbnailImage] = this->mMechanizedCombatUnit->getMCUTemplate()->getThumbnailFile();
	result[FieldNames::MCU::ProfileImage] = this->mMechanizedCombatUnit->getMCUTemplate()->getProfileImageFile();

	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString( mBattlefieldID );


	return result;
}

void MCUController::updateGuiData(const std::string& source)
{

	if(mIsPlayerMCU)
	{
		//GameManager::getSingletonPtr()->luaDebugReport("Update gui data - source " + source,0);
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->getPrimaryDataSet()->updateData(mBattlefieldID, this->getGuiValues());
		std::vector<WeaponHardpoint*> weapons = mMechanizedCombatUnit->getWeapons();
		std::vector<WeaponHardpoint*>::iterator iter;
		for(iter = weapons.begin(); iter != weapons.end(); ++iter)
		{
			if((*iter))
			{
				float accuracy = calculateAccuracy((*iter));
				(*iter)->setCurrentChanceToHit(accuracy);
				DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->getSubDataSet(mBattlefieldID)->updateData((*iter)->getHardpointNumber(),(*iter)->getGuiValues());
			}
		}
		DataProvider::getSingletonPtr()->updateDataSourceListeners(DataSourceType::MCU);
		DataProvider::getSingletonPtr()->updateDataSourceListeners(DataSourceType::WEAPON);
		//DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCU,"mcudp",this->getBattlefieldID(), false);

	}
	
	else
	{
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::ENEMY_MCU)->getPrimaryDataSet()->updateData(mBattlefieldID, this->getGuiValues());
		DataProvider::getSingletonPtr()->updateDataSourceListeners(DataSourceType::ENEMY_MCU);
	}
}

FogSquareGrid* MCUController::getCurrentFogSquareGrid()
{
	return mFogSquareGrid;
}
