#ifndef RandomMissionTypeTemplateRepository_h
#define RandomMissionTypeTemplateRepository_h

#include "Ogre.h"
#include <Templates/RandomMissionTypeTemplate.h>
#include <Repositories/Repository.h>

#include <vector>

class RandomMissionTypeTemplateRepository : public Repository<RandomMissionTypeTemplate>
{
public:
	static RandomMissionTypeTemplateRepository* getSingletonPtr();
	~RandomMissionTypeTemplateRepository();

	std::deque<RandomMissionTypeTemplate*> getAvailableTemplates();
private:
	RandomMissionTypeTemplateRepository();
	static RandomMissionTypeTemplateRepository* mRandomMissionTypeTemplateRepository;
};
#endif