#ifndef ProjectileTemplate_h
#define ProjectileTemplate_h

#include <Templates/Template.h>
#include <Controllers/MCUController.h>
#include <Ogre.h>

class WeaponTemplate;

enum ProjectileType
{
	LASER_INSTANT = 0,
	LASER_STRAIGHT = 1,
	BALLISTIC_STRAIGHT = 2,
	BALLISTIC_INDIRECT = 3,
	MISSILE_STRAIGHT = 4,
	MISSILE_INDIRECT = 5
};

class ProjectileTemplate : public Template
{
public:
	ProjectileTemplate() : mExplosionName("") {}
	virtual ~ProjectileTemplate() {}
	virtual void instantiate(MCUController* mcu, const FireInstruction& instruction) = 0;

	std::string getExplosionName(){return mExplosionName;}
	void setExplosionName(const std::string& name){mExplosionName = name;}

	std::string getFireSoundName(){return mFireSoundName;}
	void setFireSOundName(const std::string& name){mFireSoundName = name;}

	std::string getImpactSoundName(){return mImpactSoundName;}
	void setImpactSoundName(const std::string& name){mImpactSoundName = name;}
private:
	std::string mExplosionName;
	std::string mFireSoundName;
	std::string mImpactSoundName;
};

#endif