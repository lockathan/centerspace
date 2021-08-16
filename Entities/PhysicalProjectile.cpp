#include <Entities/PhysicalProjectile.h>
#include <Entities/Projectile.h>
#include <Entities/MovableEntity.h>
#include <Controllers/MCUController.h>
#include <Managers/GameManager.h>
#include <Managers/PhysicsManager.h>
#include <Managers/SQLiteDBManager.h>
#include <States/InBattleState.h>
#include <Templates/PhysicalProjectileTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <BtOgre/BtOgreExtras.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btVector3.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>

#include <Ogre.h>

using namespace Ogre;

const Real PhysicalProjectile::MaxProjectileLifetime = 5;

PhysicalProjectile::PhysicalProjectile(MCUController* source, const FireInstruction& instruction, PhysicalProjectileTemplate* projTemplate)
	:
	Projectile(source,instruction),
	mPhysicalProjectileTemplate(projTemplate),
	mTransform(),
	mCenterOfMassOffset(btTransform::getIdentity()),
	mRigidBody(0),
	mCylinderShape(0),
	mProjectileLifetime(0),
	mDirectionOfTravel(),
	mFiringAngle(),
	mDistanceToTarget(0),
	mDebug(0),
	mSceneManager(GameManager::getSingletonPtr()->getSceneManager())
{
	
	
}

PhysicalProjectile::~PhysicalProjectile()
{
	PhysicsManager::getSingletonPtr()->removeCollisionObject(mRigidBody);
	mSceneManager->destroyEntity(mEntity);
	mSceneManager->destroySceneNode(mNode);
	
	if(mDebug)
		mSceneManager->destroyManualObject(mDebug);

	delete mCylinderShape;
	delete mRigidBody;

}

PhysicalProjectileTemplate* PhysicalProjectile::getPhysicalProjectileTemplate()
{
	return mPhysicalProjectileTemplate;
}

void PhysicalProjectile::projectileAdded()
{

	mEntity = mSceneManager->createEntity("PhysicalProjectileEntity" + SQLiteDBManager::convertIntToString(this->getProjectileID()), this->getPhysicalProjectileTemplate()->getMeshName());
	mEntity->setMaterialName(this->getPhysicalProjectileTemplate()->getMaterialName());
	mNode = mSceneManager->createSceneNode("PhysicalProjectileNode" + SQLiteDBManager::convertIntToString(this->getProjectileID()));
	mNode->attachObject(mEntity);
	Quaternion q = this->getStartpoint().getRotationTo(this->getEndpoint());
	mNode->setOrientation(q);
	mTransform.setOrigin(BtOgre::Convert::toBullet(this->getStartpoint()));
	mTransform.setRotation(BtOgre::Convert::toBullet(q));

	btVector3 size = BtOgre::Convert::toBullet(mEntity->getBoundingBox().getSize());
	//size = size * 0.5f;

	mCylinderShape = new btCylinderShape(size);

	btScalar mass(mPhysicalProjectileTemplate->getWeight());
	btVector3 localInertia(0, 0, 0);

	mCylinderShape->calculateLocalInertia(mass, localInertia);

	mRigidBody = new btRigidBody(mass, this, mCylinderShape, localInertia);
	mRigidBody->setFriction(0.75f);
	mRigidBody->setDamping(0.5f,0.5f);
	mRigidBody->setGravity(btVector3(0,0,0));
	//mRigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	
	mSceneManager->getRootSceneNode()->addChild(mNode);
	mNode->translate(this->getStartpoint());
	PhysicsManager::getSingletonPtr()->addRigidBody(mRigidBody);
	

	mDistanceToTarget = this->getEndpoint().distance(this->getStartpoint());
	Vector3 impulse;

	if(mPhysicalProjectileTemplate->isArching())
	{
		mFiringAngle = (this->getEndpoint() + Vector3::UNIT_Y * (mDistanceToTarget * 0.6f)) - this->getStartpoint();
		mFiringAngle.normalise();
		//impulse = mFiringAngle * 100;

		//mRigidBody->applyCentralImpulse(BtOgre::Convert::toBullet(impulse));
	}
	else if(mPhysicalProjectileTemplate->isSelfPropelled())
	{
		mFiringAngle = (this->getEndpoint() + Vector3::UNIT_Y * (mDistanceToTarget * 0.75f)) - this->getStartpoint();
		mFiringAngle.normalise();
		//impulse = mFiringAngle * 50;

		//mRigidBody->applyCentralImpulse(BtOgre::Convert::toBullet(impulse));
	}
	else
	{
		mFiringAngle = this->getEndpoint() - this->getStartpoint();
		mFiringAngle.normalise();
		//impulse = mFiringAngle * 10;
		//mRigidBody->applyCentralImpulse(BtOgre::Convert::toBullet(impulse));
		//mRigidBody->applyCentralImpulse(BtOgre::Convert::toBullet(impulse));
	}
	mDirectionOfTravel = BtOgre::Convert::toBullet(impulse.normalisedCopy() * 2.f);
	

}

