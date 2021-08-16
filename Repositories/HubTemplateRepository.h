#ifndef HubTemplateRepository_h
#define HubTemplateRepository_h

#include "Ogre.h"
#include <Templates/HubTemplate.h>
#include <Repositories/Repository.h>

#include <vector>

class HubTemplateRepository : public Repository<HubTemplate>
{
public:
	static HubTemplateRepository* getSingletonPtr();
	~HubTemplateRepository();

	std::vector<HubTemplate*> getAvailableTemplates(int rank);
private:
	HubTemplateRepository();
	static HubTemplateRepository* mHubTemplateRepository;
};
#endif