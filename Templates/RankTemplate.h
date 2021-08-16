#ifndef RankTemplate_h
#define RankTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>

class FactionTemplate;
class RankTemplate : public Template
{
public:
	RankTemplate();
	~RankTemplate();
	
	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);

	int getRequiredExperience();
	void setRequiredExperience(int exp);

	int getRank();
	void setRank(int rank);
protected:
	FactionTemplate* mFactionTemplate;
	int mRequiredExperience;
	int mRank;
};
#endif