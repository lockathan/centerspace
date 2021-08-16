#include <Templates/TerrainTextureTemplate.h>
#include <Ogre.h>
#include <Templates/HubTemplate.h>
#include <string>
#include <hash_map>

using namespace Ogre;

TerrainTextureTemplate::TerrainTextureTemplate()
	:
	mRoughness(0),
	mTextureName("")
{

}

TerrainTextureTemplate::~TerrainTextureTemplate()
{

}

float TerrainTextureTemplate::getRoughness()
{
	return mRoughness;
}

void TerrainTextureTemplate::setRoughness(float value)
{
	mRoughness = value;
}

std::string TerrainTextureTemplate::getTextureName()
{
	return mTextureName;
}

void TerrainTextureTemplate::setTextureName(const std::string& textureName)
{
	mTextureName = textureName;
}