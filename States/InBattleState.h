#ifndef InBattleState_h
#define InBattleState_h

#include "GameState.h"

#include <Templates/RandomMissionTypeTemplate.h>
#include <Templates/StoryMissionTemplate.h>
#include <States/PlayerBattleState.h>
#include <Ogre.h>
#include <hash_map>
#include <vector>
#include <list>
#include "btBulletDynamicsCommon.h"
#include <OgreAL.h>

class BattlefieldGrid;
class BattlefieldGridCell;
class BattleResult;
class BattleResultInterpreter;
class FactionTemplate;
class GameState;
class InGameCamera;
class ExplosionSet;
class MechanizedCombatUnit;
class MCUController;
class Mission;
class MovableEntity;
class PathFindingManager;
class PlayerPreBattleState;
class PlayerBattleState;
class PlayerProfile;
class Projectile;
class Shield;
class WeaponHardpoint;

class InBattleState : public GameState 
{
public:
	~InBattleState();

	void enter();
	void exit();
	void endBattle();
	void restart();
	void cleanUp();

	void pause();
	void resume();
	void createScene();
	void destroyScene();

	void beginSetup(int sideLength);

	void update( Ogre::Real timeSinceLastFrame );
	void updateMCUVisibility();

	void startTurn();
	void endTurn();

	Ogre::Real getTurnLimit();
	void setTurnLimit(Ogre::Real limit);
	
	std::string getAIScriptName();
	void setAIScriptName(const std::string& name);

	BattlefieldGrid* getBattlefieldGrid();
	void setBattlefieldGrid(BattlefieldGrid* grid);

	PathFindingManager* getPathFindingManager();

	void addPlayer(PlayerPreBattleState* state);
	void placeMCUs();
	void removePlayer(PlayerBattleState* player);
	std::deque<PlayerBattleState*> getAllPlayers();

	void keyPressed(const OIS::KeyEvent& arg);
	void keyReleased(const OIS::KeyEvent& arg);
	void mouseMoved(const OIS::MouseEvent& arg);
	void mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	void mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	PlayerBattleState* getPlayerToRender();
	void setPlayerToRender(PlayerBattleState* player);
	
	void onSelectedElementChanged(DataSourceType source, int id);

	void fireWeapon(MCUController* source, WeaponHardpoint* weapon, bool returningFire);
	MCUController* getMCUByBattlefieldID(int id);

	void addProjectile(Projectile* proj);
	
	void addShield(Shield* shield);

	void removeMCU(MCUController* controller);

	void addParticleSystem(const std::string& templateName, const Ogre::Vector3& location);

	void addExplosion(const std::string& templateName, const Ogre::Vector3& location);

	void addMovableEntity(MovableEntity* entity);
	std::deque<MovableEntity*>& getMovableEntities();

	void addSound(OgreAL::Sound* sound);

	void updateAccuracy(MCUController* mcu, WeaponHardpoint* weapon);
	float calculateAccuracy(MCUController* mcu, WeaponHardpoint* weapon);
	float calculateCoverage(const Ogre::Vector3& origin,  MCUController* source, MCUController* target);
	bool canSee(const Ogre::Vector3& origin, const Ogre::Vector3& dest, MCUController* source, MCUController* target);
	bool canSee(const Ogre::Vector3& origin, const Ogre::Vector3& dest, MCUController* target);

	MCUController* getTarget(const Ogre::Vector3& origin, const Ogre::Vector3& dest, MCUController* source);
	
	void ProcessEvent(Rocket::Core::Event& e);
	void renderPath(MCUController* controller, std::deque<BattlefieldGridCell*>& path);
	void hidePath();

	bool isLocalPlayersTurn();
	bool isMyTurn(PlayerBattleState* player);

	void selectMCU(MCUController* mcu);
	void selectMCU(int id);
	void selectTarget(MCUController* mcu);

	void nextMCU();
	void prevMCU();

	bool mcuHasTargetBillboard(MCUController* mcu);
	void showTargetSelectBillboard();
	void hideTargetSelectBillboard();

	BattleResult* getBattleResult();
	void setBattleResult(BattleResult* result);
	
	BattleResultInterpreter* getBattleResultInterpreter();
	void setBattleResultInterpreter(BattleResultInterpreter* interpreter);

	GameState* getPostBattleState();
	void setPostBattleState(GameState* state);

	void loadMission(Mission* mission);
	static InBattleState* getSingletonPtr();
private:
	InBattleState();
	InBattleState( const InBattleState& ) { }
	InBattleState & operator = ( const InBattleState& );
	
	void loadBattlefield();
	void setupFog();
	MCUController* getMCUController(Ogre::MovableObject* mov);
	
	GameState* mPostBattleState;
	BattleResultInterpreter* mBattleResultInterpreter;

	Ogre::SceneNode* mGreenPathNode;
	Ogre::SceneNode* mRedPathNode;
	Ogre::ManualObject* mGreenPath;
	Ogre::ManualObject* mRedPath;

	Ogre::ManualObject* mCoverageDebugger;
	
	Ogre::RaySceneQuery* mRaySceneQuery;

	int mMCUCount;
	int mUpdateHightlight;

	std::deque<PlayerBattleState*> mPlayers;
	std::deque<PlayerBattleState*>::iterator mVisibilityUpdateIterator;
	std::deque<PlayerBattleState*>::iterator mCurrentPlayerTurn;

	std::deque<MCUController*> mMCUControllers;
	std::deque<MCUController*> mMCUsToRemove;
	std::deque<MCUController*> mDestroyedMCUs;

	std::deque<Ogre::ParticleSystem*> mParticleSystems;
	std::deque<ExplosionSet*> mExplosionSets;

	std::deque<MovableEntity*> mMovableEntities;
	std::deque<MovableEntity*> mMovableEntitiesToAdd;

	bool mProjectileNeedsRemoving;
	bool mBattleEnded;

	unsigned int mNextProjectileId;

	MCUController* mCurrentSelectedMCU;
	InGameCamera* mInGameCamera;

	int mNumPlayers;

	Ogre::Real mTurnTimer;
	Ogre::Real mTurnLimit;


	bool mScrollUp;
	bool mScrollDown;
	bool mScrollLeft;
	bool mScrollRight;

	BattlefieldGrid* mBattlefieldGrid;
	PlayerBattleState* mPlayerToRender;
	PathFindingManager* mPathFindingManager;
	Mission* mMission;

	std::string mAIScriptName;

	Ogre::Light* mDirectionalLight;

	Ogre::BillboardSet* mUnitSelectBillboard;
	Ogre::BillboardSet* mTargetSelectBillboard;
	
	BattleResult* mBattleResult;

	OgreAL::Sound* mSelectMCUSound;
	std::deque<OgreAL::Sound*> mSounds;

	void rayIntersection(const Ogre::Vector3& origin, const Ogre::Vector3& direction, Ogre::RaySceneQueryResult& result);
	static InBattleState *mInBattleState;
};
#endif