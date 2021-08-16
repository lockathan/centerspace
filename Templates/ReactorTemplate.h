#ifndef ReactorTemplate_h
#define ReactorTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>

class ReactorTemplate : public MCUComponentTemplate
{
public:
	ReactorTemplate();
	~ReactorTemplate();

	float getMaxEnergy();
	void setMaxEnergy(float maxEnergy);
	
	
protected:
	float mMaxEnergy;
};
#endif