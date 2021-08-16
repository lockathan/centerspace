#include <Controllers/MCUAnimationController.h>
#include <Controllers/MCUController.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/WeaponHardpoint.h>
#include <Managers/GameManager.h>
#include <Templates/AnimationDefinitionTemplate.h>
#include <Templates/MCUTemplate.h>
#include <Templates/WeaponAnimationTemplate.h>
#include <Ogre.h>

float MCUAnimationController::ANIM_FADE_SPEED = 7.5;

using namespace Ogre;

MCUAnimationController::MCUAnimationController(SceneNode* node, Entity* entity, AnimationDefinitionTemplate* anim, MCUController* controller)
	:
	mWalkingAnimationSpeed(1.f),
	mAnimationDefinitionTemplate(0),
	mBodyNode(0),
	mPivotPitch(0.f),
	mBodyEnt(0),
	mCurrentAnimID(0),
	mCurrentFiringWeapon(0),
	mAISpeedBoost(1),
	mIsFiring(false),
	mFiredWeapon(false),
	mTimer(0),
	mMoving(false),
	mEnabled(true),
	mFiringPhase(FiringPhase::NOT_FIRING),
	mMCUController(controller)
{
	mBodyEnt = entity;
	mBodyNode = node;
	mAnimationDefinitionTemplate = anim;
	mWalkingAnimationSpeed = mMCUController->getMechanizedCombatUnit()->getMCUTemplate()->getWalkingAnimationSpeed();
	setupAnimations();
	if(mMCUController->getAIGroup())
		mAISpeedBoost = 2;
}

MCUAnimationController::~MCUAnimationController()
{

}

void MCUAnimationController::setupAnimations()
{
	mBodyEnt->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
	
	AnimationStateSet* set = mBodyEnt->getAllAnimationStates();
	AnimationStateIterator iter = set->getAnimationStateIterator();

	while(iter.hasMoreElements())
	{
		mAnimNames.push_back(iter.current()->second->getAnimationName());
		mAnims.push_back(iter.current()->second);
		iter.current()->second->setEnabled(false);
		iter.current()->second->setLoop(true);
		iter.current()->second->setWeight(0);
		iter.moveNext();
	}
	
	for(int i = 0; i < mAnimationDefinitionTemplate->getNumWeaponAnimations();++i)
	{
		WeaponAnimationTemplate* weap = mAnimationDefinitionTemplate->getWeaponAnimation(i);

		if(weap)
		{
			if(weap->hasEnterAnimation())
			{
				mAnims[findAnimation(weap->getEnterAnimationName())]->setLoop(false);
			}
			
			mAnims[findAnimation(weap->getFireAnimationName())]->setLoop(false);

			if(weap->hasExitAnimation())
			{
				
				mAnims[findAnimation(weap->getExitAnimationName())]->setLoop(false);
			}
		}
	}

	mFadingIn = new bool[mAnimNames.size()];
	mFadingOut = new bool[mAnimNames.size()];

	for (unsigned int i = 0; i < mAnimNames.size(); ++i)
	{
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}
	
	
	setAnimation(findAnimation(mAnimationDefinitionTemplate->getIdleAnimationName()));
	mAnims[findAnimation(mAnimationDefinitionTemplate->getIdleAnimationName())]->setEnabled(true);
}

void MCUAnimationController::update(Ogre::Real deltaTime)
{
	
	if(!mEnabled)
		return;

	Ogre::Real baseAnimSpeed = 1;

	mTimer += deltaTime;
	
	
	if (mCurrentAnimID == findAnimation(mAnimationDefinitionTemplate->getTakeOffAnimationName()) && mTimer >= mAnims[mCurrentAnimID]->getLength())
	{
		// takeoff animation finished, so time to leave the ground!
		setAnimation(mAnimationDefinitionTemplate->getFlyAnimationName(), true);
	}
	else if (mCurrentAnimID == findAnimation(mAnimationDefinitionTemplate->getLandAnimationName()))
	{
		if (mTimer >= mAnims[mCurrentAnimID]->getLength())
		{
			if (mMoving)
			{
				setAnimation(mAnimationDefinitionTemplate->getWalkAnimationName(), true);
			}
			else
			{
				setAnimation(mAnimationDefinitionTemplate->getIdleAnimationName());
			}
		}
	}
	else if (mCurrentAnimID == findAnimation(mAnimationDefinitionTemplate->getDeathAnimationName()) && mTimer >= mAnims[mCurrentAnimID]->getLength())
	{
		mMCUController->destroy();
	}
	
	if(mIsFiring)
	{
		if(mFiringPhase == FiringPhase::ENTER_ANIM)
		{
			mAnims[mCurrentAnimID]->addTime(deltaTime * mCurrentFiringWeapon->getEnterAnimationSpeed()  * mAISpeedBoost * 4);
			if(mTimer >= mAnims[mCurrentAnimID]->getLength())
			{
				setAnimation(mCurrentFiringWeapon->getFireAnimationName(),true);
				mFiringPhase = FiringPhase::FIRE_ANIM;
			}
		}
		else if(mFiringPhase == FiringPhase::FIRE_ANIM)
		{
			mAnims[mCurrentAnimID]->addTime(deltaTime * mCurrentFiringWeapon->getFireAnimationSpeed() * mAISpeedBoost);
			if(!mFiredWeapon)
			{
				mFiredWeapon = true;
				mMCUController->fireWeapon(mCurrentFiringWeapon->getHardpointNumber());
			}

			if(mTimer >= mAnims[mCurrentAnimID]->getLength())
			{
				if(mCurrentFiringWeapon->hasExitAnimation())
				{
					setAnimation(mCurrentFiringWeapon->getExitAnimationName(),true);
					mFiringPhase = FiringPhase::EXIT_ANIM;
				}
				else
				{
					setAnimation(mAnimationDefinitionTemplate->getIdleAnimationName(),true);
					mFiringPhase = FiringPhase::NOT_FIRING;
					mIsFiring = false;
				}
			}
		}
		else if(mFiringPhase == FiringPhase::EXIT_ANIM)
		{
			mAnims[mCurrentAnimID]->addTime(deltaTime * mCurrentFiringWeapon->getExitAnimationSpeed() * mAISpeedBoost * 4);
			if(mTimer >= mAnims[mCurrentAnimID]->getLength())
			{
				setAnimation(mAnimationDefinitionTemplate->getIdleAnimationName(),true);
				mFiringPhase = FiringPhase::NOT_FIRING;
				mIsFiring = false;
			}
		}
	}
	else
	{
		if(mCurrentAnimID == findAnimation(mAnimationDefinitionTemplate->getWalkAnimationName()))
			mAnims[mCurrentAnimID]->addTime(deltaTime * mWalkingAnimationSpeed * mAISpeedBoost);
		else if (mCurrentAnimID >= 0)
			mAnims[mCurrentAnimID]->addTime(deltaTime * baseAnimSpeed * mAISpeedBoost);
	}


	//fadeAnimations(deltaTime);
}

