#ifndef SensorSystemTemplate_h
#define SensorSystemTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>

class SensorSystemTemplate : public MCUComponentTemplate
{
public:
	SensorSystemTemplate();
	~SensorSystemTemplate();

	float getSightRange();
	void setSightRange(const float& range);

	float getRadarRange();
	void setRadarRange(const float& range);
	
	float getEnergyDrainPerTurn();
	void setEnergyDrainPerTurn(const float& drain);

protected:
	float mSightRange;
	float mRadarRange;
	float mEnergyDrainPerTurn;

};
#endif