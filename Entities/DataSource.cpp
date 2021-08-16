#include <Entities/DataSource.h>
#include <Entities/DataSet.h>
#include <Entities/DataProvider.h>
#include <GUI/DataBoundElement.h>
#include <GUI/LuaScriptElement.h>
#include <hash_map>
#include <vector>
#include <string>
#include <deque>

DataSource::DataSource(DataSourceType type)
{
	mPrimaryDataSourceType = type;
	mPrimaryDataSet = new DataSet(type);
}
DataSource::DataSource(DataSourceType primary, DataSourceType sub)
{
	mPrimaryDataSourceType = primary;
	mSubDataSourceType = sub;
	mPrimaryDataSet = new DataSet(primary);
}


DataSource::~DataSource()
{
	delete mPrimaryDataSet;
	mPrimaryDataSet = 0;

	std::hash_map<int, DataSet*>::iterator iter;

	for(iter = mSubDataSets.begin(); iter != mSubDataSets.end(); ++iter)
	{
		delete (*iter).second;
		(*iter).second = 0;
	}
}

void DataSource::registerForUpdates(DataSourceUpdateListener* listener)
{
	mUpdateListeners.push_back(listener);
}

void DataSource::deregister(DataSourceUpdateListener* listener)
{
	std::deque<DataSourceUpdateListener*>::iterator eraser;

	if(mUpdateListeners.size() > 0)
	{
		for(eraser = mUpdateListeners.begin(); eraser != mUpdateListeners.end(); ++eraser)
		{
			if(listener == (*eraser))
			{
				mUpdateListeners.erase(eraser);
				return;
			}
		}
	}
}

void DataSource::updateListeners()
{
	std::deque<DataSourceUpdateListener*>::iterator iter;
	if(mPrimaryDataSourceType == DataSourceType::MCU)
	{
		int temp = 0;
	}

	for(iter = mUpdateListeners.begin(); iter != mUpdateListeners.end();++iter)
	{
		if((*iter))
			(*iter)->dataSourceUpdate();
		else
		{
			iter = mUpdateListeners.erase(iter);
		}
	}

}

DataSet* DataSource::getPrimaryDataSet()
{
	return mPrimaryDataSet;
}

void DataSource::setPrimaryDataSet(DataSet* set)
{
	if(mPrimaryDataSet)
		delete mPrimaryDataSet;

	mPrimaryDataSet = set;
}

DataSet* DataSource::getSubDataSet(int id)
{
	return mSubDataSets[id];
}

void DataSource::addSubDataSet(int id, DataSet* set)
{
	mSubDataSets[id] = set;
}

DataSourceType DataSource::getPrimaryDataSourceType()
{
	return mPrimaryDataSourceType;
}

void DataSource::setPrimaryDataSourceType(DataSourceType type)
{
	mPrimaryDataSourceType = type;
}

DataSourceType DataSource::getSubDataSourceType()
{
	return mSubDataSourceType;
}

void DataSource::setSubDataSourceType(DataSourceType type)
{
	mSubDataSourceType = type;
}

std::deque<DataSourceUpdateListener*>& DataSource::getListeners()
{
	return mUpdateListeners;
}

void DataSource::setListeners(const std::deque<DataSourceUpdateListener*>& listeners)
{
	mUpdateListeners = listeners;
}

