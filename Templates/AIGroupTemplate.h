#ifndef AIGroupTemplate_h
#define AIGroupTemplate_h

#include "Ogre.h"
#include <Templates/RequiredRankTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <hash_map>
#include <string>
#include <deque>

class AIMCUTemplate;
class AIGroup;
class FactionTemplate;
class MechanizedCombatUnit;

class AIGroupTemplate : public RequiredRankTemplate
{
public:
	enum AIGroupType
	{
		VERY_LIGHT = 0,
		LIGHT = 1,
		MEDIUM = 2,
		HEAVY = 3,
		MIXED_LIGHT_AND_HEAVY = 4,
		MIXED_LIGHT_MEDIUM_AND_HEAVY = 5
	};

	AIGroupTemplate();
	~AIGroupTemplate();
	
	std::deque<std::pair<int,AIMCUTemplate*>>& getAIMCUs();
	void setAIMCUs(std::deque<std::pair<int,AIMCUTemplate*>>& mcus);
	void addAIMCU(int count, AIMCUTemplate* mcu);
	void addAIMCU(std::pair<int, AIMCUTemplate*> mcu);
	AIGroupType getType();
	void setType(AIGroupType type);
	void setType(int type);

	std::deque<MechanizedCombatUnit*> getMCUs();

	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);

	int getSize();
	int getStrength();

protected:
	
	AIGroupType mType;
	std::deque<std::pair<int,AIMCUTemplate*>> mAIMCUS;
	FactionTemplate* mFactionTemplate;
};
#endif