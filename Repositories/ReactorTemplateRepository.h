#ifndef ReactorTemplateRepository_h
#define ReactorTemplateRepository_h

#include "Ogre.h"
#include <Templates/ReactorTemplate.h>
#include <Repositories/Repository.h>

class ReactorTemplateRepository : public Repository<ReactorTemplate>
{
public:
	static ReactorTemplateRepository* getSingletonPtr();
	~ReactorTemplateRepository();
	
	std::vector<ReactorTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);

private:
	ReactorTemplateRepository();
	static ReactorTemplateRepository* mReactorTemplateRepository;
};
#endif