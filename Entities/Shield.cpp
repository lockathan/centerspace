#include <Entities/Shield.h>

#include <Controllers/MCUController.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MovableEntity.h>
#include <Entities/Projectile.h>
#include <Managers/GameManager.h>
#include <Managers/PhysicsManager.h>
#include <Managers/SQLiteDBManager.h>
#include <States/InBattleState.h>
#include <Templates/ShieldTemplate.h>
#include <Templates/WeaponTemplate.h>

#include "btBulletDynamicsCommon.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btDefaultMotionState.h>

#include <Ogre.h>

using namespace Ogre;

const Real Shield::ShieldActiveDuration = 10;
//TODO: Shield regeneration and reactivation
Shield::Shield(ShieldTemplate* shield, MCUController* mcu, Ogre::SceneNode* node, const btTransform& transform)
	:
	MovableEntity(),
	mCurrentShieldLifetime(0),
	mMaxHealth(shield->getMaxShieldHealth()),
	mCurrentHealth(shield->getMaxShieldHealth()),
	mRadius(0),
	mActive(true),
	mMCUController(mcu),
	mShieldTemplate(shield),
	mEntity(0),
	mMCUNode(node),
	mGhostObject(0),
	mConvexShape(0),
	mCurrentPosition(Vector3::ZERO)
{
	buildShield();
	SceneManager* sceneManager = GameManager::getSingletonPtr()->getSceneManager();
	mEntity = sceneManager->createEntity("Shield" + SQLiteDBManager::convertIntToString(mMCUController->getBattlefieldID()), "ShieldMesh" + SQLiteDBManager::convertIntToString(mMCUController->getBattlefieldID()));
	mShieldNode = sceneManager->createSceneNode("ShieldNode" + SQLiteDBManager::convertIntToString(mMCUController->getBattlefieldID()));
	mShieldNode->attachObject(mEntity);

	mMCUNode->addChild(mShieldNode);
	mShieldNode->setPosition(0.f,mRadius,0.f);

	mConvexShape = new btSphereShape(mRadius);
	mGhostObject = new btPairCachingGhostObject();
	mGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	mGhostObject->setWorldTransform(transform);
	mGhostObject->setCollisionShape(mConvexShape);
	mEntity->setVisible(false);
}

Shield::~Shield()
{
	PhysicsManager::getSingletonPtr()->removeCollisionObject(mGhostObject);
	SceneManager* sceneManager = GameManager::getSingletonPtr()->getSceneManager();
	sceneManager->destroyEntity(mEntity);
	sceneManager->destroySceneNode(mShieldNode);
}
	
void Shield::applyWarhead(Warhead& warhead)
{
	if(mCurrentHealth > 0)
	{
		mCurrentShieldLifetime = 0;
		mEntity->setVisible(true);
		InBattleState::getSingletonPtr()->addShield(this);
		warhead.ShieldWasActive = true;
		float mpreviousHealth(mCurrentHealth);
		mCurrentHealth -= warhead.TotalShieldDamage;
		GameManager::getSingletonPtr()->luaDebugReport("Shield hit for " + SQLiteDBManager::convertFloatToString(warhead.TotalShieldDamage) + " damage", 0);
		if(mCurrentHealth <= 0)
		{
			GameManager::getSingletonPtr()->luaDebugReport("Shields down", 0);
			warhead.RemainingShieldDamage += mCurrentHealth;
			warhead.ShieldWasActive = false;
			mEntity->setVisible(false);
			//InBattleState::getSingletonPtr()->removeShield(this);
			mReadyToBeRemoved = true;
			mActive = false;
			mCurrentHealth = 0;
		}
		else
		{
			warhead.RemainingShieldDamage -= (mpreviousHealth - mCurrentHealth);
		}
	}
	else
	{
		mActive = false;
		warhead.ShieldWasActive = false;
	}
}

void Shield::update(const Ogre::Real& timeSinceLastFrame)
{
	if(mEntity->isVisible())
	{
		mCurrentShieldLifetime+=timeSinceLastFrame;

		if(mCurrentHealth > ShieldActiveDuration)
		{
			mEntity->setVisible(false);
			//InBattleState::getSingletonPtr()->removeShield(this);
			mReadyToBeRemoved = true;
		}
	}
	
}

void Shield::move(const btTransform& transform)
{
	mGhostObject->setWorldTransform(transform);

}

void Shield::endTurn()
{

}

float Shield::getMaxHealth()
{
	return mMaxHealth;
}

void Shield::setMaxHealth(const float& maxHealth)
{
	mMaxHealth = maxHealth;
}

float Shield::getCurrentHealth()
{
	return mCurrentHealth;
}

void Shield::setCurrentHealth(const float& currentHealth)
{
	mCurrentHealth = currentHealth;
	if(mCurrentHealth > 0)
		mActive = true;
}

