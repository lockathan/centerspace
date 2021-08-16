#include <Templates/ReactorTemplate.h>
#include <string>
#include <hash_map>

ReactorTemplate::ReactorTemplate()
{

}

ReactorTemplate::~ReactorTemplate()
{

}

float ReactorTemplate::getMaxEnergy()
{
	return mMaxEnergy;
}

void ReactorTemplate::setMaxEnergy(float maxEnergy)
{
	mMaxEnergy = maxEnergy;
}