#ifndef RandomMissionTypeTemplate_h
#define RandomMissionTypeTemplate_h

#include "Ogre.h"
#include <Templates/Template.h>



class RandomMissionTypeTemplate : public Template
{
public:
	RandomMissionTypeTemplate();
	~RandomMissionTypeTemplate();

	std::string getPlacementScriptFilename();
	void setPlacementScriptFilename(const std::string& file);

	std::string getPlayerPlacementEntryPoint();
	void setPlayerPlacementEntryPoint(const std::string& entry);

	std::string getEnemyGenerationFilename();
	void setEnemyGenerationFilename(const std::string& filename);
	
	std::string getEnemyGenerationEntryPoint();
	void setEnemyGenerationEntryPoint(const std::string& entry);

	std::string getEnemyPlacementEntryPoint();
	void setEnemyPlacementEntryPoint(const std::string& entry);

private:
	std::string mEnemyGenerationFilename;
	std::string mEnemyGenerationEntryPoint;
	std::string mPlacementScriptFilename;
	std::string mPlayerPlacementEntryPoint;
	std::string mEnemyPlacementEntryPoint;
};


#endif