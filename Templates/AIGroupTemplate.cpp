#include <Templates/AIGroupTemplate.h>
#include <Entities/MechanizedCombatUnit.h>
#include <AI/AIGroup.h>
#include <Templates/AIMCUTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <vector>
#include <deque>
AIGroupTemplate::AIGroupTemplate()
{

}

AIGroupTemplate::~AIGroupTemplate()
{

}

std::deque<std::pair<int,AIMCUTemplate*>>& AIGroupTemplate::getAIMCUs()
{
	return mAIMCUS;
}

void AIGroupTemplate::setAIMCUs(std::deque<std::pair<int,AIMCUTemplate*>>& mcus)
{
	mAIMCUS = mcus;
}

void AIGroupTemplate::addAIMCU(int count,AIMCUTemplate* mcu)
{
	if(mcu && count > 0)
	{
		std::pair<int, AIMCUTemplate*> newMCU;
		newMCU.first = count;
		newMCU.second = mcu;
		mAIMCUS.push_back(newMCU);
	}
}

void AIGroupTemplate::addAIMCU(std::pair<int, AIMCUTemplate*> mcu)
{
	if(mcu.first > 0 && mcu.second)
	{
		mAIMCUS.push_back(mcu);
	}
}

std::deque<MechanizedCombatUnit*> AIGroupTemplate::getMCUs()
{
	std::deque<MechanizedCombatUnit*> result;
	std::deque<std::pair<int,AIMCUTemplate*>>::iterator groupIterator;

	for(groupIterator = mAIMCUS.begin();groupIterator != mAIMCUS.end();++groupIterator)
	{
		for(int i = 0;i < (*groupIterator).first; ++i)
		{
			result.push_back((*groupIterator).second->instantiate(true));
		}
	}

	return result;
}

AIGroupTemplate::AIGroupType AIGroupTemplate::getType()
{
	return mType;
}

void AIGroupTemplate::setType(AIGroupTemplate::AIGroupType type)
{
	mType = type;
}

void AIGroupTemplate::setType(int type)
{
	switch(type)
	{
	case AIGroupTemplate::AIGroupType::VERY_LIGHT:
		mType = AIGroupTemplate::AIGroupType::VERY_LIGHT;
	case AIGroupTemplate::AIGroupType::LIGHT:
		mType = AIGroupTemplate::AIGroupType::LIGHT;
	case AIGroupTemplate::AIGroupType::MEDIUM:
		mType = AIGroupTemplate::AIGroupType::MEDIUM;
	case AIGroupTemplate::AIGroupType::HEAVY:
		mType = AIGroupTemplate::AIGroupType::HEAVY;
	case AIGroupTemplate::AIGroupType::MIXED_LIGHT_AND_HEAVY:
		mType = AIGroupTemplate::AIGroupType::MIXED_LIGHT_AND_HEAVY;
	case AIGroupTemplate::AIGroupType::MIXED_LIGHT_MEDIUM_AND_HEAVY:
		mType = AIGroupTemplate::AIGroupType::MIXED_LIGHT_MEDIUM_AND_HEAVY;

	}
	return;
}

FactionTemplate* AIGroupTemplate::getFactionTemplate()
{
	return mFactionTemplate;
}

void AIGroupTemplate::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

int AIGroupTemplate::getSize()
{
	return mAIMCUS.size();
}

int AIGroupTemplate::getStrength()
{
	int baseStrength(0);

	std::deque<std::pair<int,AIMCUTemplate*>>::iterator iter;

	for(iter = mAIMCUS.begin(); iter != mAIMCUS.end(); ++iter)
	{
		if((*iter).second->getMCUTemplate()->getMCUType() == MCUType::MT_LIGHT)
		{
			baseStrength+= (*iter).first;
		}

		if((*iter).second->getMCUTemplate()->getMCUType() == MCUType::MT_MEDIUM)
		{
			baseStrength+= (*iter).first * 2;
		}

		if((*iter).second->getMCUTemplate()->getMCUType() == MCUType::MT_HEAVY)
		{
			baseStrength+= (*iter).first * 3;
		}
	}

	return baseStrength;
}

