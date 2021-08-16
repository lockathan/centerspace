#ifndef RandomMission_h
#define RandomMission_h

#include <Ogre.h>
#include <Entities/Mission.h>
class BattlefieldGrid;



class RandomMission : public Mission
{
public:
	enum RandomMissionType
	{
		INSTANT_ACTION = 0
	};
	RandomMission();

	RandomMissionType getRandomMissionType();
	void setRandomMissionType(RandomMissionType type);

	static std::string RandomMissionTypeToString(RandomMissionType type);

	std::string getDescription();
protected:
	RandomMissionType mRandomMissionType;
};
#endif