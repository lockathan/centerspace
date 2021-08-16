#include <Templates/FactionHubTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/HubTemplate.h>
#include <string>
#include <hash_map>

FactionHubTemplate::FactionHubTemplate()
{

}

FactionHubTemplate::~FactionHubTemplate()
{

}

FactionTemplate* FactionHubTemplate::getPlayerFactionTemplate()
{
	return mPlayerFactionTemplate;
}

void FactionHubTemplate::setPlayerFactionTemplate(FactionTemplate* faction)
{
	mPlayerFactionTemplate = faction;
}

FactionTemplate* FactionHubTemplate::getEnemy1FactionTemplate()
{
	return mEnemy1FactionTemplate;
}

void FactionHubTemplate::setEnemy1FactionTemplate(FactionTemplate* faction)
{
	mEnemy1FactionTemplate = faction;
}

FactionTemplate* FactionHubTemplate::getEnemy2FactionTemplate()
{
	return mEnemy2FactionTemplate;
}

void FactionHubTemplate::setEnemy2FactionTemplate(FactionTemplate* faction)
{
	mEnemy2FactionTemplate = faction;
}

HubTemplate* FactionHubTemplate::getHubTemplate()
{
	return mHubTemplate;
}

void FactionHubTemplate::setHubTemplate(HubTemplate* hub)
{
	mHubTemplate = hub;
}