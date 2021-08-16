#include "InBattleState.h"
#include <Ogre.h>
#include <AI/AIGroup.h>
#include <GUI/RocketDocuments.h>
#include <Controllers/KinematicCharacterController.h>
#include <Controllers/MCUAnimationController.h>
#include <Controllers/MCUController.h>
#include <Entities/BattlefieldGrid.h>
#include <Entities/BattlefieldGridCell.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include <Entities/Enums.h>
#include <Entities/Explosion.h>
#include <Entities/ExplosionSet.h>
#include <Entities/GridSquareTemplateVector.h>
#include <Entities/GridSquareVector.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MCUPlacement.h>
#include <Entities/Mission.h>
#include <Entities/MovableEntity.h>
#include <Entities/PlayerProfile.h>
#include <Entities/Projectile.h>
#include <Entities/Shield.h>
#include <Entities/WeaponHardpoint.h>
#include <LuaBindings/LuaMCUDeque.h>
#include <Managers/GameManager.h>
#include <Managers/InGameCamera.h>
#include <Managers/LuaStateManager.h>
#include <Managers/PathFindingManager.h>
#include <Managers/PhysicsManager.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/BattlefieldTemplateRepository.h>
#include <Repositories/SoundTemplateRepository.h>

#include <Repositories/TerrainTemplateRepository.h>
#include <StateChange/BattleResult.h>
#include <StateChange/MCUComponentStatus.h>
#include <StateChange/MCUStatus.h>
#include <StateChange/PlayerPreBattleState.h>
#include <StateChange/PlayerResult.h>
#include <States/BattleResultInterpreter.h>
#include <States/GameState.h>
#include <States/InBaseState.h>
#include <States/PlayerBattleState.h>
#include <Templates/AIGroupTemplate.h>
#include <Templates/BattlefieldTemplate.h>
#include <Templates/FactionHubTemplate.h>
#include <Templates/FactionTemplate.h>
#include <Templates/HubTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/RandomMissionTypeTemplate.h>
#include <Templates/StoryMissionTemplate.h>
#include <Templates/ShieldTemplate.h>
#include <Templates/SoundTemplate.h>
#include <Templates/TargetingSystemTemplate.h>
#include <Templates/TerrainTemplate.h>
#include <Templates/WeaponHardpointTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Terrain/TerrainManager.h>


#include <BtOgre/BtOgreExtras.h>
#include <BtOgre/BtOgreGP.h>
#include "btBulletDynamicsCommon.h"
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <OgreAnimationState.h>
#include <OgreAnimation.h>
#include <algorithm>
#include <hash_map>
#include <list>
#include <deque>
#include <OgreAL.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>

#include <Rocket/Core.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/Element.h>

using namespace Ogre;

InBattleState* InBattleState::mInBattleState;

InBattleState::InBattleState() :
	GameState(),
	mMCUCount(0),
	mScrollRight(false),
	mScrollLeft(false),
	mScrollUp(false),
	mScrollDown(false),

	mNumPlayers(0),
	mTurnTimer(0),
	mUpdateHightlight(0),

	mProjectileNeedsRemoving(false),
	mBattleEnded(false),

	mCurrentSelectedMCU(0),
	mPathFindingManager(0),
	mRaySceneQuery(0),
	mRedPath(0),
	mGreenPath(0),
	mCoverageDebugger(0),
	mNextProjectileId(0),
	mPlayerToRender(0),
	mMission(0),
	mInGameCamera(InGameCamera::getSingletonPtr()),
	mBattleResult(0),
	mBattleResultInterpreter(0),
	mPostBattleState(0),
	mBattlefieldGrid(0),
	mGreenPathNode(0),
	mRedPathNode(0),
	mSelectMCUSound(0)

{
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::PLAYER);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::WEAPON);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCU);
	DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->setSubDataSourceType(DataSourceType::WEAPON);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::ENEMY_MCU);
	
}

InBattleState::~InBattleState()
{
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::PLAYER);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::WEAPON);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCU);
	DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->setSubDataSourceType(DataSourceType::WEAPON);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::ENEMY_MCU);
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();
	if(mSelectMCUSound)
	{
		soundManager->destroySound(mSelectMCUSound);
	}
	std::deque<MovableEntity*>::iterator mov;
	for(mov = mMovableEntitiesToAdd.begin(); mov != mMovableEntitiesToAdd.end(); ++mov)
	{
		mMovableEntities.push_back((*mov));
	}

	mMovableEntitiesToAdd.clear();

	for(mov = mMovableEntities.begin(); mov != mMovableEntities.end();++mov)
	{
		delete (*mov);
	}
	mMovableEntities.clear();
	std::deque<PlayerBattleState*>::iterator iter;

	for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		delete (*iter);
	}
	mPlayers.clear();

	mSceneManager->destroyAllManualObjects();
	mSceneManager->destroyAllEntities();
	delete mMission;


	if(mPathFindingManager)
	{
		delete mPathFindingManager;
		mPathFindingManager = 0;
	}

	if(mBattlefieldGrid)
	{
		delete mBattlefieldGrid;
	}

	if(mBattleResult)
	{
		delete mBattleResult;
		mBattleResult = 0;
	}
	mIsEnabled = false;
	mMCUControllers.clear();
	mMCUsToRemove.clear();
	mDestroyedMCUs.clear();
	mPlayerToRender = 0;
}

void InBattleState::enter() 
{
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();

	SoundTemplate* selectSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName("grid_select");
	if(selectSoundTemplate && !mSelectMCUSound)
	{
		mSelectMCUSound = soundManager->createSound(selectSoundTemplate->getName(), selectSoundTemplate->getFileName(), selectSoundTemplate->loop());
	}

	mBattleEnded = false;
	mScrollRight = false;
	mScrollLeft = false;
	mScrollUp = false;
	mScrollDown = false;

	mTurnTimer = 0;
	mUpdateHightlight = 0;

	mTurnLimit = 100000;
	
	mCamera = mInGameCamera->getCamera();
	mCamNode = mInGameCamera->getCamNode();

	//mCamera = this->getCamera("MainCamera");
	
	
	mRaySceneQuery = mSceneManager->createRayQuery(Ogre::Ray());
	mRaySceneQuery->setSortByDistance(true);

	//mRocketRenderer->unloadCurrentDocument();
		
	
	mRedPath = 0;
	mGreenPath = 0;
	mRedPathNode = 0;
	mGreenPathNode = 0;

	mUnitSelectBillboard = 0;
	mTargetSelectBillboard = 0;
	mNextProjectileId = 0;

	//setupFog();
	//Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("InGame");
	loadBattlefield();
	
	mRocketRenderer->loadDocument(RocketDocuments::InBattle::InBattleHUD);
	placeMCUs();
	mPathFindingManager = new PathFindingManager(mBattlefieldGrid);

	mAIScriptName = "";
	mIsEnabled = true;
}

