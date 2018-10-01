#include "utilities.h"

//=============================================================================
// Utilities::generateRandNum(int max)
// args: int max which is the highest number this function is allowed to return
// return value: int (the randomly generated number)
// description: Will generate a random number between 1 and the max given.
//=============================================================================
int Utilities::generateRandNum(int max)
{
	// seed random num generator
	//srand(static_cast<unsigned int>(time(0)));
	
	// generate random number
	int randomNumber = rand();

	// get  number between 1 and max
	int randNum = (randomNumber % max);

	return randNum;
}

//=============================================================================
// Utilities::getStraightLineDistance
// args: x1, y1, x2, y2 (floats)
// return value:  the distance between point 1 and point 2 (float)
// description: Will use the x,y coords of two points to return the distance between those two points
//=============================================================================
float Utilities::getStraightLineDistance(float x1, float y1, float x2, float y2)
{
	float distance = sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
	return distance;
}

//=============================================================================
// Utilities::calcProb
// args: floats, numerator and denominator
// return value:  bool
// description: will calculate the probability based on the numerator and denominator and then
//				return true or false. The probability it will return true is the probability calculated.
//=============================================================================
bool Utilities::calcProb(float numerator, float denominator)
{
	float prob = numerator/denominator;

	if(prob >= 1.0f)
		return true;

	int randNum = generateRandNum(101);
	float randNumDec = randNum/100.0f;

	if (randNumDec <= prob)
		return true;

	return false;
}