#include <Repositories/ArmorTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/ArmorTemplate.h>

ArmorTemplateRepository::ArmorTemplateRepository()
{

}

ArmorTemplateRepository::~ArmorTemplateRepository()
{

}

std::vector<ArmorTemplate*> ArmorTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<ArmorTemplate*> result;
	std::vector<ArmorTemplate*>::iterator componentIterator;

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


ArmorTemplateRepository* ArmorTemplateRepository::mArmorTemplateRepository;

ArmorTemplateRepository* ArmorTemplateRepository::getSingletonPtr()
{
	if(!mArmorTemplateRepository)
	{
		mArmorTemplateRepository = new ArmorTemplateRepository();
	}
	return mArmorTemplateRepository;
}