void InBattleState::exit() 
{
	mRocketRenderer->closeModalDocument();
	mRocketRenderer->unloadCurrentDocument();
	mIsEnabled = false;
	if(mGreenPath)
	{
		mGreenPath->detachFromParent();
		mSceneManager->destroyManualObject(mGreenPath);
		mGreenPath = 0;
	}
	if(mGreenPathNode)
	{
		mSceneManager->destroySceneNode(mGreenPathNode);
		mGreenPathNode = 0;
	}
	if(mRedPath)
	{
		mRedPath->detachFromParent();
		mSceneManager->destroyManualObject(mRedPath);
		mRedPath = 0;
	}
	if(mRedPathNode)
	{
		mSceneManager->destroySceneNode(mRedPathNode);
		mRedPathNode = 0;
	}
	if(mTargetSelectBillboard)
	{
		mTargetSelectBillboard->detachFromParent();
		mSceneManager->destroyBillboardSet(mTargetSelectBillboard);
		mTargetSelectBillboard = 0;
	}
	if(mUnitSelectBillboard)
	{
		mUnitSelectBillboard->detachFromParent();
		mSceneManager->destroyBillboardSet(mUnitSelectBillboard);
		mUnitSelectBillboard = 0;
	}
	
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::PLAYER);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::WEAPON);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::MCU);
	DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->setSubDataSourceType(DataSourceType::WEAPON);
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::ENEMY_MCU);

	std::deque<MovableEntity*>::iterator mov;
	for(mov = mMovableEntitiesToAdd.begin(); mov != mMovableEntitiesToAdd.end(); ++mov)
	{
		mMovableEntities.push_back((*mov));
	}

	mMovableEntitiesToAdd.clear();

	//the reason we have to do 2 run throughs is that projectiles notify their owners when they die - ie projectiles need to be destroyed first.
	for(mov = mMovableEntities.begin(); mov != mMovableEntities.end();)
	{
		if((*mov)->isProjectile())
		{
			delete (*mov);
			mov = mMovableEntities.erase(mov);
		}
		else
		{
			++mov;
		}
	}
	for(mov = mMovableEntities.begin(); mov != mMovableEntities.end();++mov)
	{
		delete (*mov);
	}
	mMovableEntities.clear();
	std::deque<PlayerBattleState*>::iterator iter;

	for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		delete (*iter);
	}

	mPlayers.clear();
	mMission->resetAIGroupMCUs();
	mGameManager->getFogSceneManager()->clearScene();
	mSceneManager->clearScene();
	mSceneManager->destroyAllAnimations();
	mSceneManager->destroyAllAnimationStates();
	mSceneManager->destroyAllMovableObjects();
	mSceneManager->destroyAllStaticGeometry();
	mSceneManager->destroyAllBillboardChains();
	mSceneManager->destroyAllBillboardSets();
	mBattlefieldGrid->setBattlefield(0);
	PhysicsManager::getSingletonPtr()->clearCollisionMappings();
	mMCUControllers.clear();
	mMCUsToRemove.clear();
	mDestroyedMCUs.clear();
	mExplosionSets.clear();
	mPlayerToRender = 0;
	mCurrentSelectedMCU = 0;

}


void InBattleState::endBattle()
{
	if(mPlayers.size() == 1)
	{
		//end the mission in victory for that player
		mBattleResult->getPlayerResult(mPlayers.front()->getPlayerProfile())->setPlayerOutcome(PlayerOutcome::PO_VICTORY);
	}

	if(mPlayers.size() > 1)
	{
		std::deque<PlayerBattleState*>::iterator iter;

		int team(mPlayers.front()->getTeam());

		bool differentTeams(false);

		for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
		{
			if((*iter)->getTeam() != team)
			{
				differentTeams = true;
				break;
			}
		}

		if(!differentTeams)
		{
			for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
			{
				mBattleResult->getPlayerResult((*iter)->getPlayerProfile())->setPlayerOutcome(PlayerOutcome::PO_VICTORY);
			}
		}
		//we have a quitter - do nothing. Result will still be PO_PLAYING
	}
	mBattleResultInterpreter->interpretResult(mBattleResult);
	mGameManager->changeState(mPostBattleState);
}

void InBattleState::restart()
{
	mRocketRenderer->closeModalDocument();
	mRocketRenderer->unloadCurrentDocument();
	exit();
	loadMission(mMission);
	enter();
}

void InBattleState::cleanUp()
{
	delete mMission;
	mMission = 0;
	if(mPathFindingManager)
	{
		delete mPathFindingManager;
		mPathFindingManager = 0;
	}

	if(mBattlefieldGrid)
	{
		delete mBattlefieldGrid;
		mBattlefieldGrid = 0;
	}

	if(mBattleResult)
	{
		delete mBattleResult;
		mBattleResult = 0;
	}
}

void InBattleState::loadBattlefield()
{
	
	float half = BattlefieldGrid::UnitSize / 2.f;
	Vector3 loc = Vector3(-half, BattlefieldGrid::UnitSize, 0);

	
	mUnitSelectBillboard = GameManager::getSingletonPtr()->getSceneManager()->createBillboardSet("unitSelectBillboard");

	mUnitSelectBillboard->setDefaultDimensions(BattlefieldGrid::UnitSize , BattlefieldGrid::UnitSize );
	mUnitSelectBillboard->setMaterialName("unitselect");
	mUnitSelectBillboard->setTextureStacksAndSlices(1, 1);
	Billboard* billboard = mUnitSelectBillboard->createBillboard(0, BattlefieldGrid::UnitSize / 2.f, 0);
	
	mTargetSelectBillboard = GameManager::getSingletonPtr()->getSceneManager()->createBillboardSet("targetSelectBillboard");
	mTargetSelectBillboard->setDefaultDimensions(BattlefieldGrid::UnitSize , BattlefieldGrid::UnitSize );
	mTargetSelectBillboard->setMaterialName("targetselect");
	mTargetSelectBillboard->setTextureStacksAndSlices(1, 1);
	billboard = mTargetSelectBillboard->createBillboard(0, BattlefieldGrid::UnitSize / 2.f, 0);
	TerrainTemplate* terrain = mBattlefieldGrid->getTerrainTemplate();
	mSceneManager->setAmbientLight(Ogre::ColourValue(terrain->getAmbientLight().x, terrain->getAmbientLight().y, terrain->getAmbientLight().z, 1.0f));

	mDirectionalLight = mSceneManager->createLight("TheSun");
	mDirectionalLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	mDirectionalLight->setDirection(terrain->getDirectionalLightDirection().x, terrain->getDirectionalLightDirection().y, terrain->getDirectionalLightDirection().z);
	mDirectionalLight->setDiffuseColour(terrain->getDirectionalLight().x, terrain->getDirectionalLight().y, terrain->getDirectionalLight().z);
	mDirectionalLight->setCastShadows(true);
	//mDirectionalLight->setShadowFarClipDistance(220);
	//mDirectionalLight->setShadowFarDistance(220);

	mSceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
	mSceneManager->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_RGB);
	mSceneManager->setShadowTextureSize(1024);
	mSceneManager->setShadowFarDistance(500);
	ShadowCameraSetupPtr shadowCamera = ShadowCameraSetupPtr(new FocusedShadowCameraSetup());
	mSceneManager->setShadowCameraSetup(shadowCamera);

	mSceneManager->setShadowTextureCasterMaterial("shadow_caster");
	
	mSceneManager->setShadowCasterRenderBackFaces(false);
	mSceneManager->setShadowTextureSelfShadow(true);
	
	mSceneManager->setShadowDirLightTextureOffset(0.55f);
	//mSceneManager->setShadowTextureCount(8);
	//return;
	
	const unsigned numShadowRTTs = mSceneManager->getShadowTextureCount();
	for (unsigned i = 0; i < numShadowRTTs; ++i) 
	{
		Ogre::TexturePtr tex = mSceneManager->getShadowTexture(i);
		Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
		vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1, 1));
		vp->setClearEveryFrame(true);
	}
	
	BtOgre::StaticMeshToShapeConverter converter;
	mBattlefieldGrid->reset();
	ManualObject* grid = mBattlefieldGrid->getBattlefield();
	StaticGeometry* sGeom = mSceneManager->createStaticGeometry("Battlefield");
	SceneNode* node = mSceneManager->createSceneNode("BattlefieldNode");

	if(MeshManager::getSingleton().resourceExists(grid->getName() + "Mesh"))
	{
		MeshManager::getSingleton().remove(grid->getName() + "Mesh");
	}

	Ogre::MeshPtr mesh = grid->convertToMesh(grid->getName() + "Mesh");
	
	//mesh->buildTangentVectors(Ogre::VES_NORMAL);
	Ogre::Entity *entity = mSceneManager->createEntity(mesh);
	entity->setCastShadows(false);
	converter.addMesh(mesh);
	mSceneManager->destroyManualObject(grid);
	node->attachObject(entity);

	
	sGeom->addSceneNode(node);
	sGeom->setCastShadows(false);
	sGeom->build();
	StaticGeometry::RegionIterator it = sGeom->getRegionIterator();
	while (it.hasMoreElements())
	{
		StaticGeometry::Region* region = it.getNext();
	} 

	PhysicsManager::getSingletonPtr()->initializeTerrain(mBattlefieldGrid, converter.createTrimesh(), sGeom);

}

