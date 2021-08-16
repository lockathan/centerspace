#include <Entities/Projectile.h>
#include <Controllers/MCUController.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MovableEntity.h>
#include <Entities/WeaponHardpoint.h>
#include <Managers/SQLiteDBManager.h>
#include <Repositories/SoundTemplateRepository.h>
#include <States/InBattleState.h>
#include <Templates/ProjectileTemplate.h>
#include <Templates/SoundTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Ogre.h>
#include <OgreAL.h>
using namespace Ogre;

Projectile::Projectile()
	:
	MovableEntity(true),
	mFireInstruction(),
	mSource(0),
	mProjectileID(0),
	mFireSound(0),
	mImpactSound(0),
	mPlayedFireSound(false),
	mPlayedImpactSound(false),
	mWaitingForSoundToFinish(false),
	mAppliedWarhead(false)
{
	mUsesVisibilityCalculations = true;
}

Projectile::Projectile(MCUController* source, const FireInstruction& instruction)
	:
	MovableEntity(),
	mFireInstruction(instruction),
	mSource(source),
	mProjectileID(0),
	mFireSound(0),
	mImpactSound(0),
	mPlayedFireSound(false),
	mPlayedImpactSound(false),
	mWaitingForSoundToFinish(false),
	mAppliedWarhead(false)
{
	mUsesVisibilityCalculations = true;

}

Projectile::~Projectile()
{
	
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();
	if(mFireSound)
	{
		soundManager->destroySound(mFireSound);
	}
	if(mImpactSound)
	{
		soundManager->destroySound(mImpactSound);
	}
	mSource->weaponFiringComplete();
}


Ogre::Vector3 Projectile::getStartpoint()
{
	return mFireInstruction.Source;
}

void Projectile::setStartpoint(const Ogre::Vector3& start)
{
	mFireInstruction.Source = start;
}

Ogre::Vector3 Projectile::getEndpoint()
{
	return mFireInstruction.Destination;
}

void Projectile::setEndpoint(const Ogre::Vector3& end)
{
	mFireInstruction.Destination = end;
}

int Projectile::getProjectileID()
{
	return mProjectileID;
}

void Projectile::setProjectileID(int id)
{
	mProjectileID = id;
	
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();

	SoundTemplate* fireSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName(mFireInstruction.WeaponHardpoint->getWeapon()->getProjectileTemplate()->getFireSoundName());
	if(fireSoundTemplate)
	{
		mFireSound = soundManager->createSound(fireSoundTemplate->getName() + SQLiteDBManager::convertIntToString(mProjectileID), fireSoundTemplate->getFileName(), fireSoundTemplate->loop());
	}
	SoundTemplate* impactSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName(mFireInstruction.WeaponHardpoint->getWeapon()->getProjectileTemplate()->getExplosionName());
	if(impactSoundTemplate)
	{
		mImpactSound = soundManager->createSound(impactSoundTemplate->getName() + SQLiteDBManager::convertIntToString(mProjectileID), impactSoundTemplate->getFileName(), impactSoundTemplate->loop());
	}
}

WeaponTemplate* Projectile::getWeaponTemplate()
{
	return mFireInstruction.WeaponHardpoint->getWeapon();
}

MCUController* Projectile::getSource()
{
	return mSource;
}

void Projectile::setSource(MCUController* source)
{
	mSource = source;
}

FireInstruction Projectile::getFireInstruction()
{
	return mFireInstruction;
}

void Projectile::applyWarhead(MCUController* target)
{
	if(!mAppliedWarhead)
	{
		mAppliedWarhead = true;
		std::string explosionTemplateName = mFireInstruction.WeaponHardpoint->getWeapon()->getProjectileTemplate()->getExplosionName();
		GameManager::getSingletonPtr()->luaDebugReport("Applying warhead", 0);

		if(explosionTemplateName != "" && explosionTemplateName != "none")
		{
			//InBattleState::getSingletonPtr()->addParticleSystem(explosionTemplateName, this->getEndpoint());
			InBattleState::getSingletonPtr()->addExplosion(explosionTemplateName, this->getEndpoint());

		}

		if(!mPlayedImpactSound && mImpactSound && this->isVisible())
		{
			mImpactSound->play();
			mPlayedImpactSound = true;
		}

		if(target)
			target->applyWarhead(this->getProjectileID(), mFireInstruction);

		//InBattleState::getSingletonPtr()->removeProjectile(this);
		if((!mFireSound && !mImpactSound) ||
			(mFireSound && !mFireSound->isPlaying()) ||
			(mImpactSound && !mImpactSound->isPlaying()))
		{
			mReadyToBeDeleted = true;
		}
		else
		{
			mWaitingForSoundToFinish = true;
		}
	}
}

void Projectile::update(const Ogre::Real& timeSinceLastFrame)
{
	if(!mPlayedFireSound && mFireSound && this->isVisible() )
	{
		mFireSound->play();
		mPlayedFireSound = true;
	}
	if(mWaitingForSoundToFinish)
	{
		if((!mFireSound || (mFireSound && !mFireSound->isPlaying())) && (!mImpactSound || (mImpactSound && !mImpactSound->isPlaying())))
		{
			mReadyToBeDeleted = true;
			mWaitingForSoundToFinish = false;
			return;
		}
	}
	updateImpl(timeSinceLastFrame);
}

bool Projectile::isOwnerVisible()
{
	return mFireInstruction.FiredBy->isVisible();
}
