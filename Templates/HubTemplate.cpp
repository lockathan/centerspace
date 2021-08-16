#include <Templates/HubTemplate.h>
#include <string>
#include <hash_map>

HubTemplate::HubTemplate()
	:
	mBaseMissionPay(0),
	mBounty(0),
	mSystemName("")
{

}

HubTemplate::~HubTemplate()
{

}

std::string HubTemplate::getSystemName()
{
	return mSystemName;
}

void HubTemplate::setSystemName(std::string name)
{
	mSystemName = name;
}

float HubTemplate::getBaseMissionPay()
{
	return mBaseMissionPay;
}

void HubTemplate::setBaseMissionPay(float value)
{
	mBaseMissionPay = value;
}

float HubTemplate::getBounty()
{
	return mBounty;
}

void HubTemplate::setBounty(float value)
{
	mBounty = value;
}