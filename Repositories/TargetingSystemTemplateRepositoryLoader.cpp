#include <Repositories/TargetingSystemTemplateRepository.h>
#include <Repositories/TargetingSystemTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Templates/TargetingSystemTemplate.h>

TargetingSystemTemplateRepositoryLoader::TargetingSystemTemplateRepositoryLoader()
{

}

TargetingSystemTemplateRepositoryLoader::~TargetingSystemTemplateRepositoryLoader()
{

}

void TargetingSystemTemplateRepositoryLoader::load(const std::string& fileName, TargetingSystemTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> drives;
	csvFileParser->parseFile(fileName,drives);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = drives.begin(); lineIterator < drives.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		TargetingSystemTemplate* item = new TargetingSystemTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setBaseAccuracy(std::atof((*itemIterator).c_str()));
		++itemIterator;	
		item->setObstructedBonus(std::atof((*itemIterator).c_str()));
		++itemIterator;	
		item->setLongRangeBonus(std::atof((*itemIterator).c_str()));
		repository->add(item);
	}
}
