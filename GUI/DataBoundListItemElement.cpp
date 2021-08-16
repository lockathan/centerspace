#include "DataBoundListItemElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <GUI/DataBoundElement.h>
#include <GUI/DataBoundListElement.h>
#include <GUI/DataProviderElement.h>
#include <Entities/DataProvider.h>
#include <string>

DataBoundListItemElement::DataBoundListItemElement(const Rocket::Core::String& tag) 
	: 
	DataBoundElement(tag)
{

}

DataBoundListItemElement::~DataBoundListItemElement()
{
	mListeners.clear();
	
}

void DataBoundListItemElement::ProcessEvent(Rocket::Core::Event& ev)
{
	Rocket::Core::Element::ProcessEvent(ev);
	if(!mInitialized)
	{
		
		mItemID = this->GetAttribute<int>(FieldNames::ItemID.c_str(),-1);
		if(mItemID > 0)
			update();
		buildElements();
		mInitialized = true;
	}

	if(ev.GetType() == "click")
	{
		if(this->GetParentNode() && this->GetParentNode()->GetTagName() == "databoundlist")
		{
			DataBoundListElement* list = dynamic_cast<DataBoundListElement*>(this->GetParentNode());
			list->changeSelectedElement(mItemID);
		}
		
	}

}

void DataBoundListItemElement::registerListener(DataBoundElement* listener)
{
	mListeners.push_back(listener);
}

void DataBoundListItemElement::deregisterListener(DataBoundElement* listener)
{
	std::vector<DataBoundElement*>::iterator eraser;

	if(mListeners.size() > 0)
	{
		for(eraser = mListeners.begin(); eraser != mListeners.end(); ++eraser)
		{
			if(listener == (*eraser))
			{
				mListeners.erase(eraser);
				return;
			}
		}
	}
}

void DataBoundListItemElement::pushItemID()
{
	std::vector<DataBoundElement*>::iterator listenerIterator;

	for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end();++listenerIterator)
	{
		(*listenerIterator)->setItemID(mItemID);
	}
}

void DataBoundListItemElement::initialize()
{
	DataBoundElement::initialize();
	if(mItemID != -1)
		this->pushItemID();
}

void DataBoundListItemElement::buildElements()
{
	
}

void DataBoundListItemElement::select()
{
	this->SetClass("selected",true);
}

void DataBoundListItemElement::deselect()
{
	this->SetClass("selected",false);
}

void DataBoundListItemElement::deregisterAll()
{
	DataBoundElement::deregisterAll();

	std::vector<DataBoundElement*>::iterator listenerIterator;

	for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end();++listenerIterator)
	{
		(*listenerIterator)->deregisterAll();
	}
	
	mListeners.clear();

}