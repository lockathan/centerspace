#include <Managers/CampaignMissionManager.h>
#include <AI/AIGroup.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/GridSquareTemplateVector.h>
#include <Entities/Mission.h>
#include <Entities/MCUPlacement.h>
#include <Entities/PlayerProfile.h>
#include <Entities/RandomMission.h>
#include <LuaBindings/LuaMCUDeque.h>
#include <Managers/GameManager.h>
#include <Managers/LuaStateManager.h>
#include <Managers/RandomMissionGenerator.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/HubTemplateRepository.h>
#include <Repositories/RandomMissionTypeTemplateRepository.h>
#include <Repositories/StoryMissionTemplateRepository.h>
#include <Repositories/TerrainTemplateRepository.h>
#include <StateChange/PlayerPreBattleState.h>
#include <States/CampaignState.h>
#include <States/InBaseState.h>
#include <States/InBattleState.h>
#include <States/InventoryState.h>
#include <States/PlayerBattleState.h>
#include <Templates/AIGroupTemplate.h>
#include <Templates/BattlefieldTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/HubTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <Terrain/TerrainImgGenerator.h>
#include <hash_map>
#include <vector>
#include <deque>
#include <string>

CampaignMissionManager::CampaignMissionManager()
{
	mRandomMissionGenerator = RandomMissionGenerator::getSingletonPtr();
	mRandomMissions.resize(10);
}

CampaignMissionManager::~CampaignMissionManager() 
{

}


CampaignState* CampaignMissionManager::getCampaignState()
{
	return mCampaignState;	
}

void CampaignMissionManager::setCampaignState(CampaignState* campaign)
{
	mCampaignState = campaign;
}

FactionTemplate* CampaignMissionManager::setFactionTemplate()
{
	return mFactionTemplate;
}

void CampaignMissionManager::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

void CampaignMissionManager::loadRandomMission(int id, PlayerProfile* player)
{
	loadRandomMission(mRandomMissions[(id-1)], player);

}

