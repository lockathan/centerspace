#include <Repositories/MCUTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUTemplate.h>

MCUTemplateRepository::MCUTemplateRepository()
{

}

MCUTemplateRepository::~MCUTemplateRepository()
{

}

std::vector<MCUTemplate*> MCUTemplateRepository::getAvailableMCUTemplates(int factionId, int rank)
{
	std::vector<MCUTemplate*> result;
	std::vector<MCUTemplate*>::iterator mcuIterator;

	for(mcuIterator = this->begin(); mcuIterator != this->end(); ++mcuIterator)
	{
		if((*mcuIterator))
		{
			if(((*mcuIterator)->getFactionTemplate()->isGenericFaction() || (*mcuIterator)->getFactionTemplate()->getId() == factionId) && (*mcuIterator)->getRequiredRank() <= rank)
				result.push_back((*mcuIterator));
		}
	}

	return result;
}

MCUTemplateRepository* MCUTemplateRepository::mMCUTemplateRepository;

MCUTemplateRepository* MCUTemplateRepository::getSingletonPtr()
{
	if(!mMCUTemplateRepository)
	{
		mMCUTemplateRepository = new MCUTemplateRepository();
	}
	return mMCUTemplateRepository;
}