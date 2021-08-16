#ifndef BeamProjectileTemplate_h
#define BeamProjectileTemplate_h

#include "Ogre.h"
#include <Templates/ProjectileTemplate.h>
#include <Controllers/MCUController.h>

class WeaponTemplate;

class BeamProjectileTemplate : public ProjectileTemplate
{
public:
	BeamProjectileTemplate();
	~BeamProjectileTemplate();

	Ogre::Real getDuration();
	void setDuration(const Ogre::Real& duration);

	std::string getMaterialName();
	void setMaterialName(const std::string& name);

	float getBeamWidth();
	void setBeamWidth(const float& width);

	void instantiate(MCUController* mcu, const FireInstruction& instruction);

private:
	Ogre::Real mDuration;
	std::string mMaterialName;
	float mBeamWidth;
};


#endif