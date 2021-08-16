#include <Repositories/SoundTemplateRepository.h>
#include <Repositories/SoundTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Templates/SoundTemplate.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/FactionTemplateRepository.h>
#include <string>
#include <vector>

SoundTemplateRepositoryLoader::SoundTemplateRepositoryLoader()
{

}

SoundTemplateRepositoryLoader::~SoundTemplateRepositoryLoader()
{

}

void SoundTemplateRepositoryLoader::load(const std::string& fileName, SoundTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> hubs;
	csvFileParser->parseFile(fileName,hubs);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = hubs.begin(); lineIterator < hubs.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		SoundTemplate* item = new SoundTemplate();
		item->setName((*itemIterator));
		++itemIterator;	
		item->setFileName((*itemIterator));
		++itemIterator;
		if((*itemIterator) == "1" || (*itemIterator) == "true" || (*itemIterator) == "TRUE" || (*itemIterator) == "True")
			item->setLoop(true);
		else
			item->setLoop(false);
		repository->add(item);
		
	}

}
