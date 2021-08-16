#include <Repositories/ReactorTemplateRepository.h>
#include <Repositories/ReactorTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Templates/ReactorTemplate.h>

ReactorTemplateRepositoryLoader::ReactorTemplateRepositoryLoader()
{

}

ReactorTemplateRepositoryLoader::~ReactorTemplateRepositoryLoader()
{

}

void ReactorTemplateRepositoryLoader::load(const std::string& fileName, ReactorTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> reactors;
	csvFileParser->parseFile(fileName, reactors);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = reactors.begin(); lineIterator < reactors.end(); ++lineIterator)
	{

		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		ReactorTemplate* item = new ReactorTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setMaxEnergy(std::atof((*itemIterator).c_str()));
		repository->add(item);
		
	}

}
