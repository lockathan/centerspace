#include <GUI/LuaScriptElement.h>
#include <GUI/RocketRenderer.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Managers/LuaStateManager.h>
#include <States/GameState.h>
#include <Rocket/Core.h>

LuaScriptElement::LuaScriptElement(const Rocket::Core::String& tag)
	:
	Rocket::Core::Element(tag),
	mScriptName(""),
	mDataBindFunctionName(""),
	mLoadedScript(false)
{
	RocketRenderer::getSingletonPtr()->registerLuaScriptElement(this);
}

LuaScriptElement::~LuaScriptElement()
{
	DataProvider::getSingletonPtr()->deregisterLuaScriptElement(this);
}

void LuaScriptElement::ProcessEvent(Rocket::Core::Event& ev)
{
	if(ev.GetType() == "show")
		this->loadScript();
}

void LuaScriptElement::loadScript()
{
	if(!mLoadedScript)
	{
		mLoadedScript = true;
		this->GetInnerRML(mScriptName);
		mDataBindFunctionName = this->GetAttribute<Rocket::Core::String>("ondatabind", "");
		LuaStateManager::getSingletonPtr()->loadScript(RocketRenderer::getSingletonPtr()->getActiveState(),  mScriptName.CString());
		DataProvider::getSingletonPtr()->registerLuaScriptElement(this);
		this->SetInnerRML("");
	}
}


void LuaScriptElement::databind(int id, DataSet* dataset)
{
	if(mLoadedScript && mDataBindFunctionName != "")
	{
		LuaStateManager::getSingletonPtr()->executeDataBindFunction(RocketRenderer::getSingletonPtr()->getActiveState(), mDataBindFunctionName.CString(), dataset, id, this);
	}
}
