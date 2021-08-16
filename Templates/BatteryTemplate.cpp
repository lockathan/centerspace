#include <Templates/BatteryTemplate.h>
#include <string>
#include <hash_map>

BatteryTemplate::BatteryTemplate()
{
	int temp = 0;
}

BatteryTemplate::~BatteryTemplate()
{

}

float BatteryTemplate::getMaxEnergy()
{
	return mMaxEnergy;
}

void BatteryTemplate::setMaxEnergy(float maxEnergy)
{
	mMaxEnergy = maxEnergy;
}