#include <Repositories/WeaponAnimationTemplateDataLoader.h>
#include <string>
#include <Repositories/CSVFileParser.h>
#include <Repositories/Repository.h>
#include <Repositories/MCUTemplateRepository.h>
#include <Templates/AnimationDefinitionTemplate.h>
#include <Templates/WeaponAnimationTemplate.h>
#include <Templates/MCUTemplate.h>

WeaponAnimationTemplateDataLoader::WeaponAnimationTemplateDataLoader()
{

}

WeaponAnimationTemplateDataLoader::~WeaponAnimationTemplateDataLoader()
{

}

void WeaponAnimationTemplateDataLoader::load(const std::string& fileName)
{

	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> aigroups;
	csvFileParser->parseFile(fileName, aigroups);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = aigroups.begin(); lineIterator < aigroups.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		WeaponAnimationTemplate* item = new WeaponAnimationTemplate();
		item->setName((*itemIterator));
		++itemIterator;

		MCUTemplate* mcu = MCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		++itemIterator;
		if(mcu)
		{
			item->setHardpointNumber(std::atoi((*itemIterator).c_str()));
			++itemIterator;
			item->setEnterAnimationName((*itemIterator));
			++itemIterator;
			item->setEnterAnimationSpeed(std::atof((*itemIterator).c_str()));
			++itemIterator;
			item->setFireAnimationName((*itemIterator));
			++itemIterator;
			item->setFireAnimationSpeed(std::atof((*itemIterator).c_str()));
			++itemIterator;
			item->setExitAnimationName((*itemIterator));
			++itemIterator;
			item->setExitAnimationSpeed(std::atof((*itemIterator).c_str()));
			
			mcu->getAnimationDefinitionTemplate()->addWeaponAnimationTemplate(item->getHardpointNumber(), item);
		}
		else
		{
			delete item;
			item = 0;
		}
		
	}

}
