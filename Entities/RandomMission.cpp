#include <Entities/RandomMission.h>
#include <Templates/TerrainTemplate.h>
#include <string>

RandomMission::RandomMission()
{
	mRandomMissionType = RandomMissionType::INSTANT_ACTION;
}
RandomMission::RandomMissionType RandomMission::getRandomMissionType()
{
	return mRandomMissionType;
}

void RandomMission::setRandomMissionType(RandomMission::RandomMissionType type)
{
	mRandomMissionType = type;
}

std::string RandomMission::RandomMissionTypeToString(RandomMission::RandomMissionType type)
{
	switch(type)
	{
	case RandomMission::RandomMissionType::INSTANT_ACTION:
		return "Instant Action";
	}
	return "";
}

std::string RandomMission::getDescription()
{
	return RandomMissionTypeToString(mRandomMissionType) + " on " + this->getTerrainTemplate()->getName();
}