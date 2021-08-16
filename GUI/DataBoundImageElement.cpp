#include "DataBoundImageElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <GUI/DataBoundListItemElement.h>
#include <GUI/DataProviderElement.h>
#include <Entities/DataProvider.h>
#include <string>

DataBoundImageElement::DataBoundImageElement(const Rocket::Core::String& tag) 
	: 
	DataBoundElement(tag),
	mImageFolder(""),
	mCssClass("")
{

}

DataBoundImageElement::~DataBoundImageElement()
{

}

void DataBoundImageElement::ProcessEvent(Rocket::Core::Event& ev)
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

void DataBoundImageElement::buildElements()
{	
	if(!mGotDataProvider)
		return;
	if(!mInitialized)
		return;
	
	std::string imageFilename("");
	
	if(mItemID == -1)
		imageFilename = mDataProvider->getValue ( mOriginalRML.CString() );
	else
		imageFilename = mDataProvider->getValue ( mItemID, mOriginalRML.CString() );

	if(imageFilename != "")
	{
		if(this->HasChildNodes())
		{
			int numChildren = this->GetNumChildren();
			Rocket::Core::Element* temp = 0;
			for(int i = 0;i < numChildren;++i)
			{
				//temp = this->GetChild(0);
				this->RemoveChild(this->GetChild(0));
				//delete temp;
				//temp->RemoveReference();
			}
		}
		imageFilename = mImageFolder.CString() + imageFilename;
		Rocket::Core::Element* imgElement = Rocket::Core::Factory::InstanceElement(NULL,"img","img",Rocket::Core::XMLAttributes());
		imgElement->SetAttribute<Rocket::Core::String>(Rocket::Core::String("src"),imageFilename.c_str());
		imgElement->SetClass(mCssClass, true);
		if(mOnClickFunction != "")
			imgElement->SetAttribute<Rocket::Core::String>(Rocket::Core::String("onclick"),mOnClickFunction);
		if(mOnHoverFunction != "")
			imgElement->SetAttribute<Rocket::Core::String>(Rocket::Core::String("onblur"),mOnHoverFunction);
		if(mOnBlurFunction != "")
			imgElement->SetAttribute<Rocket::Core::String>(Rocket::Core::String("onhover"),mOnBlurFunction);
		this->AppendChild(imgElement);
		imgElement->RemoveReference();
	}
}

void DataBoundImageElement::initialize(bool gotDataProviderElement)
{
	DataBoundElement::initialize(gotDataProviderElement);
	mCssClass = this->GetAttribute<Rocket::Core::String>("class", "");
	mImageFolder = this->GetAttribute<Rocket::Core::String>("folder", "");
	this->SetInnerRML("");
}