void CampaignMissionManager::loadRandomMission(Mission* mission, PlayerProfile* player)
{
	InBattleState::getSingletonPtr()->setBattleResultInterpreter(mCampaignState);
	InBattleState::getSingletonPtr()->setPostBattleState(InBaseState::getSingletonPtr());
	InBattleState::getSingletonPtr()->beginSetup(mission->getSideLength());
	
	
	LuaStateManager::getSingletonPtr()->loadScript(RocketRenderer::getSingletonPtr()->getActiveState(), mission->getTerrainGenerationFilename());
	GridSquareTemplateVector* grid(0);
	grid = new GridSquareTemplateVector(mission->getTerrainTemplate(), mission->getSideLength());
	LuaStateManager::getSingletonPtr()->executeTerrainFunction(RocketRenderer::getSingletonPtr()->getActiveState(), mission->getTerrainGenerationEntryPoint(), grid, mission->getLuaParam1(), mission->getLuaParam2(), mission->getLuaParam3(), mission->getLuaParam4());		
	
	int correct(0);

	for(int i = 0; i < 1000; ++i)
	{
		if(grid)
		{
			delete grid;
			grid = 0;
		}
		grid = new GridSquareTemplateVector(mission->getTerrainTemplate(), mission->getSideLength());
		if(LuaStateManager::getSingletonPtr()->executeTerrainFunction(RocketRenderer::getSingletonPtr()->getActiveState(), mission->getTerrainGenerationEntryPoint(), grid, mission->getLuaParam1(), mission->getLuaParam2(), mission->getLuaParam3(), mission->getLuaParam4()))
			break;
	}

	//GameManager::getSingletonPtr()->luaDebugReport("Number of correct terrains per 1000: ", correct);
	//GameManager::getSingletonPtr()->requestShutdown();
	

	BattlefieldGrid* mBattlefieldGrid = BattlefieldGrid::createBattlefieldGrid(grid, mission->getTerrainTemplate());
	mBattlefieldGrid->setMission(mission);
	InBattleState::getSingletonPtr()->setBattlefieldGrid(mBattlefieldGrid);
	PlayerPreBattleState* pState = new PlayerPreBattleState(player, mission->getFactionHubTemplate()->getPlayerFactionTemplate(), 1, 1);
	pState->setNeedsRender(true);
	pState->getLuaMCUDeque()->setMCUs(mCampaignState->getInventoryState()->getMCUs());
	mission->getPlayerPreBattleStates().push_back(pState);
	LuaStateManager::getSingletonPtr()->loadScript( InBattleState::getSingletonPtr(), mission->getPlacementFilename());
	LuaStateManager::getSingletonPtr()->executeMCUPlacementFunction(InBattleState::getSingletonPtr(), mission->getPlayerPlacementEntryPoint(), pState->getLuaMCUDeque(), mBattlefieldGrid, mission->getPlayerLocationX(), mission->getPlayerLocationZ());
	
	LuaStateManager::getSingletonPtr()->loadScript(InBattleState::getSingletonPtr(), mission->getEnemyGenerationFilename());
	if(mission->getFactionHubTemplate()->getEnemy1FactionTemplate())
	{
		PlayerProfile* enemy1 = new PlayerProfile();
		enemy1->setIsAI(true);
		enemy1->setName(mission->getFactionHubTemplate()->getEnemy1FactionTemplate()->getName());
		PlayerPreBattleState* enemy1PState = new PlayerPreBattleState(enemy1, mission->getFactionHubTemplate()->getEnemy1FactionTemplate(), 2,2);
		LuaStateManager::getSingletonPtr()->executeEnemyMCUGenerationFunction(InBattleState::getSingletonPtr(), mission->getEnemyGenerationEntryPoint(), enemy1PState->getFactionTemplate(), mission->getRequiredRank(), enemy1PState, pState->getLuaMCUDeque()->getStrength());
		mission->getPlayerPreBattleStates().push_back(enemy1PState);
		std::deque<AIGroup*>& aiGroups = enemy1PState->getAIGroups();
		std::deque<AIGroup*>::iterator groupIter;
		for(groupIter = aiGroups.begin(); groupIter != aiGroups.end(); ++groupIter)
		{
			LuaStateManager::getSingletonPtr()->executeMCUPlacementFunction(InBattleState::getSingletonPtr(), mission->getEnemyPlacementEntryPoint(), (*groupIter)->getLuaMCUDeque(), mBattlefieldGrid,mission->getEnemy1LocationX(),  mission->getEnemy1LocationZ());
		}
		enemy1PState->setAIScriptName("aiscript.lua");
		
	}

	if(mission->getFactionHubTemplate()->getEnemy2FactionTemplate())
	{
		PlayerProfile* enemy2 = new PlayerProfile();
		enemy2->setIsAI(true);
		enemy2->setName(mission->getFactionHubTemplate()->getEnemy1FactionTemplate()->getName());
		PlayerPreBattleState* enemy2PState = new PlayerPreBattleState(enemy2, mission->getFactionHubTemplate()->getEnemy1FactionTemplate(), 2,3);
		LuaStateManager::getSingletonPtr()->executeEnemyMCUGenerationFunction(InBattleState::getSingletonPtr(), mission->getEnemyGenerationEntryPoint(), enemy2PState->getFactionTemplate(), mission->getRequiredRank(), enemy2PState, pState->getLuaMCUDeque()->getStrength());
		mission->getPlayerPreBattleStates().push_back(enemy2PState);
		std::deque<AIGroup*>& aiGroups = enemy2PState->getAIGroups();
		std::deque<AIGroup*>::iterator groupIter;
		for(groupIter = aiGroups.begin(); groupIter != aiGroups.end(); ++groupIter)
		{
			LuaStateManager::getSingletonPtr()->executeMCUPlacementFunction(InBattleState::getSingletonPtr(), mission->getEnemyPlacementEntryPoint(), (*groupIter)->getLuaMCUDeque(), mBattlefieldGrid,mission->getEnemy1LocationX(),  mission->getEnemy1LocationZ());
		}
		enemy2PState->setAIScriptName("aiscript.lua");
	}
	InBattleState::getSingletonPtr()->loadMission(mission );

	for(int i = 0;i < 5;++i)
	{
		//delete all of the old missions except for the one we will use, and set their pointers to 0.
		if(mRandomMissions[i])
		{
			if(mRandomMissions[i] != mission)
				delete mRandomMissions[i];
			mRandomMissions[i] = 0;
		}

	}
	GameManager::getSingletonPtr()->changeState(InBattleState::getSingletonPtr());

	/*
	if(GameManager::getSingletonPtr()->getActiveState() != InBaseState::getSingletonPtr())
	{
		GameManager::getSingletonPtr()->changeState(InBaseState::getSingletonPtr());
	}
	InBattleState* battleState = InBattleState::getSingletonPtr();
	battleState->beginSetup(mission->getSideLength(), mCampaignState->getFactionHubTemplate()->getHubTemplate()->getBaseMissionPay(), mCampaignState->getFactionHubTemplate()->getHubTemplate()->getBounty());
	PostBattleState::getSingletonPtr()->setBattleResultInterpreter(mCampaignState);
	TerrainTemplate* terrain = mission->getTerrainTemplate();

	GridSquareTemplateVector* grid = new GridSquareTemplateVector(terrain, mission->getSideLength());
	
	//grid->changeSquare(3, 3, 1);
	
	//grid->createValleyOrRidge(5,5,40,5,2,1);
	//grid->createValleyOrRidge(14, 14, 14, 35, 10, 2);
	//grid->createValleyOrRidge(14,8,14,45,5,1);
	//grid->createValleyOrRidge(34, 14, 34, 35, 6, 1);
	//grid->createValleyOrRidge(34, 16, 34, 32, 3, 2, true);
	
	//grid->createValleyOrRidge(5, 5, 25, 25, 5, 2);
	//grid->createValleyOrRidge(1, 1, 8, 8, 2, 2);

	//grid->createValleyOrRidge(25, 5, 5, 25, 5, 1);
	
	//grid->createValleyOrRidge(2, 2, 6, 6, 2, 1);
	//grid->createPlateau(25, 25, 2, 5, 2);
	//grid->createPlateau(25, 25, 5, 15, 1);
	//grid->createPlateau(8, 8, 0, 2, 2);

	//grid->createValleyOrRidge(6, 2, 6, 10, 2, 2);
	//grid->createValleyOrRidge(2, 6, 10, 6, 3, 1);
	//grid->createValleyOrRidge(3, 5, 10, 5, 2, 2);
	//grid->createValleyOrRidge(56, 32, 64, 32, 3, 1);
	//grid->createValleyOrRidge(4, 2, 4, 6, 2, 2);
	//grid->changeSquare(4, 4, 1);
	//bool gridValid = grid->checkEntireGrid();
	LuaStateManager::getSingletonPtr()->loadScript(RocketRenderer::getSingletonPtr()->getActiveState(), terrain->getLuaScriptFile());
	LuaStateManager::getSingletonPtr()->executeTerrainFunction(RocketRenderer::getSingletonPtr()->getActiveState(), terrain->getLuaEntryPoint(), grid, terrain->getLuaParam1(), terrain->getLuaParam2(), terrain->getLuaParam3(), terrain->getLuaParam4());
	battleState->setBattlefieldGrid(BattlefieldGrid::createBattlefieldGrid(grid, mission->getTerrainTemplate()));

	//grid->changeSquare(40, 38, 1);
	//grid->changeSquare(40, 40, 1);
	//grid->changeSquare(40, 42, 1);
	//grid->changeSquare(42, 40, 1);

	//battleState->setBattlefieldGrid(BattlefieldGrid::createBattlefieldGrid(mission->getHeightData(),mission->getSideLength(),mission->getTerrainTemplate()));
	

	mRandomMissionGenerator->finalizeMission(mission,mCampaignState, battleState->getBattlefieldGrid());
	
	PlayerPreBattleState* pState =  new PlayerPreBattleState(player,mission->getFactionHubTemplate()->getPlayerFactionTemplate(),1);
	pState->NeedsRender = true;
	pState->MCUPlacements = mission->getPlayerFactionMCUs();
	battleState->addPlayer(pState);
	
	std::deque<AIPreBattleState*> aiPreBattleStates = mission->getAIPreBattleStates();
	std::deque<AIPreBattleState*>::iterator iter;

	int enemyCount = 1;
	for(iter = aiPreBattleStates.begin(); iter != aiPreBattleStates.end(); ++iter)
	{
		PlayerProfile* enemy = new PlayerProfile();
		enemy->setIsAI(true);
		enemy->setName("Enemy " + SQLiteDBManager::convertIntToString(enemyCount));
		PlayerPreBattleState* state = new PlayerPreBattleState(enemy,mission->getFactionHubTemplate()->getEnemy1FactionTemplate(),2);
		state->IsAI = true;
		state->AIScriptName = "aiscript.lua";
		state->AIScriptEntryPoint = "RunAI";
		state->MCUPlacements = (*iter)->getAllMCUPlacements();
		state->AIGroups = (*iter)->getEnemyAIGroups();
		++enemyCount;
		battleState->addPlayer(state);
	}
		
	GameManager::getSingletonPtr()->changeState(battleState);
	*/
}

