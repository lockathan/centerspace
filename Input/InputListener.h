#ifndef InputListener_H
#define InputListener_H


#include <OIS.h>
#include <Managers/GameManager.h>

class RocketRenderer;

class InputListener : public OIS::KeyListener, public OIS::MouseListener, public GameManager::EngineUpdateListener, public Ogre::WindowEventListener
{
public:
	class Listener
	{
	public:
		virtual void mouseMoved(const OIS::MouseEvent& arg){}
		virtual void mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id){}
		virtual void mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id){}
		virtual void keyPressed(const OIS::KeyEvent& arg){}
		virtual void keyReleased(const OIS::KeyEvent& arg){}
	};
	
	~InputListener();
	
  
	void init();
  
	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent& arg);
	bool keyReleased(const OIS::KeyEvent& arg);

	virtual void update(Ogre::Real timeSinceLastFrame);
		// Ogre::WindowEventListener
	//Adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);
	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(Ogre::RenderWindow* rw);

	int getKeyModifier();

	static InputListener* getSingletonPtr();
private:
	InputListener();
	static InputListener* mInputListener;

	bool mInit;
	OIS::InputManager *mInputManager;

	RocketRenderer* mRocketRenderer;

	//OIS Input devices
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
};
#endif