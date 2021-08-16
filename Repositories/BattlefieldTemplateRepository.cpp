#include <Repositories/BattlefieldTemplateRepository.h>
#include <Templates/HubTemplate.h>

BattlefieldTemplateRepository::BattlefieldTemplateRepository()
{

}

BattlefieldTemplateRepository::~BattlefieldTemplateRepository()
{

}

std::deque<BattlefieldTemplate*> BattlefieldTemplateRepository::getAvailableTemplates()
{
	std::deque<BattlefieldTemplate*> result;
	std::vector<BattlefieldTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			result.push_back((*componentIterator));
		}
	}

	return result;
}

std::deque<BattlefieldTemplate*> BattlefieldTemplateRepository::getAvailableTemplates(int numMCUs)
{
	std::deque<BattlefieldTemplate*> result;
	std::vector<BattlefieldTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator) && (*componentIterator)->getMinMCUs() <= numMCUs)
		{
			result.push_back((*componentIterator));
		}
	}

	return result;
}

BattlefieldTemplateRepository* BattlefieldTemplateRepository::mBattlefieldTemplateRepository;

BattlefieldTemplateRepository* BattlefieldTemplateRepository::getSingletonPtr()
{
	if(!mBattlefieldTemplateRepository)
	{
		mBattlefieldTemplateRepository = new BattlefieldTemplateRepository();
	}
	return mBattlefieldTemplateRepository;
}