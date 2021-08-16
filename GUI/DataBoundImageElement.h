#ifndef DataBoundImageElement_h
#define DataBoundImageElement_h

#include <GUI/DataBoundElement.h>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <vector>


class DataBoundImageElement : public DataBoundElement
{
public:
	DataBoundImageElement(const Rocket::Core::String& tag);
	~DataBoundImageElement();

	void ProcessEvent(Rocket::Core::Event& ev);
	void initialize(bool gotDataProviderElement = false);
	
protected:
	void buildElements();
	Rocket::Core::String mImageFolder;
	Rocket::Core::String mCssClass;
};

#endif
