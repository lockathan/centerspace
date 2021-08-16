#include "DataBoundCssProperty.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Entities/DataProvider.h>
#include <GUI/DataProviderElement.h>
#include <Managers/SQLiteDBManager.h>
#include <string>

DataBoundCssProperty::DataBoundCssProperty(const Rocket::Core::String& tag) 
	: 
	DataBoundElement(tag),
	mCssPropertyName(""),
	mMinValue(0),
	mMaxValue(0),
	mValueField(""),
	mMaxValueField(""),
	mUnit("")
{

}

DataBoundCssProperty::~DataBoundCssProperty()
{

}

void DataBoundCssProperty::ProcessEvent(Rocket::Core::Event& ev)
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

void DataBoundCssProperty::initialize(bool gotDataProviderElement)
{
	DataBoundElement::initialize(gotDataProviderElement);

	mCssPropertyName = this->GetAttribute<Rocket::Core::String>("cssproperty", "");
	mMinValue = this->GetAttribute<float>("minvalue", 0);
	mMaxValue = this->GetAttribute<float>("maxvalue", 0);
	mMaxValueField = this->GetAttribute<Rocket::Core::String>("maxvaluefield","");
	mValueField = this->GetAttribute<Rocket::Core::String>("valuefield", "");
	mUnit = this->GetAttribute<Rocket::Core::String>("unit", "");
}
	
void DataBoundCssProperty::buildElements()
{
	if(!mGotDataProvider)
		return;

	float value(0);
	float maxValue(0);

	if(mItemID == -1)
	{
		value = std::atof(mDataProvider->getValue ( mValueField.CString() ).c_str());
		maxValue = std::atof(mDataProvider->getValue ( mMaxValueField.CString()).c_str());
	}
	else
	{
		value = std::atof(mDataProvider->getValue ( mItemID, mValueField.CString()).c_str());
		maxValue = std::atof(mDataProvider->getValue ( mItemID, mMaxValueField.CString()).c_str());
	}

	float range = mMaxValue - mMinValue;
	float percent = value / maxValue;

	Rocket::Core::String cssvalue = Rocket::Core::String(SQLiteDBManager::convertFloatToString((percent * range) + mMinValue).c_str());
	cssvalue+= mUnit;

	this->SetProperty(mCssPropertyName, cssvalue);

	if(this->HasChildNodes())
	{
		int numChildren = this->GetNumChildren();
		Rocket::Core::Element* temp = 0;
		for(int i = 0;i < numChildren;++i)
		{
			//temp = this->GetChild(0);
			this->GetChild(i)->SetProperty(mCssPropertyName, cssvalue);
			//delete temp;
			//temp->RemoveReference();
		}
	}
}