void InBattleState::setupFog()
{
	Real texWidth = (mBattlefieldGrid->getSideLength() + 1) * BattlefieldGrid::UnitSize;
	Ogre::TexturePtr rtt_texture = Ogre::TextureManager::getSingleton().createManual("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, texWidth, texWidth, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
	
	Ogre::RenderTexture *renderTexture = rtt_texture->getBuffer()->getRenderTarget();
	Ogre::SceneManager* fogSM = GameManager::getSingletonPtr()->getFogSceneManager();
	Ogre::SceneNode* cameraNode = fogSM->createSceneNode("FogCameraNode");
	Ogre::Camera* fogCamera = GameManager::getSingletonPtr()->getFogCamera();
	fogCamera->setOrthoWindow(texWidth, texWidth);
	fogCamera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
	cameraNode->attachObject(fogCamera);

	renderTexture->addViewport(fogCamera);
	renderTexture->getViewport(0)->setClearEveryFrame(true);
	renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
	renderTexture->getViewport(0)->setOverlaysEnabled(false);
	
	renderTexture->setAutoUpdated(true);
	cameraNode->translate(texWidth / 2., texWidth, texWidth / 2., Ogre::Node::TS_WORLD);
	Quaternion r(Degree(-90), Vector3::UNIT_X);
	cameraNode->rotate(r);
	
}

void InBattleState::pause() 
{

}

void InBattleState::resume() 
{

}

void InBattleState::beginSetup(int sideLength)
{
	Real texWidth = (sideLength + 1) * BattlefieldGrid::UnitSize;
	Ogre::TexturePtr rtt_texture = Ogre::TextureManager::getSingleton().createManual("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, texWidth, texWidth, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
	
	Ogre::RenderTexture *renderTexture = rtt_texture->getBuffer()->getRenderTarget();
	Ogre::SceneManager* fogSM = GameManager::getSingletonPtr()->getFogSceneManager();
	Ogre::SceneNode* cameraNode = fogSM->createSceneNode("FogCameraNode");
	Ogre::Camera* fogCamera = GameManager::getSingletonPtr()->getFogCamera();
	fogCamera->setOrthoWindow(texWidth, texWidth);
	fogCamera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
	cameraNode->attachObject(fogCamera);

	renderTexture->addViewport(fogCamera);
	renderTexture->getViewport(0)->setClearEveryFrame(true);
	renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
	renderTexture->getViewport(0)->setOverlaysEnabled(false);
	
	renderTexture->setAutoUpdated(true);
	cameraNode->translate(texWidth / 2., texWidth, texWidth / 2., Ogre::Node::TS_WORLD);
	Quaternion r(Degree(-90), Vector3::UNIT_X);
	cameraNode->rotate(r);
	
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("InGame");
	//mBattleResult = new BattleResult(basePay, bounty);
}

void InBattleState::update(Real timeSinceLastFrame)
{
	if(!mIsEnabled)
		return;
	std::deque<OgreAL::Sound*>::iterator sound;
	for(sound = mSounds.begin(); sound != mSounds.end(); ++sound)
	{
		if(!(*sound)->isPlaying())
		{
			delete (*sound);
			mSounds.erase(sound);
			break;
		}
	}
	std::deque<MovableEntity*>::iterator mov;
	for(mov = mMovableEntitiesToAdd.begin(); mov != mMovableEntitiesToAdd.end(); ++mov)
	{
		mMovableEntities.push_back((*mov));
	}

	mMovableEntitiesToAdd.clear();

	std::deque<Projectile*>::iterator projectile;
	for(mov = mMovableEntities.begin(); mov != mMovableEntities.end();)
	{
		if((*mov)->readyToBeRemoved())
		{
			mov = mMovableEntities.erase(mov);
		}
		else if((*mov)->readyToBeDeleted())
		{
			if((*mov) == mCurrentSelectedMCU)
			{
				mCurrentSelectedMCU = 0;
			}
			delete (*mov);
			mov = mMovableEntities.erase(mov);
		}
		else
		{
			(*mov)->update(timeSinceLastFrame);
			++mov;
		}
	}

	if(mBattleEnded)
	{
		mBattleEnded = false;
		mIsEnabled = false;
		endBattle();
		return;
	}
	/*
	if(mProjectileNeedsRemoving)
	{
		mProjectileNeedsRemoving = false;
		std::deque<Projectile*>::iterator projectile;
		std::deque<Projectile*>::iterator eraser;

		for(eraser = mProjectilesToRemove.begin(); eraser != mProjectilesToRemove.end(); )
		{
			for(projectile = mProjectiles.begin(); projectile != mProjectiles.end(); ++projectile)
			{
				if((*projectile) == (*eraser))
				{
					mProjectiles.erase(projectile);
					(*eraser)->getSource()->getPlayerBattleState()->removeProjectile((*eraser));
					delete (*eraser);
					eraser = mProjectilesToRemove.erase(eraser);
					break;
				}
			}
		}
	}

	std::deque<MCUController*>::iterator mcuIter;
	std::deque<MCUController*>::iterator mcuToRemove;

	for(mcuToRemove = mMCUsToRemove.begin(); mcuToRemove != mMCUsToRemove.end();)
	{
		for(mcuIter = mMCUControllers.begin(); mcuIter != mMCUControllers.end(); ++mcuIter)
		{
			if((*mcuIter) == (*mcuToRemove))
			{
				mMCUControllers.erase(mcuIter);
				mcuToRemove = mMCUsToRemove.erase(mcuToRemove);
				break;
			}
		}
	}

	std::deque<Shield*>::iterator shield;
	std::deque<Shield*>::iterator shieldToRemove;

	for(shieldToRemove = mShieldsToRemove.begin(); shieldToRemove != mShieldsToRemove.end(); )
	{
		for(shield = mShields.begin(); shield != mShields.end(); ++shield)
		{
			if((*shield) == (*shieldToRemove))
			{
				mShields.erase(shield);
				shieldToRemove = mShieldsToRemove.erase(shieldToRemove);
				break;
			}
		}
	}

	
	//std::deque<ParticleSystem*>::iterator particle;
	//for(particle = mParticleSystems.begin(); particle != mParticleSystems.end(); )
	//{
		//if(!(*particle)->getEmitter(0)->getEnabled())
		//{
		//	SceneNode* pNode = (*particle)->getParentSceneNode();
		//	mSceneManager->destroyParticleSystem((*particle));
		//	mSceneManager->destroySceneNode(pNode);
		//	particle = mParticleSystems.erase(particle);
		//}
		//else
		//{
		//	++particle;
		//}
	//}
	

	std::deque<ExplosionSet*>::iterator explosion;
	for(explosion = mExplosionSets.begin(); explosion != mExplosionSets.end(); ++explosion)
	{
		(*explosion)->update(timeSinceLastFrame);
	}

	Ogre::Billboard* b(0);
	
	for(shield = mShields.begin(); shield != mShields.end(); ++shield)
	{
		(*shield)->update(timeSinceLastFrame);
	}

	std::deque<MCUController*>::iterator iter;
	for(iter = mMCUControllers.begin();iter != mMCUControllers.end();++iter)
	{
		(*iter)->update(timeSinceLastFrame);
		//(*iter)->updateFogOfWar();
	}
	*/
	/*
	(*mVisibilityUpdateIterator)->updateVisibilityState();
	++mVisibilityUpdateIterator;

	if(mVisibilityUpdateIterator == mPlayers.end())
		mVisibilityUpdateIterator = mPlayers.begin();
	*/
	
	mPlayerToRender->updateProjectileVisibility();
	
	mTurnTimer += timeSinceLastFrame;
	if(mTurnTimer > mTurnLimit)
		endTurn();

}

void InBattleState::updateMCUVisibility()
{
	std::deque<PlayerBattleState*>::iterator iter;
	for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		(*iter)->updateMCUVisibility();
	}
}

void InBattleState::createScene()
{

}
void InBattleState::destroyScene()
{

}

void InBattleState::startTurn()
{
	mTurnTimer = 0;
	for(int i = 0;i< mNumPlayers;++i)
	{
		if(mPlayers[i]->isMyTurn())
		{
			mPlayers[i]->startTurn();
		}
	}
}

void InBattleState::endTurn()
{
	mGameManager->luaDebugReport("End turn", 0);
	std::deque<PlayerBattleState*>::iterator iter;

	for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		(*iter)->endTurn();
	}
	
	++mCurrentPlayerTurn;
	if(mCurrentPlayerTurn == mPlayers.end())
		mCurrentPlayerTurn = mPlayers.begin();

	if(isLocalPlayersTurn())
	{
		mRocketRenderer->loadDocument(RocketDocuments::InBattle::InBattleHUD);
		selectMCU(mCurrentSelectedMCU);
		if(mCurrentSelectedMCU->hasTarget())
			selectTarget(mCurrentSelectedMCU->getCurrentTarget());
	}
	else
	{
		hidePath();
		mTargetSelectBillboard->detachFromParent();
		mRocketRenderer->loadDocument(RocketDocuments::InBattle::BetweenTurnsHUD);
	}
	(*mCurrentPlayerTurn)->startTurn();

}

