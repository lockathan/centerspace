#ifndef BattlefieldTemplate_h
#define BattlefieldTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>
class HubTemplate;

class BattlefieldTemplate : public Template
{
public:
	BattlefieldTemplate();
	~BattlefieldTemplate();

	int getInitialSize();
	void setInitialSize(int value);
	
	int getMinMCUs();
	void setMinMCUs(int value);
protected:
	int mInitialSize;
	int mMinMCUs;
};
#endif