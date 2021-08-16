#ifndef CoverageMap_h
#define CoverageMap_h

class BattlefieldGrid;
class BattlefieldGridCell;
class FogSquareGrid;
class PlayerBattleState;

#include <vector>
#include <deque>

enum CoverageStatus
{
	CS_NOT_VISIBLE_UNKNOWN = 0,
	CS_NOT_VISIBLE_EMPTY,
	CS_VISIBLE_EMPTY,
	CS_VISIBLE_ENEMY,
	CS_VISIBLE_LAST_SIGHTING
};

struct CoverageSquare
{
public:
	CoverageSquare() : Status(CoverageStatus::CS_NOT_VISIBLE_UNKNOWN), Interest(1) {}
	CoverageSquare(CoverageStatus cs, int interest) : Status(cs), Interest(interest){}
	CoverageStatus Status;
	int Interest;
};

struct HotSpot
{
public:
	HotSpot(int x, int z) : X(x), Z(z) {}
	int X;
	int Z;
};

class CoverageMap
{
public:
	CoverageMap(unsigned int sideLength, unsigned int size, PlayerBattleState* state);
	~CoverageMap();

	void resetCoverageMap();
	void updateCoverageMap();
	void clearSightings();
	void setVisibility(FogSquareGrid* grid);
	void computeCoverageMap();
	void addEnemySighting(int x, int z);
	void addEnemySighting(BattlefieldGridCell* location);
	std::deque<HotSpot> getEnemySightings();
	void removeEnemySighting(const HotSpot& hotspot);
	BattlefieldGridCell* getNearestHotSpot(BattlefieldGridCell* location);
	BattlefieldGridCell* getNearestHotSpot(int x, int z);
private:
	unsigned int mSideLength;
	unsigned int mSize;
	BattlefieldGrid* mGrid;
	PlayerBattleState* mPlayerBattleState;
	std::vector<CoverageSquare> mCoverageStatusMap;
	std::deque<HotSpot> mEnemySightings;
	std::deque<HotSpot> mNonSightingHotSpots;
	std::deque<HotSpot> mSightingHotSpots;

	std::vector<int> mFirstXVisible;
	std::vector<int> mFirstZVisible;
	std::vector<int> mLastZVisible;
};
#endif