#ifndef MovableEntity_h
#define MovableEntity_h

#include <Ogre.h>



class MovableEntity
{
public:
	MovableEntity() : mReadyToBeDeleted(false), mReadyToBeRemoved(false), mUsesVisibilityCalculations(false), mIsProjectile(false) {}
	MovableEntity(bool isProjectile) : mReadyToBeDeleted(false), mReadyToBeRemoved(false), mUsesVisibilityCalculations(false), mIsProjectile(isProjectile) {}

	virtual ~MovableEntity(){}

	virtual bool isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance) = 0;
	virtual bool isVisible() = 0;
	virtual void setVisible(bool value) = 0;
	bool usesVisibilityCalculations(){return mUsesVisibilityCalculations;}
	bool readyToBeDeleted(){ return mReadyToBeDeleted;}
	bool readyToBeRemoved(){ return mReadyToBeRemoved;}
	bool isProjectile(){ return mIsProjectile;}
	virtual void update(const Ogre::Real& timeSinceLastFrame) = 0;
protected:
	bool mReadyToBeDeleted;
	bool mReadyToBeRemoved;
	bool mUsesVisibilityCalculations;
	bool mIsProjectile;
};
#endif