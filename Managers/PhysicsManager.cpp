#include <Managers/PhysicsManager.h>
#include <Managers/GameManager.h>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Ogre.h>
#include <States/MyMotionState.h>
#include <Templates/TerrainTemplate.h>
#include <Entities/BattlefieldGrid.h>

#include <BtOgre/BtOgreExtras.h>
#include <BtOgre/BtOgreGP.h>
#include <BtOgre/BtOgrePG.h>

PhysicsManager::PhysicsManager()
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	
	//btVector3 worldMin(-12000,-12000,-12000);
	//btVector3 worldMax(12000,12000,12000);

	mBroadphaseInterface = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphaseInterface,mSolver,mCollisionConfiguration);
	mDynamicsWorld->setGravity(btVector3(0,-9.8f,0));
	mGhostCallback = new btGhostPairCallback();
	
	mBroadphaseInterface->getOverlappingPairCache()->setInternalGhostPairCallback(mGhostCallback);

	//mDebugDrawer = new BtOgre::DebugDrawer(GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode(), mDynamicsWorld);
	//mDynamicsWorld->setDebugDrawer(mDebugDrawer);
}

PhysicsManager::~PhysicsManager() 
{
	delete mGhostCallback;
	delete mDynamicsWorld;
	delete mSolver;
	delete mBroadphaseInterface;
	delete mDispatcher;
	delete mCollisionConfiguration;
	
}

void PhysicsManager::initializeTerrain(BattlefieldGrid* grid, btBvhTriangleMeshShape* shape, Ogre::StaticGeometry* geom)
{
	mExtents = btTransform(btQuaternion::getIdentity(),btVector3(grid->getSideLength() * BattlefieldGrid::UnitSize,grid->getMaxVertexHeight(), grid->getSideLength() * BattlefieldGrid::UnitSize));

	btDefaultMotionState* state = new btDefaultMotionState();
	btRigidBody* rigidBody = new btRigidBody(0, state, shape);
	registerCollisionObject(rigidBody,geom,"Battlefield");
	
	rigidBody->getWorldTransform().setRotation(
				 btQuaternion(
					 Ogre::Quaternion::IDENTITY.x,
					 Ogre::Quaternion::IDENTITY.y,
					 Ogre::Quaternion::IDENTITY.z,
					 Ogre::Quaternion::IDENTITY.w));
	
	 rigidBody->setFriction(1.0f);
	//rigidBody->setDamping(0.5f,0.5f);
	//rigidBody->setRestitution(0.5f);

	//rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	
	//rigidBody->setCcdMotionThreshold(0.1f);
	//rigidBody->setCcdSweptSphereRadius(0.2f); 

	mDynamicsWorld->addRigidBody(rigidBody);

}

btVector3 * PhysicsManager::getUpAxisDirections()
{
	static btVector3 sUpAxisDirection[3] = {
											btVector3(1, 0, 0),
											btVector3(0, 1, 0),
											btVector3(0, 0, 1)
											};

	return sUpAxisDirection;
}

btVector3 PhysicsManager::getNormalizedVector(const btVector3 & v)
{
	btVector3 n = v.normalized();

	if (n.length() < SIMD_EPSILON)
		n.setValue(0, 0, 0);

	return n;
}

btRigidBody * PhysicsManager::addDynamicBox(Ogre::SceneNode * node, float m)
{
	btCollisionShape * colShape = new btBoxShape(btVector3(25, 25, 25));
	btTransform boxTransform;
	boxTransform.setIdentity();

	btScalar mass(m);
	btVector3 localInertia(0, 0, 0);

	colShape->calculateLocalInertia(mass, localInertia);

	boxTransform.setOrigin(btVector3(node->getPosition().x, node->getPosition().y, node->getPosition().z));

	BtOgre::RigidBodyState* state = new BtOgre::RigidBodyState(node,boxTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, state, colShape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);
	
	
	//body->setCcdMotionThreshold(0.1f);
	//body->setCcdSweptSphereRadius(0.2f); 

	//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	mDynamicsWorld->addRigidBody(body);

	return body;
}

