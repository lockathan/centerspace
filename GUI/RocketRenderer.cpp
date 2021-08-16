/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "RocketRenderer.h"
#include <Rocket/Core.h>
#include <Rocket/Core/StreamMemory.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <Entities//DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include "FileInterfaceOgre3D.h"
#include "RenderInterfaceOgre3D.h"
#include "SystemInterfaceOgre3D.h"
#include <OgreRenderWindow.h>
#include <direct.h>
#include <Ogre.h>
#include <GUI/RocketDocumentDefinition.h>
#include <GUI/RocketDocuments.h>
#include <GUI/CampaignElement.h>
#include <GUI/DataBoundCssProperty.h>
#include <GUI/DataBoundImageElement.h>
#include <GUI/DataBoundListElement.h>
#include <GUI/DataBoundListItemElement.h>
#include <GUI/DataBoundTextElement.h>
#include <GUI/DataProviderElement.h>
#include <GUI/LuaScriptElement.h>
#include <GUI/LuaRocketEventInstancer.h>
#include <GUI/WeaponButtonElement.h>
#include <LuaLibRocket/LuaElementCache.h>
#include <Managers/GameManager.h>
#include <Managers/InGameCamera.h>
#include <States/GameState.h>
#include <OgreAL.h>
#include <Repositories/SoundTemplateRepository.h>
#include <Templates/SoundTemplate.h>

using namespace Ogre;
using namespace std;

RocketRenderer::RocketRenderer()
	:
	
	mCurrentDocument(0),
	mIsVisible(true),
	mModalDocument(0),
	mMenuClickSound(0)
{

	//get render window
	//mRenderWindow = gkWindowSystem::getSingleton().getMainWindow()->getRenderWindow();
	mGameManager = GameManager::getSingletonPtr();
	mRenderWindow = mGameManager->getRenderWindow();
	mInputListener = InputListener::getSingletonPtr();
	mDataProvider = DataProvider::getSingletonPtr();
	mContext = NULL;
	mOgreSystem = NULL;
	mOgreRenderer = NULL;
	mOgreFilesystem = NULL;

	// Switch the working directory to Ogre's bin directory.
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	char path[MAX_PATH];
	mRocketPath = getcwd(path, MAX_PATH);
	mRocketPath += "/../../";

	// Normalise the path. This path is used to specify the resource location (see line 56 below).
	_fullpath(path, mRocketPath.CString(), MAX_PATH);
	mRocketPath = Rocket::Core::String(path).Replace("\\", "/");

	// The sample path is the path to the Ogre3D sample directory. All resources are loaded
	// relative to this path.
	mSamplePath = getcwd(path, MAX_PATH);
	mSamplePath += "/../Samples/basic/ogre3d/";

#endif
	mRocketPath = ".";
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRocketPath.Replace("\\", "/").CString(), "FileSystem", "Rocket");

	// Rocket initialisation.
	mOgreRenderer = new RenderInterfaceOgre3D(mRenderWindow->getWidth(), mRenderWindow->getHeight());
	Rocket::Core::SetRenderInterface(mOgreRenderer);

	mOgreSystem = new SystemInterfaceOgre3D();
	Rocket::Core::SetSystemInterface(mOgreSystem);

	mOgreFilesystem = new FileInterfaceOgre3D();
	Rocket::Core::SetFileInterface(mOgreFilesystem);
	
	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();
	Rocket::Core::Factory::RegisterEventInstancer(new LuaRocketEventInstancer());

	// Load the fonts from the path to the sample directory.
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Fonts");
		
	DataStreamListPtr otfList =  ResourceGroupManager::getSingletonPtr()->openResources("*.otf", "Fonts");
	DataStreamList::iterator iter;

	for(iter = otfList->begin(); iter != otfList->end(); ++iter)
	{
		Rocket::Core::FontDatabase::LoadFontFace((*iter)->getName().c_str());
		(*iter)->close();
		
	}
	
	DataStreamListPtr ttfList =  ResourceGroupManager::getSingletonPtr()->openResources("*.ttf", "Fonts");

	for(iter = ttfList->begin(); iter != ttfList->end(); ++iter)
	{
		Rocket::Core::FontDatabase::LoadFontFace((*iter)->getName().c_str());
		(*iter)->close();
	}

	mContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(mRenderWindow->getWidth(), mRenderWindow->getHeight()));
	
	//Rocket::Debugger::Initialise(mContext);
	//Rocket::Debugger::SetVisible(true);
	
	Rocket::Core::ElementInstancer* campaignElementInstancer = new Rocket::Core::ElementInstancerGeneric<CampaignElement>();
	Rocket::Core::Factory::RegisterElementInstancer("campaign", campaignElementInstancer);
	campaignElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* databoundListElementInstancer = new Rocket::Core::ElementInstancerGeneric<DataBoundListElement>();
	Rocket::Core::Factory::RegisterElementInstancer("databoundlist", databoundListElementInstancer);
	databoundListElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* databoundListItemElementInstancer = new Rocket::Core::ElementInstancerGeneric<DataBoundListItemElement>();
	Rocket::Core::Factory::RegisterElementInstancer("databoundlistitem", databoundListItemElementInstancer);
	databoundListItemElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* databoundTextElementInstancer = new Rocket::Core::ElementInstancerGeneric<DataBoundTextElement>();
	Rocket::Core::Factory::RegisterElementInstancer("databoundtext", databoundTextElementInstancer);
	databoundTextElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* dataProviderElementInstancer = new Rocket::Core::ElementInstancerGeneric<DataProviderElement>();
	Rocket::Core::Factory::RegisterElementInstancer("dataprovider", dataProviderElementInstancer);
	dataProviderElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* weaponButtonElementInstancer = new Rocket::Core::ElementInstancerGeneric<WeaponButtonElement>();
	Rocket::Core::Factory::RegisterElementInstancer("weaponbutton", weaponButtonElementInstancer);
	weaponButtonElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* dataBoundCssPropertyElementInstancer = new Rocket::Core::ElementInstancerGeneric<DataBoundCssProperty>();
	Rocket::Core::Factory::RegisterElementInstancer("databoundcssproperty", dataBoundCssPropertyElementInstancer);
	dataBoundCssPropertyElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* luaScriptElementInstancer = new Rocket::Core::ElementInstancerGeneric<LuaScriptElement>();
	Rocket::Core::Factory::RegisterElementInstancer("luascript", luaScriptElementInstancer);
	luaScriptElementInstancer->RemoveReference();

	Rocket::Core::ElementInstancer* imgElementInstancer = new Rocket::Core::ElementInstancerGeneric<DataBoundImageElement>();
	Rocket::Core::Factory::RegisterElementInstancer("databoundimg", imgElementInstancer);
	imgElementInstancer->RemoveReference();

	mInGameCamera = InGameCamera::getSingletonPtr();
	// Load the mouse cursor and release the caller's reference.
	//Rocket::Core::ElementDocument* cursor = context->LoadMouseCursor("../cursor.rml");
	//if (cursor)
	//	cursor->RemoveReference();
}

