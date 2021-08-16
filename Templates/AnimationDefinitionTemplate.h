#ifndef AnimationDefinitionTemplate_h
#define AnimationDefinitionTemplate_h

#include "Ogre.h"
#include "Template.h"
#include <string>
#include <vector>

class MCUTemplate;
class WeaponAnimationTemplate;

class AnimationDefinitionTemplate : public Template
{
public:
	AnimationDefinitionTemplate();
	~AnimationDefinitionTemplate();
	
	std::string getWalkAnimationName();
	void setWalkAnimationName(std::string name);

	std::string getGetHitAnimationName();
	void setGetHitAnimationName(std::string name);

	std::string getTakeOffAnimationName();
	void setTakeOffAnimationName(std::string name);

	std::string getLandAnimationName();
	void setLandAnimationName(std::string name);

	std::string getFlyAnimationName();
	void setFlyAnimationName(std::string name);

	std::string getIdleAnimationName();
	void setIdleAnimationName(std::string name);

	std::string getDeathAnimationName();
	void setDeathAnimationName(std::string name);

	MCUTemplate* getMCUTemplate();
	void setMCUTemplate(MCUTemplate* mcu);

	int getNumWeaponAnimations();
	void setNumWeaponAnimations(int num);
	WeaponAnimationTemplate* getWeaponAnimation(int n);
	void addWeaponAnimationTemplate(int hardpoint, WeaponAnimationTemplate* anim);
	
protected:
	std::string mWalkAnimationName;
	std::string mGetHitAnimationName;
	std::string mTakeOffAnimationName;
	std::string mLandAnimationName;
	std::string mFlyAnimationName;
	std::string mIdleAnimationName;
	std::string mDeathAnimationName;
	MCUTemplate* mMCUTemplate;
	std::vector<WeaponAnimationTemplate*> mWeaponAnimations;

};
#endif