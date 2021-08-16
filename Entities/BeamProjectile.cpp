#include <Entities/BeamProjectile.h>
#include <Controllers/MCUController.h>
#include <Entities/MovableEntity.h>
#include <Managers/GameManager.h>
#include <Managers/SQLiteDBManager.h>
#include <States/InBattleState.h>
#include <Templates/BeamProjectileTemplate.h>
#include <Templates/WeaponTemplate.h>
#include <Ogre.h>

using namespace Ogre;

BeamProjectile::~BeamProjectile()
{
	if(mBeamBillboard)
	{
		SceneManager* manager = GameManager::getSingletonPtr()->getSceneManager();
		manager->destroyBillboardChain(mBeamBillboard);
	}
}

BeamProjectile::BeamProjectile(MCUController* source, const FireInstruction& instruction,BeamProjectileTemplate* beamTemplate)
	:
	Projectile(source,instruction),
	mBeamProjectileTemplate(beamTemplate),
	mBeamBillboard(0),
	mTimeElapsed(0),
	mBeamNode(0)
{
	
}

BeamProjectileTemplate* BeamProjectile::getBeamProjectileTemplate()
{
	return mBeamProjectileTemplate;
}

Ogre::Real BeamProjectile::getTimeElapsed()
{
	return mTimeElapsed;
}

void BeamProjectile::projectileAdded()
{
	mTimeElapsed = 0;

	if(mBeamProjectileTemplate->getMaterialName() != "" && mBeamProjectileTemplate->getMaterialName() != "none")
	{
		SceneManager* manager = GameManager::getSingletonPtr()->getSceneManager();
		mBeamBillboard = manager->createBillboardChain("beambillboard" + SQLiteDBManager::convertIntToString(this->getProjectileID()));
		mBeamBillboard->setMaxChainElements(4);
		mBeamBillboard->setNumberOfChains(2);
		mBeamBillboard->setMaterialName(mBeamProjectileTemplate->getMaterialName());

		BillboardChain::Element* elem = new BillboardChain::Element(this->getStartpoint(), mBeamProjectileTemplate->getBeamWidth(),0.f,ColourValue(1.0f,0.0f,0.0f),Quaternion::IDENTITY);
		mBeamBillboard->addChainElement(1, *elem);
		elem = new BillboardChain::Element(this->getEndpoint(), mBeamProjectileTemplate->getBeamWidth(),0.f,ColourValue(1.0f,0.0f,0.0f),Quaternion::IDENTITY);
		mBeamBillboard->addChainElement(1, *elem);
	
		mBeamNode = manager->getRootSceneNode()->createChildSceneNode("beambillboardnode" + SQLiteDBManager::convertIntToString(this->getProjectileID()));
		mBeamNode->attachObject(mBeamBillboard);
		mBeamBillboard->setVisible(true);
	}
}

void BeamProjectile::updateImpl(const Real& timeSinceLastFrame)
{
	mTimeElapsed+=timeSinceLastFrame;

	if(mTimeElapsed >= mBeamProjectileTemplate->getDuration())
	{
		MCUController* target = InBattleState::getSingletonPtr()->getTarget(this->getStartpoint(), this->getEndpoint(), this->getSource());
		this->applyWarhead(target);
	}
}

bool BeamProjectile::isCloserThan(const Vector3& point, Ogre::Real squaredDistance)
{
	if(point.squaredDistance(this->getStartpoint()) < squaredDistance)
		return true;
	if(point.squaredDistance(this->getEndpoint()) < squaredDistance)
		return true;
	if(point.squaredDistance(this->getStartpoint().midPoint(this->getEndpoint())) < squaredDistance)
		return true;

	return false;
}

bool BeamProjectile::isVisible()
{
	if(mBeamBillboard)
		return mBeamBillboard->isVisible();

	return this->isOwnerVisible();
}

void BeamProjectile::setVisible(bool value)
{
	if(mBeamBillboard)
		mBeamBillboard->setVisible(value);
}
