#include <Input/InputListener.h>
#include <GUI/RocketRenderer.h>

InputListener::InputListener()
	:
	mInit(false)
{
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	GameManager::getSingletonPtr()->getRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);

	windowHndStr << windowHnd;
	
#ifdef WIN32
			pl.insert(std::make_pair(std::string("w32_mouse"),			std::string("DISCL_FOREGROUND")));
			pl.insert(std::make_pair(std::string("w32_mouse"),			std::string("DISCL_NONEXCLUSIVE")));
			pl.insert(std::make_pair(std::string("w32_keyboard"),		std::string("DISCL_FOREGROUND")));
			pl.insert(std::make_pair(std::string("w32_keyboard"),		std::string("DISCL_NONEXCLUSIVE")));

			

#elif MAC
			pl.insert(std::make_pair(std::string("MacAutoRepeatOn"),	std::string("true")));
#else
			pl.insert(std::make_pair(std::string("x11_mouse_grab"),		std::string("false")));
			pl.insert(std::make_pair(std::string("x11_mouse_hide"),		std::string("false")));
			pl.insert(std::make_pair(std::string("x11_keyboard_grab"),  std::string("false")));
			pl.insert(std::make_pair(std::string("XAutoRepeatOn"),		std::string("true")));
#endif
	
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
	const OIS::MouseState& st = mMouse->getMouseState();
	st.width  = GameManager::getSingletonPtr()->getRenderWindow()->getWidth(); 
	st.height = GameManager::getSingletonPtr()->getRenderWindow()->getHeight();

	
}

InputListener::~InputListener()
{

}

void InputListener::init()
{
	mRocketRenderer = RocketRenderer::getSingletonPtr();
}

bool InputListener::mouseMoved(const OIS::MouseEvent& arg)
{
	mRocketRenderer->mouseMoved(arg);
	return true;
}

bool InputListener::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	mRocketRenderer->mousePressed(arg,id);

	return true;
}

bool InputListener::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	mRocketRenderer->mouseReleased(arg,id);
	return true;
}

bool InputListener::keyPressed(const OIS::KeyEvent& arg)
{
	mRocketRenderer->keyPressed(arg);
	return true;
}

bool InputListener::keyReleased(const OIS::KeyEvent& arg)
{
	mRocketRenderer->keyReleased(arg);
	return true;
}

void InputListener::update(Ogre::Real timeSinceLastFrame)
{
	mKeyboard->capture();
	mMouse->capture();
}


//Adjust mouse clipping area
void InputListener::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void InputListener::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw ==  GameManager::getSingletonPtr()->getRenderWindow())
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}


int InputListener::getKeyModifier()
{
	if (!mKeyboard) return 0;

	int modifier = 0;

	if (mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		modifier |= OIS::Keyboard::Ctrl;
	if (mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		modifier |= OIS::Keyboard::Shift;
	if (mKeyboard->isModifierDown(OIS::Keyboard::Alt))
		modifier |= OIS::Keyboard::Shift;

	return modifier;
}

InputListener* InputListener::mInputListener;

InputListener* InputListener::getSingletonPtr()
{
	if( !mInputListener ) 
	{
		mInputListener = new InputListener();
		mInputListener->init();
	}

	return mInputListener;
}