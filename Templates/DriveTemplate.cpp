#include <Templates/DriveTemplate.h>
#include <string>
#include <hash_map>

DriveTemplate::DriveTemplate()
	:
	mEfficiency(0),
	mJumpingEfficiency(0)
{

}

DriveTemplate::~DriveTemplate()
{

}

float DriveTemplate::getEfficiency()
{
	return mEfficiency;
}

void DriveTemplate::setEfficiency(float e)
{
	mEfficiency = e;
}

float DriveTemplate::getJumpingEfficiency()
{
	return mJumpingEfficiency;
}

void DriveTemplate::setJumpingEfficiency(float e)
{
	mJumpingEfficiency = e;
}