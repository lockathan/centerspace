#ifndef TargetingSystemTemplateRepository_h
#define TargetingSystemTemplateRepository_h

#include "Ogre.h"
#include <Templates/TargetingSystemTemplate.h>
#include <Repositories/Repository.h>

class TargetingSystemTemplateRepository : public Repository<TargetingSystemTemplate>
{
public:
	static TargetingSystemTemplateRepository* getSingletonPtr();
	~TargetingSystemTemplateRepository();

	
	std::vector<TargetingSystemTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);

private:
	TargetingSystemTemplateRepository();
	static TargetingSystemTemplateRepository* mTargetingSystemTemplateRepository;
};
#endif