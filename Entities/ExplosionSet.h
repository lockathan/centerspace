#ifndef ExplosionSet_h
#define ExplosionSet_h

#include <Ogre.h>
#include <deque>
#include <Entities/MovableEntity.h>

class Explosion;

class ExplosionSet : public MovableEntity
{
public:
	ExplosionSet(const std::string& name, const Ogre::Real& duration);
	~ExplosionSet();

	std::string getMaterialName();
	void setMaterialName(const std::string& name);

	Ogre::Real getMaxDuration();
	void setMaxDuration(const Ogre::Real& duration);

	int getNumFrames();

	bool isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance);
	bool isVisible();
	void setVisible(bool value);

	void update(const Ogre::Real& timeSinceLastFrame);

	void createExplosion(const Ogre::Vector3& location);
	void removeBillboard(Ogre::Billboard* billboard);

private:
	Ogre::BillboardSet* mBillboardSet;
	std::deque<Explosion*> mExplosions;
	std::string mMaterialName;
	Ogre::Real mMaxDuration;
	Ogre::SceneNode* mParentSceneNode;
	int mNumFrames;
};

#endif