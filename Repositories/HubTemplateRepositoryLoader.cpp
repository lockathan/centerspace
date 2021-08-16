#include <Repositories/HubTemplateRepository.h>
#include <Repositories/HubTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Templates/HubTemplate.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <string>
#include <vector>

HubTemplateRepositoryLoader::HubTemplateRepositoryLoader()
{

}

HubTemplateRepositoryLoader::~HubTemplateRepositoryLoader()
{

}

void HubTemplateRepositoryLoader::load(const std::string& fileName, HubTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> hubs;
	csvFileParser->parseFile(fileName,hubs);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = hubs.begin(); lineIterator < hubs.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		HubTemplate* item = new HubTemplate();
		item->setName((*itemIterator));
		++itemIterator;	
		item->setRequiredRank(std::atoi((*itemIterator).c_str()));
		++itemIterator;	
		item->setSystemName((*itemIterator));
		++itemIterator;
		item->setBaseMissionPay(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setBounty(std::atof((*itemIterator).c_str()));
		repository->add(item);
		
	}

}