void InBattleState::addPlayer(PlayerPreBattleState* state)
{
	mMission->getPlayerPreBattleStates().push_back(state);
}

void InBattleState::placeMCUs()
{


	bool spotlights = false;
	std::deque<PlayerPreBattleState*>::iterator playerIterator;
	
	bool setCameraPosition = false;

	for(playerIterator = mMission->getPlayerPreBattleStates().begin(); playerIterator != mMission->getPlayerPreBattleStates().end();++playerIterator)
	{
		++mNumPlayers;
		PlayerBattleState* player = new PlayerBattleState((*playerIterator)->getPlayerProfile(), (*playerIterator)->getFactionTemplate(), (*playerIterator)->getTeam(), mNumPlayers);
		player->setNeedsRender((*playerIterator)->needsRender());
		PlayerResult* pResult = new PlayerResult((*playerIterator)->getPlayerProfile(), (*playerIterator)->getTeam());
		mBattleResult->addPlayerResult(pResult);

		if((*playerIterator)->isAI())
		{
			player->setAI(true);
			player->loadScript((*playerIterator)->getAIScriptName(), (*playerIterator)->getAIScriptEntryPoint());
			player->setAIGroups((*playerIterator)->getAIGroups());
		}

		std::deque<MCUPlacement*>::iterator mcuIterator;
		
		if((*playerIterator)->needsRender())
		{
			mPlayerToRender = player;
			mPlayerToRender->setNeedsRender((*playerIterator)->needsRender());
		}

		for(mcuIterator = (*playerIterator)->getMCUPlacements().begin(); mcuIterator != (*playerIterator)->getMCUPlacements().end(); ++mcuIterator)
		{

			BattlefieldGridCell* cell = mBattlefieldGrid->getGridCell((*mcuIterator)->X, (*mcuIterator)->Z);
			
			if(mPlayerToRender->getPlayerProfile() == (*playerIterator)->getPlayerProfile() && !setCameraPosition)
			{
				Vector3 location = cell->getMiddle();
				//location.z-= BattlefieldGrid::UnitSize * 15;
				//location.y+= BattlefieldGrid::UnitSize * 15;
				Quaternion q(Degree(-135), Vector3::UNIT_Y);
				Quaternion r(Degree(-60), Vector3::UNIT_X);
				
				q = q * r;
				
				//mCamNode->rotate(q);
				//mCamNode->setPosition(location);
				
				mInGameCamera->initialize("MainCamera", q, location, Vector3(mBattlefieldGrid->getSideLength() * BattlefieldGrid::UnitSize * 0.5, 0, mBattlefieldGrid->getSideLength() * BattlefieldGrid::UnitSize * 0.5) );
				mCamera = mInGameCamera->getCamera();
				setCameraPosition = true;
			}
					
			++mMCUCount;
			
			bool deletedMesh(false);
			/*
			if(MeshManager::getSingleton().resourceExists((*mcuIterator)->MCU->getMCUTemplate()->getMeshName()))
			{
				MeshManager::getSingleton().remove((*mcuIterator)->MCU->getMCUTemplate()->getMeshName());
				deletedMesh = true;
			}
			*/
			Ogre::Entity* entity = mSceneManager->createEntity("MCU" + SQLiteDBManager::convertIntToString(mMCUCount), (*mcuIterator)->MCU->getMCUTemplate()->getMeshName());	
			entity->setMaterialName((*mcuIterator)->MCU->getMCUTemplate()->getMaterialName());
			entity->setCastShadows(true);
			if(spotlights)
			{
				Ogre::Light* spotLight = mSceneManager->createLight("spotLight" + SQLiteDBManager::convertIntToString(mMCUCount));
				spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
				spotLight->setDiffuseColour(0.35f, 0.35f, 0.35f);
				spotLight->setDirection(-1, -1, 0);
				spotLight->setCastShadows(true);
				spotLight->setPosition(cell->getMiddle()+ Vector3(40, 20, 0));
				spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
				spotLight->setShadowFarClipDistance(2000);
				spotLight->setShadowFarDistance(2000);
			}
			/*
			Skeleton::BoneIterator iter = entity->getSkeleton()->getBoneIterator();
			std::stringstream filename;
			filename << "Bones" << mMCUCount << ".txt";
			std::ofstream outputFile;

			outputFile.open(filename.str(),std::ios_base::out);
					
			outputFile << (*mcuIterator)->MCU->getMCUTemplate()->getName() << std::endl;

			while(iter.hasMoreElements())
			{
				Bone* bone = iter.getNext();
				outputFile << bone->getName() << std::endl;
			}

			outputFile.close();
			*/
			Vector3 start = cell->getMiddleFloor();
			Ogre::SceneNode* mNode = mSceneManager->getRootSceneNode()->createChildSceneNode("MCUNode"  + SQLiteDBManager::convertIntToString(mMCUCount), start);
			mNode->attachObject(entity);
					
			MCUController* mcuC = new MCUController(mNode, entity,(*mcuIterator)->MCU,player, mBattlefieldGrid->getGridCell(start.x, start.z), mBattlefieldGrid, mMCUCount);


			if(mPlayerToRender->getPlayerProfile() == (*playerIterator)->getPlayerProfile())
			{
				if(!mCurrentSelectedMCU)
				{
					mCurrentSelectedMCU = mcuC;
				}
				DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->getPrimaryDataSet()->addRecord(mcuC->getBattlefieldID(),mcuC->getGuiValues());
				DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->addSubDataSet(mcuC->getBattlefieldID(), mcuC->getMechanizedCombatUnit()->getWeaponGuiData());
			}
			else
			{
				DataProvider::getSingletonPtr()->getDataSource(DataSourceType::ENEMY_MCU)->getPrimaryDataSet()->addRecord(mcuC->getBattlefieldID(),mcuC->getGuiValues());
			}

			mMCUControllers.push_back(mcuC);
			mMovableEntities.push_back(mcuC);

			player->addMCU(mcuC);
			if((*mcuIterator)->OwnerAIGroup)
				(*mcuIterator)->OwnerAIGroup->addMCUController(mcuC);

			cell->occupyCell(mcuC);
			if(mPlayerToRender->getPlayerProfile() == (*playerIterator)->getPlayerProfile() && !mCurrentSelectedMCU)
			{
				mCurrentSelectedMCU = mcuC;
				mCurrentSelectedMCU->updateGuiData("mission start");
				DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCU,"mcudp",mCurrentSelectedMCU->getBattlefieldID(), false);
				DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCU,"mcustatus",mCurrentSelectedMCU->getBattlefieldID(), false);
			}
		}

		player->setLocal((*playerIterator)->isLocal());

		mPlayers.push_back(player);
	}

	mVisibilityUpdateIterator = mPlayers.begin();
	mCurrentPlayerTurn = mPlayers.begin();
	updateMCUVisibility();
	selectMCU(mCurrentSelectedMCU);
}

