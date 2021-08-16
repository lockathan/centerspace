#ifndef HubTemplate_h
#define HubTemplate_h

#include "Ogre.h"
#include <Templates/RequiredRankTemplate.h>
class FactionTemplate;

class HubTemplate : public RequiredRankTemplate
{
public:
	HubTemplate();
	~HubTemplate();

	std::string getSystemName();
	void setSystemName(std::string name);

	float getBaseMissionPay();
	void setBaseMissionPay(float value);

	float getBounty();
	void setBounty(float value);

protected:
	std::string mSystemName;
	
	float mBaseMissionPay;
	float mBounty;
};


#endif