float Shield::getRadius()
{
	return mRadius;
}

void Shield::setRadius(const float& radius)
{
	mRadius = radius;
}

bool Shield::isActive()
{
	return mActive;
}

void Shield::setIsActive(bool active)
{
	mActive = active;
}


MCUController* Shield::getMCUController()
{
	return mMCUController;
}

void Shield::setMCUController(MCUController* mcu)
{
	mMCUController = mcu;
}

ShieldTemplate* Shield::getShieldTemplate()
{
	return mShieldTemplate;
}

void Shield::setShieldTemplate(ShieldTemplate* shield)
{
	mShieldTemplate = shield;
}

Ogre::Entity* Shield::getEntity()
{
	return mEntity;
}

void Shield::setEntity(Ogre::Entity* entity)
{
	mEntity = entity;
}

Ogre::SceneNode* Shield::getShieldNode()
{
	return mShieldNode;
}

void Shield::setShieldNode(Ogre::SceneNode* node)
{
	mShieldNode = node;
}

Ogre::SceneNode* Shield::getMCUNode()
{
	return mMCUNode;
}

void Shield::setMCUNode(Ogre::SceneNode* node)
{
	mMCUNode = node;
}

btPairCachingGhostObject* Shield::getGhostObject()
{
	return mGhostObject;
}

void Shield::setGhostObject(btPairCachingGhostObject* ghost)
{
	mGhostObject = ghost;
}

btConvexShape* Shield::getCollisionShape()
{
	return mConvexShape;
}

void Shield::setCollisionShape(btConvexShape* shape)
{
	mConvexShape = shape;
}

Ogre::Vector3 Shield::getCurrentPosition()
{
	return mCurrentPosition;
}

void Shield::setCurrentPosition(Ogre::Vector3 pos)
{
	mCurrentPosition = pos;
}

bool Shield::isVisible()
{
	return mEntity->getVisible();
}

void Shield::setVisible(bool value)
{
	mEntity->setVisible(value);
}

bool Shield::isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance)
{
	return true;
}

void Shield::buildShield()
{
	SceneManager* sceneManager = GameManager::getSingletonPtr()->getSceneManager();
	std::string name = "ShieldMesh" + SQLiteDBManager::convertIntToString(mMCUController->getBattlefieldID());

	Vector3 size = mMCUController->getEntity()->getBoundingBox().getSize();


	if(size.x > size.y && size.x > size.z)
	{
		mRadius = (size.x / 2.f) + 0.5f;
	}
	else if(size.y > size.x && size.y > size.z)
	{
		mRadius = (size.y / 2.f) + 0.5f;
	}
	else
	{
		mRadius = (size.z / 2.f) + 0.5f;
	}

	ManualObject * manual = sceneManager->createManualObject(name);
	manual->begin(mShieldTemplate->getMaterialName(), RenderOperation::OT_TRIANGLE_LIST);
 
	float fDeltaRingAngle = (Math::PI / numRings);
	float fDeltaSegAngle = (2 * Math::PI / numSegments);
	unsigned short wVerticeIndex = 0 ;
 
	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= numRings; ++ring )
	{
		float r0 = mRadius * sinf (ring * fDeltaRingAngle);
		float y0 = mRadius * cosf (ring * fDeltaRingAngle);
 
		// Generate the group of segments for the current ring
		for(int seg = 0; seg <= numSegments; ++seg) 
		{
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);
 
			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, y0, z0);
			manual->normal(Vector3(x0, y0, z0).normalisedCopy());
			manual->textureCoord((float) seg / (float) numSegments, (float) ring / (float) numRings);
 
			if (ring != numRings) 
			{
				// each vertex (except the last) has six indicies pointing to it
				manual->index(wVerticeIndex + numSegments + 1);
				manual->index(wVerticeIndex);               
				manual->index(wVerticeIndex + numSegments);
				manual->index(wVerticeIndex + numSegments + 1);
				manual->index(wVerticeIndex + 1);
				manual->index(wVerticeIndex);
				++wVerticeIndex;
			}
		}
		// end for seg
	} // end for ring
	manual->end();
	MeshPtr mesh = manual->convertToMesh(name);
	mesh->_setBounds( AxisAlignedBox( Vector3(-mRadius, -mRadius, -mRadius), Vector3(mRadius, mRadius, mRadius) ), false );
 
	mesh->_setBoundingSphereRadius(mRadius);
	unsigned short src, dest;
	if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
	{
		mesh->buildTangentVectors(VES_TANGENT, src, dest);
	}
	sceneManager->destroyManualObject(manual);

}

void Shield::regenerate()
{
	if(mCurrentHealth < mMaxHealth)
	{
		mCurrentHealth += mShieldTemplate->getRegenRate();
		if(mCurrentHealth > mMaxHealth)
		{
			mCurrentHealth = mMaxHealth;
		}
	}
}
