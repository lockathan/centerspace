#include <Entities/DataSet.h>
#include <Entities/DataProvider.h>
#include <GUI/DataBoundElement.h>
#include <hash_map>
#include <vector>
#include <string>
#include <deque>

DataSet::DataSet(DataSourceType type)
{
	mDataSource = type;
	mDataIterator = mData.begin();
}

DataSet::~DataSet()
{
	for(unsigned int i = 0; i < mData.size();++i)
	{
		delete mData[i];
		mData[i] = 0;
	}
}

DataSourceType DataSet::getDataSource()
{
	return mDataSource;
}

void DataSet::setDataSource(DataSourceType source)
{
	mDataSource = source;
	mDataIterator = mData.begin();
}

std::deque<DataSetItem*>& DataSet::getData()
{
	return mData;
}

void DataSet::setData(std::deque<DataSetItem*>& data)
{
	mData = data;
	mDataIterator = mData.begin();
	DataProvider::getSingletonPtr()->updateDataSourceListeners(mDataSource);
}

void DataSet::updateData(int id, std::string key, std::string value)
{
	DataSetItem* result = find(id);

	if(result)
	{
		result->mData[key] = value;
	}
}

void DataSet::updateData(int id, std::hash_map<std::string, std::string>& data)
{
	DataSetItem* result = find(id);

	if(result)
	{
		result->mData = data;
	}
	else
	{
		addRecord(id, data);
	}
}

void DataSet::addRecord(int id, std::hash_map<std::string, std::string>& data)
{
	DataSetItem* item = new DataSetItem();
	item->mId = id;
	item->mData = data;
	mData.push_back(item);
	mDataIterator = mData.begin();
}

void DataSet::addRecord(DataSetItem* item)
{
	mData.push_back(item);
}

std::string DataSet::getValue(int id, std::string key)
{
	DataSetItem* result = find(id);

	if(result)
	{
		return result->mData[key];
	}

	return "";
}

DataSetItem* DataSet::getNextItem()
{
	DataSetItem* result = (*mDataIterator);
	++mDataIterator;
	return result;
}

DataSetItem* DataSet::find(int id)
{
	std::deque<DataSetItem*>::iterator iter;

	for(iter = mData.begin(); iter != mData.end(); ++iter)
	{
		if((*iter)->mId == id)
			return (*iter);
	}

	return 0;
}

bool DataSet::hasMoreItems()
{
	return mDataIterator != mData.end();
}

void DataSet::reset()
{
	mDataIterator = mData.begin();
}

unsigned int DataSet::numItems()
{
	return mData.size();
}
