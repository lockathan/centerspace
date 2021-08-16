#ifndef AnimationDefinitionTemplateDataLoader_h
#define AnimationDefinitionTemplateDataLoader_h

#include "Ogre.h"
#include <Repositories/DataLoader.h>

class AnimationDefinitionTemplateDataLoader : public DataLoader
{
public:
	AnimationDefinitionTemplateDataLoader();
	~AnimationDefinitionTemplateDataLoader();
	void load(const std::string& fileName );	
};
#endif