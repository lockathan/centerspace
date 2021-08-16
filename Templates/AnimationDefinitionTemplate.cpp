#include <Templates/AnimationDefinitionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Ogre.h>
#include <string>
#include <vector>

using namespace Ogre;

AnimationDefinitionTemplate::AnimationDefinitionTemplate()
{
	mMCUTemplate = 0;
	mWeaponAnimations.resize(10);
}

AnimationDefinitionTemplate::~AnimationDefinitionTemplate()
{

}

std::string AnimationDefinitionTemplate::getWalkAnimationName()
{
	return mWalkAnimationName;
}

void AnimationDefinitionTemplate::setWalkAnimationName(std::string name)
{
	mWalkAnimationName = name;
}

std::string AnimationDefinitionTemplate::getGetHitAnimationName()
{
	return mGetHitAnimationName;
}

void AnimationDefinitionTemplate::setGetHitAnimationName(std::string name)
{
	mGetHitAnimationName = name;
}

std::string AnimationDefinitionTemplate::getTakeOffAnimationName()
{
	return mTakeOffAnimationName;
}

void AnimationDefinitionTemplate::setTakeOffAnimationName(std::string name)
{
	mTakeOffAnimationName = name;
}

std::string AnimationDefinitionTemplate::getLandAnimationName()
{
	return mLandAnimationName;
}

void AnimationDefinitionTemplate::setLandAnimationName(std::string name)
{
	mLandAnimationName = name;
}

std::string AnimationDefinitionTemplate::getFlyAnimationName()
{
	return mFlyAnimationName;

}
void AnimationDefinitionTemplate::setFlyAnimationName(std::string name)
{
	mFlyAnimationName = name;
}

std::string AnimationDefinitionTemplate::getIdleAnimationName()
{
	return mIdleAnimationName;
}

void AnimationDefinitionTemplate::setIdleAnimationName(std::string name)
{
	mIdleAnimationName = name;
}

std::string AnimationDefinitionTemplate::getDeathAnimationName()
{
	return mDeathAnimationName;
}

void AnimationDefinitionTemplate::setDeathAnimationName(std::string name)
{
	mDeathAnimationName = name;
}

MCUTemplate* AnimationDefinitionTemplate::getMCUTemplate()
{
	return mMCUTemplate;
}

void AnimationDefinitionTemplate::setMCUTemplate(MCUTemplate* mcu)
{
	mMCUTemplate = mcu;
	mMCUTemplate->setAnimationDefinitionTemplate(this);
}

int AnimationDefinitionTemplate::getNumWeaponAnimations()
{
	return mWeaponAnimations.size();
}

void AnimationDefinitionTemplate::setNumWeaponAnimations(int num)
{
	mWeaponAnimations.resize(num);
}

WeaponAnimationTemplate* AnimationDefinitionTemplate::getWeaponAnimation(int n)
{
	return mWeaponAnimations[n];
}

void AnimationDefinitionTemplate::addWeaponAnimationTemplate(int hardpoint, WeaponAnimationTemplate* anim)
{
	mWeaponAnimations[hardpoint] = anim;
}
	