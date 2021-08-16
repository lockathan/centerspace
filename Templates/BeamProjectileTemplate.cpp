#include <Templates/BeamProjectileTemplate.h>
#include <Controllers/MCUController.h>
#include <Entities/BeamProjectile.h>
#include <States/InBattleState.h>
#include <Templates/WeaponTemplate.h>

#include <Ogre.h>

using namespace Ogre;

BeamProjectileTemplate::BeamProjectileTemplate()
	:
	ProjectileTemplate()
{

}

BeamProjectileTemplate::~BeamProjectileTemplate()
{

}

Real BeamProjectileTemplate::getDuration()
{
	return mDuration;
}

void BeamProjectileTemplate::setDuration(const Real& duration)
{
	mDuration = duration;
}

std::string BeamProjectileTemplate::getMaterialName()
{
	return mMaterialName;
}

void BeamProjectileTemplate::setMaterialName(const std::string& name)
{
	mMaterialName = name;
}

float BeamProjectileTemplate::getBeamWidth()
{
	return mBeamWidth;
}

void BeamProjectileTemplate::setBeamWidth(const float& width)
{
	mBeamWidth = width;
}

void BeamProjectileTemplate::instantiate(MCUController* mcu, const FireInstruction& instruction)
{
	InBattleState::getSingletonPtr()->addProjectile(new BeamProjectile(mcu, instruction, this));
}
