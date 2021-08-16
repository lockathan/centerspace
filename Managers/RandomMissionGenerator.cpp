#include <Managers/RandomMissionGenerator.h>
#include <Entities/Mission.h>
#include <Entities/MCUPlacement.h>
#include <Entities/RandomMission.h>
#include <Repositories/AIGroupTemplateRepository.h>
#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/RandomMissionTypeTemplateRepository.h>
#include <Repositories/TerrainTemplateRepository.h>
#include <AI/AIGroup.h>
#include <States/CampaignState.h>
#include <States/InventoryState.h>
#include <States/PlayerBattleState.h>
#include <Templates/AIGroupTemplate.h>
#include <Templates/AIMCUTemplate.h>
#include <Templates/BattlefieldTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/HubTemplate.h>
#include <Templates/RandomMissionTypeTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <Terrain/TerrainManager.h>
#include <Terrain/TerrainImgGenerator.h>
#include <vector>

RandomMissionGenerator::RandomMissionGenerator()
{

	  
}

RandomMissionGenerator::~RandomMissionGenerator() 
{

}


Mission* RandomMissionGenerator::generateMission(CampaignState* state, int missionId)
{

	std::deque<BattlefieldTemplate*> battlefields = BattlefieldTemplateRepository::getSingletonPtr()->getAvailableTemplates(state->getInventoryState()->getMCUs().size());
	std::deque<TerrainTemplate*> terrains = TerrainTemplateRepository::getSingletonPtr()->getAvailableTemplates(state->getFactionHubTemplate()->getHubTemplate()->getId());
	std::deque<RandomMissionTypeTemplate*> randomTypes = RandomMissionTypeTemplateRepository::getSingletonPtr()->getAvailableTemplates();
	int battlefieldId = rand() % battlefields.size();
	int terrainId = rand() % terrains.size();
	int typeId = rand() % randomTypes.size();

	Mission* mission = new Mission(randomTypes[typeId], battlefields[battlefieldId]->getInitialSize(), state->getRank(), terrains[terrainId], state->getFactionHubTemplate());
	mission->setId(missionId);
	mission->setName(randomTypes[typeId]->getName() + " on " + terrains[terrainId]->getName());
	
	return mission;
}

void RandomMissionGenerator::finalizeMission(Mission* mission, CampaignState* state, BattlefieldGrid* grid)
{
	/*
	int terrainSideLength = mission->getSideLength();

	int playerSeedX, playerSeedZ = 0;

	playerSeedX = rand() % (terrainSideLength - 1);
	playerSeedZ = rand() % (terrainSideLength - 1);
	mission->setPlayerFactionMCUs(TerrainManager::getSingletonPtr()->placeMCUs(terrainSideLength,playerSeedX,playerSeedZ,state->getInventoryState()->getMCUs(), grid));

	float distance;
	float minSquareDistance = (terrainSideLength / 4) * (terrainSideLength / 4);

	int numEnemyFactions(0);

	if(mission->getFactionHubTemplate()->getEnemy1FactionTemplate())
		++numEnemyFactions;
	if(mission->getFactionHubTemplate()->getEnemy2FactionTemplate())
		++numEnemyFactions;

	int faction = rand() % numEnemyFactions;

	FactionTemplate* enemy;
	if(faction == 0)
		enemy = mission->getFactionHubTemplate()->getEnemy1FactionTemplate();

	if(faction == 1)
		enemy = mission->getFactionHubTemplate()->getEnemy2FactionTemplate();
	
	if(enemy)
	{
		AIPreBattleState* prebattlestate = new AIPreBattleState();

		int enemy1SeedX, enemy1SeedZ = 0;
		std::vector<AIGroupTemplate*> groups1;
		groups1 = AIGroupTemplateRepository::getSingletonPtr()->getAvailableAIGroupTemplates(state->getFactionHubTemplate()->getEnemy1FactionTemplate()->getId(),state->getRank());

		int maxTotalMaxGroupStrength(state->getRank() * 2);
		int currentTotalGroupStrength(0);

		while(currentTotalGroupStrength < maxTotalMaxGroupStrength)
		{
			int groupId1 = rand() % groups1.size();
			AIGroupTemplate* groupTemplate = groups1[groupId1];
			int groupStrength = groupTemplate->getStrength();
			groupStrength-= (state->getRank() - groupTemplate->getRequiredRank()) * 2;
			if(groupStrength < 1)
				groupStrength = 1;

			distance = 0;
			while(distance < minSquareDistance)
			{
				enemy1SeedX = rand() % (terrainSideLength - 1);
				enemy1SeedZ = rand() % (terrainSideLength - 1);

				distance = TerrainManager::calculateSquareDistance(playerSeedX, playerSeedZ, enemy1SeedX, enemy1SeedZ);
			}
			std::deque<MCUPlacement> placements = TerrainManager::getSingletonPtr()->placeMCUs(terrainSideLength,enemy1SeedX,enemy1SeedZ,groupTemplate->getMCUs(), grid);
			AIGroup* group = groupTemplate->instantiate();
			group->setMCUPlacements(placements);
			prebattlestate->addEnemyAIGroup(group);
			currentTotalGroupStrength += groupStrength;
		}
		mission->addAIPreBattleState(prebattlestate);
	}
	*/
}


RandomMissionGenerator* RandomMissionGenerator::mRandomMissionGenerator;

RandomMissionGenerator* RandomMissionGenerator::getSingletonPtr() {
	if( !mRandomMissionGenerator ) {
		mRandomMissionGenerator = new RandomMissionGenerator();
	}

	return mRandomMissionGenerator;
}
