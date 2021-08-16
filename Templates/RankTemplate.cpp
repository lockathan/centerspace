#include <Templates/RankTemplate.h>
#include <string>
#include <hash_map>

RankTemplate::RankTemplate()
	:
	mFactionTemplate(0),
	mRequiredExperience(0),
	mRank(0)
{

}

RankTemplate::~RankTemplate()
{

}

FactionTemplate* RankTemplate::getFactionTemplate()
{
	return mFactionTemplate;
}

void RankTemplate::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

int RankTemplate::getRequiredExperience()
{
	return mRequiredExperience;
}

void RankTemplate::setRequiredExperience(int exp)
{
	mRequiredExperience = exp;
}

int RankTemplate::getRank()
{
	return mRank;
}

void RankTemplate::setRank(int rank)
{
	mRank = rank;
}