void MCUAnimationController::fadeAnimations(Ogre::Real deltaTime)
{
	for (unsigned int i = 0; i < mAnimNames.size(); ++i)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full wieght
			Ogre::Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));

			if (newWeight >= 1)
				mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out unit it has no weight, add the disable it
			Ogre::Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));

			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void MCUAnimationController::setAnimation(int id, bool reset)
{
	if (mCurrentAnimID >= 0 && mCurrentAnimID < mAnimNames.size())
	{
		// if we have an old animation, fade it out
		mFadingIn[mCurrentAnimID] = false;
		mFadingOut[mCurrentAnimID] = true;
		mAnims[mCurrentAnimID]->setEnabled(false);
		mAnims[mCurrentAnimID]->setWeight(0);
	}

	mCurrentAnimID = id;

	if(mCurrentAnimID == mAnimNames.size())
			mCurrentAnimID = 0;

	if (id >= 0)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(1);
		mFadingOut[id] = false;
		mFadingIn[id] = true;

		if (reset)
		{
			mAnims[id]->setTimePosition(0);
			mTimer = 0;
		}
	}
}
void MCUAnimationController::setAnimation(std::string animName, bool reset)
{
	setAnimation(findAnimation(animName), reset);
}

int MCUAnimationController::findAnimation(std::string name)
{
	for(unsigned int i = 0; i < mAnimNames.size();++i)
	{
		if(mAnimNames[i] == name)
			return i;
	}

	return -1;
}

void MCUAnimationController::animWalkStart()
{
	mMoving = true;
	setAnimation(mAnimationDefinitionTemplate->getWalkAnimationName());
}

void MCUAnimationController::animWalkEnd()
{
	mMoving = false;
	setAnimation(mAnimationDefinitionTemplate->getIdleAnimationName(),true);
}

void MCUAnimationController::animJumpStart()
{
	setAnimation(mAnimationDefinitionTemplate->getTakeOffAnimationName(),true);
	mTimer = 0;
	mMCUController->playJumpSound();
}

void MCUAnimationController::animJumpLoop()
{
	setAnimation(mAnimationDefinitionTemplate->getFlyAnimationName(),true);
}

void MCUAnimationController::animJumpEnd()
{
	setAnimation(mAnimationDefinitionTemplate->getLandAnimationName(),true);
	mTimer = 0;
	mMCUController->playLandSound();
}

void MCUAnimationController::animDie()
{
	setAnimation(mAnimationDefinitionTemplate->getDeathAnimationName(),true);
}

void MCUAnimationController::startFiringAnimation(int hardpoint)
{
	mCurrentFiringWeapon = mAnimationDefinitionTemplate->getWeaponAnimation(hardpoint);
	if(mCurrentFiringWeapon && !mIsFiring)
	{
		mMCUController->updateGuiData("start firing");
		mIsFiring = true;
		mFiredWeapon = false;

		if(mCurrentFiringWeapon->hasEnterAnimation())
		{
			setAnimation(mCurrentFiringWeapon->getEnterAnimationName(),true);
			mCurrentAnimID = findAnimation(mCurrentFiringWeapon->getEnterAnimationName());
			mFiringPhase = FiringPhase::ENTER_ANIM;
		}
		else
		{
			setAnimation(mCurrentFiringWeapon->getFireAnimationName(),true);
			mCurrentAnimID = findAnimation(mCurrentFiringWeapon->getFireAnimationName());
			mFiringPhase = FiringPhase::FIRE_ANIM;
		}
	}
}

bool MCUAnimationController::isEnabled()
{
	return mEnabled;
}

void MCUAnimationController::setEnabled(bool isEnabled)
{
	mEnabled = isEnabled;

	if(mEnabled)
		setupAnimations();
}

MCUController* MCUAnimationController::getMCUController()
{
	return mMCUController;
}

void MCUAnimationController::setMCUController(MCUController* controller)
{
	mMCUController = controller;
}

bool MCUAnimationController::isTakingOff()
{
	return mCurrentAnimID == findAnimation(mAnimationDefinitionTemplate->getTakeOffAnimationName());
}

bool MCUAnimationController::isInAir()
{
	return mCurrentAnimID == findAnimation(mAnimationDefinitionTemplate->getFlyAnimationName());
}

bool MCUAnimationController::isLanding()
{
	return mCurrentAnimID == findAnimation(mAnimationDefinitionTemplate->getLandAnimationName());
}
