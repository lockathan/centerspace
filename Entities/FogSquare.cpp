#include <Entities/FogSquare.h>


/*Class FogSquareRow*/

FogSquareRow::FogSquareRow(FogSquare* square) 
	: 
	mX(square->X),
	mGapsFilled(false),
	mMinZ(0),
	mMaxZ(0)
{
	addFogSquare(square);
}

FogSquareRow::~FogSquareRow()
{
	
	std::deque<FogSquare*>::iterator iter;

	for(iter = mSquares.begin(); iter != mSquares.end(); ++iter)
	{
		delete (*iter);
		iter = mSquares.erase(iter);
		if(iter == mSquares.end())
			break;
	}
	
}
	
void FogSquareRow::addFogSquare(FogSquare* square)
{
	std::deque<FogSquare*>::iterator iter;

	iter = mSquares.begin();

	while(iter != mSquares.end() && (*iter)->Z < square->Z)
	{
		++iter;
	}
		
	if(iter != mSquares.end() && (*iter)->Z == square->Z)
		return;

	mSquares.insert(iter, square);

}

int FogSquareRow::getX()
{
	return mX;
}

void FogSquareRow::fillGaps()
{
	if(mSquares.size() == 1)
	{
		mSquareIterator = mSquares.begin();
		return;
	}

	std::deque<FogSquare*>::iterator current;
	std::deque<FogSquare*>::iterator next;

	current = mSquares.begin();
	next = mSquares.begin();
	next++;

	
	while(current != mSquares.end() && next != mSquares.end())
	{
		int cZ(0), nZ(0);

		cZ = (*current)->Z;
		nZ = (*next)->Z;
		--nZ;

		std::deque<FogSquare*>::iterator insert = next;
		while(cZ != nZ)
		{
			FogSquare* square = new FogSquare();
			square->X = mX;
			square->Z = nZ;
			square->State = VisibilityState::VS_COMPLETE;
			insert = mSquares.insert(insert, square);
			--nZ;
		}

		current = mSquares.begin();
		next = mSquares.begin();
		++next;

		while(current != mSquares.end() && next != mSquares.end() && (*next)->Z - (*current)->Z == 1)
		{
			++current;
			++next;
		}

	}

	mGapsFilled = true;
	mSquareIterator = mSquares.begin();
	mMinZ = mSquares.front()->Z;
	mMaxZ = mSquares.back()->Z;
}

FogSquare* FogSquareRow::getNext()
{
	FogSquare* square = (*mSquareIterator);
	++mSquareIterator;
	return square;
}

bool FogSquareRow::hasMoreFogSquares()
{
	return mSquareIterator != mSquares.end();
}

bool FogSquareRow::gapsFilled()
{
	return mGapsFilled;
}

FogSquare* FogSquareRow::front()
{
	return mSquares.front();
}

FogSquare* FogSquareRow::back()
{
	return mSquares.back();
}

bool FogSquareRow::isInside(int z)
{
	if(z < mMinZ || z > mMaxZ)
		return false;

	return true;
}

void FogSquareRow::reset()
{
	mSquareIterator = mSquares.begin();
}

/*Class FogSquareGrid*/
FogSquareGrid::FogSquareGrid()
	:
	mMinX(0),
	mMaxX(0)
{

}

FogSquareGrid::~FogSquareGrid()
{
	
	std::deque<FogSquareRow*>::iterator iter;

	for(iter = mRows.begin(); iter != mRows.end(); ++iter)
	{
		delete (*iter);
		iter = mRows.erase(iter);
		if(iter == mRows.end())
			break;
	}
	
}

void FogSquareGrid::addFogSquare(FogSquare* square)
{
	std::deque<FogSquareRow*>::iterator iter;

	for(iter = mRows.begin(); iter != mRows.end(); ++iter)
	{
		if((*iter)->getX() == square->X)
		{
			(*iter)->addFogSquare(square);
			return;
		}
	}

	FogSquareRow* row = new FogSquareRow(square);
	
	iter = mRows.begin();

	while(iter != mRows.end() && (*iter)->getX() < row->getX())
	{
		++iter;
	}

	mRows.insert(iter, row);
}

void FogSquareGrid::insert(std::deque<FogSquare*> squares)
{
	std::deque<FogSquare*>::iterator iter;

	for(iter = squares.begin(); iter != squares.end(); ++iter)
	{
		this->addFogSquare((*iter));
	}
}

void FogSquareGrid::fillGaps()
{
	std::deque<FogSquareRow*>::iterator current;
	std::deque<FogSquareRow*>::iterator next;

	current = mRows.begin();
	next = mRows.begin();
	++next;

	while(current != mRows.end() && next != mRows.end())
	{

		if((*next)->getX() - (*current)->getX() != 1)
		{
			int x = (*current)->getX() + 1;
			FogSquare* square = new FogSquare();
			square->X = x;
			square->Z = (*current)->back()->Z;
			square->State = VisibilityState::VS_COMPLETE;
			
			FogSquareRow* row = new FogSquareRow(square);
			square = new FogSquare();
			square->X = x;
			square->Z = (*current)->front()->Z;
			square->State = VisibilityState::VS_COMPLETE;
			row->addFogSquare(square);
			current = mRows.insert(next, row);
			next = current;
			++next;
			//this->insertRow(row);
			
		}

		++current;
		++next;
	}
	
	std::deque<FogSquareRow*>::iterator iter;

	for(iter = mRows.begin(); iter != mRows.end(); ++iter)
	{
		(*iter)->fillGaps();
	}

	mRowIterator = mRows.begin();
	mMinX = mRows.front()->getX();
	mMaxX = mRows.back()->getX();
}


FogSquare* FogSquareGrid::getNext()
{
	if((*mRowIterator)->hasMoreFogSquares())
		return (*mRowIterator)->getNext();

	++mRowIterator;

	return (*mRowIterator)->getNext();
}

bool FogSquareGrid::hasMoreFogSquares()
{
	if((*mRowIterator) != mRows.back())
		return true;

	return (*mRowIterator)->hasMoreFogSquares();
}

bool FogSquareGrid::isInside(int x, int z)
{
	if(x < mMinX || x > mMaxX)
		return false;

	int index = x - mMinX;

	return mRows[index]->isInside(z);
}

void FogSquareGrid::reset()
{
	std::deque<FogSquareRow*>::iterator iter;

	for(iter = mRows.begin(); iter != mRows.end(); ++iter)
	{
		(*iter)->reset();
	}

	mRowIterator = mRows.begin();
}
