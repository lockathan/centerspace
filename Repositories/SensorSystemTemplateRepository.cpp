#include <Repositories/SensorSystemTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/SensorSystemTemplate.h>

SensorSystemTemplateRepository::SensorSystemTemplateRepository()
{

}

SensorSystemTemplateRepository::~SensorSystemTemplateRepository()
{

}

std::vector<SensorSystemTemplate*> SensorSystemTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<SensorSystemTemplate*> result;
	std::vector<SensorSystemTemplate*>::iterator componentIterator;

	for(componentIterator = this->begin(); componentIterator != this->end(); ++componentIterator)
	{
		if((*componentIterator))
		{
			if(((*componentIterator)->getFactionTemplate()->isGenericFaction() || (*componentIterator)->getFactionTemplate()->getId() == factionId) &&
				(*componentIterator)->getRequiredRank() <= rank &&
				(*componentIterator)->getSize() == size)
				result.push_back((*componentIterator));
		}
	}

	return result;
}

SensorSystemTemplateRepository* SensorSystemTemplateRepository::mSensorSystemTemplateRepository;

SensorSystemTemplateRepository* SensorSystemTemplateRepository::getSingletonPtr()
{
	if(!mSensorSystemTemplateRepository)
	{
		mSensorSystemTemplateRepository = new SensorSystemTemplateRepository();
	}
	return mSensorSystemTemplateRepository;
}