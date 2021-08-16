#include <Repositories/RandomMissionTypeTemplateRepository.h>
#include <Templates/RandomMissionTypeTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/HubTemplate.h>

RandomMissionTypeTemplateRepository::RandomMissionTypeTemplateRepository()
{

}

RandomMissionTypeTemplateRepository::~RandomMissionTypeTemplateRepository()
{

}

std::deque<RandomMissionTypeTemplate*> RandomMissionTypeTemplateRepository::getAvailableTemplates()
{
	std::deque<RandomMissionTypeTemplate*> result;
	std::vector<RandomMissionTypeTemplate*>::iterator iter;

	for(iter = this->begin(); iter != this->end(); ++iter)
	{
		if((*iter))
		{
			result.push_back((*iter));
		}
	}
	return result;
}

RandomMissionTypeTemplateRepository* RandomMissionTypeTemplateRepository::mRandomMissionTypeTemplateRepository;

RandomMissionTypeTemplateRepository* RandomMissionTypeTemplateRepository::getSingletonPtr()
{
	if(!mRandomMissionTypeTemplateRepository)
	{
		mRandomMissionTypeTemplateRepository = new RandomMissionTypeTemplateRepository();
	}
	return mRandomMissionTypeTemplateRepository;
}