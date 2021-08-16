#ifndef FactionTemplate_h
#define FactionTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>

class FactionTemplate : public Template
{
public:

	FactionTemplate();
	~FactionTemplate();
	
	std::string getDescription();
	void setDescription(const std::string& desc);

	std::string getStartScript();
	void setStartScript(const std::string& script);

	std::string getStartEntrypoint();
	void setStartEntrypoint(const std::string& entry);

	bool isGenericFaction();
	void setIsGenericFaction(bool value);
protected:
	bool mIsGenericFaction;
	std::string mDescription;
	std::string mStartScript;
	std::string mStartEntrypoint;
};
#endif