#ifndef LuaStateManager_H
#define LuaStateManager_H

#include <string>
#include <deque>
#include <Entities/Enums.h>
#include <Rocket/Core.h>

class lua_State;

class AIGroup;
class BattlefieldGrid;
class BattlefieldGridCell;
class CampaignState;
class DataSet;
class FactionTemplate;
class GameState;
class GridSquareTemplateVector;
class GroupMoveOrder;
class GroupAttackOrder;
class GroupMoveToAttackOrder;
class LuaElement;
class LuaMCUDeque;
class MCUController;
class MechanizedCombatUnit;
class PlayerPreBattleState;
class PlayerBattleState;

class LuaStateManager
{
public:
	LuaStateManager();
	~LuaStateManager();
	
	void loadScript(GameState* state, const std::string& scriptfile);

	void issueOrders(GameState* state, PlayerBattleState* pState);
	void issueOrders(GameState* state, AIGroup* group);

	void executeGroupAttackOrder(GameState* state, GroupAttackOrder* order);
	void executeGroupMoveOrder(GameState* state, GroupMoveOrder* order);
	void executeGroupMoveToAttackOrder(GameState* state, GroupMoveToAttackOrder* order);

	void returnFire(GameState* state, MCUController* controller);
	void movementFailedDueToBlockage(GameState* state, MCUController* mcu, MCUController* blockage, BattlefieldGridCell* destination);


	void executeGuiFunction(GameState* state, const std::string& functionName, Rocket::Core::Element* element);
	void executeDataBindFunction(GameState* state, const std::string& functionName, DataSet* dataset, int id , Rocket::Core::Element* dataProvider);
	bool executeTerrainFunction(GameState* state, const std::string& functionName, GridSquareTemplateVector* vector, float param1, float param2, float param3, float param4);

	void executeRepositoryLoadFunction(GameState* state, const std::string& functionName);
	void executeCampaignStartScript(GameState* state, const std::string& functionName, CampaignState* cState);
	
	void executeEnemyMCUGenerationFunction(GameState* state, const std::string& functionName, FactionTemplate* faction, int rank, PlayerPreBattleState* pState, int playerGroupStrength);
	void executeMCUPlacementFunction(GameState* state, const std::string& functionName,LuaMCUDeque * deque, BattlefieldGrid* grid, int x, int z);

	
	
	
	lua_State* getNewState();

	static LuaStateManager* getSingletonPtr();
private:
	bool loadedScript(const std::string& scriptfile, std::deque<std::string>& scripts);
	static LuaStateManager* mLuaStateManager;

};
#endif