#ifndef PhysicalProjectile_h
#define PhysicalProjectile_h

#include <Entities/Projectile.h>
#include <Ogre.h>
#include <Controllers/MCUController.h>

#include <LinearMath/btVector3.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include "btBulletDynamicsCommon.h"

class btCollisionShape;
class btCylinderShape;
class btRigidBody;
class btCollisionWorld;
class btCollisionDispatcher;
class btGhostObject;
class btPairCachingGhostObject;
class btTransform;
class btConvexShape;


class MCUController;
class PhysicalProjectileTemplate;
class WeaponTemplate;

class PhysicalProjectile : public Projectile, public btMotionState
{
public:
	PhysicalProjectile(MCUController* source, const FireInstruction& instruction, PhysicalProjectileTemplate* projTemplate);
	~PhysicalProjectile();

	PhysicalProjectileTemplate* getPhysicalProjectileTemplate();

	void projectileAdded();
	void updateImpl(const Ogre::Real& timeSinceLastFrame);
	bool isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance);

	bool isVisible();
	void setVisible(bool value);

	void getWorldTransform(btTransform &ret) const;
	void setWorldTransform(const btTransform &in);

	Ogre::SceneNode* getNode();
	void setNode(Ogre::SceneNode *node);

	Ogre::Entity* getEntity();
	void setEntity(Ogre::Entity* entity);

	static const Ogre::Real MaxProjectileLifetime;
private:
	PhysicalProjectileTemplate* mPhysicalProjectileTemplate;
	btTransform mTransform;
	btTransform mCenterOfMassOffset;
	btRigidBody* mRigidBody;
	btCylinderShape* mCylinderShape;
	Ogre::SceneNode *mNode;
	Ogre::Entity* mEntity;
	Ogre::ManualObject* mDebug;
	Ogre::Real mProjectileLifetime;
	btVector3 mDirectionOfTravel;
	Ogre::Vector3 mFiringAngle;
	Ogre::Real mDistanceToTarget;
	Ogre::SceneManager* mSceneManager;
};
#endif