#ifndef MCUAnimationController_h
#define MCUAnimationController_h

#include <Ogre.h>
#include <Managers/GameManager.h>

class AnimationDefinitionTemplate;
class MCUController;
class WeaponAnimationTemplate;

enum FiringPhase
{
	NOT_FIRING = 0,
	ENTER_ANIM = 1,
	FIRE_ANIM = 2,
	EXIT_ANIM = 3
};

class MCUAnimationController : public GameManager::EngineUpdateListener
{
public:
	MCUAnimationController(Ogre::SceneNode* node, Ogre::Entity* entity, AnimationDefinitionTemplate* anim, MCUController* controller);
	~MCUAnimationController();

	Ogre::SceneNode * getBodySceneNode();
	Ogre::Vector3 getPosition() const;

	void animWalkStart();
	void animWalkEnd();
	void animJumpStart();
	void animJumpLoop();
	void animJumpEnd();
	void animDie();
	void startFiringAnimation(int hardpoint);

	void setIsMoving(bool isMoving);
	
	bool isEnabled();
	void setEnabled(bool isEnabled);
	
	MCUController* getMCUController();
	void setMCUController(MCUController* controller);

	bool isTakingOff();
	bool isInAir();
	bool isLanding();
	virtual void update( Ogre::Real timeSinceLastFrame );
private:
	static float ANIM_FADE_SPEED;

	void setupAnimations();
	void fadeAnimations(Ogre::Real deltaTime);
	void setAnimation(int id, bool reset = false);
	void setAnimation(std::string animName, bool reset = false);

	float mWalkingAnimationSpeed;
	int mAISpeedBoost;

	int findAnimation(std::string name);

	AnimationDefinitionTemplate* mAnimationDefinitionTemplate;
	Ogre::SceneNode * mBodyNode;
	Ogre::Real mPivotPitch;

	Ogre::Entity * mBodyEnt;
	std::vector<Ogre::AnimationState*> mAnims;
	
	std::vector<std::string> mAnimNames;
	int mCurrentAnimID;
	WeaponAnimationTemplate* mCurrentFiringWeapon;
	bool mIsFiring;
	bool mFiredWeapon;

	bool* mFadingIn;
	bool* mFadingOut;
	Ogre::Real mTimer; // general timer to see how long animations have been playing
	bool mMoving; // used to decide what animation we want to use after we've landed
	bool mEnabled;
	FiringPhase mFiringPhase;
	MCUController* mMCUController;
};

#endif