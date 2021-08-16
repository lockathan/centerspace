#include <Templates/TargetingSystemTemplate.h>

TargetingSystemTemplate::TargetingSystemTemplate()
	:
	mBaseAccuracy(0),
	mObstructedBonus(0),
	mLongRangeBonus(0)
{

}
	
TargetingSystemTemplate::~TargetingSystemTemplate()
{

}

float TargetingSystemTemplate::getBaseAccuracy()
{
	return mBaseAccuracy;
}

void TargetingSystemTemplate::setBaseAccuracy(float value)
{
	mBaseAccuracy = value;
}
	
float TargetingSystemTemplate::getObstructedBonus()
{
	return mObstructedBonus;
}

void TargetingSystemTemplate::setObstructedBonus(float value)
{
	mObstructedBonus = value;
}
	
float TargetingSystemTemplate::getLongRangeBonus()
{
	return mLongRangeBonus;
}

void TargetingSystemTemplate::setLongRangeBonus(float value)
{
	mLongRangeBonus = value;
}