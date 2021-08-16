#include <Repositories/AnimationDefinitionTemplateDataLoader.h>
#include <string>
#include <Repositories/CSVFileParser.h>
#include <Repositories/Repository.h>
#include <Repositories/MCUTemplateRepository.h>
#include <Templates/AnimationDefinitionTemplate.h>
#include <Templates/MCUTemplate.h>

AnimationDefinitionTemplateDataLoader::AnimationDefinitionTemplateDataLoader()
{

}

AnimationDefinitionTemplateDataLoader::~AnimationDefinitionTemplateDataLoader()
{

}

void AnimationDefinitionTemplateDataLoader::load(const std::string& fileName)
{

	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> aigroups;
	csvFileParser->parseFile(fileName, aigroups);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = aigroups.begin(); lineIterator < aigroups.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		AnimationDefinitionTemplate* item = new AnimationDefinitionTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setWalkAnimationName((*itemIterator));
		++itemIterator;
		item->setGetHitAnimationName((*itemIterator));
		++itemIterator;
		item->setTakeOffAnimationName((*itemIterator));
		++itemIterator;
		item->setLandAnimationName((*itemIterator));
		++itemIterator;
		item->setFlyAnimationName((*itemIterator));
		++itemIterator;
		item->setIdleAnimationName((*itemIterator));
		++itemIterator;
		item->setDeathAnimationName((*itemIterator));
		++itemIterator;
		item->setMCUTemplate(MCUTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));

		
	}

}
