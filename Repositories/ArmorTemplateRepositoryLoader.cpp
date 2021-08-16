#include <Repositories/ArmorTemplateRepository.h>
#include <Repositories/ArmorTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Templates/ArmorTemplate.h>

ArmorTemplateRepositoryLoader::ArmorTemplateRepositoryLoader()
{

}

ArmorTemplateRepositoryLoader::~ArmorTemplateRepositoryLoader()
{

}

void ArmorTemplateRepositoryLoader::load(const std::string& fileName, ArmorTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> reactors;
	csvFileParser->parseFile(fileName, reactors);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = reactors.begin(); lineIterator < reactors.end(); ++lineIterator)
	{

		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		ArmorTemplate* item = new ArmorTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setLaserDamageTaken(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setPlasmaDamageTaken(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setCannonDamageTaken(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setMissileDamageTaken(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setPiercingDamageTaken(std::atof((*itemIterator).c_str()));
		repository->add(item);
		
	}

}
