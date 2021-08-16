#ifndef DriveTemplate_h
#define DriveTemplate_h

#include "Ogre.h"
#include <Templates/MCUComponentTemplate.h>

class DriveTemplate : public MCUComponentTemplate
{
public:
	DriveTemplate();
	~DriveTemplate();

	float getEfficiency();
	void setEfficiency(float e);
	
	float getJumpingEfficiency();
	void setJumpingEfficiency(float e);

protected:
	float mEfficiency;
	float mJumpingEfficiency;

};
#endif