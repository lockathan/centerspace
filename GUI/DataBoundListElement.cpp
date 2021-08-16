#include "DataBoundListElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <GUI/DataBoundListItemElement.h>
#include <GUI/DataProviderElement.h>
#include <Entities/DataProvider.h>
#include <string>

DataBoundListElement::DataBoundListElement(const Rocket::Core::String& tag) 
	: 
	DataBoundElement(tag),
	mSelectedElement(0),
	mUseSelectBehaviour(true)
{
	int temp = 0;
}

DataBoundListElement::~DataBoundListElement()
{
	int temp = 0;
	//DataBoundElement::~DataBoundElement();
}

void DataBoundListElement::ProcessEvent(Rocket::Core::Event& ev)
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

void DataBoundListElement::buildElements()
{	
	if(!mGotDataProvider)
		return;
	
	DataSet* ds = mDataProvider->getDataSet();
	if(ds)
	{
		//DataProvider::getSingletonPtr()->initializeAllListeners();
		
		if(ds->numItems() < mListItems.size())
		{
			int currentItem(0);
			DataSetItem* item;

			while(ds->hasMoreItems())
			{
				item = ds->getNextItem();

				if(item->mData[FieldNames::ItemID] != "")
				{
					mListItems[currentItem]->setItemID(std::atoi(item->mData[FieldNames::ItemID].c_str()));
					mListItems[currentItem]->pushItemID();
				}

				++currentItem;
			}

			while(currentItem != mListItems.size())
			{
				mListItems[currentItem]->deregisterAll();
				this->RemoveChild(mListItems[currentItem]);
				//mListItems.pop_back();
				++currentItem;
			}

			mListItems.resize(ds->numItems());

		}
		else
		{
			std::vector<DataBoundListItemElement*>::iterator listenerIterator;
			DataSetItem* item;
			for(listenerIterator = mListItems.begin(); listenerIterator != mListItems.end();++listenerIterator)
			{
				
				item = ds->getNextItem();
				(*listenerIterator)->setItemID(std::atoi(item->mData[FieldNames::ItemID].c_str()));
				(*listenerIterator)->pushItemID();
			}

			while(ds->hasMoreItems())
			{
				item = ds->getNextItem();
			
				if(item->mData[FieldNames::ItemID] != "")
				{

					Rocket::Core::XMLAttributes attributes;
					attributes.Set(FieldNames::ItemID.c_str(),item->mData[FieldNames::ItemID].c_str());
					Rocket::Core::Element* listItemElement = Rocket::Core::Factory::InstanceElement(NULL,"databoundlistitem","databoundlistitem",attributes);
					listItemElement->SetInnerRML(mOriginalRML);
					if(mOnClickFunction != "")
						listItemElement->SetAttribute<Rocket::Core::String>(Rocket::Core::String("onclick"),mOnClickFunction);
					if(mOnHoverFunction != "")
						listItemElement->SetAttribute<Rocket::Core::String>(Rocket::Core::String("onblur"),mOnHoverFunction);
					if(mOnBlurFunction != "")
						listItemElement->SetAttribute<Rocket::Core::String>(Rocket::Core::String("onhover"),mOnBlurFunction);
					this->AppendChild(listItemElement);
					mListItems.push_back(dynamic_cast<DataBoundListItemElement*>(listItemElement));
				}
			}
		}

		ds->reset();
		
		this->DirtyLayout();
		DataProvider::getSingletonPtr()->initializeAllListeners();
		Rocket::Core::String content;
		this->GetInnerRML(content);
		//this->Update();
	}
}

void DataBoundListElement::initialize(bool gotDataProviderElement)
{
	DataBoundElement::initialize(gotDataProviderElement);
	
	mUseSelectBehaviour = this->GetAttribute<bool>("useselectbehaviour",true);

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
}

void DataBoundListElement::clearDataSource()
{
	int currentItem(0);
	while(currentItem != mListItems.size())
	{
		mListItems[currentItem]->deregisterAll();
		this->RemoveChild(mListItems[currentItem]);
		//mListItems.pop_back();
		++currentItem;
	}

	mListItems.resize(0);
}

void DataBoundListElement::changeSelectedElement(int id, bool propagate)
{
	if(mUseSelectBehaviour)
	{
		std::vector<DataBoundListItemElement*>::iterator listenerIterator;

		for(listenerIterator = mListItems.begin(); listenerIterator != mListItems.end();++listenerIterator)
		{
			if((*listenerIterator)->getItemID() == mSelectedElement)
				(*listenerIterator)->deselect();

			if((*listenerIterator)->getItemID() == id)
				(*listenerIterator)->select();
		}
		mSelectedElement = id;
		if(propagate)
			mDataProvider->changeSelectedElement(id);
	}
}

void DataBoundListElement::deregisterAll()
{
	DataBoundElement::deregisterAll();

	std::vector<DataBoundListItemElement*>::iterator listenerIterator;

	for(listenerIterator = mListItems.begin(); listenerIterator != mListItems.end();++listenerIterator)
	{
		(*listenerIterator)->deregisterAll();
	}

	mListItems.clear();
}