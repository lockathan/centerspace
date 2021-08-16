#ifndef DriveTemplateRepository_h
#define DriveTemplateRepository_h

#include "Ogre.h"
#include <Templates/DriveTemplate.h>
#include <Repositories/Repository.h>

class DriveTemplateRepository : public Repository<DriveTemplate>
{
public:
	static DriveTemplateRepository* getSingletonPtr();
	~DriveTemplateRepository();

	
	std::vector<DriveTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);
private:
	DriveTemplateRepository();
	static DriveTemplateRepository* mDriveTemplateRepository;
};
#endif