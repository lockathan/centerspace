#include <Repositories/AIGroupTemplateRepository.h>
#include <Repositories/AIGroupTemplateRepositoryLoader.h>
#include <Repositories/FactionTemplateRepository.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/AIMCUTemplateRepository.h>
#include <Templates/AIGroupTemplate.h>
#include <Templates/AIMCUTemplate.h>

AIGroupTemplateRepositoryLoader::AIGroupTemplateRepositoryLoader()
{

}

AIGroupTemplateRepositoryLoader::~AIGroupTemplateRepositoryLoader()
{

}

void AIGroupTemplateRepositoryLoader::load(const std::string& fileName, AIGroupTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> aigroups;
	csvFileParser->parseFile(fileName, aigroups);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = aigroups.begin(); lineIterator < aigroups.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		AIGroupTemplate* item = new AIGroupTemplate();
		item->setName((*itemIterator));
		++itemIterator;		
		item->setRequiredRank(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setFactionTemplate(FactionTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;		
		item->setType(std::atoi((*itemIterator).c_str()));
		AIMCUTemplate* aiMCU(0);
		int count = 0;

		//MCU 1
		++itemIterator;
		aiMCU = AIMCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		++itemIterator;
		count = std::atoi((*itemIterator).c_str());

		if(aiMCU && count > 0)
		{
			item->addAIMCU(count,aiMCU);
		}
		aiMCU = 0;
		count = 0;

		//MCU 2
		++itemIterator;
		aiMCU = AIMCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		++itemIterator;
		count = std::atoi((*itemIterator).c_str());

		if(aiMCU && count > 0)
		{
			item->addAIMCU(count,aiMCU);
		}
		aiMCU = 0;
		count = 0;

		//MCU 3
		++itemIterator;
		aiMCU = AIMCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		++itemIterator;
		count = std::atoi((*itemIterator).c_str());

		if(aiMCU && count > 0)
		{
			item->addAIMCU(count,aiMCU);
		}
		aiMCU = 0;
		count = 0;

		//MCU 4
		++itemIterator;
		aiMCU = AIMCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		++itemIterator;
		count = std::atoi((*itemIterator).c_str());

		if(aiMCU && count > 0)
		{
			item->addAIMCU(count,aiMCU);
		}
		aiMCU = 0;
		count = 0;

		//MCU 5
		++itemIterator;
		aiMCU = AIMCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		++itemIterator;
		count = std::atoi((*itemIterator).c_str());

		if(aiMCU && count > 0)
		{
			item->addAIMCU(count,aiMCU);
		}
		aiMCU = 0;
		count = 0;
		repository->add(item);
	}

}
