#ifndef Explosion_h
#define Explosion_h

#include <Ogre.h>
#include <Entities/MovableEntity.h>

class ExplosionSet;

class Explosion : public MovableEntity
{
	
public:
	Explosion(ExplosionSet* parent, Ogre::Billboard* billboard);
	~Explosion();

	bool isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance);
	bool isVisible();
	void setVisible(bool value);

	void update(const Ogre::Real& timeSinceLastFrame);
private:
	Ogre::Real mElapsedTime;
	Ogre::Real mMaxDuration;
	Ogre::Real mInterval;
	Ogre::Billboard* mBillboard;
	ExplosionSet* mParent;
	int mNumFrames;

};

#endif