void InBattleState::removePlayer(PlayerBattleState* player)
{
	std::deque<PlayerBattleState*>::iterator iter;

	for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		if((*iter) == player)
		{
			delete (*iter);
			mPlayers.erase(iter);
			break;
		}
	}
	bool finished(true);
	if(mPlayers.size() > 1)
	{
		int team = mPlayers.front()->getTeam();

		
		for(iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
		{
			if((*iter)->getTeam() != team)
			{
				finished = false;
			}
		}
	}
	if(finished)
	{
		mBattleEnded = true;

		//endBattle();
	}


}

std::deque<PlayerBattleState*> InBattleState::getAllPlayers()
{
	return mPlayers;
}

Real InBattleState::getTurnLimit()
{
	return mTurnLimit;
}

void InBattleState::setTurnLimit(Real limit)
{
	mTurnLimit = limit;
}

std::string InBattleState::getAIScriptName()
{
	return mAIScriptName;
}

void InBattleState::setAIScriptName(const std::string& name)
{
	mAIScriptName = name;
}

BattlefieldGrid* InBattleState::getBattlefieldGrid()
{
	return mBattlefieldGrid;
}

void InBattleState::setBattlefieldGrid(BattlefieldGrid* grid)
{
	mBattlefieldGrid = grid;
}

PathFindingManager* InBattleState::getPathFindingManager()
{
	return mPathFindingManager;
}

void InBattleState::keyPressed(const OIS::KeyEvent& arg)
{
	if(arg.key == OIS::KC_F5)
	{
		mRocketRenderer->reloadCurrentDocument();
	}
}

void InBattleState::keyReleased(const OIS::KeyEvent& arg)
{

}

void InBattleState::mouseMoved(const OIS::MouseEvent& arg)
{

}

void InBattleState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(!mIsEnabled)
		return;

	if(arg.state.buttonDown(OIS::MouseButtonID::MB_Left) && (*mCurrentPlayerTurn) == mPlayerToRender)
	{
		Ray mouseRay = mCamera->getCameraToViewportRay(arg.state.X.abs/float(arg.state.width), arg.state.Y.abs/float(arg.state.height));
	
		ClosestRayResultCallback rayCallback = PhysicsManager::getSingletonPtr()->closestRayTestCallback(BtOgre::Convert::toBullet(mouseRay.getOrigin()), BtOgre::Convert::toBullet(mouseRay.getPoint(100000)));
		
		if(rayCallback.hasHit())
		{
			BattlefieldGridCell* cell = mBattlefieldGrid->getGridCell(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.z());
			mGameManager->luaDebugReport("Type is " + cell->getTypeString() + ", pos is x: " + SQLiteDBManager::convertIntToString(cell->getX()) + " z: " + SQLiteDBManager::convertIntToString(cell->getZ()),0);
			CollisionObjectMapping* mapping = rayCallback.getCollisionObjectMapping();

			if(mapping->MCUController && mapping->MCUController->isVisible())
			{
				if(mCurrentSelectedMCU)
				{
					if( mapping->MCUController->getPlayerBattleState()->getTeam() != mCurrentSelectedMCU->getPlayerBattleState()->getTeam())
					{
						selectTarget(mapping->MCUController);
					}
					else if(mapping->MCUController->getPlayerBattleState() == mPlayerToRender)
					{
						selectMCU(mapping->MCUController);
					}

					mGameManager->updateCoords(rayCallback.m_hitPointWorld.x(),rayCallback.m_hitPointWorld.y(),rayCallback.m_hitPointWorld.z(),mapping->MCUController->getMechanizedCombatUnit()->getName());
				}
				else
				{
					if(mapping->MCUController->getPlayerBattleState() == mPlayerToRender)
					{
						selectMCU(mapping->MCUController);
					}

					mGameManager->updateCoords(rayCallback.m_hitPointWorld.x(),rayCallback.m_hitPointWorld.y(),rayCallback.m_hitPointWorld.z(),mapping->Name);
				}
			}
			else
			{
				//build movement path.
				if(mCurrentSelectedMCU)
				{
					
					if(cell && (*mCurrentPlayerTurn) == mPlayerToRender)
					{
						if(cell == mCurrentSelectedMCU->getCurrentGoal())
						{
							mCurrentSelectedMCU->moveAlongPath();
						}
						else
						{
							mCurrentSelectedMCU->setCurrentPath(mPathFindingManager->findPath(mCurrentSelectedMCU->getCurrentLocation(), cell, mCurrentSelectedMCU->getMechanizedCombatUnit()->getDrive()));
							renderPath(mCurrentSelectedMCU, mCurrentSelectedMCU->getCurrentPath());
						}
					}
				}
			}
		}
	}
}

void InBattleState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{

}

