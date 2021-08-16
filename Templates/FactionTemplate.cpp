#include <Templates/FactionTemplate.h>
#include <Ogre.h>
#include <string>
#include <hash_map>

FactionTemplate::FactionTemplate()
	:
	mIsGenericFaction(false),
	mDescription(""),
	mStartScript(""),
	mStartEntrypoint("")
{

}

FactionTemplate::~FactionTemplate()
{

}

std::string FactionTemplate::getStartScript()
{
	return mStartScript;
}

void FactionTemplate::setStartScript(const std::string& script)
{
	mStartScript = script;
}

std::string FactionTemplate::getStartEntrypoint()
{
	return mStartEntrypoint;
}

void FactionTemplate::setStartEntrypoint(const std::string& entry)
{
	mStartEntrypoint = entry;
}

std::string FactionTemplate::getDescription()
{
	return mDescription;
}


void FactionTemplate::setDescription(const std::string& desc)
{
	mDescription = desc;
}


bool FactionTemplate::isGenericFaction()
{
	return mIsGenericFaction;
}

void FactionTemplate::setIsGenericFaction(bool value)
{
	mIsGenericFaction = value;
}
