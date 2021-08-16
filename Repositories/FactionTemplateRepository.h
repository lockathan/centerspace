#ifndef FactionTemplateRepository_h
#define FactionTemplateRepository_h

#include "Ogre.h"
#include <Templates/FactionTemplate.h>
#include <Repositories/Repository.h>

class FactionTemplateRepository : public Repository<FactionTemplate>
{
public:
	static FactionTemplateRepository* getSingletonPtr();
	~FactionTemplateRepository();
private:
	FactionTemplateRepository();
	static FactionTemplateRepository* mFactionTemplateRepository;
};
#endif