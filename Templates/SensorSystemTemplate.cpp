#include <Templates/SensorSystemTemplate.h>

SensorSystemTemplate::SensorSystemTemplate()
	:
	mSightRange(0),
	mRadarRange(0),
	mEnergyDrainPerTurn(0)
{

}

SensorSystemTemplate::~SensorSystemTemplate()
{

}

float SensorSystemTemplate::getSightRange()
{
	return mSightRange;
}

void SensorSystemTemplate::setSightRange(const float& range)
{
	mSightRange = range;
}

float SensorSystemTemplate::getRadarRange()
{
	return mRadarRange;
}

void SensorSystemTemplate::setRadarRange(const float& range)
{
	mRadarRange = range;
}

float SensorSystemTemplate::getEnergyDrainPerTurn()
{
	return mEnergyDrainPerTurn;
}

void SensorSystemTemplate::setEnergyDrainPerTurn(const float& drain)
{
	mEnergyDrainPerTurn = drain;
}