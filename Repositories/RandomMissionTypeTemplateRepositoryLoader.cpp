#include <Repositories/RandomMissionTypeTemplateRepository.h>
#include <Repositories/RandomMissionTypeTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Templates/RandomMissionTypeTemplate.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/FactionHubTemplateRepository.h>
#include <Repositories/TerrainTemplateRepository.h>
#include <string>
#include <vector>

RandomMissionTypeTemplateRepositoryLoader::RandomMissionTypeTemplateRepositoryLoader()
{

}

RandomMissionTypeTemplateRepositoryLoader::~RandomMissionTypeTemplateRepositoryLoader()
{

}

void RandomMissionTypeTemplateRepositoryLoader::load(const std::string& fileName, RandomMissionTypeTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> missions;
	csvFileParser->parseFile(fileName,missions);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = missions.begin(); lineIterator < missions.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		RandomMissionTypeTemplate* item = new RandomMissionTypeTemplate();
		item->setName((*itemIterator));
		++itemIterator;	
		item->setPlacementScriptFilename((*itemIterator).c_str());
		++itemIterator;	
		item->setPlayerPlacementEntryPoint((*itemIterator).c_str());
		++itemIterator;
		item->setEnemyGenerationFilename((*itemIterator).c_str());
		++itemIterator;
		item->setEnemyGenerationEntryPoint((*itemIterator).c_str());
		++itemIterator;
		item->setEnemyPlacementEntryPoint((*itemIterator).c_str());
		repository->add(item);
		
	}

}