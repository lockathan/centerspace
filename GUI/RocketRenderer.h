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

#ifndef ROCKETAPPLICATION_H
#define ROCKETAPPLICATION_H

#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Ogre.h>
#include <OgrePlatform.h>
#include <Input/InputListener.h>
#include <GUI/RocketDocumentDefinition.h>

#include <OgreAL.h>

class DataProvider;
class FileInterfaceOgre3D;
class GameManager;
class GameState;
class InGameCamera;
class LuaScriptElement;

namespace Rocket {
namespace Core {

class Context;

}
}

class SystemInterfaceOgre3D;
class RenderInterfaceOgre3D;

class RocketRenderer : 
	public Ogre::RenderQueueListener,
	public Rocket::Core::EventListener,
	public InputListener::Listener
{
public:
	RocketRenderer();
	virtual ~RocketRenderer();

	void unloadCurrentDocument();
	bool loadDocument(RocketDocumentDefinition document);
	bool loadModalDocument(RocketDocumentDefinition document);
	void closeModalDocument();
	void reloadCurrentDocument();

	virtual void ProcessEvent(Rocket::Core::Event& event);
	
	virtual void mouseMoved(const OIS::MouseEvent& arg);
	virtual void mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual void keyPressed(const OIS::KeyEvent& arg);
	virtual void keyReleased(const OIS::KeyEvent& arg);

	Rocket::Core::ElementDocument* getDocument(RocketDocumentDefinition document);

	int getKeyModifierState(int mod);

	void toggleVisibility(bool state);
	bool isVisible();

	GameState* getActiveState();
	void setActiveState(GameState* state);

	void registerLuaScriptElement(LuaScriptElement* element);
	void initMenuSound();

	OgreAL::Sound* getMenuClickSound();

	static RocketRenderer* getSingletonPtr();

protected:


	/// Called from Ogre before a queue group is rendered.
	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const std::string& invocation, bool& skipThisInvocation);
	/// Called from Ogre after a queue group is rendered.
	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const std::string& invocation, bool& repeatThisInvocation);

private:
	
	OgreAL::Sound* mMenuClickSound;
	// Configures Ogre's rendering system for rendering Rocket.
	void configureRenderSystem();
	// Builds an OpenGL-style orthographic projection matrix.
	void buildProjectionMatrix(Ogre::Matrix4& matrix);

	int getRocketKeyCode(int key);
	int getOISKeyCode(int key);
	bool mIsVisible;
	// Absolute path to the libRocket directory.
	Rocket::Core::String mRocketPath;
	// Absolute path to the Ogre3d sample directory;
	Rocket::Core::String mSamplePath;

	Rocket::Core::Context* mContext;
	Rocket::Core::ElementDocument* mCurrentDocument;
	RocketDocumentDefinition mCurrentDocumentDefinition;

	Rocket::Core::ElementDocument* mModalDocument;
	RocketDocumentDefinition mModalDocumentDefinition;

	SystemInterfaceOgre3D* mOgreSystem;
	RenderInterfaceOgre3D* mOgreRenderer;
	FileInterfaceOgre3D* mOgreFilesystem;

	Ogre::RenderWindow* mRenderWindow;
	GameManager* mGameManager;
	InputListener* mInputListener;
	DataProvider* mDataProvider;
	GameState* mActiveState;
	InGameCamera* mInGameCamera;
	std::deque<LuaScriptElement*> mLuaScriptElements;

	
	static RocketRenderer* mRocketRenderer;
};

#endif
