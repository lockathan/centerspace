#ifndef DataBoundCssProperty_h
#define DataBoundCssProperty_h


#include <GUI/DataBoundElement.h>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <vector>

class DataBoundCssProperty : public DataBoundElement
{
public:
	DataBoundCssProperty(const Rocket::Core::String& tag);
	~DataBoundCssProperty();

	void initialize(bool gotDataProviderElement = false);
	
	virtual void ProcessEvent(Rocket::Core::Event& ev);
protected:
	void buildElements();
	Rocket::Core::String mCssPropertyName;
	float mMinValue;
	float mMaxValue;
	Rocket::Core::String mValueField;
	Rocket::Core::String mMaxValueField;
	Rocket::Core::String mUnit;

};

#endif
