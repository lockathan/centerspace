#ifndef FactionHubTemplate_h
#define FactionHubTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>
class FactionTemplate;
class HubTemplate;

class FactionHubTemplate : public Template
{
public:
	FactionHubTemplate();
	~FactionHubTemplate();

	FactionTemplate* getPlayerFactionTemplate();
	void setPlayerFactionTemplate(FactionTemplate* faction);

	FactionTemplate* getEnemy1FactionTemplate();
	void setEnemy1FactionTemplate(FactionTemplate* faction);

	FactionTemplate* getEnemy2FactionTemplate();
	void setEnemy2FactionTemplate(FactionTemplate* faction);

	HubTemplate* getHubTemplate();
	void setHubTemplate(HubTemplate* hub);

	
protected:
	FactionTemplate* mPlayerFactionTemplate;
	FactionTemplate* mEnemy1FactionTemplate;
	FactionTemplate* mEnemy2FactionTemplate;
	HubTemplate* mHubTemplate;
};


#endif