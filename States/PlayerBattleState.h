#ifndef PlayerBattleState_h
#define PlayerBattleState_h

#include <hash_map>
#include <deque>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MCUPlacement.h>

class AIGroup;
class CoverageMap;
class FactionTemplate;
class GroupOrder;
class MovableEntity;
class PlayerProfile;
class MCUController;
class Projectile;

class PlayerBattleState
{
public:
	PlayerBattleState();
	PlayerBattleState(PlayerProfile* player,FactionTemplate* faction, int team, int playerNumber);
	~PlayerBattleState();

	void startTurn();
	void endTurn();
	void endMyTurn();

	PlayerProfile* getPlayerProfile();
	void setPlayer(PlayerProfile* player);
	
	int getTeam();
	void setTeam(int team);

	bool isMyTurn();
	void setMyTurn(bool turn);

	int getPlayerNumber();
	void setPlayerNumber(int n);

	std::deque<MCUController*> getMCUs();
	void addMCU(MCUController* mcu);
	void setMCUs(std::deque<MCUController*> mcus);
	void removeMCU(MCUController* mcu);	

	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);
		
	bool isLocal();
	void setLocal(bool value);

	bool needsRender();
	void setNeedsRender(bool value);

	bool isAI();
	void setAI(bool value);

	std::deque<Projectile*> getProjectiles();
	void addProjectile(Projectile* proj);
	void removeProjectile(Projectile* proj);

	void loadScript(const std::string& scriptName, const std::string& scriptEntryPoint);
	
	std::string getAIScriptEntryPoint();
	std::string getAIScriptName();

	int getAverageMCUXPos();
	int getAverageMCUZPos();

	void updateVisibilityState();
	void updateProjectileVisibility();
	void updateMCUVisibility();

	std::hash_map<std::string,std::string> getGuiValues();

	void setAIGroups(std::deque<AIGroup*> formations);
	void addAIGroup(AIGroup* formation);
	
	bool hasMoreAIGroups();
	AIGroup* getNextAIGroup();

	bool hasMoreVisibleMCUs();
	MCUController* getNextVisibleMCU();
	std::deque<MCUController*> getAllVisibleMCUs();

	void notifyOfMovementChange(MCUController* controller);
	void notifyOfEnemyVisibilityChange(MCUController* controller, MCUController* enemy);
	void notifyOfNewOrders(AIGroup* group, GroupOrder* order);
	void notifyOfOrderCompletion(AIGroup* group, GroupOrder* order);
	void notifyOfFinishedTurn(AIGroup* group);

	void loopFinished();

	CoverageMap* getCoverageMap();
	void updateCoverageMap(bool reset = true);

	MCUController* getNextMCU(MCUController* mcu);
	MCUController* getPrevMCU(MCUController* mcu);
private:
	void runAIScript();
	
	PlayerProfile* mPlayerProfile;
	FactionTemplate* mFactionTemplate;
	int mTeam;
	int mPlayerNumber;
	bool mMyTurn;
	bool mLocal;
	bool mNeedsRender;
	bool mFirstVisibilityRun;
	bool mIsAI;

	std::deque<MCUController*> mMCUs;
	std::deque<MCUController*> mVisibleMCUs;
	std::deque<MCUController*>::iterator mNextVisibleMCU;
	std::deque<MCUController*> mNotVisibleMCUs;
	std::deque<MCUController*> mRadarVisibleMCUs;

	std::deque<Projectile*> mProjectiles;
	std::string mAIScriptName;
	std::string mAIScriptEntryPoint;

	std::deque<AIGroup*> mAIGroups;
	std::deque<AIGroup*>::iterator mAIGroupIterator;
	CoverageMap* mCoverageMap;
};
#endif