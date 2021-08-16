#ifndef MCUController_h
#define MCUController_h

#include <Entities/MCUComponent.h>
#include <Entities/MovableEntity.h>
#include <Ogre.h>
#include <list>
#include <deque>
#include <hash_map>
#include <OgreAL.h>

class AIGroup;
class BattlefieldGrid;
class BattlefieldGridCell;
class MCUAnimationController;
class MCUStatus;
class MechanizedCombatUnit;
class PlayerBattleState;
class PathFindingNode;
class Shield;
class btKinematicCharacterController;
class KinematicCharacterController;
class WeaponHardpoint;
class WeaponTemplate;

class MCUController;

struct FireInstruction
{
public:
	FireInstruction() : Source(), Destination(), WeaponHardpoint(0), Executed(false), Initialized(false), ReturningFire(false) {}
	FireInstruction(const Ogre::Vector3 source, const Ogre::Vector3 dest, MCUController* firedBy, WeaponHardpoint* weapon, bool returningFire) : Source(source), Destination(dest), FiredBy(firedBy), WeaponHardpoint(weapon), Executed(false), Initialized(true), ReturningFire(returningFire)	{}

	Ogre::Vector3 Source;
	Ogre::Vector3 Destination;
	MCUController* FiredBy;
	WeaponHardpoint* WeaponHardpoint;
	bool Executed;
	bool Initialized;
	bool ReturningFire;
};

struct FogSquare;
class FogSquareGrid;

class MCUController : public MovableEntity
{
public:
	MCUController();
	MCUController(Ogre::SceneNode* node,Ogre::Entity* entity,MechanizedCombatUnit* mcu, PlayerBattleState* player,BattlefieldGridCell* location, BattlefieldGrid* grid, int battlefieldId);
	MCUController(Ogre::SceneNode* node,Ogre::Entity* entity, MCUAnimationController* anim,KinematicCharacterController* physics,MechanizedCombatUnit* mcu, PlayerBattleState* player,BattlefieldGridCell* location, BattlefieldGrid* grid, Ogre::Real height);
	~MCUController();

	void MCUController::startTurn();
	void MCUController::endTurn();

	Ogre::Entity* getEntity();
	void setEntity(Ogre::Entity* entity);

	MCUAnimationController* getMCUAnimationController();
	void setMCUAnimationController(MCUAnimationController* anim);

	KinematicCharacterController* getKinematicCharacterController();
	void setKinematicCharacterController(KinematicCharacterController* physics);

	MechanizedCombatUnit* getMechanizedCombatUnit();
	void setMechanizedCombatUnit(MechanizedCombatUnit* mcu);

	PlayerBattleState* getPlayerBattleState();
	void setPlayerBattleState(PlayerBattleState* player);

	BattlefieldGridCell* getCurrentLocation();
	void setCurrentLocation(BattlefieldGridCell* location);

	Shield* getShield();
	void setShield(Shield* shield);

	std::deque<BattlefieldGridCell*>& getCurrentPath();
	void setCurrentPath(PathFindingNode* goal);

	BattlefieldGridCell* getCurrentGoal();
	void moveAlongPath();
	bool isMoving();
	void stopMoving(bool blocked = false, MCUController* occupant = 0, bool notify = true);
	void giveStopOrder();
	bool lowMovementEnergy();
	bool hasPath();

	BattlefieldGrid* getGrid();
	void setBattlefieldGrid(BattlefieldGrid* grid);

	bool hasTarget();
	float rangeToTarget();
	
	MCUController* getCurrentTarget();
	void setCurrentTarget(MCUController* target);

	void targetedBy(MCUController* source);
	void noLongerTargetedBy(MCUController* source);
	void targetDestroyed();
	void reportKill();

	AIGroup* getAIGroup();
	void setAIGroup(AIGroup* group);

	int getBattlefieldID();
	void setBattlefieldID(int id);

	bool isActive();
	void setActive(bool value);
	
	bool isVisible();
	void setVisible(bool value);

