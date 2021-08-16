#ifndef BeamProjectile_h
#define BeamProjectile_h


#include <Entities/Projectile.h>
#include <Ogre.h>

class BeamProjectileTemplate;
class MCUController;
class WeaponTemplate;

class BeamProjectile : public Projectile
{
public:
	BeamProjectile(MCUController* source, const FireInstruction& instruction,BeamProjectileTemplate* beamTemplate);
	~BeamProjectile();

	BeamProjectileTemplate* getBeamProjectileTemplate();
	Ogre::Real getTimeElapsed();

	void projectileAdded();
	void updateImpl(const Ogre::Real& timeSinceLastFrame);

	bool isCloserThan(const Ogre::Vector3& point, Ogre::Real squaredDistance);
	bool isVisible();
	void setVisible(bool value);
private:
	Ogre::BillboardChain* mBeamBillboard;
	Ogre::SceneNode* mBeamNode;
	Ogre::Real mTimeElapsed;
	BeamProjectileTemplate* mBeamProjectileTemplate;
};
#endif