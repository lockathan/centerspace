#include <Terrain/Divider.h>
#include <math.h>

void Divider::initialize(float difference, int steps)
{
	totalSteps = steps;
	steps = steps;
	difference = difference;
	
	store.clear();

	while(steps > 0)
	{
		float next = (difference / totalSteps);
				
		if(steps == 1)
		{
			store.push_front(next);
			return;
		}
				
		//float next = (difference / steps) * (generator.nextBoolean() ? (1 + generator.nextFloat()) : (1 - generator.nextFloat()));
				
		//difference-= next;
		store.push_front(next);
				
		steps--;
	}
}

float Divider::pop()
{
	float result = store.front();
	store.pop_front();
	return result;
}


void IntegerDivider::initialize(int heightDifference, int steps)
{
	totalSteps = steps;
	steps = steps;
	difference = heightDifference;
	int average = difference / steps;
	int differenceBank =  abs(heightDifference- (average * steps));
	int differenceMultiplier = (heightDifference > (average * steps)) ? 1 : -1;

	store.clear();

	while(steps > 0)
	{
		int currentDifference = 0;
		if(differenceBank != 0)
		{
			if(steps == 1)
			{
				currentDifference = differenceBank;
			}
			else if(2 * average > differenceBank)
			{
				currentDifference = (rand() % differenceBank);
			}
			else
			{
				if(average != 0)
					currentDifference = (rand() % (2 * average));
				else
					currentDifference = rand() % 1;
			}

			differenceBank-=currentDifference;
			currentDifference*=differenceMultiplier;
		}

		int next = average + currentDifference;
				
		store.push_front(next);
				
		steps--;
	}
}

int IntegerDivider::pop()
{
	int result = store.front();
	store.pop_front();
	return result;
}