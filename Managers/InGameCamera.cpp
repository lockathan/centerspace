#include <Managers/InGameCamera.h>
#include <Managers/GameManager.h>
#include <Managers/PhysicsManager.h>
#include <Entities/BattlefieldGrid.h>
#include <BtOgre/BtOgreExtras.h>
#include <BtOgre/BtOgreGP.h>
#include <BtOgre/BtOgrePG.h>
#include <Ogre.h>

using namespace Ogre;

const Real InGameCamera::MinDistance = 30;
const Real InGameCamera::MaxDistance = 600;

InGameCamera::InGameCamera()
	:
	mEnabled(false),
	mGameManager(GameManager::getSingletonPtr()),
	mSceneManager(mGameManager->getSceneManager()),
	mCamera(0),
	mCamNode(0),
	mCamTargetNode(0),
	mCalculatedScreenMidpoints(false),
	mMidScreenX(0),
	mMidScreenY(0),
	mCanZoomIn(true),
	mCanZoomOut(true)
{
	
}

InGameCamera::~InGameCamera() 
{
	
	
}

void InGameCamera::initialize(std::string camName, Quaternion q, Vector3& targetPos, Vector3& centre)
{
	mCamera = mGameManager->getCamera(camName);
	mCamera->setOrientation(Quaternion::IDENTITY);
	mCamNode = mCamera->getParentSceneNode();
	mCalculatedScreenMidpoints = false;
	mMidScreenX = 0;
	mMidScreenY = 0;

	mCamTargetNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CamTargetNode");
	
	if(mCamNode->isInSceneGraph())
		mSceneManager->getRootSceneNode()->removeChild(mCamNode);
	
	mCamTargetNode->addChild(mCamNode);

	mScrollRight = false;
	mScrollLeft = false;
	mScrollUp = false;
	mScrollDown = false;

	mRotate = 0.25;
	mMove = 500;
	mMaxZoomInPerFrame = 180;
	mMaxZoomOutPerFrame = 180;

	mMaxMovementPerFrameX = 0;
	mMaxMovementPerFrameY = 0;

	mDirection = Ogre::Vector3::ZERO;
	mZoom = Ogre::Vector3::ZERO;

	//mCamNode->rotate(q);
	//mCamNode->setPosition(pos);
	
	Vector3 temp = Vector3(targetPos);
	temp.y = 0;
	Vector3 dir = (centre - temp);
	dir.normalise();
	//mCamTargetNode->setPosition(targetPos);
	Vector3 pos = targetPos + Vector3(0,BattlefieldGrid::UnitSize * 10, 0) - dir * BattlefieldGrid::UnitSize * 10;
	//mCamNode->setPosition(Vector3(-dir.x * BattlefieldGrid::UnitSize * 15,BattlefieldGrid::UnitSize * 15, -dir.z * BattlefieldGrid::UnitSize * 15));
	Quaternion q2 = Vector3::NEGATIVE_UNIT_Z.getRotationTo(dir);
	//Quaternion q2 = Quaternion::IDENTITY;
	
	Quaternion r(Degree(-60), Vector3::UNIT_X);
	q2 = q2 * r;
	mCamNode->rotate(q2);
	Vector3 camDirection = -(mCamNode->getOrientation() * Vector3::UNIT_Z);
	
	TerrainRayTestResultCallback callback = PhysicsManager::getSingletonPtr()->terrainRayTest(BtOgre::Convert::toBullet(pos), BtOgre::Convert::toBullet(pos + (camDirection * 10000)));

	if(callback.hasHit())
	{
		Vector3 targetLoc = BtOgre::Convert::toOgre(callback.m_hitPointWorld);
		mCamTargetNode->setPosition(targetLoc);
		mCamNode->setPosition(pos - targetLoc);
	}
	
	mCamera->setNearClipDistance(0.5f);
	mCamera->setFarClipDistance(25000);
	mEnabled = true;
}

