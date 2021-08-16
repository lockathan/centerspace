#include <Managers/LuaStateManager.h>
#include <Ogre.h>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "luaconf.h"
	#include "lauxlib.h"
}

#include "tolua++.h"
#include <LuaBindings/CenterSpace.hpp>
#include <Controllers/MCUController.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <AI/GroupAttackOrder.h>
#include <AI/GroupMoveOrder.h>
#include <AI/GroupMoveToAttackOrder.h>
#include <AI/AIGroup.h>
#include <Entities/DataSet.h>
#include <Entities/GridSquareTemplateVector.h>
#include <Entities/MechanizedCombatUnit.h>
#include <LuaBindings/LuaMCUDeque.h>
#include <StateChange/PlayerPreBattleState.h>
#include <States/GameState.h>
#include <States/CampaignState.h>
#include <States/PlayerBattleState.h>
#include <Templates/BatteryTemplate.h>
#include <Templates/FactionTemplate.h>

#include <string>
#include <deque>

#include <Rocket/Core.h>
#include <LuaLibRocket/LuaElement.h>
#include <LuaLibRocket/LuaElementCache.h>

using namespace Ogre;

LuaStateManager::LuaStateManager()
{

}

LuaStateManager::~LuaStateManager() 
{
	
}

void LuaStateManager::loadScript(GameState* state, const std::string& scriptfile)
{
	
	if(!loadedScript(scriptfile, state->getLoadedScripts()))
	{
		DataStreamPtr ds = ResourceGroupManager::getSingletonPtr()->openResource(scriptfile);
		ds->seek(0);
		String file = ds->getAsString();
		//luaL_dofile(lState, file.c_str());
		luaL_dostring(state->getLuaState(), file.c_str());
		ds->close();
		state->getLoadedScripts().push_back(scriptfile);
	}
}

void LuaStateManager::issueOrders(GameState* state, PlayerBattleState* pState)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	pState->updateCoverageMap();
	lua_getglobal(lState, "IssueOrders");
	tolua_pushusertype(lState, (void*)(pState), "PlayerBattleState");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred issuing player orders: " + error, 0);
		pState->endMyTurn();
	}
}

void LuaStateManager::issueOrders(GameState* state, AIGroup* group)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	group->getPlayerBattleState()->updateCoverageMap(false);
	lua_getglobal(lState, "IssueGroupOrders");
	tolua_pushusertype(lState, (void*)(group), "AIGroup");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred issuing group orders: " + error, 0);
		group->finishedTurn();
	}
}

void LuaStateManager::executeGroupAttackOrder(GameState* state, GroupAttackOrder* order)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, "ExecuteGroupAttackOrder");
	tolua_pushusertype(lState, (void*)(order), "GroupAttackOrder");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred executing group attack order: " + error, 0);
		order->getOwner()->finishedGroupAttackOrder();
	}
}

void LuaStateManager::executeGroupMoveOrder(GameState* state, GroupMoveOrder* order)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, "ExecuteGroupMoveOrder");
	tolua_pushusertype(lState, (void*)(order), "GroupMoveOrder");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred executing group move order: " + error, 0);
		order->getOwner()->finishedGroupMoveOrder();
	}
}

void LuaStateManager::executeGroupMoveToAttackOrder(GameState* state, GroupMoveToAttackOrder* order)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, "ExecuteGroupMoveToAttackOrder");
	tolua_pushusertype(lState, (void*)(order), "GroupMoveToAttackOrder");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred executing group move to attack order: " + error, 0);
		order->getOwner()->finishedGroupMoveToAttackOrder();
	}
}

void LuaStateManager::returnFire(GameState* state, MCUController* controller)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, "ReturnFire");
	tolua_pushusertype(lState, (void*)(controller), "MCUController");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling return fire function: " + error, 0);
	}
}

void LuaStateManager::movementFailedDueToBlockage(GameState* state, MCUController* mcu, MCUController* blockage, BattlefieldGridCell* destination)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, "MovementFailedDueToBlockage");
	tolua_pushusertype(lState, (void*)(mcu), "MCUController");
	tolua_pushusertype(lState, (void*)(blockage), "MCUController");
	tolua_pushusertype(lState, (void*)(destination), "BattlefieldGridCell");
	if (lua_pcall(lState, 3, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling GUI function: " + error, 0);
	}
}