void InBattleState::ProcessEvent(Rocket::Core::Event& e)
{
	if(e.GetType() == "click" && e.GetTargetElement()->GetId() == "endturn")
	{
		if(mPlayerToRender == (*mCurrentPlayerTurn))
		{
			this->endTurn();
		}
	}
	if(e.GetType() == "click" && e.GetTargetElement()->GetId() == "loadinbattlemenu")
	{
		RocketRenderer::getSingletonPtr()->loadModalDocument(RocketDocuments::InBattle::InBattleMenu);
	}
}

PlayerBattleState* InBattleState::getPlayerToRender()
{
	return mPlayerToRender;
}

void InBattleState::setPlayerToRender(PlayerBattleState* player)
{
	mPlayerToRender = player;
}

MCUController* InBattleState::getMCUController(Ogre::MovableObject* mov)
{
	CollisionObjectMapping* mapping = PhysicsManager::getSingletonPtr()->findCollisionObjectMapping(mov);

	if(mapping)
	{
		MCUController* result = mapping->MCUController;
		//delete mapping;
		//mapping = 0;
		return result;
	}
	return 0;
}

void InBattleState::renderPath(MCUController* controller, std::deque<BattlefieldGridCell*>& path)
{
	if(controller->getPlayerBattleState() != mPlayerToRender)
		return;

	if(mGreenPath)
	{
		mGreenPath->detachFromParent();
		mSceneManager->destroyManualObject(mGreenPath);
		mGreenPath = 0;
	}
	if(mRedPath)
	{
		mRedPath->detachFromParent();
		mSceneManager->destroyManualObject(mRedPath);
		mRedPath = 0;
	}

	if(path.size() > 0)
	{
		std::deque<BattlefieldGridCell*>::iterator iter = path.begin();

		mGreenPath = mSceneManager->createManualObject("GreenPath");
		mGreenPath->begin("movement_allowed");
		float remaining = controller->getRemainingReactorPower();
		float current = 0;
		
		int numSquares = 0;
		Vector3 negXNegZ, negXPosZ, posXNegZ, posXPosZ;
		BattlefieldGridCell* previous = controller->getCurrentLocation();

		while(iter != path.end() && current <= remaining)
		{
			negXNegZ = (*iter)->getNegXNegZVertex();
			negXPosZ = (*iter)->getNegXPosZVertex();
			posXNegZ = (*iter)->getPosXNegZVertex();
			posXPosZ = (*iter)->getPosXPosZVertex();

			negXNegZ.y+=0.6f;
			negXPosZ.y+=0.6f;
			posXNegZ.y+=0.6f;
			posXPosZ.y+=0.6f;

			mGreenPath->position(negXNegZ);
			mGreenPath->textureCoord(0.f,0.f);
			mGreenPath->colour(0.f,1.f,0.f);
			mGreenPath->position(posXNegZ);
			mGreenPath->textureCoord(1.f,0.f);
			mGreenPath->colour(0.f,1.f,0.f);
			mGreenPath->position(negXPosZ);
			mGreenPath->textureCoord(0.f,1.f);
			mGreenPath->colour(0.f,1.f,0.f);
			mGreenPath->position(posXPosZ);
			mGreenPath->textureCoord(1.f,1.f);
			mGreenPath->colour(0.f,1.f,0.f);

			float thisSquare = controller->getMechanizedCombatUnit()->getMovementCostPerSquare() * (*iter)->getMovementCost() * mBattlefieldGrid->getTerrainTemplate()->getGravity();
			
			if(previous->getX() != (*iter)->getX() && previous->getZ() != (*iter)->getZ())
			{
				//diagonal movement
				thisSquare*=1.414f;
			}
			
			current+= thisSquare;
			++numSquares;
			previous = (*iter);
			++iter;
		}

		for(int i = 0; i < numSquares;++i)
		{
			int pos = i * 4;
			mGreenPath->triangle(pos, pos + 2, pos + 1);
			mGreenPath->triangle(pos + 2, pos + 3, pos + 1);
		}
		
		mGreenPath->end();
		if(!mGreenPathNode)
		{
			mGreenPathNode = mSceneManager->getRootSceneNode()->createChildSceneNode("GreenPathNode");
		}
		mGreenPathNode->attachObject(mGreenPath);
		
		if(iter != path.end() && current > remaining)
		{
			mRedPath = mSceneManager->createManualObject("RedPath");
			mRedPath->begin("movement_disallowed");

			numSquares = 0;
			while(iter != path.end())
			{
				negXNegZ = (*iter)->getNegXNegZVertex();
				negXPosZ = (*iter)->getNegXPosZVertex();
				posXNegZ = (*iter)->getPosXNegZVertex();
				posXPosZ = (*iter)->getPosXPosZVertex();

				negXNegZ.y+=0.6f;
				negXPosZ.y+=0.6f;
				posXNegZ.y+=0.6f;
				posXPosZ.y+=0.6f;

				mRedPath->position(negXNegZ);
				mRedPath->textureCoord(0.f,0.f);
				mRedPath->colour(0.f,1.f,0.f);
				mRedPath->position(posXNegZ);
				mRedPath->textureCoord(1.f,0.f);
				mRedPath->colour(0.f,1.f,0.f);
				mRedPath->position(negXPosZ);
				mRedPath->textureCoord(0.f,1.f);
				mRedPath->colour(0.f,1.f,0.f);
				mRedPath->position(posXPosZ);
				mRedPath->textureCoord(1.f,1.f);
				mRedPath->colour(0.f,1.f,0.f);

				++numSquares;
				++iter;
			}
			
			for(int i = 0; i < numSquares;++i)
			{
				int pos = i * 4;
				mRedPath->triangle(pos, pos + 2, pos + 1);
				mRedPath->triangle(pos + 2, pos + 3, pos + 1);
			}

			mRedPath->end();
			
			if(!mRedPathNode)
			{
				mRedPathNode = mSceneManager->getRootSceneNode()->createChildSceneNode("RedPathNode");
			}
			mGreenPathNode->attachObject(mRedPath);
		}
	}
}

void InBattleState::hidePath()
{
	if(mGreenPath)
	{
		mSceneManager->destroyManualObject(mGreenPath);
		mGreenPath = 0;
	}
	if(mRedPath)
	{
		mSceneManager->destroyManualObject(mRedPath);
		mRedPath = 0;
	}
}

void InBattleState::fireWeapon(MCUController* source, WeaponHardpoint* weapon, bool returningFire)
{
	GameManager::getSingletonPtr()->luaDebugReport("Firing weapon", 0);

	if(!source->canFire(weapon))
		return;
	if(!source->getCurrentTarget())
		return;
	if(source->isFiring())
		return;

	if(returningFire)
	{
		int temp = 0;
	}
	Vector3 vS = Vector3::ZERO;
	Vector3 vD = Vector3::ZERO;
	
	vS = source->getFiringSourcePosition(weapon);

	//vS = source->getNode()->convertLocalToWorldPosition(weapon->getWeaponHardpointTemplate()->getBarrelLocation());
	
	

	float accuracy = calculateAccuracy(source, weapon);
	//DEBUG ONLY
	//accuracy = 1.f;

	bool hit(false);
	int success = rand() % 1001;
	if(success >= ((1.f - accuracy) * 1000))
	{
		hit = true;
		vD = source->getCurrentTarget()->getMiddle();
	}
	else
	{
		vD = source->getCurrentTarget()->getMiddle();
		//how badly did we miss?
		float miss((1 - accuracy) - (success / 1000.f) * BattlefieldGrid::UnitSize * 2.5);

		int xDir(0);

		if(rand() == 0)
			xDir = -1;
		else
			xDir = 1;

		Vector3 direction = vD - vS;
		direction.normalise();
		Vector3 perpendicularDir1 = direction.crossProduct(Vector3(0,1,0));
		
		vD = vD + (perpendicularDir1 * miss * xDir);
		direction = vD - vS;
		direction.normalise();

		ClosestNotMeRayResultCallback callback = PhysicsManager::getSingletonPtr()->closestNotMeRayTestCallback(source->getKinematicCharacterController()->getGhostObject(),BtOgre::Convert::toBullet(vS), BtOgre::Convert::toBullet((vS + (direction * BattlefieldGrid::UnitSize * 100) ) ));
		
		if(callback.hasHit())
		{
			vD = BtOgre::Convert::toOgre( callback.m_hitPointWorld );
		}
	}
	
	source->beginFiringWeapon(weapon, vS, vD, returningFire);
}