void InGameCamera::update(Ogre::Real timeSinceLastFrame)
{
	if(!mEnabled)
		return;

	bool translated(false);

	Vector3 scrollDirection = Vector3(0,0,0);
	Vector3 camDirection = mCamera->getDerivedDirection();
	Vector3 currentLoc = mCamera->getDerivedPosition();
	Vector3 newPos;

	if(mScrollUp)
	{
		scrollDirection+=Vector3(0,0,1);
	}

	if(mScrollDown)
	{
		scrollDirection+=Vector3(0,0,-1);
	}

	if(mScrollLeft)
	{
		scrollDirection+=Vector3(1,0,0);
	}

	if(mScrollRight)
	{
		scrollDirection+=Vector3(-1,0,0);
	}
	
	float actualZoom = 0;

	if(mZoom.z > mMaxZoomInPerFrame)
	{
		actualZoom = mMaxZoomInPerFrame;
		mDirection.z += actualZoom;
		mZoom.z+= -mMaxZoomInPerFrame;
	}
	else if(mZoom.z < 0 && mZoom.z < -mMaxZoomOutPerFrame)
	{
		actualZoom = -mMaxZoomOutPerFrame;
		mDirection.z+=actualZoom;
		mZoom.z+= mMaxZoomOutPerFrame;
	}
	else
	{
		actualZoom = mZoom.z;
		mDirection.z+=mZoom.z;
		mZoom.z = 0;
	}
	
	if(mDirection != Vector3::ZERO)
	{
		Vector3 zoom = camDirection.normalisedCopy();
		zoom = zoom * -mDirection.z * timeSinceLastFrame;
		mCamNode->translate(zoom, Ogre::Node::TS_WORLD);
		//translated = true;
	}

	mDirection.z-=actualZoom;

	Vector3 mZoomMovement = Ogre::Vector3::ZERO;

	if(mZoom.x > mMaxMovementPerFrameX)
	{
		mZoomMovement.x=mMaxMovementPerFrameX;
		mZoom.x-= mMaxMovementPerFrameX;
	}
	else
	{
		mZoomMovement.x=mZoom.x;
		mZoom.x = 0;
	}

	if(mZoom.y > mMaxMovementPerFrameY)
	{
		mZoomMovement.z = mMaxMovementPerFrameY;
		mZoom.y -= mMaxMovementPerFrameY;
	}
	else
	{
		mZoomMovement.z = mZoom.y;
		mZoom.y = 0;
	}

	if(mZoomMovement != Vector3::ZERO)
	{
		
		newPos = currentLoc + (mZoomMovement * timeSinceLastFrame);

		if(PhysicsManager::getSingletonPtr()->terrainRayTest(BtOgre::Convert::toBullet(newPos), BtOgre::Convert::toBullet(newPos + (camDirection * 10000))).hasHit())
			mCamTargetNode->translate(mZoomMovement * timeSinceLastFrame, Ogre::Node::TS_WORLD);

		translated = true;
	}

	if(mScrollLeft || mScrollRight || mScrollUp || mScrollDown)
	{
		Vector3 facing = Vector3(camDirection);
		facing.y = 0;

		if(mScrollUp)
			scrollDirection = (facing * mMove / 2.f) * timeSinceLastFrame;
		if(mScrollDown)
			scrollDirection = -(facing * mMove / 2.f) * timeSinceLastFrame;
		if(mScrollRight)
			scrollDirection = (facing.crossProduct(Vector3::UNIT_Y) * mMove / 2.f) * timeSinceLastFrame;
		if(mScrollLeft)
			scrollDirection = -(facing.crossProduct(Vector3::UNIT_Y) * mMove / 2.f) * timeSinceLastFrame;

		
		newPos = currentLoc + scrollDirection;
		Vector3 target = newPos + (camDirection * 10000);

		if(PhysicsManager::getSingletonPtr()->terrainRayTest(BtOgre::Convert::toBullet(newPos), BtOgre::Convert::toBullet(newPos + (camDirection * 10000))).hasHit())
			mCamTargetNode->translate(scrollDirection, Ogre::Node::TS_WORLD);

		translated = true;
	}

	TerrainRayTestResultCallback callback = PhysicsManager::getSingletonPtr()->terrainRayTest(BtOgre::Convert::toBullet(currentLoc), BtOgre::Convert::toBullet(currentLoc + camDirection * 10000));

	if(callback.hasHit())
	{
		Vector3 hitpoint = BtOgre::Convert::toOgre(callback.m_hitPointWorld);
		Real squareDistance = (currentLoc - hitpoint).squaredLength();

		if(squareDistance > (MaxDistance * MaxDistance))
		{
			mCanZoomIn = true;
			mCanZoomOut = false;
			mZoom.z = 0;
		}
		else if(squareDistance < (MinDistance * MinDistance))
		{
			mCanZoomIn = false;
			mCanZoomOut = true;
			mZoom.z = 0;
		}
		else
		{
			mCanZoomIn = true;
			mCanZoomOut = true;
		}
	}
	else
	{
		mCanZoomIn = false;
		mCanZoomOut = true;
		mZoom.z = 0;
	}
	

	if(translated)
	{
		return;

		Vector3 absolutePos = mCamTargetNode->getPosition() + mCamNode->getPosition();
		callback = PhysicsManager::getSingletonPtr()->terrainRayTest(BtOgre::Convert::toBullet(absolutePos), BtOgre::Convert::toBullet(absolutePos + (camDirection * 10000)));

		if(callback.hasHit())
		{
			Vector3 targetLoc = BtOgre::Convert::toOgre(callback.m_hitPointWorld);
			mCamTargetNode->setPosition(targetLoc);
			mCamNode->setPosition(absolutePos - targetLoc);
			mCamNode->lookAt(mCamTargetNode->getPosition(), Ogre::Node::TransformSpace::TS_WORLD);
		}
	}
}

