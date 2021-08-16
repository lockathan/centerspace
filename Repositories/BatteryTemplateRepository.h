#ifndef BatteryTemplateRepository_h
#define BatteryTemplateRepository_h

#include "Ogre.h"
#include <Templates/BatteryTemplate.h>
#include <Repositories/Repository.h>
#include <vector>

class BatteryTemplateRepository : public Repository<BatteryTemplate>
{
public:
	static BatteryTemplateRepository* getSingletonPtr();
	~BatteryTemplateRepository();

	std::vector<BatteryTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);

private:
	BatteryTemplateRepository();
	static BatteryTemplateRepository* mBatteryTemplateRepository;
};
#endif