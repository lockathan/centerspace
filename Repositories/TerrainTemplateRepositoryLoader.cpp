#include <Repositories/TerrainTemplateRepository.h>
#include <Repositories/TerrainTemplateRepositoryLoader.h>
#include <Repositories/Repository.h>
#include <Repositories/CSVFileParser.h>
#include <Repositories/HubTemplateRepository.h>
#include <Repositories/TerrainTextureTemplateRepository.h>
#include <Templates/TerrainTemplate.h>
#include <math.h>
#include <string>


using namespace Ogre;

TerrainTemplateRepositoryLoader::TerrainTemplateRepositoryLoader()
{

}

TerrainTemplateRepositoryLoader::~TerrainTemplateRepositoryLoader()
{

}

void TerrainTemplateRepositoryLoader::load(const std::string& fileName, TerrainTemplateRepository* repository)
{
	CSVFileParser* csvFileParser = CSVFileParser::getSingletonPtr();
	
	std::vector<std::vector<std::string>> terrains;
	csvFileParser->parseFile(fileName, terrains);
	
	std::vector<std::vector<std::string>>::iterator lineIterator;

	for(lineIterator = terrains.begin(); lineIterator < terrains.end(); ++lineIterator)
	{
		std::vector<std::string>::iterator itemIterator;
		itemIterator = (*lineIterator).begin();
		TerrainTemplate* item = new TerrainTemplate();
		item->setName((*itemIterator));
		++itemIterator;
		item->setDescription((*itemIterator));
		++itemIterator;
		item->setMinHeight(std::atoi((*itemIterator).c_str()));
		
		++itemIterator;
		item->setMaxHeight(std::min(8,std::atoi((*itemIterator).c_str())));

		++itemIterator;
		item->setHubTemplate(HubTemplateRepository::getSingletonPtr()->findByName((*itemIterator)));
		++itemIterator;
		item->setGravity(std::atof((*itemIterator).c_str()));
		
		++itemIterator;
		item->setMaterialName((*itemIterator));
		
		int levelId(0);
		TerrainTextureTemplateRepository* tRepo = TerrainTextureTemplateRepository::getSingletonPtr();

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		if((*itemIterator) != "none" && (*itemIterator) != "" && (*itemIterator) != "-1")
		{
			item->setTerrainTextureTemplate(levelId, tRepo->findByName((*itemIterator)));
		}
		++levelId;

		++itemIterator;
		item->setLuaScriptFile((*itemIterator));
		++itemIterator;
		item->setLuaEntryPoint((*itemIterator));
		++itemIterator;
		item->setLuaParam1(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setLuaParam2(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setLuaParam3(std::atof((*itemIterator).c_str()));
		++itemIterator;
		item->setLuaParam4(std::atof((*itemIterator).c_str()));

		
		Vector3 light(Vector3::ZERO);
		++itemIterator;
		light.x = std::atof((*itemIterator).c_str());
		++itemIterator;
		light.y = std::atof((*itemIterator).c_str());
		++itemIterator;
		light.z = std::atof((*itemIterator).c_str());
		item->setAmbientLight(light);

		light = Vector3::ZERO;
		++itemIterator;
		light.x = std::atof((*itemIterator).c_str());
		++itemIterator;
		light.y = std::atof((*itemIterator).c_str());
		++itemIterator;
		light.z = std::atof((*itemIterator).c_str());
		item->setDirectionalLight(light);

		light = Vector3::ZERO;
		++itemIterator;
		light.x = std::atof((*itemIterator).c_str());
		++itemIterator;
		light.y = std::atof((*itemIterator).c_str());
		++itemIterator;
		light.z = std::atof((*itemIterator).c_str());
		item->setDirectionalLightDirection(light);

		repository->add(item);
		
	}
}