RocketRenderer::~RocketRenderer()
{
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();
	if(mMenuClickSound)
	{
		soundManager->destroySound(mMenuClickSound);
	}
  // Shutdown Rocket.
	mContext->RemoveReference();
	Rocket::Core::Shutdown();

	delete mOgreSystem;
	mOgreSystem = NULL;

	delete mOgreRenderer;
	mOgreRenderer = NULL;

	delete mOgreFilesystem;
	mOgreFilesystem = NULL;
}

// Called from Ogre before a queue group is rendered.
void RocketRenderer::renderQueueStarted(Ogre::uint8 queueGroupId, const std::string& invocation, bool& ROCKET_UNUSED(skipThisInvocation))
{
	if (mIsVisible && queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
	{
		mContext->Update();

		configureRenderSystem();
		mContext->Render();
	}
}

// Called from Ogre after a queue group is rendered.
void RocketRenderer::renderQueueEnded(Ogre::uint8 ROCKET_UNUSED(queueGroupId), const std::string& ROCKET_UNUSED(invocation), bool& ROCKET_UNUSED(repeatThisInvocation))
{
}

// Configures Ogre's rendering system for rendering Rocket.
void RocketRenderer::configureRenderSystem()
{
	Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();

	// Set up the projection and view matrices.
	Ogre::Matrix4 projection_matrix;
	buildProjectionMatrix(projection_matrix);
	render_system->_setProjectionMatrix(projection_matrix);
	render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

	// Disable lighting, as all of Rocket's geometry is unlit.
	render_system->setLightingEnabled(false);
	// Disable depth-buffering; all of the geometry is already depth-sorted.
	render_system->_setDepthBufferParams(false, false);
	// Rocket generates anti-clockwise geometry, so enable clockwise-culling.
	render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
	// Disable fogging.
	render_system->_setFog(Ogre::FOG_NONE);
	// Enable writing to all four channels.
	render_system->_setColourBufferWriteEnabled(true, true, true, true);
	// Unbind any vertex or fragment programs bound previously by the application.
	render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
	render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

	// Set texture settings to clamp along both axes.
	Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
	addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
	addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
	addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
	render_system->_setTextureAddressingMode(0, addressing_mode);

	// Set the texture coordinates for unit 0 to be read from unit 0.
	render_system->_setTextureCoordSet(0, 0);
	// Disable texture coordinate calculation.
	render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
	// Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
	// except in the case of scaling tiled decorators.
	render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
	// Disable texture coordinate transforms.
	render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
	// Reject pixels with an alpha of 0.
	render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
	// Disable all texture units but the first.
	render_system->_disableTextureUnitsFrom(1);

	// Enable simple alpha blending.
	render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

	// Disable depth bias.
	render_system->_setDepthBias(0, 0);
}

// Builds an OpenGL-style orthographic projection matrix.
void RocketRenderer::buildProjectionMatrix(Ogre::Matrix4& projection_matrix)
{
	float z_near = -1;
	float z_far = 1;

	projection_matrix = Ogre::Matrix4::ZERO;

	// Set up matrices.
	projection_matrix[0][0] = 2.0f / mRenderWindow->getWidth();
	projection_matrix[0][3]= -1.0000000f;
	projection_matrix[1][1]= -2.0f / mRenderWindow->getHeight();
	projection_matrix[1][3]= 1.0000000f;
	projection_matrix[2][2]= -2.0f / (z_far - z_near);
	projection_matrix[3][3]= 1.0000000f;
}


void RocketRenderer::ProcessEvent(Rocket::Core::Event& ev)
{
	/*
	if(ev.GetType() == "changeselectedelement")
	{
		int id = ev.GetTargetElement()->GetAttribute<int>("elementid",0);
		mDataProvider->getDataSource(mDataProvider->getDataSource(ev.GetTargetElement()->GetAttribute<std::string>("datasource","none")))->setCurrentId(id);
		//mDataProvider->changeSelectedPrimaryElement(mDataProvider->getDataSource(ev.GetTargetElement()->GetAttribute<std::string>("datasource","none")),
		mCurrentDocument->DispatchEvent("updatedata",Rocket::Core::XMLAttributes());
	}
	*/
	
	mActiveState->ProcessEvent(ev);
}

void RocketRenderer::unloadCurrentDocument()
{
	if(mCurrentDocument)
	{
		LuaElementCache::getSingletonPtr()->clearAllLuaElements();
		mLuaScriptElements.clear();
		mCurrentDocument->RemoveEventListener("click",this);
		mCurrentDocument->RemoveEventListener("mousedown",this);
		mCurrentDocument->RemoveEventListener("mouseup",this);
		mCurrentDocument->RemoveEventListener("mousemove",this);
		mCurrentDocument->RemoveEventListener("keydown",this);
		mCurrentDocument->RemoveEventListener("keyup",this);
		mCurrentDocument->RemoveEventListener("submit",this);
		mCurrentDocument->RemoveEventListener("show",this);
		mCurrentDocument->RemoveEventListener("updatedata",this);
		mCurrentDocument->RemoveEventListener("loadcareers",this);
		mCurrentDocument->RemoveEventListener("showeditcomponent",this);
		mCurrentDocument->RemoveEventListener("changeselectedelement",this);
		mCurrentDocument->Close();
		//mCurrentDocument->RemoveReference();
		
		mCurrentDocument->RemoveReference();
		mCurrentDocument = 0;
		//delete mCurrentDocument;
	}
}

bool RocketRenderer::loadDocument(RocketDocumentDefinition document)
{
	if(mModalDocument)
	{
		closeModalDocument();
	}

	if(mCurrentDocument && mCurrentDocumentDefinition.ID != document.ID && mCurrentDocumentDefinition.ID != "")
	{
		LuaElementCache::getSingletonPtr()->clearAllLuaElements();
		mLuaScriptElements.clear();
		mCurrentDocument->RemoveEventListener("click",this);
		mCurrentDocument->RemoveEventListener("mousedown",this);
		mCurrentDocument->RemoveEventListener("mouseup",this);
		mCurrentDocument->RemoveEventListener("mousemove",this);
		mCurrentDocument->RemoveEventListener("keydown",this);
		mCurrentDocument->RemoveEventListener("keyup",this);
		mCurrentDocument->RemoveEventListener("submit",this);
		mCurrentDocument->RemoveEventListener("show",this);
		mCurrentDocument->RemoveEventListener("updatedata",this);
		mCurrentDocument->RemoveEventListener("loadcareers",this);
		mCurrentDocument->RemoveEventListener("showeditcomponent",this);
		mCurrentDocument->RemoveEventListener("changeselectedelement",this);
		mCurrentDocument->Close();
		//mCurrentDocument->RemoveReference();
		
		mCurrentDocument->RemoveReference();
		mCurrentDocument = 0;
		//delete mCurrentDocument;
	}


	mCurrentDocumentDefinition = document;
	mCurrentDocument = mContext->LoadDocument(document.Filename);
	
	if (mCurrentDocument)
	{
		mCurrentDocument->AddEventListener("click",this);
		mCurrentDocument->AddEventListener("mousedown",this);
		mCurrentDocument->AddEventListener("mouseup",this);
		mCurrentDocument->AddEventListener("mousemove",this);
		mCurrentDocument->AddEventListener("keydown",this);
		mCurrentDocument->AddEventListener("keyup",this);

		mCurrentDocument->AddEventListener("submit",this);
		mCurrentDocument->AddEventListener("show",this);
		mCurrentDocument->AddEventListener("updatedata",this);
		mCurrentDocument->AddEventListener("loadcareers",this);
		mCurrentDocument->AddEventListener("showeditcomponent",this);
		mCurrentDocument->AddEventListener("changeselectedelement",this);

		mCurrentDocument->Show();
		
		std::deque<LuaScriptElement*>::iterator iter;
		for(iter = mLuaScriptElements.begin(); iter != mLuaScriptElements.end(); ++iter)
		{
			(*iter)->loadScript();
		}

		return true;
	}
	return false;
}

bool RocketRenderer::loadModalDocument(RocketDocumentDefinition document)
{
	GameManager::getSingletonPtr()->luaDebugReport("Loading modal document", 0);
	mModalDocumentDefinition = document;
	mModalDocument = mContext->LoadDocument(mModalDocumentDefinition.Filename);

	if(mModalDocument)
	{
		mModalDocument->AddEventListener("click",this);
		mModalDocument->AddEventListener("mousedown",this);
		mModalDocument->AddEventListener("mouseup",this);
		mModalDocument->AddEventListener("mousemove",this);
		mModalDocument->AddEventListener("keydown",this);
		mModalDocument->AddEventListener("keyup",this);

		mModalDocument->AddEventListener("submit",this);
		mModalDocument->AddEventListener("show",this);
		mModalDocument->AddEventListener("updatedata",this);
		mModalDocument->AddEventListener("loadcareers",this);
		mModalDocument->AddEventListener("showeditcomponent",this);
		mModalDocument->AddEventListener("changeselectedelement",this);

		mInGameCamera->stopAllMovement();
		mModalDocument->Show(Rocket::Core::ElementDocument::MODAL);
		return true;
	}
	return false;
}

void RocketRenderer::closeModalDocument()
{
	GameManager::getSingletonPtr()->luaDebugReport("Closing modal document", 0);
	if(mModalDocument)
	{
		mModalDocument->RemoveEventListener("click",this);
		mModalDocument->RemoveEventListener("mousedown",this);
		mModalDocument->RemoveEventListener("mouseup",this);
		mModalDocument->RemoveEventListener("mousemove",this);
		mModalDocument->RemoveEventListener("keydown",this);
		mModalDocument->RemoveEventListener("keyup",this);
		mModalDocument->RemoveEventListener("submit",this);
		mModalDocument->RemoveEventListener("show",this);
		mModalDocument->RemoveEventListener("updatedata",this);
		mModalDocument->RemoveEventListener("loadcareers",this);
		mModalDocument->RemoveEventListener("showeditcomponent",this);
		mModalDocument->RemoveEventListener("changeselectedelement",this);
		mModalDocument->Close();
		//mCurrentDocument->RemoveReference();
		
		mModalDocument->RemoveReference();
		mModalDocument = 0;
	}
}

void RocketRenderer::reloadCurrentDocument()
{
	loadDocument(mCurrentDocumentDefinition);
}

int RocketRenderer::getKeyModifierState(int mod)
{
	int rmod = 0;

	
	return rmod;
}
void RocketRenderer::mouseMoved(const OIS::MouseEvent& arg)
{
	Rocket::Core::Element* hover = mContext->GetHoverElement();
	if(hover)
	{
		const Rocket::Core::Property* prop = hover->GetProperty("background-color");
		Rocket::Core::String value = prop->ToString();
		if(value == "transparent")
		{
			int temp = 0;
		}
	}
	if(hover && hover->GetAttribute<Rocket::Core::String>("isbackground","false") == "true")
	{
		mActiveState->mouseMoved(arg);
		mInGameCamera->mouseMoved(arg);
		//hover->RemoveReference();
	}
	else
	{
		mInGameCamera->stopAllMovement();
	}

	mContext->ProcessMouseMove(arg.state.X.abs, arg.state.Y.abs, mInputListener->getKeyModifier());
}

void RocketRenderer::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	Rocket::Core::Element* hover = mContext->GetHoverElement();
	if(hover)
	{
		//const Rocket::Core::Property* backgroundColor = hover->GetProperty("background-color");
		//Rocket::Core::String bgColor = backgroundColor->ToString();

		//const Rocket::Core::Property* backgroundImage = hover->GetProperty("background-image-src");
		//Rocket::Core::String bgImage = backgroundColor->ToString();

		if(hover->GetAttribute<Rocket::Core::String>("isbackground","false") == "true")
		{
			mActiveState->mousePressed(arg,id);
			mInGameCamera->mousePressed(arg, id);
			//hover->RemoveReference();
		}

	}

	mContext->ProcessMouseButtonDown(id, mInputListener->getKeyModifier());
}

