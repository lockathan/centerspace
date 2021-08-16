#include <Repositories/BeamProjectileTemplateRepositoryLoader.h>
#include <string>
#include <Repositories/CSVFileParser.h>
#include <Repositories/Repository.h>
#include <Repositories/BeamProjectileTemplateRepository.h>
#include <Templates/BeamProjectileTemplate.h>
#include <Templates/WeaponTemplate.h>

BeamProjectileTemplateRepositoryLoader::BeamProjectileTemplateRepositoryLoader()
{

}

BeamProjectileTemplateRepositoryLoader::~BeamProjectileTemplateRepositoryLoader()
{

}

void BeamProjectileTemplateRepositoryLoader::load(const std::string& fileName, BeamProjectileTemplateRepository* r)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> battlefields;
	csvFileParser->parseFile(fileName,battlefields);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = battlefields.begin(); lineIterator < battlefields.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		BeamProjectileTemplate* item = new BeamProjectileTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setDuration(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setMaterialName((*itemIterator));
		++itemIterator;
		item->setBeamWidth(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setExplosionName((*itemIterator));
		++itemIterator;
		item->setFireSOundName((*itemIterator));
		++itemIterator;
		item->setImpactSoundName((*itemIterator));
		r->add(item);
		
	}
}
