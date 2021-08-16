#ifndef Projectile_h
#define Projectile_h

#include <Ogre.h>
#include <Controllers/MCUController.h>
#include <Entities/MovableEntity.h>
#include <OgreAL.h>

class WeaponTemplate;

class Projectile : public MovableEntity
{
public:
	Projectile();
	Projectile(MCUController* source, const FireInstruction& instruction);
	virtual ~Projectile();
	
	Ogre::Vector3 getStartpoint();
	void setStartpoint(const Ogre::Vector3& start);

	Ogre::Vector3 getEndpoint();
	void setEndpoint(const Ogre::Vector3& end);

	int getProjectileID();
	void setProjectileID(int id);
	
	WeaponTemplate* getWeaponTemplate();

	MCUController* getSource();
	void setSource(MCUController* source);
	
	FireInstruction getFireInstruction();

	void applyWarhead(MCUController* target);

	virtual void projectileAdded() = 0;

	void update(const Ogre::Real& timeSinceLastFrame);
	virtual void updateImpl(const Ogre::Real& timeSinceLastFrame) = 0;
protected:
	bool isOwnerVisible();
private:
	FireInstruction mFireInstruction;
	MCUController* mSource;
	int mProjectileID;
	OgreAL::Sound* mFireSound;
	OgreAL::Sound* mImpactSound;
	bool mPlayedFireSound;
	bool mPlayedImpactSound;
	bool mWaitingForSoundToFinish;
	bool mAppliedWarhead;
};
#endif