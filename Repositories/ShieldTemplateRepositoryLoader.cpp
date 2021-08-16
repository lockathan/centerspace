#include <Repositories/ShieldTemplateRepository.h>
#include <Repositories/ShieldTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Templates/ShieldTemplate.h>

ShieldTemplateRepositoryLoader::ShieldTemplateRepositoryLoader()
{

}

ShieldTemplateRepositoryLoader::~ShieldTemplateRepositoryLoader()
{

}

void ShieldTemplateRepositoryLoader::load(const std::string& fileName, ShieldTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> reactors;
	csvFileParser->parseFile(fileName, reactors);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = reactors.begin(); lineIterator < reactors.end(); ++lineIterator)
	{

		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		ShieldTemplate* item = new ShieldTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setMaxShieldHealth(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setEnergyDrainPerTurn(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setRegenRate(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setMaterialName((*itemIterator));
		repository->add(item);
		
	}

}
