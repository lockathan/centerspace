#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/MCUComponentTemplate.h>
#include <vector>
#include <string>

void MCUComponentTemplateRepositoryLoader::load(std::vector<std::string>::iterator& itemIterator, MCUComponentTemplate* item)
{
		item->setName((*itemIterator));
		++itemIterator;	
		item->setCost(std::atof((*itemIterator).c_str()));
		++itemIterator;	
		item->setWeight(std::atof((*itemIterator).c_str()));
		++itemIterator;		
		item->setRequiredRank(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setFactionTemplate(FactionTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDescription((*itemIterator));
		++itemIterator;
		item->setSize(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxHealth(atof((*itemIterator).c_str()));
		++itemIterator;
}

MCUComponentTemplateRepositoryLoader* MCUComponentTemplateRepositoryLoader::mMCUComponentTemplateRepositoryLoader;

MCUComponentTemplateRepositoryLoader* MCUComponentTemplateRepositoryLoader::getSingletonPtr()
{
	if(!mMCUComponentTemplateRepositoryLoader)
	{
		mMCUComponentTemplateRepositoryLoader = new MCUComponentTemplateRepositoryLoader();
	}
	return mMCUComponentTemplateRepositoryLoader;
}
