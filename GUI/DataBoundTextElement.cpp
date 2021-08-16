#include "DataBoundTextElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Entities/DataProvider.h>
#include <GUI/DataProviderElement.h>
#include <string>

DataBoundTextElement::DataBoundTextElement(const Rocket::Core::String& tag) 
	: 
	DataBoundElement(tag)
{

}

DataBoundTextElement::~DataBoundTextElement()
{

}

void DataBoundTextElement::ProcessEvent(Rocket::Core::Event& ev)
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
}

void DataBoundTextElement::buildElements()
{
	if(mOriginalRML.CString() == FieldNames::MCU::RemainingBatteryPower || mOriginalRML.CString() == FieldNames::MCU::RemainingReactorPower)
		int temp = 0;

	if(!mGotDataProvider)
		return;

	std::string content;

	if(mItemID == -1)
		content = mDataProvider->getValue ( mOriginalRML.CString() );
	else
		content = mDataProvider->getValue ( mItemID, mOriginalRML.CString() );

	if(content != "")
	{
		this->SetInnerRML(content.c_str());
		//Rocket::Core::Element* descElement = Rocket::Core::Factory::InstanceElement(NULL,"div","div",Rocket::Core::XMLAttributes());
		//descElement->SetInnerRML(Rocket::Core::String(content.c_str()));
		//this->AppendChild(descElement);
		//descElement->RemoveReference();

	}
	else
	{
		this->SetInnerRML(content.c_str());
	}
}