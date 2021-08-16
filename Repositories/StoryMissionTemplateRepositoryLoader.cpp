#include <Repositories/StoryMissionTemplateRepository.h>
#include <Repositories/StoryMissionTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Templates/StoryMissionTemplate.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/FactionHubTemplateRepository.h>
#include <Repositories/TerrainTemplateRepository.h>
#include <string>
#include <vector>

StoryMissionTemplateRepositoryLoader::StoryMissionTemplateRepositoryLoader()
{

}

StoryMissionTemplateRepositoryLoader::~StoryMissionTemplateRepositoryLoader()
{

}

void StoryMissionTemplateRepositoryLoader::load(const std::string& fileName, StoryMissionTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> missions;
	csvFileParser->parseFile(fileName,missions);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = missions.begin(); lineIterator < missions.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		StoryMissionTemplate* item = new StoryMissionTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setTerrainTemplate(TerrainTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setFactionHubTemplate(FactionHubTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setSideLength(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setRequiredRank(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMissionScriptFilename((*itemIterator));
		++itemIterator;
		item->setTerrainGenerationFilename((*itemIterator));
		++itemIterator;
		item->setTerrainGenerationEntryPoint((*itemIterator));
		++itemIterator;
		item->setEnemyGenerationFilename((*itemIterator));
		++itemIterator;
		item->setEnemyGenerationEntryPoint((*itemIterator));
		++itemIterator;
		item->setPlacementFilename((*itemIterator));
		++itemIterator;
		item->setPlayerPlacementEntryPoint((*itemIterator));
		++itemIterator;
		item->setPlayerLocationX(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setPlayerLocationZ(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setEnemy1LocationX(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setEnemy1LocationZ(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setEnemy2LocationX(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setEnemy2LocationZ(atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setLuaParam1(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setLuaParam2(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setLuaParam3(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setLuaParam4(std::atof((*itemIterator).c_str()));
		repository->add(item);
		
	}

}