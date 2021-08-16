#ifndef DataBoundElement_h
#define DataBoundElement_h


#include <vector>
#include <Rocket/Core.h>

class DataProviderElement;

class DataSourceUpdateListener
{
	public:
		virtual void dataSourceUpdate()=0;
};

class DataBoundElement : public Rocket::Core::Element, public DataSourceUpdateListener
{
public:
	DataBoundElement(const Rocket::Core::String& tag);
	virtual ~DataBoundElement();
	void update();
	virtual void initialize(bool gotDataProviderElement = false);
	virtual void postInitialize() {}
	virtual void buildElements() = 0;
	virtual void clearDataSource() {}

	void computeItemID();

	void setItemID(int id);
	int getItemID();
	void dataSourceUpdate(){}

	virtual void OnChildRemove(Rocket::Core::Element* child);
	virtual void OnUpdate();
	
	void setDataProviderElement(DataProviderElement* element);
	DataProviderElement* getDataProviderElement();

	virtual void deregisterAll();

	bool isInitialized();
protected:
	bool mRegisteredListener;
	bool mInitialized;
	bool mInitializing;
	int mItemID;
	Rocket::Core::String mOriginalRML;
	Rocket::Core::String mOnDataUpdateLuaFunctionName;
	Rocket::Core::String mOnClickFunction;
	Rocket::Core::String mOnHoverFunction;
	Rocket::Core::String mOnBlurFunction;
	DataProviderElement* mDataProvider;
	bool mGotDataProvider;
};

#endif
