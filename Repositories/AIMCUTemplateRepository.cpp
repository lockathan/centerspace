#include <Repositories/AIMCUTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/AIMCUTemplate.h>

AIMCUTemplateRepository::AIMCUTemplateRepository()
{

}

AIMCUTemplateRepository::~AIMCUTemplateRepository()
{

}

std::vector<AIMCUTemplate*> AIMCUTemplateRepository::getAvailableAIMCUTemplates(int factionId, int rank)
{
	std::vector<AIMCUTemplate*> result;
	std::vector<AIMCUTemplate*>::iterator mcuIterator;

	for(mcuIterator = this->begin(); mcuIterator != this->end(); ++mcuIterator)
	{
		if((*mcuIterator))
		{
			if(((*mcuIterator)->getFactionTemplate()->isGenericFaction() || (*mcuIterator)->getFactionTemplate()->getId()) && (*mcuIterator)->getRequiredRank() <= rank)
			result.push_back((*mcuIterator));
		}
	}

	return result;
}

AIMCUTemplateRepository* AIMCUTemplateRepository::mAIMCUTemplateRepository;

AIMCUTemplateRepository* AIMCUTemplateRepository::getSingletonPtr()
{
	if(!mAIMCUTemplateRepository)
	{
		mAIMCUTemplateRepository = new AIMCUTemplateRepository();
	}
	return mAIMCUTemplateRepository;
}