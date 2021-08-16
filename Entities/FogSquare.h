#ifndef FogSquare_h
#define FogSquare_h

#include <deque>

enum VisibilityState
{
	VS_COMPLETE = 0,
	VS_NEG_X_NEG_Z = 1,
	VS_NEG_X_POS_Z = 2,
	VS_POS_X_NEG_Z = 3,
	VS_POS_X_POS_Z = 4,
	VS_OBSTRUCTED = 5
};

struct FogSquare
{
	FogSquare() : X(0), Z(0), State(VS_OBSTRUCTED) {}
	int X;
	int Z;
	VisibilityState State;
};

class FogSquareRow
{
	
public:
	FogSquareRow(FogSquare* square);
	~FogSquareRow();
	void addFogSquare(FogSquare* square);
	int getX();
	void fillGaps();
	FogSquare* getNext();
	bool hasMoreFogSquares();
	bool gapsFilled();
	FogSquare* front();
	FogSquare* back();
	bool isInside(int z);
	void reset();
private:
	int mX;
	bool mGapsFilled;
	std::deque<FogSquare*> mSquares;
	std::deque<FogSquare*>::iterator mSquareIterator;
	int mMinZ;
	int mMaxZ;
};


class FogSquareGrid
{
public:
	FogSquareGrid();
	~FogSquareGrid();
	void addFogSquare(FogSquare* square);
	void insert(std::deque<FogSquare*> squares);
	void fillGaps();
	FogSquare* getNext();
	bool hasMoreFogSquares();
	bool isInside(int x, int z);
	void reset();
private:
	std::deque<FogSquareRow*> mRows;
	std::deque<FogSquareRow*>::iterator mRowIterator;
	int mMinX;
	int mMaxX;
};
#endif