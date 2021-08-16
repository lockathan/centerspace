#include <Repositories/BatteryTemplateRepository.h>
#include <Repositories/BatteryTemplateRepositoryLoader.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Templates/BatteryTemplate.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <string>
#include <vector>

BatteryTemplateRepositoryLoader::BatteryTemplateRepositoryLoader()
{

}

BatteryTemplateRepositoryLoader::~BatteryTemplateRepositoryLoader()
{

}

void BatteryTemplateRepositoryLoader::load(const std::string& fileName, BatteryTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> batteries;
	csvFileParser->parseFile(fileName,batteries);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = batteries.begin(); lineIterator < batteries.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		BatteryTemplate* item = new BatteryTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setMaxEnergy(std::atof((*itemIterator).c_str()));
		repository->add(item);
		
	}

}