	bool isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance);

	bool isRadarVisible();
	void setRadarVisible(bool value);

	bool isPlayerMCU();
	void setPlayerMCU(bool value);

	Ogre::Bone* getFiringBone(WeaponHardpoint* weapon);
	Ogre::Vector3 getFiringSourcePosition(WeaponHardpoint* weapon);

	bool canFire(int hardpoint);
	bool canFire(WeaponHardpoint* weapon);
	bool canFireAtLeastOneWeapon();
	bool canFireUpon(MCUController* target);

	bool shieldsUp();
	
	bool reachedDestination();

	void beginFiringWeapon(WeaponHardpoint* weapon, const Ogre::Vector3& source, const Ogre::Vector3& dest, bool returningFire);
	void fireWeapon(int weaponHardpoint);

	WeaponHardpoint* getBestAntiShieldWeapon();
	WeaponHardpoint* getBestAntiArmorWeapon();
	bool isFiring();
	void applyWarhead(int projId, const FireInstruction& instruction);
	void weaponFiringComplete();

	void update(const Ogre::Real& timeSinceLastFrame);
	void enableAnimations();
	Ogre::Real Sequence;

	Ogre::Vector3 getPosition();
	Ogre::Vector3 getMiddle();
	Ogre::SceneNode* getNode();

	
	float getRemainingReactorPower();
	float getRemainingBatteryPower();

	float getTotalRemainingPower();

	void drainEnergyFromBattery(float amount);
	void drainEnergyFromReactor(float amount);
	void drainWeaponEnergy(float amount);

	void componentDestroyed(MCUComponent* component);
	void startDestructionSequence();
	void destroy();
	std::deque<MCUComponent*>& getAllComponents();
	
	void updateFogOfWar();
	void renderFogOfWar();
	bool canSee(MCUController* target);

	std::hash_map<std::string,std::string> getGuiValues();
	void updateGuiData(const std::string& source);

	float getShortestWeaponRange();
	float getLongestWeaponRange();

	float remainingHealthPercent();
	bool sensorActive();
	FogSquareGrid* getCurrentFogSquareGrid();

	float calculateAccuracy(WeaponHardpoint* weapon);
	
	void playJumpSound();
	void playLandSound();

private:
	void initialize();
	void executeCurrentFiringInstruction();

	bool areEqual(Ogre::Quaternion q1, Ogre::Quaternion q2, float marginOfError = 0.005f);

	float calculateCoverage(const Ogre::Vector3& origin);
	bool canSee(const Ogre::Vector3& origin, const Ogre::Vector3& dest);

	bool mActive;
	bool mSensorActive;
	bool mInitializedFogOfWar;
	bool mNeedsFogUpdate;
	bool mNeedsFogRender;
	bool mIsJumping;
	bool mJumped;
	bool mIsJumpingDown;
	bool mLanding;
	bool mStartedTurning;
	float mYawAmount;
	float mYawAmountNeeded;

	Ogre::Vector3 mDirection;
	Ogre::Vector3 mPosition;
	Ogre::Vector3 mTarget;
	Ogre::Real mMovementSpeed;
	Ogre::Real mDistanceRemaining;
	Ogre::Real mHeight;

	Ogre::Entity* mEntity;
	Ogre::SceneNode* mNode;


	Ogre::SceneNode* mFogSceneNode;
	Ogre::ManualObject* mFogManualObject;
	std::deque<Ogre::Vector3> mFogRayTestDirections;

	MCUAnimationController* mMCUAnimationController;
	KinematicCharacterController* mKinematicCharacterController;
	MechanizedCombatUnit* mMechanizedCombatUnit;
	MCUStatus* mMCUStatus;
	PlayerBattleState* mPlayerBattleState;
	BattlefieldGridCell* mCurrentLocation;
	Shield* mShield;
	std::deque<BattlefieldGridCell*> mCurrentPath;
	bool mHasPath;
	bool mMoving;
	bool mOrderedToStop;
	bool mReachedDestination;
	bool mTurning;
	bool mTurnToFire;
	bool mIsFiring;
	bool mIsPlayerMCU;

	Ogre::Quaternion mRotationToTarget;
	FireInstruction mCurrentFireInstruction;
	
	int mBattlefieldID;

	int mAISpeedBoost;

	BattlefieldGrid* mBattlefieldGrid;

	MCUController* mCurrentTarget;
	MCUController* mLastFiredUponBy;
	std::deque<MCUController*> mTargetedBy;

	MCUComponent* mChassisComponent;
	MCUComponent* mArmorComponent;
	MCUComponent* mBatteryComponent;
	MCUComponent* mDriveComponent;
	MCUComponent* mReactorComponent;
	MCUComponent* mShieldComponent;
	MCUComponent* mSensorSystemComponent;
	MCUComponent* mTargetingSystemComponent;
	
	MCUComponent* mTargetedEnemyComponent;
	std::deque<MCUComponent*> mWeaponHardpointComponents;
	std::deque<MCUComponent*> mAllComponents;
	
	Ogre::BillboardChain* mRadarBillBoard;

	float mRemainingBatteryPower;
	float mRemainingReactorPower;
	FogSquareGrid* mFogSquareGrid;
	AIGroup* mAIGroup;
	MCUController* mLastDamagedBy;

	OgreAL::Sound* mWalkSound;
	OgreAL::Sound* mJumpSound;
	OgreAL::Sound* mLandSound;
	OgreAL::Sound* mTurnSound;

};

#endif