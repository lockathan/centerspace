#include <Repositories/SensorSystemTemplateRepository.h>
#include <Repositories/SensorSystemTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Templates/SensorSystemTemplate.h>

SensorSystemTemplateRepositoryLoader::SensorSystemTemplateRepositoryLoader()
{

}

SensorSystemTemplateRepositoryLoader::~SensorSystemTemplateRepositoryLoader()
{

}

void SensorSystemTemplateRepositoryLoader::load(const std::string& fileName, SensorSystemTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> drives;
	csvFileParser->parseFile(fileName,drives);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = drives.begin(); lineIterator < drives.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		SensorSystemTemplate* item = new SensorSystemTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setSightRange(std::atof((*itemIterator).c_str()));
		++itemIterator;	
		item->setRadarRange(std::atof((*itemIterator).c_str()));
		++itemIterator;	
		item->setEnergyDrainPerTurn(std::atof((*itemIterator).c_str()));
		repository->add(item);
	}
}
