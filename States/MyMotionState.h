#ifndef MyMotionState_h
#define MyMotionState_h
#include "Ogre.h"
#include "btBulletCollisionCommon.h"

class MyMotionState : public btDefaultMotionState
{
	protected:
		Ogre::SceneNode * mVisibleObj;
		btTransform mTransform;
		btTransform mCOM;

	public:
		MyMotionState(const btTransform & initialPos, Ogre::SceneNode * node)
		{
			mVisibleObj = node;
			mTransform = initialPos;
			mCOM = btTransform::getIdentity();
		}

		MyMotionState(const btTransform & initialPos)
		{
			mVisibleObj = NULL;
			mTransform = initialPos;
		}

		~MyMotionState() {}

		void setNode(Ogre::SceneNode * node)
		{
			mVisibleObj = node;
		}

		btTransform getWorldTransform() const
		{
			return mCOM.inverse() * mTransform;
		}

		void getWorldTransform(btTransform & worldTrans) const
		{
			worldTrans = mCOM.inverse() * mTransform;
		}

		void setWorldTransform(const btTransform & worldTrans)
		{
			if (mVisibleObj == NULL)
				return;

			mTransform = worldTrans;
			btTransform transform = mTransform * mCOM;
			btQuaternion rot = transform.getRotation();
			btVector3 pos = transform.getOrigin();
			mVisibleObj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
			mVisibleObj->setPosition(pos.x(), pos.y(), pos.z());
		}
};


#endif