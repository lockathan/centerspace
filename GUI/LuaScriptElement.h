#ifndef LuaScriptElement_h
#define LuaScriptElement_h

class DataSet;

#include <Rocket/Core.h>

class LuaScriptElement : public Rocket::Core::Element
{
public:
	LuaScriptElement(const Rocket::Core::String& tag);
	~LuaScriptElement();

	void ProcessEvent(Rocket::Core::Event& ev);

	void loadScript();
	void databind(int id, DataSet* dataset);
private:
	Rocket::Core::String mScriptName;
	Rocket::Core::String mDataBindFunctionName;
	bool mLoadedScript;
};

#endif
