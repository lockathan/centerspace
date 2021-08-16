#include <Repositories/DriveTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/DriveTemplate.h>

DriveTemplateRepository::DriveTemplateRepository()
{

}

DriveTemplateRepository::~DriveTemplateRepository()
{

}

std::vector<DriveTemplate*> DriveTemplateRepository::getAvailableTemplates(int factionId, int rank,int size)
{
	std::vector<DriveTemplate*> result;
	std::vector<DriveTemplate*>::iterator componentIterator;

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

DriveTemplateRepository* DriveTemplateRepository::mDriveTemplateRepository;

DriveTemplateRepository* DriveTemplateRepository::getSingletonPtr()
{
	if(!mDriveTemplateRepository)
	{
		mDriveTemplateRepository = new DriveTemplateRepository();
	}
	return mDriveTemplateRepository;
}