void CampaignMissionManager::generateRandomMissions()
{
	for(int i = 0;i < 5;++i)
	{
		if(mRandomMissions[i])
		{
			delete mRandomMissions[i];
			mRandomMissions[i] = 0;
		}

		mRandomMissions[i] = mRandomMissionGenerator->generateMission(mCampaignState,i + 1);
	}
}

DataSet* CampaignMissionManager::getRandomMissionDataSet()
{
	
	std::deque<DataSetItem*> result;
	std::deque<Mission*>::iterator randomIterator;
	
	generateRandomMissions();

	for(randomIterator = mRandomMissions.begin(); randomIterator != mRandomMissions.end(); ++randomIterator)
	{
		if(!(*randomIterator))
			break;

		DataSetItem* item = new DataSetItem();
		
		item->mId = (*randomIterator)->getId();
		item->mData[FieldNames::ItemID] = SQLiteDBManager::convertIntToString((*randomIterator)->getId());
		item->mData[FieldNames::Mission::MissionName] = (*randomIterator)->getName();
		item->mData[FieldNames::Mission::Terrain] = (*randomIterator)->getTerrainTemplate()->getName();
		item->mData[FieldNames::Mission::SystemName] = (*randomIterator)->getFactionHubTemplate()->getHubTemplate()->getName();
		item->mData[FieldNames::Mission::LongDescription] = (*randomIterator)->getName();
		item->mData[FieldNames::Mission::MissionType] = "Random";
		
		result.push_back(item);

	}

	DataSet* ds = new DataSet(DataSourceType::RANDOM_MISSIONS);
	ds->setData(result);

	return ds;
	
	return 0;
}


CampaignMissionManager* CampaignMissionManager::mCampaignMissionManager;

CampaignMissionManager* CampaignMissionManager::getSingletonPtr() 
{
	if( !mCampaignMissionManager ) {
		mCampaignMissionManager = new CampaignMissionManager();
	}

	return mCampaignMissionManager;
}
