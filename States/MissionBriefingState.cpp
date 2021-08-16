#include "MissionBriefingState.h"
#include <OgreTextureUnitState.h>
#include <Ogre.h>

using namespace Ogre;

MissionBriefingState::MissionBriefingState()
	:
	GameState()
{

}

MissionBriefingState::~MissionBriefingState()
{

}

void MissionBriefingState::enter() 
{
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	mIsEnabled = true;   
}

void MissionBriefingState::exit() 
{
	mIsEnabled = true;
}

void MissionBriefingState::pause() 
{

}

void MissionBriefingState::resume() 
{

}

void MissionBriefingState::update(Real timeSinceLastFrame)
{
	if(!mIsEnabled)
		return;
}

void MissionBriefingState::createScene()
{

}
void MissionBriefingState::destroyScene()
{

}

MissionBriefingState* MissionBriefingState::mMissionBriefingState;

MissionBriefingState* MissionBriefingState::getSingletonPtr() 
{
	if( !mMissionBriefingState ) {
		mMissionBriefingState = new MissionBriefingState();
	}

	return mMissionBriefingState;
}