void RocketRenderer::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	Rocket::Core::Element* hover = mContext->GetHoverElement();

	if(hover && hover->GetAttribute<Rocket::Core::String>("isbackground","false") == "true")
	{
		mActiveState->mouseReleased(arg,id);
		mInGameCamera->mouseReleased(arg, id);
		//hover->RemoveReference();
	}

	mContext->ProcessMouseButtonUp(id, mInputListener->getKeyModifier());
}

void RocketRenderer::keyPressed(const OIS::KeyEvent& arg)
{
	bool processed = false;
	Rocket::Core::Input::KeyIdentifier key_identifier = (Rocket::Core::Input::KeyIdentifier)getRocketKeyCode(arg.key);
	if (key_identifier != Rocket::Core::Input::KI_UNKNOWN)
		processed = mContext->ProcessKeyDown(key_identifier, getKeyModifierState(mInputListener->getKeyModifier()));

	// Send through the ASCII value as text input if it is printable.
	if (arg.text >= 32)
		processed = mContext->ProcessTextInput((Rocket::Core::word)arg.text);
	else if (key_identifier == Rocket::Core::Input::KI_RETURN)
		processed = mContext->ProcessTextInput((Rocket::Core::word)'\n');

	//if(!processed)
	//{
		mActiveState->keyPressed(arg);
		mInGameCamera->keyPressed(arg);
	//}
}