MCUController* InBattleState::getMCUByBattlefieldID(int id)
{
	std::deque<MCUController*>::iterator iter;

	for(iter = mMCUControllers.begin(); iter != mMCUControllers.end(); ++iter)
	{
		if((*iter)->getBattlefieldID() == id)
			return (*iter);
	}
	return 0;
}

void InBattleState::onSelectedElementChanged(DataSourceType source, int id)
{
	if(source == DataSourceType::WEAPON)
	{

	}
}

void InBattleState::addProjectile(Projectile* proj)
{
	mMovableEntitiesToAdd.push_back(proj);
	proj->setProjectileID(mNextProjectileId);
	++mNextProjectileId;
	proj->projectileAdded();

	proj->getSource()->getPlayerBattleState()->addProjectile(proj);
}

void InBattleState::addShield(Shield* shield)
{
	mMovableEntitiesToAdd.push_back(shield);
}

void InBattleState::removeMCU(MCUController* controller)
{
	mMCUsToRemove.push_back(controller);
	mDestroyedMCUs.push_back(controller);
}

void InBattleState::addParticleSystem(const std::string& templateName, const Ogre::Vector3& location)
{
	ParticleSystem* system = mSceneManager->createParticleSystem("ParticleSystem" + SQLiteDBManager::convertIntToString(mParticleSystems.size()), templateName);
	system->fastForward(2.0);
	SceneNode* particleNode = mSceneManager->createSceneNode("ParticleSystemNode" + SQLiteDBManager::convertIntToString(mParticleSystems.size() ));
	particleNode->attachObject(system);
	particleNode->translate(location, Ogre::Node::TS_WORLD);
	mParticleSystems.push_back(system);
	mSceneManager->getRootSceneNode()->addChild(particleNode);
}

void InBattleState::addExplosion(const std::string& templateName, const Ogre::Vector3& location)
{
	std::deque<ExplosionSet*>::iterator explosion;
	ExplosionSet* set(0);

	for(explosion = mExplosionSets.begin(); explosion != mExplosionSets.end(); ++explosion)
	{
		if((*explosion)->getMaterialName() == templateName)
			set = (*explosion);
	}

	if(set)
	{
		set->createExplosion(location);
	}
	else
	{
		set = new ExplosionSet(templateName, 0.65);
		set->createExplosion(location);
		mExplosionSets.push_back(set);
		mMovableEntitiesToAdd.push_back(set);
	}
}

void InBattleState::addMovableEntity(MovableEntity* entity)
{

}

std::deque<MovableEntity*>& InBattleState::getMovableEntities()
{
	return mMovableEntities;
}

void InBattleState::addSound(OgreAL::Sound* sound)
{
	mSounds.push_back(sound);
}

void InBattleState::updateAccuracy(MCUController* mcu, WeaponHardpoint* weapon)
{
	if(mcu && weapon)
	{
		float accuracy = calculateAccuracy(mcu, weapon);
		DataProvider::getSingletonPtr()->getDataSource(DataSourceType::MCU)->getPrimaryDataSet()->updateData(mcu->getBattlefieldID(),FieldNames::MCU::ChanceToHitTarget,SQLiteDBManager::convertFloatToString(accuracy));
		DataProvider::getSingletonPtr()->updateDataSourceListeners(DataSourceType::MCU);
	}
}

