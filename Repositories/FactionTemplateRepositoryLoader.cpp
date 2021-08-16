#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/FactionTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Templates/FactionTemplate.h>

FactionTemplateRepositoryLoader::FactionTemplateRepositoryLoader()
{

}

FactionTemplateRepositoryLoader::~FactionTemplateRepositoryLoader()
{

}

void FactionTemplateRepositoryLoader::load(const std::string& fileName, FactionTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> factions;
	csvFileParser->parseFile(fileName, factions);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = factions.begin(); lineIterator < factions.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		FactionTemplate* item = new FactionTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setDescription((*itemIterator));
		++itemIterator;
		if((*itemIterator) == "1" || (*itemIterator) == "true" || (*itemIterator) == "TRUE" || (*itemIterator) == "True")
			item->setIsGenericFaction(true);
		else
			item->setIsGenericFaction(false);
		++itemIterator;
		item->setStartScript((*itemIterator));
		++itemIterator;
		item->setStartEntrypoint((*itemIterator));
		repository->add(item);
	}

}
