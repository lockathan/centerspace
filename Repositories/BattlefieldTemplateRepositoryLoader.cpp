#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/BattlefieldTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/HubTemplateRepository.h>
#include <Templates/BattlefieldTemplate.h>

BattlefieldTemplateRepositoryLoader::BattlefieldTemplateRepositoryLoader()
{

}

BattlefieldTemplateRepositoryLoader::~BattlefieldTemplateRepositoryLoader()
{

}

void BattlefieldTemplateRepositoryLoader::load(const std::string& fileName, BattlefieldTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> battlefields;
	csvFileParser->parseFile(fileName,battlefields);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = battlefields.begin(); lineIterator < battlefields.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		BattlefieldTemplate* item = new BattlefieldTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setInitialSize(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMinMCUs(std::atoi((*itemIterator).c_str()));
		repository->add(item);
		
	}
}