void InGameCamera::mouseMoved(const OIS::MouseEvent& arg)
{
	if(!mEnabled)
		return;

	if(!mCalculatedScreenMidpoints)
	{
		mMidScreenX = arg.state.width / 2;
		mMidScreenY = arg.state.height / 2;
		
		mMaxMovementPerFrameX = mMidScreenX;
		mMaxMovementPerFrameY = mMidScreenY;
	}

	if(arg.state.Z.rel != 0)
	{
		if(arg.state.Z.rel > 0 && mCanZoomIn)
		{
			if( Math::Abs( arg.state.X.abs - mMidScreenX) > mMidScreenX / 4)
				mZoom.x-=(arg.state.X.abs - mMidScreenX) * 2;
				
			if(Math::Abs( arg.state.Y.abs - mMidScreenY) > mMidScreenY / 4)
				mZoom.y-=(arg.state.Y.abs - mMidScreenY) * 2;

			mZoom.z -= arg.state.Z.rel * 10;
		}
		else if(arg.state.Z.rel < 0 && mCanZoomOut)
		{
			mZoom.z -= arg.state.Z.rel * 10;
		}
		
	}

	if(arg.state.X.abs < 25 && arg.state.X.abs > 0)
		mScrollLeft = true;
	else
		mScrollLeft= false;
	
	if(arg.state.width - arg.state.X.abs < 25 && arg.state.width - arg.state.X.abs > 0)
		mScrollRight = true;
	else
		mScrollRight= false;

	if(arg.state.Y.abs < 25 && arg.state.Y.abs > 0) 
		mScrollUp = true;
	else
		mScrollUp= false;

	if(arg.state.height - arg.state.Y.abs < 25 && arg.state.height - arg.state.Y.abs > 0)
		mScrollDown = true;
	else
		mScrollDown= false;

	if(arg.state.buttonDown(OIS::MouseButtonID::MB_Right))
	{
		//mCamTargetNode->translate(arg.state.X.rel * Vector3::UNIT_X, Ogre::Node::TransformSpace::TS_LOCAL);
		//mCamNode->pitch (Ogre::Degree (-mRotate * arg.state.Y.rel), Ogre::Node::TransformSpace::TS_LOCAL);
		mCamTargetNode->yaw (Ogre::Degree (-mRotate * arg.state.X.rel), Ogre::Node::TransformSpace::TS_WORLD);
		
		
	}

	return;
	

}

