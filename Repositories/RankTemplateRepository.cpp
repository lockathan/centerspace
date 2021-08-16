#include <Repositories/RankTemplateRepository.h>
#include <Templates/RankTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/HubTemplate.h>

RankTemplateRepository::RankTemplateRepository()
{

}

RankTemplateRepository::~RankTemplateRepository()
{

}

RankTemplate* RankTemplateRepository::getRank(int rank, FactionTemplate* faction)
{
	RankTemplate* result(0);

	std::vector<RankTemplate*>::iterator iter;

	for(iter = this->begin(); iter != this->end(); ++iter)
	{
		if((*iter))
		{
			if((*iter)->getRank() == rank && (*iter)->getFactionTemplate() == faction)
			{
				result = (*iter);
				break;
			}
		}
	}

	return result;
}
	
RankTemplate* RankTemplateRepository::getRankByExperience(int experience, FactionTemplate* faction)
{
	RankTemplate* result(0);

	std::vector<RankTemplate*>::iterator iter;
	int currentMaxExperience(0);
	for(iter = this->begin(); iter != this->end(); ++iter)
	{
		if((*iter))
		{
			if((*iter)->getRequiredExperience() <= experience && (*iter)->getFactionTemplate() == faction && (*iter)->getRequiredExperience() >= currentMaxExperience)
			{
				result = (*iter);
				currentMaxExperience = (*iter)->getRequiredExperience();
			}
		}
	}

	return result;
}

std::deque<RankTemplate*> RankTemplateRepository::getAvailableTemplates()
{
	std::deque<RankTemplate*> result;
	std::vector<RankTemplate*>::iterator iter;

	for(iter = this->begin(); iter != this->end(); ++iter)
	{
		if((*iter))
		{
			result.push_back((*iter));
		}
	}
	return result;
}

RankTemplateRepository* RankTemplateRepository::mRankTemplateRepository;

RankTemplateRepository* RankTemplateRepository::getSingletonPtr()
{
	if(!mRankTemplateRepository)
	{
		mRankTemplateRepository = new RankTemplateRepository();
	}
	return mRankTemplateRepository;
}