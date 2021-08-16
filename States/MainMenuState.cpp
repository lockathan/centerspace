#include "MainMenuState.h"
#include <Managers/GameManager.h>
#include <Ogre.h>
#include <GUI/RocketDocuments.h>
#include <Managers/SQLiteDBManager.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include <Entities/PlayerProfile.h>
#include <Repositories/FactionTemplateRepository.h>
#include <States/InBaseState.h>
#include <States/CampaignState.h>
#include <States/ProfileManagerState.h>
#include <Templates/FactionTemplate.h>
#include <vector>
#include <Rocket/Core.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/Element.h>
#include <Rocket/Controls/ElementFormControlInput.h>

using namespace Ogre;


MainMenuState::MainMenuState()
	:
	GameState(),
	mSQLiteDBManager(SQLiteDBManager::getSingletonPtr())
{
	
}

void MainMenuState::enter() 
{
	mIsEnabled = true;
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	mCamera = this->getCamera("MainCamera");
	mRocketRenderer->loadDocument(RocketDocuments::MainMenu::Main);
}

void MainMenuState::exit() 
{
	mIsEnabled = false;
}

void MainMenuState::pause() 
{
}

void MainMenuState::resume() 
{

}

void MainMenuState::update(Real timeSinceLastFrame)
{
	if(!mIsEnabled)
		return;
	//mSceneManager->getSceneNode("HeadNode")->translate(transVector * timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	//mDynamicsWorld->stepSimulation(timeSinceLastFrame,60);
}

void MainMenuState::mouseMoved(const OIS::MouseEvent& arg)
{

}

void MainMenuState::keyPressed(const OIS::KeyEvent& arg)
{
}

void MainMenuState::keyReleased(const OIS::KeyEvent& arg)
{
	//transVector.y = 0;
}

void MainMenuState::ProcessEvent(Rocket::Core::Event& ev)
{


	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "loadmultiplayermenu")
	{
		mRocketRenderer->loadDocument(RocketDocuments::MainMenu::Multiplayer);
	}
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "loadoptionsmenu")
	{
		mRocketRenderer->loadDocument(RocketDocuments::MainMenu::Options);
	}
	
	if(ev.GetType() == "click" && ev.GetTargetElement()->GetId() == "backtomainmenubutton")
	{
		mRocketRenderer->loadDocument(RocketDocuments::MainMenu::Main);
	}

	if(ev.GetType() == "click" && ev.GetTargetElement()->GetAttribute<Rocket::Core::String>("name","notcareerbutton") == "loadcampaignbutton")
	{
		int playerProfileId = ev.GetTargetElement()->GetAttribute< int >("playerprofileid", 0);
		int factionId = ev.GetTargetElement()->GetAttribute< int >("factionid", 0);
		InBaseState* inbase = InBaseState::getSingletonPtr();
		inbase->loadCampaignState(playerProfileId,factionId);
		mGameManager->changeState(inbase);
	}
}

void MainMenuState::loadSinglePlayer()
{
	mRocketRenderer->loadDocument(RocketDocuments::MainMenu::Campaign);
}

void MainMenuState::loadCampaignByName(const std::string& faction)
{
	InBaseState::getSingletonPtr()->loadCampaignState(ProfileManagerState::getSingletonPtr()->getCurrentPlayer()->getId(), FactionTemplateRepository::getSingletonPtr()->findByName(faction)->getId());
	mGameManager->changeState(InBaseState::getSingletonPtr());
}

void MainMenuState::onSelectedElementChanged(DataSourceType source, int id)
{

}

MainMenuState* MainMenuState::mMainMenuState;

MainMenuState* MainMenuState::getSingletonPtr() 
{
	if( !mMainMenuState ) 
	{
		mMainMenuState = new MainMenuState();
	}

	return mMainMenuState;
}