void RocketRenderer::keyReleased(const OIS::KeyEvent& arg)
{
	bool processed = false;
	processed = mContext->ProcessKeyUp((Rocket::Core::Input::KeyIdentifier)getRocketKeyCode(arg.key),mInputListener->getKeyModifier());
	
	//if(!processed)
	//{
		mActiveState->keyReleased(arg);
		mInGameCamera->keyReleased(arg);
	//}
}

GameState* RocketRenderer::getActiveState()
{
	return mActiveState;
}

void RocketRenderer::setActiveState(GameState* state)
{
	mActiveState = state;
}

void RocketRenderer::registerLuaScriptElement(LuaScriptElement* element)
{
	mLuaScriptElements.push_back(element);
}

Rocket::Core::ElementDocument* RocketRenderer::getDocument(RocketDocumentDefinition document)
{
	Rocket::Core::ElementDocument* test = mContext->GetDocument(document.ID);
	return mContext->GetDocument(document.ID);
}

int RocketRenderer::getRocketKeyCode(int key)
{
	
#define CASE(c, ret) case (c): return ret
	using namespace Rocket::Core::Input;
	switch (key)
	{
		CASE(OIS::KC_GRAVE			,    KI_OEM_3);
		CASE(OIS::KC_0				,    KI_1);
		CASE(OIS::KC_1				,    KI_2);
		CASE(OIS::KC_2				,    KI_3);
		CASE(OIS::KC_3				,    KI_4);
		CASE(OIS::KC_4				,    KI_5);
		CASE(OIS::KC_5				,    KI_6);
		CASE(OIS::KC_6				,    KI_7);
		CASE(OIS::KC_7				,    KI_8);
		CASE(OIS::KC_8				,    KI_9);
		CASE(OIS::KC_9				,    KI_0);
		CASE(OIS::KC_NUMPAD0        ,    KI_NUMPAD0);
		CASE(OIS::KC_NUMPAD1        ,    KI_NUMPAD1);
		CASE(OIS::KC_NUMPAD2        ,    KI_NUMPAD2);
		CASE(OIS::KC_NUMPAD3        ,    KI_NUMPAD3);
		CASE(OIS::KC_NUMPAD4        ,    KI_NUMPAD4);
		CASE(OIS::KC_NUMPAD5        ,    KI_NUMPAD5);
		CASE(OIS::KC_NUMPAD6        ,    KI_NUMPAD6);
		CASE(OIS::KC_NUMPAD7        ,    KI_NUMPAD7);
		CASE(OIS::KC_NUMPAD8        ,    KI_NUMPAD8);
		CASE(OIS::KC_NUMPAD9        ,    KI_NUMPAD9);
		CASE(OIS::KC_DIVIDE			,    KI_DIVIDE);
		CASE(OIS::KC_NUMPADENTER    ,    KI_NUMPADENTER);
		CASE(OIS::KC_NUMPADEQUALS   ,    KI_OEM_NEC_EQUAL);
		CASE(OIS::KC_DECIMAL        ,    KI_DECIMAL);
		CASE(OIS::KC_MULTIPLY		,    KI_MULTIPLY);
		CASE(OIS::KC_ADD			,    KI_ADD);
		CASE(OIS::KC_A				,    KI_A);
		CASE(OIS::KC_B				,    KI_B);
		CASE(OIS::KC_C				,    KI_C);
		CASE(OIS::KC_D				,    KI_D);
		CASE(OIS::KC_E				,    KI_E);
		CASE(OIS::KC_F				,    KI_F);
		CASE(OIS::KC_G				,    KI_G);
		CASE(OIS::KC_H				,    KI_H);
		CASE(OIS::KC_I				,    KI_I);
		CASE(OIS::KC_J				,    KI_J);
		CASE(OIS::KC_K				,    KI_K);
		CASE(OIS::KC_L				,    KI_L);
		CASE(OIS::KC_M				,    KI_M);
		CASE(OIS::KC_N				,    KI_N);
		CASE(OIS::KC_O				,    KI_O);
		CASE(OIS::KC_P				,    KI_P);
		CASE(OIS::KC_Q				,    KI_Q);
		CASE(OIS::KC_R				,    KI_R);
		CASE(OIS::KC_S				,    KI_S);
		CASE(OIS::KC_T				,    KI_T);
		CASE(OIS::KC_U				,    KI_U);
		CASE(OIS::KC_V				,    KI_V);
		CASE(OIS::KC_W				,    KI_W);
		CASE(OIS::KC_X				,    KI_X);
		CASE(OIS::KC_Y				,    KI_Y);
		CASE(OIS::KC_Z				,    KI_Z);
		CASE(OIS::KC_CAPITAL		,    KI_CAPITAL);
		CASE(OIS::KC_LCONTROL		,    KI_LCONTROL);
		CASE(OIS::KC_LMENU			,	 KI_LMENU);
		CASE(OIS::KC_RMENU			,    KI_RMENU);
		CASE(OIS::KC_RCONTROL		,    KI_RCONTROL);
		CASE(OIS::KC_RSHIFT			,    KI_RSHIFT);
		CASE(OIS::KC_LSHIFT			,    KI_LSHIFT);
		CASE(OIS::KC_ESCAPE         ,    KI_ESCAPE);
		CASE(OIS::KC_TAB			,    KI_TAB);
		CASE(OIS::KC_RETURN         ,    KI_RETURN);
		CASE(OIS::KC_SPACE			,    KI_SPACE);
		CASE(OIS::KC_BACK			,    KI_BACK);
		CASE(OIS::KC_DELETE         ,    KI_DELETE);
		CASE(OIS::KC_SEMICOLON		,    KI_OEM_1);
		CASE(OIS::KC_PERIOD			,    KI_OEM_PERIOD);
		CASE(OIS::KC_COMMA			,    KI_OEM_COMMA);
		CASE(OIS::KC_APOSTROPHE     ,    KI_OEM_7);
		CASE(OIS::KC_MINUS			,    KI_OEM_MINUS);
		CASE(OIS::KC_SLASH			,    KI_OEM_2);
		CASE(OIS::KC_BACKSLASH		,    KI_OEM_5);
		CASE(OIS::KC_EQUALS			,    KI_OEM_PLUS);
		CASE(OIS::KC_LEFT			,    KI_LEFT);
		CASE(OIS::KC_DOWN			,    KI_DOWN);
		CASE(OIS::KC_RIGHT			,    KI_RIGHT);
		CASE(OIS::KC_UP				,    KI_UP);
		CASE(OIS::KC_F1				,    KI_F1);
		CASE(OIS::KC_F2				,    KI_F2);
		CASE(OIS::KC_F3				,    KI_F3);
		CASE(OIS::KC_F4				,    KI_F4);
		CASE(OIS::KC_F5				,    KI_F5);
		CASE(OIS::KC_F6				,    KI_F6);
		CASE(OIS::KC_F7				,    KI_F7);
		CASE(OIS::KC_F8				,    KI_F8);
		CASE(OIS::KC_F9				,    KI_F9);
		CASE(OIS::KC_F10			,    KI_F10);
		CASE(OIS::KC_F11			,    KI_F11);
		CASE(OIS::KC_F12			,    KI_F12);
		CASE(OIS::KC_HOME			,    KI_HOME);
		CASE(OIS::KC_PGUP			,    KI_PRIOR);
		CASE(OIS::KC_PGDOWN			,    KI_NEXT);
		CASE(OIS::KC_END			,    KI_END);
		CASE(OIS::KC_INSERT			,    KI_INSERT);
		CASE(OIS::KC_PAUSE			,    KI_PAUSE);
		
		default: break;
	}
	return KI_UNKNOWN;

#undef CASE
}

