#include <Repositories/WeaponTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Templates/Enums.h>
WeaponTemplateRepository::WeaponTemplateRepository()
{

}

WeaponTemplateRepository::~WeaponTemplateRepository()
{

}

std::vector<WeaponTemplate*> WeaponTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<WeaponTemplate*> result;
	std::vector<WeaponTemplate*>::iterator componentIterator;

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

std::vector<WeaponTemplate*> WeaponTemplateRepository::getAvailableTemplates(int factionId, int rank, int size, HardpointType hardpointType)
{
	std::vector<WeaponTemplate*> result;
	std::vector<WeaponTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			if( ((*componentIterator)->getFactionTemplate()->isGenericFaction() || (*componentIterator)->getFactionTemplate()->getId() == factionId) &&
				(*componentIterator)->getRequiredRank() <= rank &&
				(*componentIterator)->getSize() <= size &&
				(*componentIterator)->canFit(hardpointType))
				result.push_back((*componentIterator));
		}
	}

	return result;
}

WeaponTemplateRepository* WeaponTemplateRepository::mWeaponTemplateRepository;

WeaponTemplateRepository* WeaponTemplateRepository::getSingletonPtr()
{
	if(!mWeaponTemplateRepository)
	{
		mWeaponTemplateRepository = new WeaponTemplateRepository();
	}
	return mWeaponTemplateRepository;
}