#ifndef DataBoundTextElement_h
#define DataBoundTextElement_h


#include <GUI/DataBoundElement.h>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <vector>

class DataBoundTextElement : public DataBoundElement
{
public:
	DataBoundTextElement(const Rocket::Core::String& tag);
	~DataBoundTextElement();

	virtual void ProcessEvent(Rocket::Core::Event& ev);
protected:
	void buildElements();

};

#endif
