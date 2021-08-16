#include <Repositories/FactionHubTemplateRepository.h>
#include <Repositories/FactionHubTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/HubTemplateRepository.h>
#include <Templates/FactionHubTemplate.h>

FactionHubTemplateRepositoryLoader::FactionHubTemplateRepositoryLoader()
{

}

FactionHubTemplateRepositoryLoader::~FactionHubTemplateRepositoryLoader()
{

}

void FactionHubTemplateRepositoryLoader::load(const std::string& fileName, FactionHubTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> factionhubs;
	csvFileParser->parseFile(fileName, factionhubs);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = factionhubs.begin(); lineIterator < factionhubs.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		FactionHubTemplate* item = new FactionHubTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setPlayerFactionTemplate(FactionTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setEnemy1FactionTemplate(FactionTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setEnemy2FactionTemplate(FactionTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setHubTemplate(HubTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		repository->add(item);
	}

}
