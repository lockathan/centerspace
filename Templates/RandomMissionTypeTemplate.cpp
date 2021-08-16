#include <Templates/RandomMissionTypeTemplate.h>
#include <string>

RandomMissionTypeTemplate::RandomMissionTypeTemplate()
	:
	mPlacementScriptFilename(""),
	mPlayerPlacementEntryPoint(""),
	mEnemyGenerationFilename(""),
	mEnemyGenerationEntryPoint(""),
	mEnemyPlacementEntryPoint("")
{

}

RandomMissionTypeTemplate::~RandomMissionTypeTemplate()
{

}

std::string RandomMissionTypeTemplate::getPlacementScriptFilename()
{
	return mPlacementScriptFilename;
}

void RandomMissionTypeTemplate::setPlacementScriptFilename(const std::string& file)
{
	mPlacementScriptFilename = file;
}

std::string RandomMissionTypeTemplate::getPlayerPlacementEntryPoint()
{
	return mPlayerPlacementEntryPoint;
}

void RandomMissionTypeTemplate::setPlayerPlacementEntryPoint(const std::string& entry)
{
	mPlayerPlacementEntryPoint = entry;
}

std::string RandomMissionTypeTemplate::getEnemyGenerationFilename()
{
	return mEnemyGenerationFilename;
}

void RandomMissionTypeTemplate::setEnemyGenerationFilename(const std::string& filename)
{
	mEnemyGenerationFilename = filename;
}

std::string RandomMissionTypeTemplate::getEnemyGenerationEntryPoint()
{
	return mEnemyGenerationEntryPoint;
}

void RandomMissionTypeTemplate::setEnemyGenerationEntryPoint(const std::string& entry)
{
	mEnemyGenerationEntryPoint = entry;
}

std::string RandomMissionTypeTemplate::getEnemyPlacementEntryPoint()
{
	return mEnemyPlacementEntryPoint;
}

void RandomMissionTypeTemplate::setEnemyPlacementEntryPoint(const std::string& entry)
{
	mEnemyPlacementEntryPoint = entry;
}
