#ifndef RequiredRankTemplate_h
#define RequiredRankTemplate_h

#include <Templates/Template.h>
#include <string>

class RequiredRankTemplate : public Template
{
public:
	int getRequiredRank();
	void setRequiredRank(int rank);
protected:
	int mRequiredRank;
};

#endif