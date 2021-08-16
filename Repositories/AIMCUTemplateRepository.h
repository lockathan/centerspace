#ifndef AIMCUTemplateRepository_h
#define AIMCUTemplateRepository_h

#include "Ogre.h"
#include <Templates/AIMCUTemplate.h>
#include <Repositories/Repository.h>
#include <vector>

class AIMCUTemplateRepository : public Repository<AIMCUTemplate>
{
public:
	static AIMCUTemplateRepository* getSingletonPtr();
	~AIMCUTemplateRepository();

	std::vector<AIMCUTemplate*> getAvailableAIMCUTemplates(int factionId, int rank);

private:
	AIMCUTemplateRepository();
	static AIMCUTemplateRepository* mAIMCUTemplateRepository;
};
#endif