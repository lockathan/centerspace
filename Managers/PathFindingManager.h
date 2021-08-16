#ifndef PathFindingManager_H
#define PathFindingManager_H

#include <vector>

class BattlefieldGrid;
class BattlefieldGridCell;
class PathFindingNode;
class DriveTemplate;

class PathFindingManager
{
public:
	PathFindingManager(BattlefieldGrid* grid);
	~PathFindingManager();

	PathFindingNode* findPath(BattlefieldGridCell* start, BattlefieldGridCell* end, DriveTemplate* drive);
	
private:
	PathFindingNode* findLowest(std::vector<PathFindingNode*> &set);
	
	PathFindingNode* removeLowest(std::vector< PathFindingNode*> &set);
	std::vector<PathFindingNode*> getNeighbours(PathFindingNode* search);

	float estimateCostToGoal(PathFindingNode* current, PathFindingNode* goal);

	void resetAll();

	std::vector<PathFindingNode*> mAll;
	std::vector<PathFindingNode*> mOpen;
	std::vector<PathFindingNode*> mClosed;
	BattlefieldGrid* mGrid;
	int mSideLength;
	float mMaxRoughness;

};
#endif