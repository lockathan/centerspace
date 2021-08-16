#ifndef Shield_h
#define Shield_h

#include <Ogre.h>

#include <LinearMath/btVector3.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>

#include <Entities/MovableEntity.h>

class btCollisionShape;
class btRigidBody;
class btCollisionWorld;
class btCollisionDispatcher;
class btGhostObject;
class btPairCachingGhostObject;
class btTransform;
class btConvexShape;


class MCUController;
class Projectile;
class ShieldTemplate;
class WeaponTemplate;

struct Warhead;

class Shield : public MovableEntity
{
public:
	Shield(ShieldTemplate* shield, MCUController* mcu, Ogre::SceneNode* node, const btTransform& transform);
	~Shield();
	
	void applyWarhead(Warhead& warhead);
	void update(const Ogre::Real& timeSinceLastFrame);
	void move(const btTransform& transform);
	void endTurn();

	float getMaxHealth();
	void setMaxHealth(const float& maxHealth);

	float getCurrentHealth();
	void setCurrentHealth(const float& currentHealth);

	float getRadius();
	void setRadius(const float& radius);

	bool isActive();
	void setIsActive(bool active);

	MCUController* getMCUController();
	void setMCUController(MCUController* mcu);

	ShieldTemplate* getShieldTemplate();
	void setShieldTemplate(ShieldTemplate* shield);

	Ogre::Entity* getEntity();
	void setEntity(Ogre::Entity* entity);

	Ogre::SceneNode* getMCUNode();
	void setMCUNode(Ogre::SceneNode* node);

	Ogre::SceneNode* getShieldNode();
	void setShieldNode(Ogre::SceneNode* node);

	btPairCachingGhostObject* getGhostObject();
	void setGhostObject(btPairCachingGhostObject* ghost);

	btConvexShape* getCollisionShape();
	void setCollisionShape(btConvexShape* shape);

	Ogre::Vector3 getCurrentPosition();
	void setCurrentPosition(Ogre::Vector3 pos);

	bool isVisible();
	void setVisible(bool value);
	bool isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance);

	void regenerate();
private:
	void buildShield();

	static const Ogre::Real ShieldActiveDuration;
	Ogre::Real mCurrentShieldLifetime;

	float mMaxHealth;
	float mCurrentHealth;
	
	float mRadius;
	
	bool mActive;

	MCUController* mMCUController;
	ShieldTemplate* mShieldTemplate;

	Ogre::Entity* mEntity;
	Ogre::SceneNode* mMCUNode;
	Ogre::SceneNode* mShieldNode;

	btPairCachingGhostObject* mGhostObject;
	btConvexShape* mConvexShape;//is also in m_ghostObject, but it needs to be convex, so we store it here to avoid upcast
	btManifoldArray mManifoldArray;
	Ogre::Vector3 mCurrentPosition;

	static const int numRings = 16;
	static const int numSegments = 16;
};
#endif