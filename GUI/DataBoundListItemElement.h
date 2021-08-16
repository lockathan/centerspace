#ifndef DataBoundListItemElement_h
#define DataBoundListItemElement_h

#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <vector>

#include <GUI/DataBoundElement.h>

class DataBoundListItemElement : public DataBoundElement
{
public:
	DataBoundListItemElement(const Rocket::Core::String& tag);
	~DataBoundListItemElement();

	
	
	void registerListener(DataBoundElement* listener);
	void deregisterListener(DataBoundElement* listener);

	void pushItemID();

	void initialize();

	void select();
	void deselect();

	virtual void ProcessEvent(Rocket::Core::Event& ev);
	
	virtual void deregisterAll();
protected:
	void buildElements();
	std::vector<DataBoundElement*> mListeners;

};

#endif