void LuaStateManager::executeGuiFunction(GameState* state, const std::string& functionName, Rocket::Core::Element* element)
{
	if(!state->isEnabled())
		return;

	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, functionName.c_str());
	LuaElement* lElement = new LuaElement(element);
	tolua_pushusertype(lState, (void*)(lElement), "LuaElement");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling GUI function: " + error, 0);
	}
}

void LuaStateManager::executeDataBindFunction(GameState* state, const std::string& functionName, DataSet* dataset, int id, Rocket::Core::Element* dataProvider)
{
	
	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, functionName.c_str());
	tolua_pushusertype(lState, (void*)(dataset), "DataSet");
	tolua_pushnumber(lState, id);
	LuaElement* lElement = new LuaElement(dataProvider);
	tolua_pushusertype(lState, (void*)(lElement), "LuaElement");
	
	if (lua_pcall(lState, 3, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling databind function: " + error, 0);
	}
}

bool LuaStateManager::executeTerrainFunction(GameState* state, const std::string& functionName, GridSquareTemplateVector* vector, float param1, float param2, float param3, float param4)
{
	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, functionName.c_str());
	tolua_pushusertype(lState, (void*)(vector), "GridSquareTemplateVector");
	tolua_pushnumber(lState, param1);
	tolua_pushnumber(lState, param2);
	tolua_pushnumber(lState, param3);
	tolua_pushnumber(lState, param4);

	if (lua_pcall(lState, 5, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling terrain generation function: " + error, 0);
	}
	return vector->checkEntireGrid();
}

void LuaStateManager::executeRepositoryLoadFunction(GameState* state, const std::string& functionName)
{
	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, functionName.c_str());
	if (lua_pcall(lState, 0, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling repository load function: " + error, 0);
	}
}

void LuaStateManager::executeCampaignStartScript(GameState* state, const std::string& functionName, CampaignState* cState)
{
	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, functionName.c_str());
	tolua_pushusertype(lState, (void*)(cState), "CampaignState");
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling campaign start function: " + error, 0);
	}
}

void LuaStateManager::executeEnemyMCUGenerationFunction(GameState* state, const std::string& functionName, FactionTemplate* faction, int rank, PlayerPreBattleState* pState, int playerGroupStrength)
{
	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, functionName.c_str());
	tolua_pushusertype(lState, (void*)(pState), "PlayerPreBattleState");
	tolua_pushnumber(lState, rank);
	tolua_pushusertype(lState, (void*)(faction), "FactionTemplate");
	tolua_pushnumber(lState, playerGroupStrength);
	
	if (lua_pcall(lState, 4, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling enemy mcu generation function: " + error, 0);
	}
}

void LuaStateManager::executeMCUPlacementFunction(GameState* state, const std::string& functionName,LuaMCUDeque * deque, BattlefieldGrid* grid, int x, int z)
{
	lua_State* lState = state->getLuaState();
	lua_getglobal(lState, functionName.c_str());
	tolua_pushusertype(lState, (void*)(deque), "LuaMCUDeque");
	tolua_pushusertype(lState, (void*)(grid), "BattlefieldGrid");
	tolua_pushnumber(lState, x);
	tolua_pushnumber(lState, z);
	
	if (lua_pcall(lState, 4, 0, 0) != 0)
	{
		std::string error = lua_tostring(lState, -1);
		GameManager::getSingletonPtr()->luaDebugReport("Error occurred calling mcu placement function: " + error, 0);
	}
}

lua_State* LuaStateManager::getNewState()
{
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);   // initalize all lua standard library functions
	tolua_open(state);
	tolua_CenterSpace_open(state);
	return state;
}


bool LuaStateManager::loadedScript(const std::string& scriptfile, std::deque<std::string>& scripts)
{
	std::deque<std::string>::iterator iter;

	for(iter = scripts.begin(); iter != scripts.end(); ++iter)
	{
		if((*iter) == scriptfile)
			return true;
	}
	return false;
}

LuaStateManager* LuaStateManager::mLuaStateManager;

LuaStateManager* LuaStateManager::getSingletonPtr()
{
	if(!mLuaStateManager)
	{
		mLuaStateManager = new LuaStateManager();
	}
	return mLuaStateManager;
}

