#ifndef ShieldTemplateRepository_h
#define ShieldTemplateRepository_h

#include "Ogre.h"
#include <Templates/ShieldTemplate.h>
#include <Repositories/Repository.h>

class ShieldTemplateRepository : public Repository<ShieldTemplate>
{
public:
	static ShieldTemplateRepository* getSingletonPtr();
	~ShieldTemplateRepository();
	
	std::vector<ShieldTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);

private:
	ShieldTemplateRepository();
	static ShieldTemplateRepository* mShieldTemplateRepository;
};
#endif