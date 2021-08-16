#include "WeaponButtonElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Controllers/MCUController.h>
#include <Entities/DataProvider.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/WeaponHardpoint.h>
#include <GUI/DataProviderElement.h>
#include <States/InBattleState.h>
#include <string>

WeaponButtonElement::WeaponButtonElement(const Rocket::Core::String& tag) : DataBoundElement(tag)
{
	mWeaponHardpoint = 0;
}

WeaponButtonElement::~WeaponButtonElement()
{

}

void WeaponButtonElement::ProcessEvent(Rocket::Core::Event& ev)
{
	Rocket::Core::Element::ProcessEvent(ev);
	
	if(!mInitialized)
	{	
		initialize();
	}
	
	if(ev.GetType() == "updatedata")
	{
		buildElements();
	}

	if(ev.GetType() == "click" && ev.GetTargetElement() == this && InBattleState::getSingletonPtr()->isLocalPlayersTurn())
	{
		InBattleState::getSingletonPtr()->fireWeapon(mMCUController, mWeaponHardpoint, false);
	}
	
	if(ev.GetType() == "mouseover" && ev.GetTargetElement() == this)
	{
		if(mMCUController->hasTarget())
		{
			InBattleState::getSingletonPtr()->updateAccuracy(mMCUController, mWeaponHardpoint);
		}
	}
}

void WeaponButtonElement::buildElements()
{
	if(!mGotDataProvider)
		return;

	std::string content = "";
	bool canFire = false;

	if(mItemID == -1)
		content = mDataProvider->getValue ( mOriginalRML.CString() );
	else
	{
		int id = std::atoi(mDataProvider->getValue( FieldNames::ItemID ).c_str());
		
		if(id >= 0)
		{
			mMCUController = InBattleState::getSingletonPtr()->getMCUByBattlefieldID(id);
			if(mMCUController)
			{
				mWeaponHardpoint = mMCUController->getMechanizedCombatUnit()->getWeaponHardpoint(mItemID);

				if(mWeaponHardpoint)
				{
					bool gotTarget = false;
					bool inRange = false;
					bool weaponAvailable = false;
					weaponAvailable = mMCUController->canFire(mWeaponHardpoint);
					gotTarget = mMCUController->hasTarget();
					float range = mMCUController->rangeToTarget();
					inRange = range <= mWeaponHardpoint->getWeapon()->getRange();
					canFire = weaponAvailable && gotTarget && inRange;
				}
			}
			else
			{
				return;
			}
		}

		content = mDataProvider->getValue ( mItemID, mOriginalRML.CString() );
	}

	if(content != "")
	{
		this->SetInnerRML(content.c_str());
		this->DirtyLayout();
	}
	if(canFire)
	{
		this->SetClass("weaponbuttonenabled", true);
		this->SetClass("weaponbuttondisabled", false);

	}
	else
	{
		this->SetClass("weaponbuttonenabled", false);
		this->SetClass("weaponbuttondisabled", true);
	}
}

MCUController* WeaponButtonElement::getMCUController()
{
	return mMCUController;
}

void WeaponButtonElement::setMCUController(MCUController* controller)
{
	mMCUController = controller;
}

WeaponHardpoint* WeaponButtonElement::getWeaponHardpoint()
{
	return mWeaponHardpoint;
}
void WeaponButtonElement::setWeaponHardpoint(WeaponHardpoint* weapon)
{
	mWeaponHardpoint = weapon;
}
