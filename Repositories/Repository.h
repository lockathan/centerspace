#ifndef Repository_h
#define Repository_h

#include "Ogre.h"

template <class T>
class Repository
{
public:
	T* at ( int n );
	const T* at ( int n ) const;
	T* findByName(const std::string& name);
	T* findByNameOrID(const std::string& name);

	int size();
	int getNextId();
	int add(T*);
	typename std::vector<T*>::iterator begin();
	typename std::vector<T*>::iterator end();
protected:
	std::vector<T*> mRepository;
	Repository();
	virtual ~Repository();
	int nextId;
	int mCurrentCapacity;
	int mSize;
};

template <class T>
Repository<T>::Repository()
{
	mCurrentCapacity = 10;
	mSize = 0;
	mRepository.resize(mCurrentCapacity);
	nextId = 1;
}


template <class T>
Repository<T>::~Repository()
{
	std::vector<T*>::iterator iter;

	for(iter = mRepository.begin(); iter != mRepository.end(); ++iter)
	{
		if(*iter)
		{
			delete *iter;
		}
	}
}

template <class T>
T* Repository<T>::at(int index)
{
	if(index > mCurrentCapacity)
		return 0;

	return mRepository[index];
}

template <class T>
const T* Repository<T>::at ( int n ) const
{
	if(n > mCurrentCapacity)
		return 0;

	return mRepository[n];
}


template <class T>
T* Repository<T>::findByName(const std::string& name)
{
	std::vector<T*>::iterator iter;

	for(iter = mRepository.begin(); iter != mRepository.end(); ++iter)
	{
		if((*iter))
		{
			if((*iter)->getName() == name)
				return (*iter);
		}
	}

	return 0;
}

template <class T>
T* Repository<T>::findByNameOrID(const std::string& name)
{
	int id = std::atoi(name.c_str());

	if(id > 0)
	{
		return at(id);
	}
	else
	{
		return findByName(name);
	}
}

template <class T>
int Repository<T>::size()
{
	return mSize;
}


template <class T>
int Repository<T>::getNextId()
{
	return nextId;
}

template <class T>
int Repository<T>::add(T* t)
{
	mSize++;
	if(mSize >= mCurrentCapacity)
	{
		mCurrentCapacity+=10;
		mRepository.resize(mCurrentCapacity);
	}
	mRepository[nextId] = t;
	t->setId(nextId);
	nextId++;
	return nextId;
}

template <class T>
typename std::vector<T*>::iterator Repository<T>::begin()
{
	return mRepository.begin();
}

template <class T>
typename std::vector<T*>::iterator Repository<T>::end()
{
	return mRepository.end();
}

#endif