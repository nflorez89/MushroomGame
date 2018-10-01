#ifndef _DEBRIS_H               // Prevent multiple definitions if this 
#define _DEBRIS_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "utilities.h"

class Debris : public Entity
{
protected:
	mushroomWarNS::DEBRIS_TYPE type;
public:
	// constructor
	Debris();

	float getNutrientBonus();

	inline mushroomWarNS::DEBRIS_TYPE getDebrisType() { return type;}
};

#endif