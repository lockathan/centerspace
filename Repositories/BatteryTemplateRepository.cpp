#include <Repositories/BatteryTemplateRepository.h>
#include <Templates/BatteryTemplate.h>
#include <Templates/FactionTemplate.h>

BatteryTemplateRepository::BatteryTemplateRepository()
{

}

BatteryTemplateRepository::~BatteryTemplateRepository()
{

}

std::vector<BatteryTemplate*> BatteryTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<BatteryTemplate*> result;
	std::vector<BatteryTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			if(((*componentIterator)->getFactionTemplate()->isGenericFaction() || (*componentIterator)->getFactionTemplate()->getId() == factionId) && (*componentIterator)->getRequiredRank() <= rank && (*componentIterator)->getSize() == size)
				result.push_back((*componentIterator));
		}
	}

	return result;
}

BatteryTemplateRepository* BatteryTemplateRepository::mBatteryTemplateRepository;

BatteryTemplateRepository* BatteryTemplateRepository::getSingletonPtr()
{
	if(!mBatteryTemplateRepository)
	{
		mBatteryTemplateRepository = new BatteryTemplateRepository();
	}
	return mBatteryTemplateRepository;
}