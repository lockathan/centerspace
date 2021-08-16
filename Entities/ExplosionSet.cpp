#include <Entities/Explosion.h>
#include <Entities/ExplosionSet.h>
#include <Entities/MovableEntity.h>
#include <Managers/GameManager.h>

#include <Ogre.h>
#include <deque>

using namespace Ogre;

ExplosionSet::ExplosionSet(const std::string& name, const Ogre::Real& duration)
	:
	MovableEntity(),
	mMaterialName(name),
	mMaxDuration(duration),
	mBillboardSet(0),
	mParentSceneNode(0),
	mNumFrames(0)
{
	SceneManager* manager = GameManager::getSingletonPtr()->getSceneManager();

	mBillboardSet = manager->createBillboardSet(mMaterialName + "BillboardSet");
	mBillboardSet->setDefaultDimensions(10, 10);
	mParentSceneNode = manager->createSceneNode(mMaterialName + "SceneNode");
	mParentSceneNode->attachObject(mBillboardSet);
	mBillboardSet->setMaterialName(mMaterialName);
	mBillboardSet->setTextureStacksAndSlices(4, 4);
	mNumFrames = 16;
	manager->getRootSceneNode()->addChild(mParentSceneNode);
}

ExplosionSet::~ExplosionSet()
{
	std::deque<Explosion*>::iterator explosion;
	Billboard* b = new Billboard();
	
	for(explosion = mExplosions.begin(); explosion != mExplosions.end();++explosion)
	{
		
		delete (*explosion);
	}

	SceneManager* manager = GameManager::getSingletonPtr()->getSceneManager();
	if(mBillboardSet)
		manager->destroyBillboardSet(mBillboardSet);

	if(mParentSceneNode)
		manager->destroySceneNode(mParentSceneNode);
}

std::string ExplosionSet::getMaterialName()
{
	return mMaterialName;
}

void ExplosionSet::setMaterialName(const std::string& name)
{
	mMaterialName = name;
}

Ogre::Real ExplosionSet::getMaxDuration()
{
	return mMaxDuration;
}

void ExplosionSet::setMaxDuration(const Ogre::Real& duration)
{
	mMaxDuration = duration;
}

int ExplosionSet::getNumFrames()
{
	return mNumFrames;
}

bool ExplosionSet::isVisible()
{
	return mBillboardSet->isVisible();
}

void ExplosionSet::setVisible(bool value)
{
	//mBillboardSet->setVisible(value);

}

bool ExplosionSet::isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance)
{
	return true;
}

void ExplosionSet::update(const Real& timeSinceLastFrame)
{
	std::deque<Explosion*>::iterator explosion;

	for(explosion = mExplosions.begin(); explosion != mExplosions.end();)
	{
		if((*explosion)->readyToBeDeleted())
		{
			delete (*explosion);
			explosion = mExplosions.erase(explosion);
		}
		else
		{
			++explosion;
		}
	}

	for(explosion = mExplosions.begin(); explosion != mExplosions.end(); ++explosion)
	{
		(*explosion)->update(timeSinceLastFrame);
	}
}

void ExplosionSet::createExplosion(const Ogre::Vector3& location)
{
	Billboard* billboard = mBillboardSet->createBillboard(location);
	Explosion* e = new Explosion(this, billboard);
	mExplosions.push_back(e);
}

void ExplosionSet::removeBillboard(Ogre::Billboard* billboard)
{
	mBillboardSet->removeBillboard(billboard);
}
