#include <Templates/BattlefieldTemplate.h>
#include <Ogre.h>
#include <Templates/HubTemplate.h>
#include <string>
#include <hash_map>

BattlefieldTemplate::BattlefieldTemplate()
	:
	mInitialSize(0),
	mMinMCUs(0)
{

}

BattlefieldTemplate::~BattlefieldTemplate()
{

}

int BattlefieldTemplate::getInitialSize()
{
	return mInitialSize;
}

void BattlefieldTemplate::setInitialSize(int value)
{
	mInitialSize = value;
}

int BattlefieldTemplate::getMinMCUs()
{
	return mMinMCUs;
}

void BattlefieldTemplate::setMinMCUs(int value)
{
	mMinMCUs = value;
}
