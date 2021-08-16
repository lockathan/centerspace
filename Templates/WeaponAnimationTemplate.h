#ifndef WeaponAnimationTemplate_h
#define WeaponAnimationTemplate_h

#include "Ogre.h"
#include "Template.h"
#include <string>


class WeaponAnimationTemplate : public Template
{
public:
	WeaponAnimationTemplate();
	~WeaponAnimationTemplate();
	
	int getHardpointNumber();
	void setHardpointNumber(int num);
	
	bool hasEnterAnimation();
	std::string getEnterAnimationName();
	void setEnterAnimationName(std::string name);
	
	std::string getFireAnimationName();
	void setFireAnimationName(std::string name);
	
	bool hasExitAnimation();
	std::string getExitAnimationName();
	void setExitAnimationName(std::string name);

	float getEnterAnimationSpeed();
	void setEnterAnimationSpeed(float speed);
	
	float getFireAnimationSpeed();
	void setFireAnimationSpeed(float speed);
	
	float getExitAnimationSpeed();
	void setExitAnimationSpeed(float speed);
protected:
	int mHardpointNumber;
	float mEnterAnimationSpeed;
	float mFireAnimationSpeed;
	float mExitAnimationSpeed;
	std::string mEnterAnimationName;
	std::string mFireAnimationName;
	std::string mExitAnimationName;

};
#endif