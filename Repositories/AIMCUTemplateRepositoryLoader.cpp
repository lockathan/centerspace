#include <Repositories/AIMCUTemplateRepository.h>
#include <Repositories/AIMCUTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/ArmorTemplateRepository.h>
#include <Repositories/BatteryTemplateRepository.h>
#include <Repositories/DriveTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUTemplateRepository.h>
#include <Repositories/ReactorTemplateRepository.h>
#include <Repositories/SensorSystemTemplateRepository.h>
#include <Repositories/ShieldTemplateRepository.h>
#include <Repositories/TargetingSystemTemplateRepository.h>
#include <Repositories/WeaponTemplateRepository.h>

AIMCUTemplateRepositoryLoader::AIMCUTemplateRepositoryLoader()
{

}

AIMCUTemplateRepositoryLoader::~AIMCUTemplateRepositoryLoader()
{

}

void AIMCUTemplateRepositoryLoader::load(const std::string& fileName, AIMCUTemplateRepository* repository)
{

	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> aimcus;
	csvFileParser->parseFile(fileName, aimcus);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = aimcus.begin(); lineIterator < aimcus.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		AIMCUTemplate* item = new AIMCUTemplate();
		item->setName((*itemIterator));
		++itemIterator;	
		item->setRequiredRank(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setFactionTemplate(FactionTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDefaultArmor(ArmorTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDefaultBattery(BatteryTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDefaultDrive(DriveTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDefaultReactor(ReactorTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDefaultSensorSystem(SensorSystemTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDefaultShield(ShieldTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setDefaultTargetingSystem(TargetingSystemTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setMCUTemplate(MCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		
		
		int currentWeaponHardpoint (0);

		//Weapon 1
		++itemIterator;
		WeaponTemplate* weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));

		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;

		//Weapon 2
		++itemIterator;
		weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;

		//Weapon 3
		++itemIterator;
		weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;

		//Weapon 4
		++itemIterator;
		weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;

		//Weapon 5
		++itemIterator;
		weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;

		//Weapon 6
		++itemIterator;
		weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;

		//Weapon 7
		++itemIterator;
		weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;

		//Weapon 8
		++itemIterator;
		weapon = WeaponTemplateRepository::getSingletonPtr()->findByName((*itemIterator));
		if(weapon)
		{
			item->setWeapon(currentWeaponHardpoint,weapon);
		}
		++currentWeaponHardpoint;
		weapon = 0;


		repository->add(item);
		
	}

}
