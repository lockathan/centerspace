#include "CampaignElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Managers/SQLiteDBManager.h>
#include <Entities/PlayerProfile.h>
#include <States/CampaignState.h>
#include <States/MainMenuState.h>
#include <States/ProfileManagerState.h>
#include <Templates/FactionTemplate.h>
#include <Repositories/FactionTemplateRepository.h>
#include <string>

CampaignElement::CampaignElement(const Rocket::Core::String& tag) : Rocket::Core::Element(tag)
{
	mInitialized = false;
	mSQLiteDBManager = SQLiteDBManager::getSingletonPtr();
	
	
}

CampaignElement::~CampaignElement()
{

}

void CampaignElement::ProcessEvent(Rocket::Core::Event& ev)
{
	Rocket::Core::Element::ProcessEvent(ev);
	if(!mInitialized)
	{
		int factionId = this->GetAttribute<int>("factionid",0);
		mFactionTemplate = FactionTemplateRepository::getSingletonPtr()->at(factionId);
		mCampaignState = mSQLiteDBManager->getCampaignState(ProfileManagerState::getSingletonPtr()->getCurrentPlayer()->getId(), factionId);

		buildElements();
		mInitialized = true;
	}

}

void CampaignElement::buildElements()
{
	Rocket::Core::XMLAttributes attributes;
	attributes.Set("name","loadcampaignbutton");
	attributes.Set("playerprofileid", ProfileManagerState::getSingletonPtr()->getCurrentPlayer()->getId());
	attributes.Set("factionid",mFactionTemplate->getId());
	if(mCampaignState)
		attributes.Set("CampaignStateId",mCampaignState->getId());

	Rocket::Core::Element* buttonElement = Rocket::Core::Factory::InstanceElement(NULL,"button","button",attributes);
	std::string buttonText;

	if(!mCampaignState)
		buttonText = "Start " + mFactionTemplate->getName() + " Campaign";
	else
		buttonText = "Continue " + mFactionTemplate->getName() + " Campaign";
	
	buttonElement->SetInnerRML(Rocket::Core::String(buttonText.c_str()));

	this->AppendChild(buttonElement);
	buttonElement->RemoveReference();
}