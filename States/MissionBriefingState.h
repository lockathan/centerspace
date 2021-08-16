#ifndef MissionBriefingState_H
#define MissionBriefingState_H

#include <OgreCamera.h>

#include "GameState.h"

class MissionBriefingState : public GameState {
public:
	~MissionBriefingState();

	void enter();
	void exit();

	void pause();
	void resume();
	void createScene();
	void destroyScene();

	void update( Ogre::Real timeSinceLastFrame );

	static MissionBriefingState* getSingletonPtr();
private:
	MissionBriefingState();
	MissionBriefingState( const MissionBriefingState& ) { }
	MissionBriefingState & operator = ( const MissionBriefingState& );

	static MissionBriefingState *mMissionBriefingState;
};
#endif