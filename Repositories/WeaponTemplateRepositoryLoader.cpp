#include <Repositories/WeaponTemplateRepository.h>
#include <Repositories/WeaponTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/BeamProjectileTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Repositories/PhysicalProjectileTemplateRepository.h>
#include <Templates/WeaponTemplate.h>

WeaponTemplateRepositoryLoader::WeaponTemplateRepositoryLoader()
{

}

WeaponTemplateRepositoryLoader::~WeaponTemplateRepositoryLoader()
{

}

void WeaponTemplateRepositoryLoader::load(const std::string& fileName, WeaponTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> weapons;
	csvFileParser->parseFile(fileName, weapons);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = weapons.begin(); lineIterator < weapons.end(); ++lineIterator)
	{

		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		WeaponTemplate* item = new WeaponTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setWeaponType(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setShieldDamage(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setArmorDamage(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setPiercingDamage(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setRange(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setRoundsPerTurn(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxAmmo(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setEnergyDrain(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setIndirectFireCapable(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		ProjectileType type = (ProjectileType)std::atoi((*itemIterator).c_str());
		++itemIterator;
		item->setDamageType((DamageType)(std::atoi((*itemIterator).c_str())));
		++itemIterator;
		switch(type)
		{
		case ProjectileType::LASER_INSTANT:
			item->setProjectileTemplate(BeamProjectileTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
			break;
		case ProjectileType::BALLISTIC_STRAIGHT:
			item->setProjectileTemplate(PhysicalProjectileTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
			break;
		case ProjectileType::MISSILE_INDIRECT:
			item->setProjectileTemplate(PhysicalProjectileTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
			break;
		default:
			break;
		}

		repository->add(item);
		
	}

}
