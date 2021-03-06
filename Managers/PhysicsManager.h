#ifndef PhysicsManager_H
#define PhysicsManager_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <LinearMath/btDefaultMotionState.h>
#include <Managers/GameManager.h>

#include <BtOgre/BtOgreExtras.h>
#include <BtOgre/BtOgreGP.h>
#include <BtOgre/BtOgrePG.h>

class BattlefieldGrid;
class MCUController;
class ClosestRayResultCallback;
class ClosestNotMeRayResultCallback;
class ClosestNotMeConvexResultCallback;
class VisibilityTestRayResultCallback;
class TerrainRayTestResultCallback;

struct CollisionObjectMapping
{
public:
	CollisionObjectMapping() : MainCollisionObject(0), ShieldCollisionObject(0), MainMovableObject(0), ShieldMovableObject(0), StaticGeometry(0), MCUController(0), Name(""){}
	btCollisionObject* MainCollisionObject;
	btCollisionObject* ShieldCollisionObject;
	Ogre::MovableObject* MainMovableObject;
	Ogre::MovableObject* ShieldMovableObject;
	Ogre::StaticGeometry* StaticGeometry;
	MCUController* MCUController;
	std::string Name;
};

class PhysicsManager : public GameManager::EngineUpdateListener
{
public:

	
	~PhysicsManager();

	void addCollisionShape(btCollisionShape * colShape);
	btDiscreteDynamicsWorld * getDynamicsWorld();
	btCollisionWorld * getCollisionWorld();
	btBroadphaseInterface * getBroadphase();
	void setRootSceneNode(Ogre::SceneNode * node);
	btVector3 toBullet(const Ogre::Vector3 & vec) const;

	void initializeTerrain(BattlefieldGrid* grid, btBvhTriangleMeshShape* shape, Ogre::StaticGeometry* geom);
	static btVector3 * getUpAxisDirections();
	static btVector3 getNormalizedVector(const btVector3 & v);
	btRigidBody * addDynamicBox(Ogre::SceneNode * node, float m = 1.0f);
	void removeRigidBody(btRigidBody* body);
	btRigidBody * addRigidBody(const btTransform& transform, btCollisionShape * shape, btScalar mass, Ogre::SceneNode * node = NULL);
	void addRigidBody(btRigidBody* body);

	void addCharacterController(btCharacterControllerInterface* character, btGhostObject* ghost);
	void removeCharacterController(btCharacterControllerInterface* character, btGhostObject* ghost);

	void removeCollisionObject(btCollisionObject* collision);
	void registerCollisionObject(btCollisionObject* mcu, btCollisionObject* shield, Ogre::MovableObject* mainMovable,Ogre::MovableObject* shieldMovable, MCUController* controller, std::string name);
	void registerCollisionObject(btCollisionObject* collision, Ogre::StaticGeometry* geom, std::string name);

	void clearCollisionMappings();
	void update(Ogre::Real timeSinceLastFrame);
	
	ClosestNotMeRayResultCallback closestNotMeRayTestCallback(btCollisionObject* source, const btVector3& rayFrom, const btVector3& rayTo);
	ClosestRayResultCallback closestRayTestCallback(const btVector3& rayFrom, const btVector3& rayTo);
	VisibilityTestRayResultCallback visibilityTestRayTestCallback(CollisionObjectMapping* mapping, const btVector3& rayFrom, const btVector3& rayTo);
	ClosestNotMeConvexResultCallback closestNotMeProjectileCollisionCallback(btCollisionObject* source, const btVector3& location, const btVector3& direction, btConvexShape* shape);
	TerrainRayTestResultCallback terrainRayTest(const btVector3& rayFrom, const btVector3& rayTo);

	CollisionObjectMapping* findCollisionObjectMapping(btCollisionObject* collision);
	CollisionObjectMapping* findCollisionObjectMapping(Ogre::MovableObject* movable);
	CollisionObjectMapping* findCollisionObjectMapping(Ogre::StaticGeometry* geom);

	static PhysicsManager* getSingletonPtr();

private:
	PhysicsManager();
	PhysicsManager( const PhysicsManager& ) { }
	PhysicsManager & operator = ( const PhysicsManager& );



	BtOgre::DebugDrawer* mDebugDrawer;

	static PhysicsManager *mPhysicsManager;

	btGhostPairCallback* mGhostCallback;
	btCollisionDispatcher*	mDispatcher;
	btBroadphaseInterface *				mBroadphaseInterface;
	btSequentialImpulseConstraintSolver*	mSolver;
	btDiscreteDynamicsWorld* mDynamicsWorld;
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	Ogre::SceneNode * mRootSceneNode;
	std::deque<CollisionObjectMapping> mCollisionObjectMappings;
	btTransform mExtents;
};

class ClosestRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
private:
	CollisionObjectMapping* mCollisionObjectMapping;

public:
	ClosestRayResultCallback(const btVector3& rayFrom,  const btVector3& rayTo) : mCollisionObjectMapping(0), btCollisionWorld::ClosestRayResultCallback(rayFrom, rayTo){}
	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult & rayResult, bool normalInWorldSpace)
	{
	
		setCollisionObjectMapping(PhysicsManager::getSingletonPtr()->findCollisionObjectMapping(rayResult.m_collisionObject));

		return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
	}
	void setCollisionObjectMapping(CollisionObjectMapping* mapping) {mCollisionObjectMapping = mapping;}
	CollisionObjectMapping* getCollisionObjectMapping() {return mCollisionObjectMapping;}
};

class ClosestNotMeRayResultCallback : public ClosestRayResultCallback
{
	private:
		btCollisionObject * mMe;
	public:
		ClosestNotMeRayResultCallback(btCollisionObject * me, const btVector3& rayFrom,  const btVector3& rayTo) : ClosestRayResultCallback(rayFrom, rayTo),	mMe(me)
		{}

		btScalar addSingleResult(btCollisionWorld::LocalRayResult & rayResult, bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject == mMe)
				return 1.0;

			setCollisionObjectMapping(PhysicsManager::getSingletonPtr()->findCollisionObjectMapping(rayResult.m_collisionObject));

			return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
		}
			
};

class VisibilityTestRayResultCallback : public ClosestRayResultCallback
{
	private:
		CollisionObjectMapping* mTarget;
		bool mVisible;
	public:
		VisibilityTestRayResultCallback(const btVector3& rayFrom,  const btVector3& rayTo, CollisionObjectMapping* target) : ClosestRayResultCallback(rayFrom, rayTo), mTarget(target), mVisible(false)
		{}

		btScalar addSingleResult(btCollisionWorld::LocalRayResult & rayResult, bool normalInWorldSpace)
		{
			CollisionObjectMapping* mapping = PhysicsManager::getSingletonPtr()->findCollisionObjectMapping(rayResult.m_collisionObject);

			if(mapping->StaticGeometry)
			{
				mVisible = false;
				setCollisionObjectMapping(mapping);
				return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
			}
			else if(mapping == mTarget)
			{
				mVisible = true;
				setCollisionObjectMapping(mapping);
				return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
			}
			else
			{
				return 1.0;
			}

		}

		bool isVisible() {return mVisible;}
			
};

class TerrainRayTestResultCallback : public ClosestRayResultCallback
{
private:
	bool mHitTerrain;
public:
	TerrainRayTestResultCallback(const btVector3& rayFrom,  const btVector3& rayTo) : ClosestRayResultCallback(rayFrom, rayTo), mHitTerrain(false)
	{}
	btScalar addSingleResult(btCollisionWorld::LocalRayResult & rayResult, bool normalInWorldSpace)
		{
			CollisionObjectMapping* mapping = PhysicsManager::getSingletonPtr()->findCollisionObjectMapping(rayResult.m_collisionObject);

			if(mapping)
			{
				if(mapping->StaticGeometry)
				{
					mHitTerrain = true;
					setCollisionObjectMapping(mapping);
					return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
				}
				else
				{
					return 1.0;
				}
			}
			return 1.0;
		}

	bool hitTerrain() {return mHitTerrain;}
};

class ClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
	private:
		btCollisionObject * mMe;
		CollisionObjectMapping* mCollisionObjectMapping;
		const btVector3 mUp;
		btScalar mMinSlopeDot;

	public:
		ClosestNotMeConvexResultCallback(btCollisionObject * me, const btVector3 & up, btScalar minSlopeDot) :
			btCollisionWorld::ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)),
			mMe(me),
			mCollisionObjectMapping(0),
			mUp(up),
			mMinSlopeDot(minSlopeDot)
		{}

		btScalar addSingleResult(btCollisionWorld::LocalConvexResult & convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == mMe)
				return 1.0;

				

			btVector3 hitNormalWorld;

			if (normalInWorldSpace)
				hitNormalWorld = convexResult.m_hitNormalLocal;
			else
				hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal;

			btScalar dotUp = mUp.dot(hitNormalWorld);
				
			mCollisionObjectMapping = PhysicsManager::getSingletonPtr()->findCollisionObjectMapping(convexResult.m_hitCollisionObject);
				
			return btCollisionWorld::ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
		}

		CollisionObjectMapping* getCollisionObjectMapping() {return mCollisionObjectMapping;}
};

#endif