#include "GameState.h"
#include <Managers/GameManager.h>
#include <Input/InputListener.h>
#include <GUI/RocketRenderer.h>
#include <Managers/LuaStateManager.h>
#include <Repositories/SoundTemplateRepository.h>
#include <Templates/SoundTemplate.h>


using namespace Ogre;

GameState::GameState()
	:
	mLuaState(0)
{
	mIsEnabled = false;
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	mGameManager = GameManager::getSingletonPtr();
	mSceneManager =mGameManager->getSceneManager();
	mSceneManager->addRenderQueueListener(mRocketRenderer);
	mLuaState = LuaStateManager::getSingletonPtr()->getNewState();
	

}

GameState::~GameState()
{
	destroyScene();
}


void GameState::changeState( GameState *state ) 
{ 
	GameManager::getSingletonPtr()->changeState( state ); 
}

void GameState::pushState( GameState *state ) 
{ 
	GameManager::getSingletonPtr()->pushState( state ); 
}

void GameState::popState() 
{ 
	GameManager::getSingletonPtr()->popState(); 
}

void GameState::requestShutdown() 
{
	GameManager::getSingletonPtr()->requestShutdown();
}

void GameState::createScene()
{

}
void GameState::destroyScene()
{

}

bool GameState::isEnabled()
{
	return mIsEnabled;
}

void GameState::setEnabled(bool enabled)
{
	mIsEnabled = enabled;
}


Camera* GameState::getCamera(std::string name)
{
	mCamera = mGameManager->getCamera(name);
	mCamNode = mCamera->getParentSceneNode();
	return mCamera;
}

Ogre::SceneNode* GameState::getCamNode()
{
	return mCamNode;
}

std::deque<std::string>& GameState::getLoadedScripts()
{
	return mLoadedLuaScripts;
}

lua_State* GameState::getLuaState()
{
	return mLuaState;
}
