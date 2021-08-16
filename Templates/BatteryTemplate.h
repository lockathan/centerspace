#ifndef BatteryTemplate_h
#define BatteryTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>
class DbValue;

class BatteryTemplate : public MCUComponentTemplate
{
public:
	BatteryTemplate();
	~BatteryTemplate();

	float getMaxEnergy();
	void setMaxEnergy(float maxEnergy);
	
	
	

protected:
	float mMaxEnergy;
};


#endif