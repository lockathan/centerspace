#include "DataProviderElement.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include <GUI/DataBoundElement.h>
#include <GUI/DataBoundListElement.h>
#include <GUI/DataBoundListItemElement.h>
#include <GUI/LuaScriptElement.h>
#include <Managers/GameManager.h>
#include <States/GameState.h>
#include <string>
#include <deque>

DataProviderElement::DataProviderElement(const Rocket::Core::String& tag) 
	: 	
	Rocket::Core::Element(tag),
	mInitialized(false),
	mEnabled(true),
	mVisible(true),
	mIsSubDataSource(false),
	mRegisteredListener(false),
	mIsUpdating(false),
	mBufferModified(false),
	mElementsToRemove(false),
	mDataSource(DataSourceType::COULD_NOT_FIND),
	mSubDataSourceType(DataSourceType::COULD_NOT_FIND),
	mItemID(-1),
	mDataSourceID(""),
	mDataProvider(0),
	mParent(0)
	
{
	mDataProvider = DataProvider::getSingletonPtr();
}

DataProviderElement::~DataProviderElement()
{
	mDataProvider->deregister(this);
	mDataProvider->deregister(this, mDataSource);
	if(mSubDataSourceType != DataSourceType::COULD_NOT_FIND && mSubDataSourceType != DataSourceType::NO_DATA_SOURCE)
	{
		mDataProvider->deregister(this, mSubDataSourceType);
	}
	mIsUpdating = true;

	while(mElementsToRemove)
	{
		mElementsToRemove = false;
		std::deque<DataBoundElement*>::iterator eraser;
		std::deque<DataBoundElement*>::iterator remove;

		if(mListeners.size() > 0)
		{
			for(remove = mRemoveBuffer.begin(); remove != mRemoveBuffer.end(); ++remove)
			{
				for(eraser = mListeners.begin(); eraser != mListeners.end(); ++eraser)
				{
					if((*remove) == (*eraser))
					{
						mListeners.erase(eraser);
						break;
					}
				}
			}

			if(!mElementsToRemove)
				mRemoveBuffer.clear();
		}		
	}

	std::deque<DataBoundElement*>::iterator listenerIterator;

	for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end();++listenerIterator)
	{
		if((*listenerIterator))
			(*listenerIterator)->setDataProviderElement(0);
	}

	mListenerBuffer.clear();
	mListeners.clear();
	mRemoveBuffer.clear();
	mSubProviders.clear();

}

void DataProviderElement::ProcessEvent(Rocket::Core::Event& ev)
{
	Rocket::Core::Element::ProcessEvent(ev);
	if(!mInitialized)
	{
		initialize();
	}
}

void DataProviderElement::initialize()
{
	mDataSource = mDataProvider->getDataSource(this->GetAttribute<Rocket::Core::String>("datasource","could_not_find").CString());
	
	//try again later
	if(mDataSource == DataSourceType::COULD_NOT_FIND)
		return;

	mDataSourceID = this->GetAttribute<Rocket::Core::String>("id","null").CString();

	
	mInitialized = true;

	Rocket::Core::Element* parent = this->GetParentNode();
	while(parent)
	{
		if(parent->GetTagName() == "dataprovider")
		{
			mIsSubDataSource = true;
			mSubDataSourceType = mDataSource;
			mParent = dynamic_cast<DataProviderElement*>(parent);
			mParent->registerSubProvider(this);
			return;
		}
		parent = parent->GetParentNode();
	}
	mDataProvider->registerForUpdates(this);
	mDataProvider->registerForUpdates(this, mDataSource);
		
	mItemID = this->GetAttribute<int>("itemid",-1);

	std::deque<DataProviderElement*>::iterator iter;

	for(iter = mSubProviders.begin(); iter != mSubProviders.end(); ++iter)
	{
		(*iter)->setParentDataSourceType(mDataSource);
	}

	mDataProvider->initializeAllListeners();

	if(this->GetAttribute<Rocket::Core::String>("enabled","true").CString() == "false")
	{
		mEnabled = false;
		return;
	}
		

	update();
}

void DataProviderElement::dataSourceUpdate()
{
	if(!mEnabled)
	{
		mEnabled = true;
	}
	
	update();
}

void DataProviderElement::registerListener(DataBoundElement* listener)
{
	if(!mIsUpdating)
		mListeners.push_back(listener);
	else
	{
		mListenerBuffer.push_back(listener);
		mBufferModified = true;	
	}
}

void DataProviderElement::removeListener(DataBoundElement* listener)
{
	mRemoveBuffer.push_back(listener);
	mElementsToRemove = true;
}

