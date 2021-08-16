#ifndef ArmorTemplateRepository_h
#define ArmorTemplateRepository_h

#include "Ogre.h"
#include <Templates/ArmorTemplate.h>
#include <Repositories/Repository.h>

class ArmorTemplateRepository : public Repository<ArmorTemplate>
{
public:
	static ArmorTemplateRepository* getSingletonPtr();
	~ArmorTemplateRepository();
	
	std::vector<ArmorTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);

private:
	ArmorTemplateRepository();
	static ArmorTemplateRepository* mArmorTemplateRepository;
};
#endif