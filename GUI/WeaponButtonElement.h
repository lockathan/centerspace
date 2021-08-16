#ifndef WeaponButtonElement_h
#define WeaponButtonElement_h


#include <GUI/DataBoundElement.h>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <vector>

class MCUController;
class WeaponHardpoint;

class WeaponButtonElement : public DataBoundElement
{
public:
	WeaponButtonElement(const Rocket::Core::String& tag);
	~WeaponButtonElement();

	virtual void ProcessEvent(Rocket::Core::Event& ev);

	MCUController* getMCUController();
	void setMCUController(MCUController* controller);

	WeaponHardpoint* getWeaponHardpoint();
	void setWeaponHardpoint(WeaponHardpoint* weapon);

protected:
	void buildElements();
	MCUController* mMCUController;
	WeaponHardpoint* mWeaponHardpoint;
};

#endif
