#include <Repositories/RankTemplateRepository.h>
#include <Repositories/RankTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Templates/RankTemplate.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/FactionHubTemplateRepository.h>
#include <Repositories/TerrainTemplateRepository.h>
#include <string>
#include <vector>

RankTemplateRepositoryLoader::RankTemplateRepositoryLoader()
{

}

RankTemplateRepositoryLoader::~RankTemplateRepositoryLoader()
{

}

void RankTemplateRepositoryLoader::load(const std::string& fileName, RankTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> missions;
	csvFileParser->parseFile(fileName,missions);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = missions.begin(); lineIterator < missions.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		RankTemplate* item = new RankTemplate();
		item->setName((*itemIterator));
		++itemIterator;	
		item->setRank(std::atoi((*itemIterator).c_str()));
		++itemIterator;	
		item->setFactionTemplate(FactionTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;	
		item->setRequiredExperience(std::atoi((*itemIterator).c_str()));
		
		repository->add(item);
		
	}

}