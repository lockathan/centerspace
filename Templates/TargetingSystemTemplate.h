#ifndef TargetingSystemTemplate_h
#define TargetingSystemTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>

class TargetingSystemTemplate : public MCUComponentTemplate
{
public:
	TargetingSystemTemplate();
	~TargetingSystemTemplate();

	float getBaseAccuracy();
	void setBaseAccuracy(float value);
	
	float getObstructedBonus();
	void setObstructedBonus(float value);
	
	float getLongRangeBonus();
	void setLongRangeBonus(float value);

protected:
	float mBaseAccuracy;
	float mObstructedBonus;
	float mLongRangeBonus;

};
#endif