void InGameCamera::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(!mEnabled)
		return;

	if(arg.state.Z.rel == 0)
		mInGameCamera->setZoomZ(0);
}

void InGameCamera::keyPressed(const OIS::KeyEvent& arg)
{
	switch (arg.key)
	{
		case OIS::KC_UP:
		case OIS::KC_W:
			mInGameCamera->setScrollUp(true);
			break;
 
		case OIS::KC_DOWN:
		case OIS::KC_S:
			mInGameCamera->setScrollDown(true);
			break;
 
		case OIS::KC_LEFT:
		case OIS::KC_A:
			mInGameCamera->setScrollLeft(true);
			break;
 
		case OIS::KC_RIGHT:
		case OIS::KC_D:
			mInGameCamera->setScrollRight(true);
			break;
 
		case OIS::KC_PGDOWN:
		case OIS::KC_E:
			mZoom = -mMove * 10;
			break;
 
		case OIS::KC_PGUP:
		case OIS::KC_Q:
			mZoom = mMove * 10;
			break;
 
		default:
			break;
	}
}

void InGameCamera::keyReleased(const OIS::KeyEvent& arg)
{
	switch (arg.key)
	{
		case OIS::KC_UP:
		case OIS::KC_W:
			mInGameCamera->setScrollUp(false);
			break;
 
		case OIS::KC_DOWN:
		case OIS::KC_S:
			mInGameCamera->setScrollDown(false);
			break;
 
		case OIS::KC_LEFT:
		case OIS::KC_A:
			mInGameCamera->setScrollLeft(false);
			break;
 
		case OIS::KC_RIGHT:
		case OIS::KC_D:
			mInGameCamera->setScrollRight(false);
			break;
 
		case OIS::KC_PGDOWN:
		case OIS::KC_E:
			mZoom = 0;
			break;
 
		case OIS::KC_PGUP:
		case OIS::KC_Q:
			mZoom = 0;
			break;
 
		default:
			break;
		}
}


void InGameCamera::addZoomX(float value)
{
	mZoom.x += value;
}

void InGameCamera::addZoomY(float value)
{
	mZoom.y += value;
}

void InGameCamera::addZoomZ(float value)
{
	mZoom.z += value;
}

void InGameCamera::setZoomX(float value)
{
	mZoom.x = value;
}

void InGameCamera::setZoomY(float value)
{
	mZoom.y = value;
}

void InGameCamera::setZoomZ(float value)
{
	mZoom.z = value;
}

void InGameCamera::setScrollUp(bool value)
{
	mScrollUp = value;
}

void InGameCamera::setScrollDown(bool value)
{
	mScrollDown = value;
}

void InGameCamera::setScrollLeft(bool value)
{
	mScrollLeft = value;
}

void InGameCamera::setScrollRight(bool value)
{
	mScrollRight = value;
}

void InGameCamera::setMaxMovementPerFrameX(float value)
{
	mMaxMovementPerFrameX = value;
}

void InGameCamera::setMaxMovementPerFrameY(float value)
{
	mMaxMovementPerFrameY = value;
}

Camera* InGameCamera::getCamera()
{
	return mCamera;
}

SceneNode* InGameCamera::getCamNode()
{
	return mCamNode;
}

void InGameCamera::stopAllMovement()
{
	if(!mEnabled)
		return;

	mScrollDown = false;
	mScrollLeft = false;
	mScrollUp = false;
	mScrollRight = false;
	mZoom.z = 0;
}

InGameCamera* InGameCamera::mInGameCamera;

InGameCamera* InGameCamera::getSingletonPtr()
{
	if(!mInGameCamera)
	{
		mInGameCamera = new InGameCamera();
	}
	return mInGameCamera;
}