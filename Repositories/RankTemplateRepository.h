#ifndef RankTemplateRepository_h
#define RankTemplateRepository_h

#include "Ogre.h"
#include <Templates/RankTemplate.h>
#include <Repositories/Repository.h>

#include <vector>

class RankTemplateRepository : public Repository<RankTemplate>
{
public:
	static RankTemplateRepository* getSingletonPtr();
	~RankTemplateRepository();

	RankTemplate* getRank(int rank, FactionTemplate* faction);
	RankTemplate* getRankByExperience(int experience, FactionTemplate* faction);

	std::deque<RankTemplate*> getAvailableTemplates();
private:
	RankTemplateRepository();
	static RankTemplateRepository* mRankTemplateRepository;
};
#endif