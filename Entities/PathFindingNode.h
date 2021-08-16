#ifndef PathFindingNode_h
#define PathFindingNode_h

#include <hash_map>
#include <vector>
class BattlefieldGridCell;

class PathFindingNode
{
public:
	PathFindingNode();
	PathFindingNode(BattlefieldGridCell* actual, bool isStart = false);
	PathFindingNode(PathFindingNode* parent, float totalMovementCost, BattlefieldGridCell* actual, bool isStart = false);
	~PathFindingNode();
	
	PathFindingNode* getParent();
	void setParent(PathFindingNode* parent);

	float getMovementCost();
	void setMovementCost(float movement);

	float getTotalMovementCost();
	void setTotalMovementCost(float total);

	float getEstimatedCostToGoal();
	void setEstimatedCostToGoal(float value);

	float getWholePathCost();

	int getX();
	void setX(int x);

	int getZ();
	void setZ(int z);

	BattlefieldGridCell* getActual();
	void setActual(BattlefieldGridCell* actual);

	bool isDiagonal(PathFindingNode* neighbour);
	bool isSloped(PathFindingNode* neighbour);
	bool isCliff(PathFindingNode* neighbour);

protected:
	PathFindingNode* mParent;
	BattlefieldGridCell* mActual;
	float mMovementCost;
	float mTotalMovementCost;
	float mEstimatedCostToGoal;
	float mWholeCost;
	bool mOccupied;
	bool mIsStart;
	int mX;
	int mZ;
};
#endif