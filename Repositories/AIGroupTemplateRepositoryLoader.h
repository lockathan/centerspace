#ifndef AIGroupTemplateRepositoryLoader_h
#define AIGroupTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/AIGroupTemplateRepository.h>

class AIGroupTemplateRepositoryLoader : public RepositoryLoader
{
public:
	AIGroupTemplateRepositoryLoader();
	~AIGroupTemplateRepositoryLoader();
	void load(const std::string& fileName, AIGroupTemplateRepository* r);
private:
	
};
#endif