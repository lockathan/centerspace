#include <Entities/Explosion.h>
#include <Entities/ExplosionSet.h>
#include <Managers/GameManager.h>
#include <Entities/MovableEntity.h>
#include <Ogre.h>

using namespace Ogre;

Explosion::Explosion(ExplosionSet* parent, Billboard* billboard)
	:
	MovableEntity(),
	mElapsedTime(0),
	mMaxDuration(parent->getMaxDuration()),
	mParent(parent),
	mBillboard(billboard),
	mNumFrames(mParent->getNumFrames())
{
	mInterval = mMaxDuration / mNumFrames;
}

Explosion::~Explosion()
{
	
	mParent->removeBillboard(mBillboard);
}

bool Explosion::isVisible()
{
	return true;
}

void Explosion::setVisible(bool value)
{
	//

}

bool Explosion::isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance)
{
	return true;
}

void Explosion::update(const Real& timeSinceLastFrame)
{
	Light* mylight(0);
	
	mElapsedTime+=timeSinceLastFrame;

	if(mElapsedTime > mMaxDuration)
	{
		mReadyToBeDeleted = true;
		return;
	}

	int frame = mElapsedTime / mInterval;
	if(frame > (mNumFrames - 1))
	{
		frame = mNumFrames - 1;
		mReadyToBeDeleted = true;
	}
	mBillboard->setTexcoordIndex(frame);

}
