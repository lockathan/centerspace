#include <GUI/LuaRocketEventInstancer.h>
#include <Managers/GameManager.h>
#include <Managers/LuaStateManager.h>
#include <Rocket/Core.h>
#include <GUI/RocketRenderer.h>
#include <Repositories/SoundTemplateRepository.h>
#include <States/GameState.h>
#include <Templates/SoundTemplate.h>
#include <OgreAL.h>

LuaRocketEventInstancer::LuaRocketEventInstancer()
{

}

LuaRocketEventInstancer::~LuaRocketEventInstancer()
{

}

Rocket::Core::Event* LuaRocketEventInstancer::InstanceEvent(Rocket::Core::Element* target, const Rocket::Core::String& name, const Rocket::Core::Dictionary& parameters, bool interruptible)
{
	Rocket::Core::String luaFunctionName;

	if(target)
	{
		if(name == "click")
		{
			luaFunctionName = target->GetAttribute<Rocket::Core::String>("onclick", "");
			if(luaFunctionName != "")
			{
				OgreAL::Sound* menuClickSound = RocketRenderer::getSingletonPtr()->getMenuClickSound();
				if(menuClickSound)
				{
					menuClickSound->play();
				}
				LuaStateManager::getSingletonPtr()->executeGuiFunction(RocketRenderer::getSingletonPtr()->getActiveState(), luaFunctionName.CString(), target);
			}
		}
		else if(name == "hover")
		{
			luaFunctionName = target->GetAttribute<Rocket::Core::String>("onhover", "");
			if(luaFunctionName != "")
			{
				LuaStateManager::getSingletonPtr()->executeGuiFunction(RocketRenderer::getSingletonPtr()->getActiveState(), luaFunctionName.CString(), target);
			}
		}
		else if(name == "blur")
		{
			luaFunctionName = target->GetAttribute<Rocket::Core::String>("onblur", "");
			if(luaFunctionName != "")
			{
				LuaStateManager::getSingletonPtr()->executeGuiFunction(RocketRenderer::getSingletonPtr()->getActiveState(), luaFunctionName.CString(), target);
			}
		}
	}

	return new Rocket::Core::Event(target, name, parameters, interruptible);
}

void LuaRocketEventInstancer::ReleaseEvent(Rocket::Core::Event* e)
{
	delete e;
}

void LuaRocketEventInstancer::Release()
{
	delete this;
}

