#ifndef DataSource_h
#define DataSource_h

#include <Entities/DataProvider.h>
#include <hash_map>
#include <vector>
#include <deque>
#include "Enums.h"

class DataSet;
class DataSourceUpdateListener;
class LuaScriptElement;

class DataSource
{
public:
	DataSource(DataSourceType type);
	DataSource(DataSourceType primary, DataSourceType sub);

	~DataSource();
	
	DataSet* getPrimaryDataSet();
	void setPrimaryDataSet(DataSet* set);

	DataSet* getSubDataSet(int id);
	void addSubDataSet(int id, DataSet* set);
	
	DataSourceType getPrimaryDataSourceType();
	void setPrimaryDataSourceType(DataSourceType type);

	DataSourceType getSubDataSourceType();
	void setSubDataSourceType(DataSourceType type);

	void registerForUpdates(DataSourceUpdateListener* listener);
	void deregister(DataSourceUpdateListener* listener);

	void updateListeners();

	std::deque<DataSourceUpdateListener*>& getListeners();
	void setListeners(const std::deque<DataSourceUpdateListener*>& listeners);
	
protected:
	DataSet* mPrimaryDataSet;
	std::deque<DataSourceUpdateListener*> mUpdateListeners;
	std::hash_map<int,DataSet*> mSubDataSets;
	DataSourceType mPrimaryDataSourceType;
	DataSourceType mSubDataSourceType;
};
#endif