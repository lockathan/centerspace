#include <Repositories/PhysicalProjectileTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/CSVFileParser.h>
#include <Repositories/Repository.h>
#include <Repositories/PhysicalProjectileTemplateRepository.h>
#include <Templates/PhysicalProjectileTemplate.h>
#include <Templates/WeaponTemplate.h>

PhysicalProjectileTemplateRepositoryLoader::PhysicalProjectileTemplateRepositoryLoader()
{

}

PhysicalProjectileTemplateRepositoryLoader::~PhysicalProjectileTemplateRepositoryLoader()
{

}

void PhysicalProjectileTemplateRepositoryLoader::load(const std::string& fileName, PhysicalProjectileTemplateRepository* r)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> battlefields;
	csvFileParser->parseFile(fileName,battlefields);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = battlefields.begin(); lineIterator < battlefields.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		PhysicalProjectileTemplate* item = new PhysicalProjectileTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setMeshName((*itemIterator));
		++itemIterator;
		item->setMaterialName((*itemIterator));
		++itemIterator;
		item->setMovementSpeed(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setWeight(std::atof((*itemIterator).c_str()));
		++itemIterator;
		if((*itemIterator) == "1" || (*itemIterator) == "true" || (*itemIterator) == "TRUE" || (*itemIterator) == "True")
			item->setSelfPropelled(true);
		else
			item->setSelfPropelled(false);
		++itemIterator;
		if((*itemIterator) == "1" || (*itemIterator) == "true" || (*itemIterator) == "TRUE" || (*itemIterator) == "True")
			item->setArching(true);
		else
			item->setArching(false);
		++itemIterator;
		item->setExplosionName((*itemIterator));
		++itemIterator;
		item->setFireSOundName((*itemIterator));
		++itemIterator;
		item->setImpactSoundName((*itemIterator));
		r->add(item);
		
	}
}