float InBattleState::calculateAccuracy(MCUController* mcu, WeaponHardpoint* weapon)
{
	if(!weapon->canFire())
		return 0.f;

	float range = mcu->rangeToTarget();
	
	if(range > weapon->getWeapon()->getRange())
		return 0.f;

	Vector3 origin = mcu->getMiddle();
	
	float coverage = calculateCoverage(origin,mcu, mcu->getCurrentTarget());
	
	TargetingSystemTemplate* system = mcu->getMechanizedCombatUnit()->getTargetingSystem();

	float rangeTerm = (range / weapon->getWeapon()->getRange());
	//Ranged accuracy decrease represents the decrease in accuracy as range decreases. Min = 0.5 Max = 1.0. High values are better, meaning smaller values for rangeTerm are better.
	float rangedAccuracyDecrease = (((1 - rangeTerm) / 2) + 0.5f);

	if(coverage <= 0.15f)
	{
		if(weapon->getWeapon()->isIndirectFireCapable())
		{
			return std::min(1.f,system->getBaseAccuracy() * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
		}
		else
		{
			return 0.f;
		}
	}
	else if(coverage > 0.15f && coverage < 0.5f)
	{
		if(weapon->getWeapon()->isIndirectFireCapable())
		{
			return std::min (1.f,system->getBaseAccuracy() * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
		}
		else
		{
			return std::min(1.f, system->getBaseAccuracy() * coverage * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
		}
	}
	else
	{
		return std::min(1.f, system->getBaseAccuracy() * coverage * rangedAccuracyDecrease + rangeTerm * system->getLongRangeBonus() + (1 - coverage) * system->getObstructedBonus());
	}

}

float InBattleState::calculateCoverage(const Ogre::Vector3& origin,  MCUController* source, MCUController* target)
{
	if(!target || !source)
		return 0;

	Vector3 dest = target->getMiddle();

	int numCanSee(0), maxCanSee(0);
	
	/*
	if(mCoverageDebugger)
		mSceneManager->destroyManualObject(mCoverageDebugger);

	mCoverageDebugger = mSceneManager->createManualObject("coveragedebugger");
	mCoverageDebugger->begin("BaseWhiteNoMaterial",Ogre::RenderOperation::OT_LINE_LIST);
	*/

	Skeleton::BoneIterator iter = target->getEntity()->getSkeleton()->getBoneIterator();

	while(iter.hasMoreElements())
	{
		Bone* bone = iter.getNext();
		
		dest = target->getNode()->convertLocalToWorldPosition( bone->getPosition() );
		dest.y+= target->getEntity()->getBoundingBox().getSize().y / 2.f;
		//mCoverageDebugger->position(origin);
		//mCoverageDebugger->position(dest);
		numCanSee+= canSee(origin, dest,source,target) ? 1 : 0;
		++maxCanSee;

	}

	//mCoverageDebugger->end();
	//mSceneManager->getRootSceneNode()->attachObject(mCoverageDebugger);

	return numCanSee / (maxCanSee * 1.0f);
}


bool InBattleState::canSee(const Ogre::Vector3& origin, const Ogre::Vector3& dest, MCUController* source, MCUController* target)
{
	CollisionObjectMapping* mapping = PhysicsManager::getSingletonPtr()->closestNotMeRayTestCallback(source->getKinematicCharacterController()->getGhostObject(),BtOgre::Convert::toBullet(origin), BtOgre::Convert::toBullet(dest)).getCollisionObjectMapping();

	if(mapping)
	{
		if(mapping->MainMovableObject == target->getEntity() || mapping->ShieldMovableObject == target->getShield()->getEntity())
		{
			
			return true;
		}
		
		
		return false;
	}

	return false;
}


bool InBattleState::canSee(const Ogre::Vector3& origin, const Ogre::Vector3& dest, MCUController* target)
{
	VisibilityTestRayResultCallback callback = PhysicsManager::getSingletonPtr()->visibilityTestRayTestCallback(PhysicsManager::getSingletonPtr()->findCollisionObjectMapping(target->getEntity()), BtOgre::Convert::toBullet(origin), BtOgre::Convert::toBullet(dest));

	return callback.isVisible();
}

MCUController* InBattleState::getTarget(const Ogre::Vector3& origin, const Ogre::Vector3& dest, MCUController* source)
{
	CollisionObjectMapping* mapping = PhysicsManager::getSingletonPtr()->closestNotMeRayTestCallback(source->getKinematicCharacterController()->getGhostObject(),BtOgre::Convert::toBullet(origin), BtOgre::Convert::toBullet(dest)).getCollisionObjectMapping();
	if(mapping)
	{
		MCUController* mcu = mapping->MCUController;
		//delete mapping;
		return mcu;
	}
	return 0;
}

void InBattleState::rayIntersection(const Ogre::Vector3& origin, const Ogre::Vector3& direction, Ogre::RaySceneQueryResult& result)
{
	Ray ray(origin, direction);
	
	mRaySceneQuery->setRay(ray);

	result = mRaySceneQuery->execute();
}

void InBattleState::selectMCU(MCUController* mcu)
{
	hidePath();
	
	mCurrentSelectedMCU = mcu;
	if(mCurrentSelectedMCU->getPlayerBattleState() == (*mCurrentPlayerTurn))
		mCurrentSelectedMCU->setPlayerMCU(true);
	else
		mCurrentSelectedMCU->setPlayerMCU(false);

	mCurrentSelectedMCU->updateGuiData("select mcu");
	DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCU,"mcudp",mCurrentSelectedMCU->getBattlefieldID(), false);
	DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCU,"mcustatus",mCurrentSelectedMCU->getBattlefieldID(), false);
	DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::MCU,"mcuThumbs",mCurrentSelectedMCU->getBattlefieldID(), false);

	if(mCurrentSelectedMCU->getPlayerBattleState() == mPlayerToRender)
	{
		if(mCurrentSelectedMCU->hasPath())
			renderPath(mCurrentSelectedMCU,mCurrentSelectedMCU->getCurrentPath());
		else
			hidePath();

		if(mCurrentSelectedMCU->getCurrentTarget())
			selectTarget(mCurrentSelectedMCU->getCurrentTarget());
		else
		{
			if(mTargetSelectBillboard->isAttached())
			{
				mTargetSelectBillboard->detachFromParent();
			}
			mTargetSelectBillboard->setVisible(false);
		}
	}
	if(mCurrentSelectedMCU->getPlayerBattleState() == mPlayerToRender && mCurrentSelectedMCU->getCurrentTarget())
	{
		selectTarget(mCurrentSelectedMCU->getCurrentTarget());
	}
	if(mUnitSelectBillboard->isAttached())
	{
		mUnitSelectBillboard->detachFromParent();
	}
	mCurrentSelectedMCU->getNode()->attachObject(mUnitSelectBillboard);
	if(mSelectMCUSound)
	{
		mSelectMCUSound->play();
	}
}

void InBattleState::selectMCU(int id)
{
	MCUController* mcu = getMCUByBattlefieldID(id);

	if(mcu)
	{
		selectMCU(mcu);
	}
}

void InBattleState::selectTarget(MCUController* mcu)
{
	mCurrentSelectedMCU->setCurrentTarget(mcu);
	mCurrentSelectedMCU->updateGuiData("select mcutarget in battle state");
	mCurrentSelectedMCU->getCurrentTarget()->updateGuiData("select mcu target target in battle state");
	DataProvider::getSingletonPtr()->changeSelectedPrimaryElement(DataSourceType::ENEMY_MCU,"enemymcudp",mcu->getBattlefieldID(), false);
	if(mTargetSelectBillboard->isAttached())
	{
		mTargetSelectBillboard->detachFromParent();
	}
	mTargetSelectBillboard->setVisible(true);
	mCurrentSelectedMCU->getCurrentTarget()->getNode()->attachObject(mTargetSelectBillboard);
}

void InBattleState::nextMCU()
{
	MCUController* next = mPlayerToRender->getNextMCU(mCurrentSelectedMCU);
	if(next)
	{
		selectMCU(next);
	}
}

void InBattleState::prevMCU()
{
	MCUController* prev = mPlayerToRender->getPrevMCU(mCurrentSelectedMCU);
	if(prev)
	{
		selectMCU(prev);
	}
}

bool InBattleState::mcuHasTargetBillboard(MCUController* mcu)
{
	if(mCurrentSelectedMCU)
	{
		if(mCurrentSelectedMCU->hasTarget())
		{
			if(mCurrentSelectedMCU->getCurrentTarget() == mcu)
			{
				return true;
			}
		}
	}
	return false;
}

void InBattleState::showTargetSelectBillboard()
{
	if(mCurrentSelectedMCU)
	{
		if(mCurrentSelectedMCU->hasTarget())
		{
			if(mTargetSelectBillboard->isAttached())
			{
				mTargetSelectBillboard->detachFromParent();
			}
			mTargetSelectBillboard->setVisible(true);
			mCurrentSelectedMCU->getCurrentTarget()->getNode()->attachObject(mTargetSelectBillboard);
		}
	}
}

void InBattleState::hideTargetSelectBillboard()
{
	if(mTargetSelectBillboard->isAttached())
	{
		mTargetSelectBillboard->detachFromParent();
	}
	mTargetSelectBillboard->setVisible(false);
}

BattleResult* InBattleState::getBattleResult()
{
	return mBattleResult;
}

void InBattleState::setBattleResult(BattleResult* result)
{
	mBattleResult = result;
}

BattleResultInterpreter* InBattleState::getBattleResultInterpreter()
{
	return mBattleResultInterpreter;
}
	
void InBattleState::setBattleResultInterpreter(BattleResultInterpreter* interpreter)
{
	mBattleResultInterpreter = interpreter;
}

GameState* InBattleState::getPostBattleState()
{
	return mPostBattleState;
}

void InBattleState::setPostBattleState(GameState* state)
{
	mPostBattleState = state;
}

bool InBattleState::isLocalPlayersTurn()
{
	return (*mCurrentPlayerTurn) == mPlayerToRender;
}

bool InBattleState::isMyTurn(PlayerBattleState* player)
{
	return (*mCurrentPlayerTurn) == player;
}

void InBattleState::loadMission(Mission* mission)
{
	mMission = mission;
	
	mBattleResult = new BattleResult(mission->getFactionHubTemplate()->getHubTemplate()->getBaseMissionPay(), mission->getFactionHubTemplate()->getHubTemplate()->getBounty());
	
}

InBattleState* InBattleState::getSingletonPtr() 
{
	if( !mInBattleState ) 
	{
		mInBattleState = new InBattleState();
	}

	return mInBattleState;
}

