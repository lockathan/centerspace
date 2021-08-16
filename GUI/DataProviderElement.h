#ifndef DataProviderElement_h
#define DataProviderElement_h

#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <Entities/DataProvider.h>
#include <GUI/DataBoundElement.h>
#include <vector>
#include <deque>

class DataProvider;

class DataProviderElement : public Rocket::Core::Element, public DataSourceUpdateListener
{
public:
	DataProviderElement(const Rocket::Core::String& tag);
	~DataProviderElement();

	void initialize();
	void registerListener(DataBoundElement* listener);
	void removeListener(DataBoundElement* listener);
	void clearDataSource();
	DataProviderElement* getDataProviderElement(DataSourceType type, std::string id);

	void update();
	void initializeListeners();

	std::string getValue(std::string key);
	std::string getValue(int id, std::string key);

	std::string getSubValue(int id, std::string key);

	void changeSelectedElement(int id, bool propagate = true);
	void onSelectedElementChanged();


	DataSet* getDataSet();
	DataSet* getSubDataSet();

	virtual void ProcessEvent(Rocket::Core::Event& ev);
	virtual void OnChildRemove(Rocket::Core::Element* child);

	void registerSubProvider(DataProviderElement* provider);
	void setParentDataSourceType(DataSourceType type);

	void dataSourceUpdate();
	
	DataSourceType getDataSourceType();
	
	void setSubProviderCurrentID(DataSourceType type,std::string dataSourceID, int id);

	int getItemID();
	void setItemID(int id);

	bool isEnabled();
	void setEnabled(bool enabled);

	bool isVisible();
	void setVisible(bool visible);

	std::string getDataSourceID();
	void setDataSourceID(std::string id);

	bool isInitialized();
	
	void registerLuaScriptElement(LuaScriptElement* element);
	void deregisterLuaScriptElement(LuaScriptElement* element);

protected:
	int mSequence;
	bool mVisible;
	std::deque<DataBoundElement*> mListeners;
	std::deque<DataBoundElement*> mListenerBuffer;
	std::deque<DataBoundElement*> mRemoveBuffer;

	void updateImpl();

	bool mIsUpdating;
	bool mBufferModified;
	bool mElementsToRemove;

	std::deque<DataProviderElement*> mSubProviders;

	DataSourceType mDataSource;
	DataSourceType mSubDataSourceType;
	int mItemID;

	DataProvider* mDataProvider;
	DataProviderElement* mParent;

	bool mRegisteredListener;
	bool mInitialized;
	bool mEnabled;
	bool mIsSubDataSource;

	std::string mDataSourceID;
	void buildElements();
	
	std::deque<LuaScriptElement*> mLuaScriptElements;
	std::deque<LuaScriptElement*> mLuaScriptElementsToRemove;

};

#endif
