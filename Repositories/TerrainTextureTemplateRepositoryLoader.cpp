#include <Repositories/TerrainTextureTemplateRepository.h>
#include <Repositories/TerrainTextureTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/HubTemplateRepository.h>
#include <Templates/TerrainTextureTemplate.h>
#include <math.h>
#include <string>


using namespace Ogre;

TerrainTextureTemplateRepositoryLoader::TerrainTextureTemplateRepositoryLoader()
{

}

TerrainTextureTemplateRepositoryLoader::~TerrainTextureTemplateRepositoryLoader()
{

}

void TerrainTextureTemplateRepositoryLoader::load(const std::string& fileName, TerrainTextureTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> terrains;
	csvFileParser->parseFile(fileName, terrains);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = terrains.begin(); lineIterator < terrains.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		TerrainTextureTemplate* item = new TerrainTextureTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setTextureName((*itemIterator));
		++itemIterator;
		item->setRoughness(std::atoi((*itemIterator).c_str()));

		repository->add(item);
		
	}
}
