#ifndef PlayerPreBattleState_h
#define PlayerPreBattleState_h

#include <deque>
#include <Entities/MCUPlacement.h>

class AIGroup;
class AIGroupTemplate;
class FactionTemplate;
class LuaMCUDeque;
class PlayerProfile;

class PlayerPreBattleState
{
public:
	PlayerPreBattleState();
	PlayerPreBattleState(PlayerProfile* player,FactionTemplate* faction, int team, int playerNumber);
	~PlayerPreBattleState();

	PlayerProfile* getPlayerProfile();
	void setPlayer(PlayerProfile* player);

	int getTeam();
	void setTeam(int team);

	int getPlayerNumber();
	void setPlayerNumber(int n);

	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);
		
	bool isLocal();
	void setLocal(bool value);
	
	bool needsRender();
	void setNeedsRender(bool value);

	bool isAI();
	void setAI(bool value);

	std::string getAIScriptName();
	void setAIScriptName(const std::string& script);

	std::string getAIScriptEntryPoint();
	void setAIScriptEntryPoint(const std::string& entry);

	std::deque<MCUPlacement*>& getMCUPlacements();

	std::deque<AIGroupTemplate*>& getAIGroupTemplates();
	void addAIGroupTemplate(AIGroupTemplate* group);

	std::deque<AIGroup*>& getAIGroups();
	void addAIGroup(AIGroup* group);

	LuaMCUDeque* getLuaMCUDeque();
	void setLuaMCUDeque(LuaMCUDeque* deque);

	bool isCellOccupied(int x, int z);

	void resetAIGroupMCUs();
private:
	PlayerProfile* mPlayerProfile;
	FactionTemplate* mFactionTemplate;
	int mTeam;
	int mPlayerNumber;
	bool mLocal;
	bool mNeedsRender;
	bool mIsAI;
	bool mFetchedMCUPlacements;
	std::string mAIScriptName;
	std::string mAIScriptEntryPoint;
	std::deque<MCUPlacement*> mAllMCUPlacements;
	std::deque<AIGroupTemplate*> mAIGroupTemplates;
	std::deque<AIGroup*> mAIGroups;
	LuaMCUDeque* mLuaMCUDeque;
};
#endif