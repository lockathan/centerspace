#ifndef DataBoundListElement_h
#define DataBoundListElement_h

#include <GUI/DataBoundElement.h>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <vector>

class DataBoundListItemElement;

class DataBoundListElement : public DataBoundElement
{
public:
	DataBoundListElement(const Rocket::Core::String& tag);
	~DataBoundListElement();

	void initialize(bool gotDataProviderElement = false);
	void clearDataSource();
	virtual void ProcessEvent(Rocket::Core::Event& ev);

	void changeSelectedElement(int id, bool propagate = true);
	
	virtual void deregisterAll();
protected:
	int mSelectedElement;
	bool mUseSelectBehaviour;
	void buildElements();
	std::vector<DataBoundListItemElement*> mListItems;
};

#endif
