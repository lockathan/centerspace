#include <Repositories/MCUTemplateRepository.h>
#include <Repositories/MCUTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/ArmorTemplateRepository.h>
#include <Repositories/BatteryTemplateRepository.h>
#include <Repositories/DriveTemplateRepository.h>
#include <Repositories/FactionTemplateRepository.h>
#include <Repositories/MCUComponentTemplateRepositoryLoader.h>
#include <Repositories/ReactorTemplateRepository.h>
#include <Repositories/SensorSystemTemplateRepository.h>
#include <Repositories/ShieldTemplateRepository.h>
#include <Repositories/TargetingSystemTemplateRepository.h>
#include <Ogre.h>

using namespace Ogre;

MCUTemplateRepositoryLoader::MCUTemplateRepositoryLoader()
{

}

MCUTemplateRepositoryLoader::~MCUTemplateRepositoryLoader()
{

}

void MCUTemplateRepositoryLoader::load(const std::string& fileName, MCUTemplateRepository* repository)
{

	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> mcus;
	csvFileParser->parseFile(fileName, mcus);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = mcus.begin(); lineIterator < mcus.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		MCUTemplate* item = new MCUTemplate();
		MCUComponentTemplateRepositoryLoader::getSingletonPtr()->load(itemIterator, item);
		item->setMaxArmorSize(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxBatterySize(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxDriveSize(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxReactorSize(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxShieldSize(std::atoi((*itemIterator).c_str()));
		++itemIterator;
		item->setMaxComponentSlots(std::atoi((*itemIterator).c_str()));
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
		item->setDefaultTargetingSystemTemplate(TargetingSystemTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));

		++itemIterator;
		item->setMeshName((*itemIterator));
		++itemIterator;
		item->setMaterialName((*itemIterator));
		
		++itemIterator;
		item->setMovementSpeed(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setWalkingAnimationSpeed(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setMCUType((*itemIterator));
		++itemIterator;
		item->setScale(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setThumbnailFile((*itemIterator));
		++itemIterator;
		item->setProfileImageFile((*itemIterator));
		++itemIterator;
		item->setWalkSoundName((*itemIterator));

		MeshManager::getSingleton().createOrRetrieve(item->getMeshName(), "General");
		repository->add(item);
		
	}

}
