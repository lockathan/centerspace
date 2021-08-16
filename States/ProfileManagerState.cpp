#include <States/ProfileManagerState.h>
#include <Entities/DataProvider.h>
#include <Entities/DataSet.h>
#include <Entities/DataSource.h>
#include <Entities/PlayerProfile.h>
#include <GUI/RocketDocuments.h>
#include <GUI/RocketRenderer.h>
#include <Managers/GameManager.h>
#include <Managers/SQLiteDBManager.h>
#include <States/MainMenuState.h>

#include <Rocket/Core.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/Element.h>
#include <Rocket/Controls.h>

#include <Ogre.h>

using namespace Ogre;

ProfileManagerState::ProfileManagerState()
	:
	mCurrentPlayer(0),
	mSelectedProfileId(0),
	mSQLiteDBManager(SQLiteDBManager::getSingletonPtr())
{

}

ProfileManagerState::~ProfileManagerState() 
{
	if(mCurrentPlayer)
	{
		delete mCurrentPlayer;
		mCurrentPlayer = 0;
	}
	
}

void ProfileManagerState::enter()
{
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	if(mCurrentPlayer)
	{
		delete mCurrentPlayer;
		mCurrentPlayer = 0;
	}
	mIsEnabled = true;
	mCamera = this->getCamera("MainCamera");

	DataProvider* mDataProvider = DataProvider::getSingletonPtr();
		
	mDataProvider->clearDataSource(DataSourceType::PLAYER);
	std::vector<PlayerProfile*> profiles = mSQLiteDBManager->getPlayerProfiles();
	std::deque<DataSetItem*> data;
	std::vector<PlayerProfile*>::iterator iter;
	for(iter = profiles.begin(); iter != profiles.end(); ++iter)
	{
		DataSetItem* item = new DataSetItem();
		item->mData = (*iter)->getGuiValues();
		item->mId = (*iter)->getId();
		data.push_back(item);
	}

	DataSet* playerData = new DataSet(DataSourceType::PLAYER);
	playerData->setData(data);
	mDataProvider->getDataSource(DataSourceType::PLAYER)->setPrimaryDataSet(playerData);

	mRocketRenderer->loadDocument(RocketDocuments::ProfileMenu::ProfileMain);
}

void ProfileManagerState::exit()
{
	DataProvider::getSingletonPtr()->clearDataSource(DataSourceType::PLAYER);
}

PlayerProfile* ProfileManagerState::getCurrentPlayer()
{
	return mCurrentPlayer;
}

void ProfileManagerState::setCurrentPlayer(PlayerProfile* player)
{
	mCurrentPlayer = player;
}

void ProfileManagerState::ProcessEvent(Rocket::Core::Event& ev)
{
	
}

void ProfileManagerState::newProfile()
{
	Rocket::Core::ElementDocument* profileMain = mRocketRenderer->getDocument(RocketDocuments::ProfileMenu::ProfileMain);
	Rocket::Controls::ElementFormControlInput* userProfileTextField = dynamic_cast<Rocket::Controls::ElementFormControlInput*>(profileMain->GetElementById("newprofilename"));
	Rocket::Core::String newProfileName = userProfileTextField->GetValue();
	if(newProfileName.Length() <= 2)
		return;

	PlayerProfile* player = new PlayerProfile();
	player->setName(newProfileName.CString());
	mSQLiteDBManager->addPlayerProfile(player);
		
		
	DataProvider* mDataProvider = DataProvider::getSingletonPtr();
		
	mDataProvider->clearDataSource(DataSourceType::PLAYER, true);
	std::vector<PlayerProfile*> profiles = mSQLiteDBManager->getPlayerProfiles();
	std::deque<DataSetItem*> data;
	std::vector<PlayerProfile*>::iterator iter;

	for(iter = profiles.begin(); iter != profiles.end(); ++iter)
	{
		DataSetItem* item = new DataSetItem();
		item->mData = (*iter)->getGuiValues();
		item->mId = (*iter)->getId();
		data.push_back(item);
	}

	DataSet* playerData = new DataSet(DataSourceType::PLAYER);
	playerData->setData(data);
	mDataProvider->getDataSource(DataSourceType::PLAYER)->setPrimaryDataSet(playerData);
	mDataProvider->updateDataSourceListeners(DataSourceType::PLAYER);
		
	userProfileTextField->SetValue("");
	userProfileTextField->Blur();
	profileMain->Focus();
}

void ProfileManagerState::selectProfile(const std::string& profileId)
{
	int id = std::atoi(profileId.c_str());

	if(id >= 0)
	{
		onSelectedElementChanged(DataSourceType::PLAYER, id);
	}
}

void ProfileManagerState::onSelectedElementChanged(DataSourceType source, int id)
{
	if(source == DataSourceType::PLAYER)
	{
		mSelectedProfileId = id;
		mCurrentPlayer = mSQLiteDBManager->getPlayerProfile(mSelectedProfileId);
		mGameManager->changeState(MainMenuState::getSingletonPtr());
	}
}

ProfileManagerState* ProfileManagerState::mProfileManagerState;

ProfileManagerState* ProfileManagerState::getSingletonPtr()
{
	if(!mProfileManagerState)
	{
		mProfileManagerState = new ProfileManagerState();
	}
	return mProfileManagerState;
}