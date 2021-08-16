#include <Repositories/TerrainTextureTemplateRepository.h>
#include <Templates/HubTemplate.h>

TerrainTextureTemplateRepository::TerrainTextureTemplateRepository()
{

}

TerrainTextureTemplateRepository::~TerrainTextureTemplateRepository()
{

}

TerrainTextureTemplateRepository* TerrainTextureTemplateRepository::mTerrainTextureTemplateRepository;

TerrainTextureTemplateRepository* TerrainTextureTemplateRepository::getSingletonPtr()
{
	if(!mTerrainTextureTemplateRepository)
	{
		mTerrainTextureTemplateRepository = new TerrainTextureTemplateRepository();
	}
	return mTerrainTextureTemplateRepository;
}