int RocketRenderer::getOISKeyCode(int key)
{
	
#define CASE(c, ret) case (ret): return c
	using namespace Rocket::Core::Input;
	switch (key)
	{
		CASE(OIS::KC_GRAVE			,    KI_OEM_3);
		CASE(OIS::KC_0				,    KI_1);
		CASE(OIS::KC_1				,    KI_2);
		CASE(OIS::KC_2				,    KI_3);
		CASE(OIS::KC_3				,    KI_4);
		CASE(OIS::KC_4				,    KI_5);
		CASE(OIS::KC_5				,    KI_6);
		CASE(OIS::KC_6				,    KI_7);
		CASE(OIS::KC_7				,    KI_8);
		CASE(OIS::KC_8				,    KI_9);
		CASE(OIS::KC_9				,    KI_0);
		CASE(OIS::KC_NUMPAD0        ,    KI_NUMPAD0);
		CASE(OIS::KC_NUMPAD1        ,    KI_NUMPAD1);
		CASE(OIS::KC_NUMPAD2        ,    KI_NUMPAD2);
		CASE(OIS::KC_NUMPAD3        ,    KI_NUMPAD3);
		CASE(OIS::KC_NUMPAD4        ,    KI_NUMPAD4);
		CASE(OIS::KC_NUMPAD5        ,    KI_NUMPAD5);
		CASE(OIS::KC_NUMPAD6        ,    KI_NUMPAD6);
		CASE(OIS::KC_NUMPAD7        ,    KI_NUMPAD7);
		CASE(OIS::KC_NUMPAD8        ,    KI_NUMPAD8);
		CASE(OIS::KC_NUMPAD9        ,    KI_NUMPAD9);
		CASE(OIS::KC_DIVIDE			,    KI_DIVIDE);
		CASE(OIS::KC_NUMPADENTER    ,    KI_NUMPADENTER);
		CASE(OIS::KC_NUMPADEQUALS   ,    KI_OEM_NEC_EQUAL);
		CASE(OIS::KC_DECIMAL        ,    KI_DECIMAL);
		CASE(OIS::KC_MULTIPLY		,    KI_MULTIPLY);
		CASE(OIS::KC_ADD			,    KI_ADD);
		CASE(OIS::KC_A				,    KI_A);
		CASE(OIS::KC_B				,    KI_B);
		CASE(OIS::KC_C				,    KI_C);
		CASE(OIS::KC_D				,    KI_D);
		CASE(OIS::KC_E				,    KI_E);
		CASE(OIS::KC_F				,    KI_F);
		CASE(OIS::KC_G				,    KI_G);
		CASE(OIS::KC_H				,    KI_H);
		CASE(OIS::KC_I				,    KI_I);
		CASE(OIS::KC_J				,    KI_J);
		CASE(OIS::KC_K				,    KI_K);
		CASE(OIS::KC_L				,    KI_L);
		CASE(OIS::KC_M				,    KI_M);
		CASE(OIS::KC_N				,    KI_N);
		CASE(OIS::KC_O				,    KI_O);
		CASE(OIS::KC_P				,    KI_P);
		CASE(OIS::KC_Q				,    KI_Q);
		CASE(OIS::KC_R				,    KI_R);
		CASE(OIS::KC_S				,    KI_S);
		CASE(OIS::KC_T				,    KI_T);
		CASE(OIS::KC_U				,    KI_U);
		CASE(OIS::KC_V				,    KI_V);
		CASE(OIS::KC_W				,    KI_W);
		CASE(OIS::KC_X				,    KI_X);
		CASE(OIS::KC_Y				,    KI_Y);
		CASE(OIS::KC_Z				,    KI_Z);
		CASE(OIS::KC_CAPITAL		,    KI_CAPITAL);
		CASE(OIS::KC_LCONTROL		,    KI_LCONTROL);
		CASE(OIS::KC_LMENU			,	 KI_LMENU);
		CASE(OIS::KC_RMENU			,    KI_RMENU);
		CASE(OIS::KC_RCONTROL		,    KI_RCONTROL);
		CASE(OIS::KC_RSHIFT			,    KI_RSHIFT);
		CASE(OIS::KC_LSHIFT			,    KI_LSHIFT);
		CASE(OIS::KC_ESCAPE         ,    KI_ESCAPE);
		CASE(OIS::KC_TAB			,    KI_TAB);
		CASE(OIS::KC_RETURN         ,    KI_RETURN);
		CASE(OIS::KC_SPACE			,    KI_SPACE);
		CASE(OIS::KC_BACK			,    KI_BACK);
		CASE(OIS::KC_DELETE         ,    KI_DELETE);
		CASE(OIS::KC_SEMICOLON		,    KI_OEM_1);
		CASE(OIS::KC_PERIOD			,    KI_OEM_PERIOD);
		CASE(OIS::KC_COMMA			,    KI_OEM_COMMA);
		CASE(OIS::KC_APOSTROPHE     ,    KI_OEM_7);
		CASE(OIS::KC_MINUS			,    KI_OEM_MINUS);
		CASE(OIS::KC_SLASH			,    KI_OEM_2);
		CASE(OIS::KC_BACKSLASH		,    KI_OEM_5);
		CASE(OIS::KC_EQUALS			,    KI_OEM_PLUS);
		CASE(OIS::KC_LEFT			,    KI_LEFT);
		CASE(OIS::KC_DOWN			,    KI_DOWN);
		CASE(OIS::KC_RIGHT			,    KI_RIGHT);
		CASE(OIS::KC_UP				,    KI_UP);
		CASE(OIS::KC_F1				,    KI_F1);
		CASE(OIS::KC_F2				,    KI_F2);
		CASE(OIS::KC_F3				,    KI_F3);
		CASE(OIS::KC_F4				,    KI_F4);
		CASE(OIS::KC_F5				,    KI_F5);
		CASE(OIS::KC_F6				,    KI_F6);
		CASE(OIS::KC_F7				,    KI_F7);
		CASE(OIS::KC_F8				,    KI_F8);
		CASE(OIS::KC_F9				,    KI_F9);
		CASE(OIS::KC_F10			,    KI_F10);
		CASE(OIS::KC_F11			,    KI_F11);
		CASE(OIS::KC_F12			,    KI_F12);
		CASE(OIS::KC_HOME			,    KI_HOME);
		CASE(OIS::KC_PGUP			,    KI_PRIOR);
		CASE(OIS::KC_PGDOWN			,    KI_NEXT);
		CASE(OIS::KC_END			,    KI_END);
		CASE(OIS::KC_INSERT			,    KI_INSERT);
		CASE(OIS::KC_PAUSE			,    KI_PAUSE);
		
		default: break;
	}
	return KI_UNKNOWN;

#undef CASE
}


void RocketRenderer::toggleVisibility(bool state)
{
	mIsVisible = state;
}

bool RocketRenderer::isVisible()
{
	return mIsVisible;
}

void RocketRenderer::initMenuSound()
{
	OgreAL::SoundManager* soundManager = GameManager::getSingletonPtr()->getSoundManager();
	SoundTemplate* clickSoundTemplate = SoundTemplateRepository::getSingletonPtr()->findByName("click");
	if(clickSoundTemplate)
	{
		mMenuClickSound = soundManager->createSound(clickSoundTemplate->getName(), clickSoundTemplate->getFileName(), clickSoundTemplate->loop());
	}
}

OgreAL::Sound* RocketRenderer::getMenuClickSound()
{
	return mMenuClickSound;
}

RocketRenderer* RocketRenderer::mRocketRenderer;

RocketRenderer* RocketRenderer::getSingletonPtr() 
{
	if( !mRocketRenderer ) 
	{
		mRocketRenderer = new RocketRenderer();
	}

	return mRocketRenderer;
}