#ifndef BattlefieldTemplateRepository_h
#define BattlefieldTemplateRepository_h

#include "Ogre.h"
#include <Templates/BattlefieldTemplate.h>
#include <Repositories/Repository.h>

class BattlefieldTemplateRepository : public Repository<BattlefieldTemplate>
{
public:
	std::deque<BattlefieldTemplate*> getAvailableTemplates();
	std::deque<BattlefieldTemplate*> getAvailableTemplates(int numMCUs);

	static BattlefieldTemplateRepository* getSingletonPtr();
	~BattlefieldTemplateRepository();
private:
	static BattlefieldTemplateRepository* mBattlefieldTemplateRepository;
	BattlefieldTemplateRepository();
};
#endif