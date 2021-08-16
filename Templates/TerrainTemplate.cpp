#include <Templates/TerrainTemplate.h>
#include <Ogre.h>
#include <Templates/HubTemplate.h>
#include <Templates/TerrainTextureTemplate.h>
#include <string>
#include <hash_map>

using namespace Ogre;

TerrainTemplate::TerrainTemplate()
	:
	mDescription(""),
	mMinHeight(0),
	mMaxHeight(0),
	mGravity(0),
	
	mHubTemplate(0),
	mMaterialName(""),
	
	mLuaScriptFile(""),
	mLuaEntryPoint(""),
	mLuaParam1(0),
	mLuaParam2(0),
	mLuaParam3(0),
	mLuaParam4(0),

	mAmbientLight(Vector3::ZERO),
	mDirectionalLight(Vector3::ZERO),
	mDirectionLightDirection(Vector3::ZERO)
{
	mTerrainTextures.resize(10);
}

TerrainTemplate::~TerrainTemplate()
{

}

std::string TerrainTemplate::getMaterialName()
{
	return mMaterialName;
}

void TerrainTemplate::setMaterialName(std::string materialName)
{
	mMaterialName = materialName;
}

std::string TerrainTemplate::getDescription()
{
	return mDescription;
}

void TerrainTemplate::setDescription(std::string desc)
{
	mDescription = desc;
}

int TerrainTemplate::getMinHeight()
{
	return mMinHeight;
}

void TerrainTemplate::setMinHeight(int value)
{
	mMinHeight = value;
}

int TerrainTemplate::getMaxHeight()
{
	return mMaxHeight;
}

void TerrainTemplate::setMaxHeight(int value)
{
	mMaxHeight = value;
}

float TerrainTemplate::getGravity()
{
	return mGravity;
}

void TerrainTemplate::setGravity(float g)
{
	mGravity = g;
}

TerrainTextureTemplate* TerrainTemplate::getTerrainTextureTemplate(int i)
{
	return mTerrainTextures[i];
}

void TerrainTemplate::setTerrainTextureTemplate(int i, TerrainTextureTemplate* texture)
{
	mTerrainTextures[i] = texture;
}

HubTemplate* TerrainTemplate::getHubTemplate()
{
	return mHubTemplate;
}

void TerrainTemplate::setHubTemplate(HubTemplate* hub)
{
	mHubTemplate = hub;
}

std::string TerrainTemplate::getLuaScriptFile()
{
	return mLuaScriptFile;
}

void TerrainTemplate::setLuaScriptFile(const std::string& file)
{
	mLuaScriptFile = file;
}

std::string TerrainTemplate::getLuaEntryPoint()
{
	return mLuaEntryPoint;
}

void TerrainTemplate::setLuaEntryPoint(const std::string& entry)
{
	mLuaEntryPoint = entry;
}

float TerrainTemplate::getLuaParam1()
{
	return mLuaParam1;
}

void TerrainTemplate::setLuaParam1(float param)
{
	mLuaParam1 = param;
}
	
float TerrainTemplate::getLuaParam2()
{
	return mLuaParam2;
}

void TerrainTemplate::setLuaParam2(float param)
{
	mLuaParam2 = param;
}

float TerrainTemplate::getLuaParam3()
{
	return mLuaParam3;
}

void TerrainTemplate::setLuaParam3(float param)
{
	mLuaParam3 = param;
}

float TerrainTemplate::getLuaParam4()
{
	return mLuaParam4;
}

void TerrainTemplate::setLuaParam4(float param)
{
	mLuaParam4 = param;
}

Vector3 TerrainTemplate::getAmbientLight()
{
	return mAmbientLight;
}

void TerrainTemplate::setAmbientLight(const Vector3& light)
{
	mAmbientLight = light;
}

Vector3 TerrainTemplate::getDirectionalLight()
{
	return mDirectionalLight;
}

void TerrainTemplate::setDirectionalLight(const Vector3& light)
{
	mDirectionalLight = light;
}

Vector3 TerrainTemplate::getDirectionalLightDirection()
{
	return mDirectionLightDirection;
}

void TerrainTemplate::setDirectionalLightDirection(const Vector3& dir)
{
	mDirectionLightDirection = dir;
}