#ifndef TerrainTemplate_h
#define TerrainTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>
#include <hash_map>

class HubTemplate;
class TerrainTextureTemplate;

class TerrainTemplate : public Template
{
public:
	TerrainTemplate();
	~TerrainTemplate();

	int getMinHeight();
	void setMinHeight(int value);
	int getMaxHeight();
	void setMaxHeight(int value);

	float getGravity();
	void setGravity(float g);

	TerrainTextureTemplate* getTerrainTextureTemplate(int i);
	void setTerrainTextureTemplate(int i, TerrainTextureTemplate* texture);

	HubTemplate* getHubTemplate();
	void setHubTemplate(HubTemplate* hub);
	
	std::string getMaterialName();
	void setMaterialName(std::string materialName);

	std::string getDescription();
	void setDescription(std::string desc);

	std::string getLuaScriptFile();
	void setLuaScriptFile(const std::string& file);

	std::string getLuaEntryPoint();
	void setLuaEntryPoint(const std::string& entry);

	float getLuaParam1();
	void setLuaParam1(float param);
	
	float getLuaParam2();
	void setLuaParam2(float param);

	float getLuaParam3();
	void setLuaParam3(float param);

	float getLuaParam4();
	void setLuaParam4(float param);

	Ogre::Vector3 getAmbientLight();
	void setAmbientLight(const Ogre::Vector3& light);

	Ogre::Vector3 getDirectionalLight();
	void setDirectionalLight(const Ogre::Vector3& light);

	Ogre::Vector3 getDirectionalLightDirection();
	void setDirectionalLightDirection(const Ogre::Vector3& dir);
private:
	std::vector<TerrainTextureTemplate*> mTerrainTextures;

	std::string mDescription;
	float mMinHeight;
	float mMaxHeight;
	float mGravity;

	HubTemplate* mHubTemplate;
	std::string mMaterialName;
	
	std::string mLuaScriptFile;
	std::string mLuaEntryPoint;
	float mLuaParam1;
	float mLuaParam2;
	float mLuaParam3;
	float mLuaParam4;

	Ogre::Vector3 mAmbientLight;
	Ogre::Vector3 mDirectionalLight;
	Ogre::Vector3 mDirectionLightDirection;
};
#endif