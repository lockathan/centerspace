#include <Repositories/WeaponHardpointTemplateDataLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Templates/MCUTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <Repositories/WeaponTemplateRepository.h>
#include <Repositories/MCUTemplateRepository.h>


WeaponHardpointTemplateDataLoader::WeaponHardpointTemplateDataLoader()
{

}

WeaponHardpointTemplateDataLoader::~WeaponHardpointTemplateDataLoader()
{

}

void WeaponHardpointTemplateDataLoader::load(const std::string& fileName)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	MCUTemplateRepository* mcuTemplateRepository = MCUTemplateRepository::getSingletonPtr();
	WeaponTemplateRepository* weaponTemplateRepository = WeaponTemplateRepository::getSingletonPtr();

	std::vector<std::vector<std::string>> hardpoints;
	csvFileParser->parseFile(fileName,hardpoints);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = hardpoints.begin(); lineIterator < hardpoints.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();

		WeaponHardpointTemplate* item = new WeaponHardpointTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		
		MCUTemplate* mcu = mcuTemplateRepository->findByName((*itemIterator));
		++itemIterator;
		WeaponTemplate* weapon = weaponTemplateRepository->findByName((*itemIterator));
		item->setDefaultWeapon(weapon);

		++itemIterator;
		
		item->setHardpointType(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxWeaponSize(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		float x = std::atof((*itemIterator).c_str());
		++itemIterator;
		float y = std::atof((*itemIterator).c_str());
		++itemIterator;
		float z = std::atof((*itemIterator).c_str());
		item->setBarrelOffset(x,y,z);
		++itemIterator;
		item->setFiringBoneName((*itemIterator));
		mcu->addWeaponHardpointTemplate(item);
		
	}

}
