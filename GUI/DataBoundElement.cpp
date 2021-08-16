#include "DataBoundElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Entities/DataProvider.h>
#include <GUI/DataProviderElement.h>
#include <GUI/DataBoundListItemElement.h>
#include <GUI/RocketRenderer.h>
#include <Managers/LuaStateManager.h>
#include <States/GameState.h>
#include <string>

DataBoundElement::DataBoundElement(const Rocket::Core::String& tag) 
	: 
	Rocket::Core::Element(tag),
	mItemID(-1),
	mInitialized(false),
	mInitializing(false),
	mRegisteredListener(false),
	mGotDataProvider(false),
	mOriginalRML(""),
	mOnDataUpdateLuaFunctionName(""),
	mOnClickFunction(""),
	mOnHoverFunction(""),
	mOnBlurFunction("")
{
	DataProvider::getSingletonPtr()->registerListener(this);
}


DataBoundElement::~DataBoundElement()
{
	deregisterAll();
}

void DataBoundElement::initialize(bool gotDataProviderElement)
{
	if(mInitialized)
	{
		if(gotDataProviderElement)
		{	
			mGotDataProvider = true;
		}
		return;
	}

	if(mInitializing)
		return;

	mInitializing = true;

	Rocket::Core::Element* parent = this->GetParentNode();
	while(parent)
	{
		if(parent->GetTagName() == "dataprovider")
		{
			mDataProvider = dynamic_cast<DataProviderElement*>(parent);
			mDataProvider->registerListener(this);
			
			if(gotDataProviderElement)
				mGotDataProvider = true;
			else
				mGotDataProvider = mDataProvider->isInitialized();

			break;
		}
		parent = parent->GetParentNode();
	}

	computeItemID();

	this->GetInnerRML(mOriginalRML);
	mOnDataUpdateLuaFunctionName = this->GetAttribute<Rocket::Core::String>("ondataupdate", "");
	this->buildElements();
	this->DirtyLayout();
	mInitialized = true;
	mInitializing = false;
}


void DataBoundElement::computeItemID()
{
	mItemID = this->GetAttribute<int>("itemid",-1);
	
	if(mItemID == -1)
	{
		Rocket::Core::Element* parent = this->GetParentNode();
		while(parent)
		{
			if(parent->GetTagName() == "databoundlistitem")
			{
				DataBoundListItemElement* listItem = dynamic_cast<DataBoundListItemElement*>(parent);
				if(!mRegisteredListener)
				{
					listItem->registerListener(this);
					mRegisteredListener = true;
				}
				mItemID = listItem->getItemID();
				this->SetAttribute<int>("itemid", mItemID);
				break;
			}
			parent = parent->GetParentNode();
		}
	}

}

void DataBoundElement::setItemID(int id)
{
	mItemID = id;
	this->SetAttribute<int>("itemid", mItemID);
	if(mOnDataUpdateLuaFunctionName != "")
	{
		LuaStateManager::getSingletonPtr()->executeGuiFunction(RocketRenderer::getSingletonPtr()->getActiveState(), mOnDataUpdateLuaFunctionName.CString(), this);
	}
}

int DataBoundElement::getItemID()
{
	return mItemID;
}

void DataBoundElement::OnChildRemove(Rocket::Core::Element* child)
{
	//In most cases, when OnChildRemove is called, the parent data provider no longer exists, and it would be unsafe to even check it.

	Rocket::Core::Element::OnChildRemove(child);
	if(child == this)
	{

		if(mRegisteredListener)
		{
			Rocket::Core::Element* parent = this->GetParentNode();
			while(parent)
			{
				if(parent->GetTagName() == "databoundlistitem")
				{
					DataBoundListItemElement* listItem = dynamic_cast<DataBoundListItemElement*>(parent);
					if(!mRegisteredListener)
					{
						listItem->deregisterListener(this);
					}
					mItemID = listItem->getItemID();
					break;
				}
				parent = parent->GetParentNode();
			}
		}
	}
	
	//this->RemoveReference();
}

void DataBoundElement::OnUpdate()
{
	Rocket::Core::Element::OnUpdate();

	if(!mInitialized)
	{
		initialize();
	}

}


void DataBoundElement::update()
{
	computeItemID();
	buildElements();
	if(mOnDataUpdateLuaFunctionName != "")
	{
		LuaStateManager::getSingletonPtr()->executeGuiFunction(RocketRenderer::getSingletonPtr()->getActiveState(),  mOnDataUpdateLuaFunctionName.CString(), this);
	}
}

void DataBoundElement::setDataProviderElement(DataProviderElement* element)
{
	mDataProvider = element;
}

DataProviderElement* DataBoundElement::getDataProviderElement()
{
	return mDataProvider;
}

void DataBoundElement::deregisterAll()
{

	if(mDataProvider)
		mDataProvider->removeListener(this);

	mItemID = -1;
	this->SetInnerRML("");

}

bool DataBoundElement::isInitialized()
{
	return mInitialized;
}
