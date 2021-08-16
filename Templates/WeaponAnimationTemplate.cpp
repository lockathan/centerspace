#include <Templates/WeaponAnimationTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Ogre.h>
#include <string>

using namespace Ogre;

WeaponAnimationTemplate::WeaponAnimationTemplate()
	:
	
	mHardpointNumber(0),
	mEnterAnimationSpeed(1.f),
	mFireAnimationSpeed(1.f),
	mExitAnimationSpeed(1.f),
	mEnterAnimationName(""),
	mFireAnimationName(""),
	mExitAnimationName("")
{

}

WeaponAnimationTemplate::~WeaponAnimationTemplate()
{

}


int WeaponAnimationTemplate::getHardpointNumber()
{
	return mHardpointNumber;
}

void WeaponAnimationTemplate::setHardpointNumber(int num)
{
	mHardpointNumber = num;
}
	
bool WeaponAnimationTemplate::hasEnterAnimation()
{
	return mEnterAnimationName != "" && mEnterAnimationName != "none" && mEnterAnimationName != "NONE";
}

std::string WeaponAnimationTemplate::getEnterAnimationName()
{
	return mEnterAnimationName;
}

void WeaponAnimationTemplate::setEnterAnimationName(std::string name)
{
	mEnterAnimationName = name;
}
	
std::string WeaponAnimationTemplate::getFireAnimationName()
{
	return mFireAnimationName;
}

void WeaponAnimationTemplate::setFireAnimationName(std::string name)
{
	mFireAnimationName = name;
}
	
bool WeaponAnimationTemplate::hasExitAnimation()
{
	return mExitAnimationName != "" && mExitAnimationName != "none" && mExitAnimationName != "NONE";
}

std::string WeaponAnimationTemplate::getExitAnimationName()
{
	return mExitAnimationName;
}

void WeaponAnimationTemplate::setExitAnimationName(std::string name)
{
	mExitAnimationName = name;
}

float WeaponAnimationTemplate::getEnterAnimationSpeed()
{
	return mEnterAnimationSpeed;
}

void WeaponAnimationTemplate::setEnterAnimationSpeed(float speed)
{
	mEnterAnimationSpeed = speed;
}

float WeaponAnimationTemplate::getFireAnimationSpeed()
{
	return mFireAnimationSpeed;
}

void WeaponAnimationTemplate::setFireAnimationSpeed(float speed)
{
	mFireAnimationSpeed = speed;
}

float WeaponAnimationTemplate::getExitAnimationSpeed()
{
	return mExitAnimationSpeed;
}

void WeaponAnimationTemplate::setExitAnimationSpeed(float speed)
{
	mExitAnimationSpeed = speed;
}
