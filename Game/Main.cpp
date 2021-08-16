#include <OgreException.h>
#include <OgreLogManager.h>
#include <Managers/GameManager.h>
#include <Managers/SQLiteDBManager.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) 
{
#else
int main( int argc, char **argv ) {
#endif
	GameManager *gameManager = GameManager::getSingletonPtr();

	try 
	{
		// Initialise the game and switch to the first state
		gameManager->startGame();
	}
	catch ( Ogre::Exception& ex ) 
	{
		Ogre::LogManager::getSingletonPtr()->getDefaultLog()->logMessage("Fatal exception occurred, quitting.", Ogre::LogMessageLevel::LML_CRITICAL);
		Ogre::LogManager::getSingletonPtr()->getDefaultLog()->logMessage(ex.getFullDescription(), Ogre::LogMessageLevel::LML_CRITICAL);
		Ogre::LogManager::getSingletonPtr()->getDefaultLog()->logMessage("Occurred in " + ex.getFile() + " at line number " + SQLiteDBManager::convertIntToString(ex.getLine()), Ogre::LogMessageLevel::LML_CRITICAL);

		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				MessageBox( NULL, ex.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL );
		#else
				std::cerr << "An exception has occured: " << ex.getFullDescription();
		#endif
	}
	
	delete gameManager;
	return 0;
}