btRigidBody * PhysicsManager::addRigidBody(const btTransform& transform, btCollisionShape * shape, btScalar mass, Ogre::SceneNode * node)
{
	btVector3 localInertia(0, 0, 0);

	shape->calculateLocalInertia(mass, localInertia);

	MyMotionState * motionState = new MyMotionState(transform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);
	body->setCcdMotionThreshold(0.1);
	
	mDynamicsWorld->addRigidBody(body);
	return body;
}

void PhysicsManager::addRigidBody(btRigidBody* body)
{
	mDynamicsWorld->addRigidBody(body);
}

void PhysicsManager::addCharacterController(btCharacterControllerInterface* character, btGhostObject* ghost)
{
	mDynamicsWorld->addCollisionObject(ghost, btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	mDynamicsWorld->addAction(character);
}

void PhysicsManager::removeCharacterController(btCharacterControllerInterface* character, btGhostObject* ghost)
{
	mDynamicsWorld->removeAction(character);
	mDynamicsWorld->removeCollisionObject(ghost);
	
	std::deque<CollisionObjectMapping>::iterator iter;
	for(iter = mCollisionObjectMappings.begin(); iter != mCollisionObjectMappings.end(); ++iter)
	{
		if((*iter).MainCollisionObject)
		{
			if((*iter).MainCollisionObject == ghost)
			{
				mCollisionObjectMappings.erase(iter);
				return;
			}
		}
		if((*iter).ShieldCollisionObject)
		{
			if((*iter).ShieldCollisionObject == ghost)
			{
				mCollisionObjectMappings.erase(iter);
				return;
			}
		}
	}

}

void PhysicsManager::removeCollisionObject(btCollisionObject* collision)
{
	mDynamicsWorld->removeCollisionObject(collision);
	std::deque<CollisionObjectMapping>::iterator iter;
	for(iter = mCollisionObjectMappings.begin(); iter != mCollisionObjectMappings.end(); ++iter)
	{
		if((*iter).MainCollisionObject)
		{
			if((*iter).MainCollisionObject == collision)
			{
				mCollisionObjectMappings.erase(iter);
				return;
			}
		}
		if((*iter).ShieldCollisionObject)
		{
			if((*iter).ShieldCollisionObject == collision)
			{
				mCollisionObjectMappings.erase(iter);
				return;
			}
		}
	}
}

void PhysicsManager::registerCollisionObject(btCollisionObject* mcu, btCollisionObject* shield, Ogre::MovableObject* mainMovable,Ogre::MovableObject* shieldMovable,  MCUController* controller,std::string name)
{
	CollisionObjectMapping mapping;
	mapping.MainCollisionObject = mcu;
	mapping.ShieldCollisionObject = shield;
	mapping.MainMovableObject = mainMovable;
	mapping.ShieldMovableObject = shieldMovable;
	mapping.MCUController = controller;
	mapping.Name = name;

	mCollisionObjectMappings.push_back(mapping);
}

void PhysicsManager::registerCollisionObject(btCollisionObject* collision, Ogre::StaticGeometry* geom, std::string name)
{
	CollisionObjectMapping mapping;
	mapping.MainCollisionObject = collision;
	mapping.StaticGeometry = geom;
	mapping.Name = name;

	mCollisionObjectMappings.push_back(mapping);
}

void PhysicsManager::clearCollisionMappings()
{
	std::deque<CollisionObjectMapping>::iterator iter;

	for(iter = mCollisionObjectMappings.begin(); iter != mCollisionObjectMappings.end(); ++iter)
	{
		if((*iter).MainCollisionObject)
		{
			mDynamicsWorld->removeCollisionObject((*iter).MainCollisionObject);
		}
		if((*iter).ShieldCollisionObject)
		{
			mDynamicsWorld->removeCollisionObject((*iter).ShieldCollisionObject);
		}
	}
	mCollisionObjectMappings.clear();
}

void PhysicsManager::removeRigidBody(btRigidBody* body)
{
	mDynamicsWorld->removeRigidBody(body);
}

btDiscreteDynamicsWorld * PhysicsManager::getDynamicsWorld()
{
	return mDynamicsWorld;
}

btCollisionWorld * PhysicsManager::getCollisionWorld()
{
	return mDynamicsWorld->getCollisionWorld();
}

btBroadphaseInterface * PhysicsManager::getBroadphase()
{
	return mBroadphaseInterface;
}

void PhysicsManager::setRootSceneNode(Ogre::SceneNode * node)
{
	mRootSceneNode = node;
}

btVector3 toBullet(const Ogre::Vector3 & vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

void PhysicsManager::update(Ogre::Real timeSinceLastFrame)
{
	//mDynamicsWorld->stepSimulation(timeSinceLastFrame / 1000.f,1,(1.f / 60.f));
	mDynamicsWorld->stepSimulation(timeSinceLastFrame,1,(1.f / 60.f));
	
	//mDebugDrawer->step();
}

ClosestNotMeRayResultCallback PhysicsManager::closestNotMeRayTestCallback(btCollisionObject* source, const btVector3& rayFrom, const btVector3& rayTo)
{
	ClosestNotMeRayResultCallback result(source,rayFrom, rayTo);
	mDynamicsWorld->rayTest(rayFrom, rayTo, result);


	return result;

}
	//btCollisionWorld::ClosestRayResultCallback rayCallback(btRayFrom,btRayTo);

ClosestRayResultCallback PhysicsManager::closestRayTestCallback(const btVector3& rayFrom, const btVector3& rayTo)
{
	ClosestRayResultCallback rayCallback = ClosestRayResultCallback(rayFrom, rayTo);

	mDynamicsWorld->rayTest(rayFrom,rayTo,rayCallback);
	
	return rayCallback;
}

VisibilityTestRayResultCallback PhysicsManager::visibilityTestRayTestCallback(CollisionObjectMapping* mapping, const btVector3& rayFrom, const btVector3& rayTo)
{
	VisibilityTestRayResultCallback rayCallback = VisibilityTestRayResultCallback(rayFrom, rayTo, mapping);

	mDynamicsWorld->rayTest(rayFrom,rayTo,rayCallback);

	return rayCallback;
}

TerrainRayTestResultCallback PhysicsManager::terrainRayTest(const btVector3& rayFrom, const btVector3& rayTo)
{
	TerrainRayTestResultCallback callback = TerrainRayTestResultCallback(rayFrom, rayTo);

	mDynamicsWorld->rayTest(rayFrom, rayTo, callback);

	return callback;
}

ClosestNotMeConvexResultCallback PhysicsManager::closestNotMeProjectileCollisionCallback(btCollisionObject* source, const btVector3& location, const btVector3& direction, btConvexShape* shape)
{
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin(location);
	end.setOrigin(direction);

	ClosestNotMeConvexResultCallback callback(source,btVector3(0.0f, 1.0f, 0.0f),0.707f);

	//mDynamicsWorld->contactTest(source, callback);
	mDynamicsWorld->convexSweepTest(shape,start,end,callback);

	return callback;
	
}

CollisionObjectMapping* PhysicsManager::findCollisionObjectMapping(btCollisionObject* collision)
{
	std::deque<CollisionObjectMapping>::iterator iter;

	for(iter = mCollisionObjectMappings.begin(); iter != mCollisionObjectMappings.end(); ++iter)
	{
		if((*iter).MainCollisionObject == collision || (*iter).ShieldCollisionObject == collision)
			return &*iter;
	}

	return 0;
}

CollisionObjectMapping* PhysicsManager::findCollisionObjectMapping(Ogre::MovableObject* movable)
{
	std::deque<CollisionObjectMapping>::iterator iter;

	for(iter = mCollisionObjectMappings.begin(); iter != mCollisionObjectMappings.end(); ++iter)
	{
		if((*iter).MainMovableObject == movable || (*iter).ShieldMovableObject == movable)
			return &*iter;
	}

	return 0;
}

CollisionObjectMapping* PhysicsManager::findCollisionObjectMapping(Ogre::StaticGeometry* geom)
{
	std::deque<CollisionObjectMapping>::iterator iter;

	for(iter = mCollisionObjectMappings.begin(); iter != mCollisionObjectMappings.end(); ++iter)
	{
		if((*iter).StaticGeometry == geom)
			return &*iter;
	}

	return 0;
}


PhysicsManager* PhysicsManager::mPhysicsManager;

PhysicsManager* PhysicsManager::getSingletonPtr()
{
	if(!mPhysicsManager)
	{
		mPhysicsManager = new PhysicsManager();
	}
	return mPhysicsManager;
}