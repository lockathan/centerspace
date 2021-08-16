#ifndef RankTemplateRepositoryLoader_h
#define RankTemplateRepositoryLoader_h

#include "Ogre.h"
#include <Repositories/RepositoryLoader.h>
#include <Repositories/RankTemplateRepository.h>

class RankTemplateRepositoryLoader : public RepositoryLoader
{
public:
	RankTemplateRepositoryLoader();
	~RankTemplateRepositoryLoader();
	void load(const std::string& fileName, RankTemplateRepository* r);	
};
#endif