void PhysicalProjectile::updateImpl(const Real& timeSinceLastFrame)
{
	Vector3 directionToTarget = this->getEndpoint() - BtOgre::Convert::toOgre(mRigidBody->getWorldTransform().getOrigin());
	Real distanceRemaining = directionToTarget.normalise();
	Vector3 forceToApply;

	//mFiringAngle = Vector3::UNIT_Y;

	if(mDistanceToTarget != 0 && (this->getPhysicalProjectileTemplate()->isArching() || this->getPhysicalProjectileTemplate()->isSelfPropelled()))
	{
		Real amount = (mDistanceToTarget - distanceRemaining) / mDistanceToTarget;
		
		if(amount < 0.5)
			amount += 0.05;
		else
			amount -= 0.05;

		forceToApply = (amount * directionToTarget) + ((1 - amount) * mFiringAngle);
	}
	else
	{
		forceToApply = directionToTarget;
	}
	forceToApply.normalise();
	forceToApply = forceToApply * this->getPhysicalProjectileTemplate()->getMovementSpeed() * timeSinceLastFrame;

	mRigidBody->setLinearVelocity(BtOgre::Convert::toBullet(forceToApply * 100));

	forceToApply.normalise();
	forceToApply = forceToApply * timeSinceLastFrame * 20;
	ClosestNotMeConvexResultCallback callback = PhysicsManager::getSingletonPtr()->closestNotMeProjectileCollisionCallback(mRigidBody,mRigidBody->getWorldTransform().getOrigin(), mRigidBody->getWorldTransform().getOrigin() + BtOgre::Convert::toBullet(forceToApply), mCylinderShape);

	if(callback.getCollisionObjectMapping())
	{
		CollisionObjectMapping* mapping = callback.getCollisionObjectMapping();

		if(mapping->StaticGeometry)
		{
			GameManager::getSingletonPtr()->updateDebugText("NOT HIT");
			mReadyToBeDeleted = true;

		}

		if(mapping->MCUController && mapping->MCUController != this->getSource())
		{
			GameManager::getSingletonPtr()->updateDebugText("HIT");
			this->applyWarhead(mapping->MCUController);
		}
	}

	mProjectileLifetime+= timeSinceLastFrame;

	if(mProjectileLifetime >= MaxProjectileLifetime)
	{
		GameManager::getSingletonPtr()->updateDebugText("OUT OF TIME");
		mReadyToBeDeleted = true;
	}
	
}

bool PhysicalProjectile::isCloserThan(const Vector3& point, Ogre::Real squaredDistance)
{
	Vector3 loc = mNode->convertLocalToWorldPosition(mEntity->getBoundingBox().getCenter());
	return point.squaredDistance(loc) < squaredDistance;
}

bool PhysicalProjectile::isVisible()
{
	return mEntity->isVisible();
}

void PhysicalProjectile::setVisible(bool value)
{
	mEntity->setVisible(value);
}

void PhysicalProjectile::getWorldTransform(btTransform &ret) const
{
	ret = mTransform;
}

void PhysicalProjectile::setWorldTransform(const btTransform &in)
{
	if (mNode == NULL)
		return;

	mTransform = in;
	btTransform transform = in * mCenterOfMassOffset;

	btQuaternion rot = transform.getRotation();
	btVector3 pos = transform.getOrigin();
	mNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	mNode->setPosition(pos.x(), pos.y(), pos.z());
}

Ogre::SceneNode* PhysicalProjectile::getNode()
{
	return mNode;
}

void PhysicalProjectile::setNode(Ogre::SceneNode *node)
{
	mNode = node;
}

Ogre::Entity* PhysicalProjectile::getEntity()
{
	return mEntity;
}

void PhysicalProjectile::setEntity(Ogre::Entity* entity)
{
	mEntity = entity;
}
