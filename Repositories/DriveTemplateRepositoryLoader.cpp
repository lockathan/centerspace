#include <Repositories/DriveTemplateRepository.h>
#include <Repositories/DriveTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Templates/DriveTemplate.h>

DriveTemplateRepositoryLoader::DriveTemplateRepositoryLoader()
{

}

DriveTemplateRepositoryLoader::~DriveTemplateRepositoryLoader()
{

}

void DriveTemplateRepositoryLoader::load(const std::string& fileName, DriveTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> drives;
	csvFileParser->parseFile(fileName,drives);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = drives.begin(); lineIterator < drives.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		DriveTemplate* item = new DriveTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setEfficiency(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setJumpingEfficiency(std::atof((*itemIterator).c_str()));
		repository->add(item);
	}
}
