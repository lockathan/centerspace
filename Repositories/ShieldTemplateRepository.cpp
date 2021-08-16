#include <Repositories/ShieldTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/ShieldTemplate.h>

ShieldTemplateRepository::ShieldTemplateRepository()
{

}

ShieldTemplateRepository::~ShieldTemplateRepository()
{

}

std::vector<ShieldTemplate*> ShieldTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<ShieldTemplate*> result;
	std::vector<ShieldTemplate*>::iterator componentIterator;

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


ShieldTemplateRepository* ShieldTemplateRepository::mShieldTemplateRepository;

ShieldTemplateRepository* ShieldTemplateRepository::getSingletonPtr()
{
	if(!mShieldTemplateRepository)
	{
		mShieldTemplateRepository = new ShieldTemplateRepository();
	}
	return mShieldTemplateRepository;
}