void DataProviderElement::clearDataSource()
{
	if(!mInitialized)
		return;

	while(mElementsToRemove)
	{
		mElementsToRemove = false;
		std::deque<DataBoundElement*>::iterator eraser;
		std::deque<DataBoundElement*>::iterator remove;

		if(mListeners.size() > 0)
		{
			for(remove = mRemoveBuffer.begin(); remove != mRemoveBuffer.end(); ++remove)
			{
				for(eraser = mListeners.begin(); eraser != mListeners.end(); ++eraser)
				{
					if((*remove) == (*eraser))
					{
						mListeners.erase(eraser);
						break;
					}
				}
			}

			if(!mElementsToRemove)
				mRemoveBuffer.clear();
		}		
	}

	std::deque<DataBoundElement*>::iterator listenerIterator;

	for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end();++listenerIterator)
	{
		if((*listenerIterator))
			(*listenerIterator)->clearDataSource();
	}
}

DataProviderElement* DataProviderElement::getDataProviderElement(DataSourceType type, std::string id)
{
	if(mDataSourceID == id && mDataSource == type)
		return this;


	std::deque<DataProviderElement*>::iterator subIterator;

	for(subIterator = mSubProviders.begin(); subIterator != mSubProviders.end(); ++subIterator)
	{
		if((*subIterator)->getDataSourceType() == type && (*subIterator)->getDataSourceID() == id)
			return (*subIterator);
	}

	return 0;
}

void DataProviderElement::registerSubProvider(DataProviderElement* provider)
{
	mSubProviders.push_back(provider);
}

void DataProviderElement::setParentDataSourceType(DataSourceType type)
{
	mSubDataSourceType = mDataSource;
	mDataSource = type;

	mDataProvider->registerForUpdates(this, mDataSource);
	mDataProvider->registerForUpdates(this, mSubDataSourceType);
	mDataProvider->initializeAllListeners();

	mInitialized = true;
}

void DataProviderElement::update()
{
	updateImpl();
	if(mBufferModified)
	{
		mBufferModified = false;
		std::deque<DataBoundElement*> removeFromBuffer;

		std::deque<DataBoundElement*>::iterator listenerIterator;
		std::deque<DataBoundElement*>::iterator eraser;
		
		for(listenerIterator = mListenerBuffer.begin(); listenerIterator != mListenerBuffer.end();++listenerIterator)
		{
			mListeners.push_back((*listenerIterator));
			removeFromBuffer.push_back((*listenerIterator));
		}
		
		for(listenerIterator = removeFromBuffer.begin(); listenerIterator != removeFromBuffer.end(); ++listenerIterator)
		{
			for(eraser = mListenerBuffer.begin(); eraser != mListenerBuffer.end(); ++eraser)
			{
				if((*listenerIterator) == (*eraser))
				{
					mListenerBuffer.erase(eraser);
					break;
				}
			}
		}

		updateImpl();
	}
	
}

void DataProviderElement::updateImpl()
{
	if(!mIsUpdating)
	{
		mIsUpdating = true;
		
		std::deque<LuaScriptElement*>::iterator remove;
		std::deque<LuaScriptElement*>::iterator luaIter;
		for(remove = mLuaScriptElementsToRemove.begin(); remove != mLuaScriptElementsToRemove.end();)
		{
			bool removed(false);
			for(luaIter = mLuaScriptElements.begin(); luaIter != mLuaScriptElements.end(); ++luaIter)
			{
				if((*luaIter) == (*remove))
				{
					if((*luaIter)->GetParentNode())
					{
						mLuaScriptElements.erase(luaIter);
						//(*luaIter)->RemoveReference();
						remove = mLuaScriptElementsToRemove.erase(remove);
						removed = true;
						break;
					}
				}
			}
			//didnt find it
			if(!removed)
				remove = mLuaScriptElementsToRemove.erase(remove);
		}
		for(luaIter = mLuaScriptElements.begin(); luaIter != mLuaScriptElements.end(); ++luaIter)
		{
			(*luaIter)->databind(mItemID, mDataProvider->getDataSource(mDataSource)->getPrimaryDataSet());
		}
		while(mElementsToRemove)
		{
			mElementsToRemove = false;
			std::deque<DataBoundElement*>::iterator eraser;
			std::deque<DataBoundElement*>::iterator elemToRemove;

			if(mListeners.size() > 0)
			{
				for(elemToRemove = mRemoveBuffer.begin(); elemToRemove != mRemoveBuffer.end(); )
				{
					for(eraser = mListeners.begin(); eraser != mListeners.end(); ++eraser)
					{
						if((*elemToRemove) == (*eraser))
						{
							mListeners.erase(eraser);
							elemToRemove = mRemoveBuffer.erase(elemToRemove);
							break;
						}
					}
				}

				if(!mElementsToRemove)
					mRemoveBuffer.clear();
			}
			
		}

		std::deque<DataProviderElement*>::iterator subIterator;

		for(subIterator = mSubProviders.begin(); subIterator != mSubProviders.end(); ++subIterator)
		{
			if((*subIterator))
				(*subIterator)->update();
			else
				mSubProviders.erase(subIterator);
		}

		std::deque<DataBoundElement*>::iterator listenerIterator;

		for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end(); ++listenerIterator)
		{
			if((*listenerIterator))
				(*listenerIterator)->update();
			else
				mListeners.erase(listenerIterator);
		}

		mIsUpdating = false;
	}
}

