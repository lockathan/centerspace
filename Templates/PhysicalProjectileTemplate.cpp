#include <Templates/PhysicalProjectileTemplate.h>
#include <Controllers/MCUController.h>
#include <Entities/PhysicalProjectile.h>
#include <States/InBattleState.h>
#include <Templates/WeaponTemplate.h>
#include <Ogre.h>

using namespace Ogre;

PhysicalProjectileTemplate::PhysicalProjectileTemplate()
	:
	ProjectileTemplate()
{

}

PhysicalProjectileTemplate::~PhysicalProjectileTemplate()
{

}

std::string PhysicalProjectileTemplate::getMeshName()
{
	return mMeshName;
}

void PhysicalProjectileTemplate::setMeshName(const std::string& name)
{
	mMeshName = name;
}

std::string PhysicalProjectileTemplate::getMaterialName()
{
	return mMaterialName;
}

void PhysicalProjectileTemplate::setMaterialName(const std::string& name)
{
	mMaterialName = name;
}

float PhysicalProjectileTemplate::getMovementSpeed()
{
	return mMovementSpeed;
}

void PhysicalProjectileTemplate::setMovementSpeed(const float& speed)
{
	mMovementSpeed = speed;
}

float PhysicalProjectileTemplate::getWeight()
{
	return mWeight;
}

void PhysicalProjectileTemplate::setWeight(const float& weight)
{
	mWeight = weight;
}

bool PhysicalProjectileTemplate::isSelfPropelled()
{
	return mSelfPropelled;
}

void PhysicalProjectileTemplate::setSelfPropelled(bool value)
{
	mSelfPropelled = value;
}

bool PhysicalProjectileTemplate::isArching()
{
	return mArching;
}

void PhysicalProjectileTemplate::setArching(bool value)
{
	mArching = value;
}

void PhysicalProjectileTemplate::instantiate(MCUController* mcu, const FireInstruction& instruction)
{
	InBattleState::getSingletonPtr()->addProjectile(new PhysicalProjectile(mcu, instruction,this));
}
