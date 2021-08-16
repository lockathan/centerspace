#ifndef InGameCamera_H
#define InGameCamera_H

#include <Ogre.h>
#include <Managers/GameManager.h>
#include <Input/InputListener.h>

class InGameCamera : public GameManager::EngineUpdateListener, public InputListener::Listener
{
public:
	~InGameCamera();

	void initialize(std::string camName, Ogre::Quaternion q, Ogre::Vector3& targetPos, Ogre::Vector3& centre);
	
	void mouseMoved(const OIS::MouseEvent& arg);
	void mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	
	void keyPressed(const OIS::KeyEvent& arg);
	void keyReleased(const OIS::KeyEvent& arg);

	void update(Ogre::Real timeSinceLastFrame);
	
	void addZoomX(float value);
	void addZoomY(float value);
	void addZoomZ(float value);

	void setZoomX(float value);
	void setZoomY(float value);
	void setZoomZ(float value);
	
	void setScrollUp(bool value);
	void setScrollDown(bool value);
	void setScrollLeft(bool value);
	void setScrollRight(bool value);

	void setMaxMovementPerFrameX(float value);
	void setMaxMovementPerFrameY(float value);
	
	Ogre::Camera* getCamera();
	Ogre::SceneNode* getCamNode();

	void stopAllMovement();

	static InGameCamera* getSingletonPtr();
private:
	InGameCamera();
	InGameCamera( const InGameCamera& ) { }
	InGameCamera & operator = ( const InGameCamera& );
	
	GameManager* mGameManager;
	Ogre::SceneManager* mSceneManager;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mCamNode;
	Ogre::SceneNode* mCamTargetNode;

	bool mCanZoomIn;
	bool mCanZoomOut;

	bool mScrollUp;
	bool mScrollDown;
	bool mScrollLeft;
	bool mScrollRight;

	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::Vector3 mZoom;
	Ogre::Real mMaxZoomInPerFrame;
	Ogre::Real mMaxZoomOutPerFrame;

	Ogre::Real mMaxMovementPerFrameX;
	Ogre::Real mMaxMovementPerFrameY;
	
	bool mCalculatedScreenMidpoints;
	int mMidScreenX;
	int mMidScreenY;

	Ogre::Vector3 mDirection;

	bool mEnabled;
	static InGameCamera* mInGameCamera;
	static const Ogre::Real MinDistance;
	static const Ogre::Real MaxDistance;
};

#endif