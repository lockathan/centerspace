#ifndef TerrainTextureTemplate_h
#define TerrainTextureTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>
#include <hash_map>


class TerrainTextureTemplate : public Template
{
public:
	TerrainTextureTemplate();
	~TerrainTextureTemplate();

	float getRoughness();
	void setRoughness(float value);

	std::string getTextureName();
	void setTextureName(const std::string& textureName);

private:
	float mRoughness;
	std::string mTextureName;
};
#endif