#ifndef DataSet_h
#define DataSet_h

#include <Entities/DataProvider.h>
#include <hash_map>
#include <vector>
#include <deque>

class DataSourceUpdateListener;

class DataSetItem
{
public:
	DataSetItem() : mId(0){}
	std::hash_map<std::string, std::string> mData;
	int mId;
};

class DataSet
{
public:
	DataSet(DataSourceType type);
	~DataSet();
	
	DataSourceType getDataSource();
	void setDataSource(DataSourceType source);

	std::deque<DataSetItem*>& getData();
	void setData(std::deque<DataSetItem*>& data);
	void updateData(int id, std::string key, std::string value);
	void updateData(int id, std::hash_map<std::string, std::string>& data);

	void addRecord(int id, std::hash_map<std::string, std::string>& data);
	void addRecord(DataSetItem* item);
	DataSetItem* getNextItem();
	DataSetItem* find(int id);

	bool hasMoreItems();
	void reset();
	unsigned int numItems();

	std::string getValue(int id, std::string key);
	
private:
	std::deque<DataSetItem*>::iterator mDataIterator;
	DataSourceType mDataSource;
	std::deque<DataSetItem*> mData;

	
};
#endif