void DataProviderElement::initializeListeners()
{
	std::deque<DataBoundElement*>::iterator listenerIterator;

	for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end(); ++listenerIterator)
	{
		(*listenerIterator)->update();
	}
}

std::string DataProviderElement::getValue(std::string key)
{
	if(!mInitialized || !mEnabled || (!mIsSubDataSource && mItemID == -1))
		return "";
	
	if(mIsSubDataSource && mItemID == -1)
	{
		return mParent->getValue(key);
	}
	else if(mIsSubDataSource)
	{
		return mParent->getSubValue(mItemID, key);
	}

	return mDataProvider->getDataSource(mDataSource)->getPrimaryDataSet()->getValue(mItemID, key);
}

std::string DataProviderElement::getValue(int id, std::string key)
{	
	if(!mInitialized || !mEnabled || id == -1)
		return "";
	
	if(mIsSubDataSource)
	{
		return mParent->getSubValue(id, key);
	}

	return mDataProvider->getDataSource(mDataSource)->getPrimaryDataSet()->getValue(id,key);
}

std::string DataProviderElement::getSubValue(int id, std::string key)
{
	if(!mInitialized || !mEnabled || id == -1)
		return "";

	return mDataProvider->getDataSource(mDataSource)->getSubDataSet(mItemID)->getValue(id, key);
}

void DataProviderElement::changeSelectedElement(int id, bool propagate)
{
	if(mInitialized)
	{
		mItemID = id;
		onSelectedElementChanged();

		std::deque<DataProviderElement*>::iterator subIterator;

		for(subIterator = mSubProviders.begin(); subIterator != mSubProviders.end(); ++subIterator)
		{
			if((*subIterator))
				(*subIterator)->onSelectedElementChanged();
		}

		if(propagate)
			GameManager::getSingletonPtr()->getActiveState()->onSelectedElementChanged(mDataSource,id);
		else
		{
			std::deque<DataBoundElement*>::iterator listenerIterator;

			for(listenerIterator = mListeners.begin(); listenerIterator != mListeners.end(); ++listenerIterator)
			{
				if((*listenerIterator)->GetTagName() == "databoundlist")
				{
					DataBoundListElement* list = dynamic_cast<DataBoundListElement*>((*listenerIterator));
					list->changeSelectedElement(id, propagate);
				}
			}
		}
	}
}

void DataProviderElement::onSelectedElementChanged()
{
	update();
}

DataSet* DataProviderElement::getDataSet()
{
	if(!mInitialized)
		return 0;
	if(mIsSubDataSource)
	{
		return mParent->getSubDataSet();
	}

	return mDataProvider->getDataSource(mDataSource)->getPrimaryDataSet();
}

DataSet* DataProviderElement::getSubDataSet()
{
	if(!mInitialized)
		return 0;

	return mDataProvider->getDataSource(mDataSource)->getSubDataSet(mItemID);
}
DataSourceType DataProviderElement::getDataSourceType()
{
	return mDataSource;
}

void DataProviderElement::setSubProviderCurrentID(DataSourceType type,std::string dataSourceID, int id)
{
	std::deque<DataProviderElement*>::iterator iter;

	for(iter = mSubProviders.begin(); iter != mSubProviders.end(); ++iter)
	{
		if((*iter)->getDataSourceID() == dataSourceID && (*iter)->getDataSourceType() == type)
		{
			return;
			(*iter)->setItemID(id);
		}
	}
}

int DataProviderElement::getItemID()
{
	return mItemID;
}

void DataProviderElement::setItemID(int id)
{
	mItemID = id;
	update();
}

bool DataProviderElement::isEnabled()
{
	return mEnabled;
}

void DataProviderElement::setEnabled(bool enabled)
{
	mEnabled = enabled;
}

bool DataProviderElement::isVisible()
{
	return mVisible;
}

void DataProviderElement::setVisible(bool visible)
{
	mVisible = visible;

	if(!mVisible)
	{
		this->SetClass("hidden", true);
	}
	else
	{
		this->SetClass("hidden", false);
	}
}

std::string DataProviderElement::getDataSourceID()
{
	return mDataSourceID;
}

void DataProviderElement::setDataSourceID(std::string id)
{
	mDataSourceID = id;
}

void DataProviderElement::buildElements()
{
	
}


void DataProviderElement::OnChildRemove(Rocket::Core::Element* child)
{
	Rocket::Core::Element::OnChildRemove(child);
	if(child == this)
	{
		mDataProvider->deregister(this);
		mDataProvider->deregister(this, mDataSource);
	}
}

bool DataProviderElement::isInitialized()
{
	return mInitialized;
}

void DataProviderElement::registerLuaScriptElement(LuaScriptElement* element)
{
	mLuaScriptElements.push_back(element);
}

void DataProviderElement::deregisterLuaScriptElement(LuaScriptElement* element)
{
	mLuaScriptElementsToRemove.push_back(element);
}
