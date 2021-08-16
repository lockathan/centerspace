#ifndef FactionHubTemplateRepository_h
#define FactionHubTemplateRepository_h

#include "Ogre.h"
#include <Templates/FactionHubTemplate.h>
#include <Repositories/Repository.h>

class FactionHubTemplateRepository : public Repository<FactionHubTemplate>
{
public:
	FactionHubTemplate* getFactionHubTemplate(int factionId, int hubId);
	static FactionHubTemplateRepository* getSingletonPtr();
	~FactionHubTemplateRepository();
private:
	FactionHubTemplateRepository();
	static FactionHubTemplateRepository* mFactionHubTemplateRepository;
};
#endif