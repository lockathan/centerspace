#ifndef Divider_h
#define Divider_h

#include <list>

class Divider
{
public:
	Divider(){}
	~Divider(){}
	void initialize(float difference, int steps);
	float pop();
protected:
	float difference;
	int steps;
	int totalSteps;
	std::list<float> store;
};

class IntegerDivider
{
	public:
	IntegerDivider(){}
	~IntegerDivider(){}
	void initialize(int heightDifference, int steps);
	int pop();
protected:
	int difference;
	int steps;
	int totalSteps;
	std::list<int> store;
};
#endif