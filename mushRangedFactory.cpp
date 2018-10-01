#include "mushRangedFactory.h"
#include "rangedMushroom.h"

//=============================================================================
//  Factory method for creating close combat unit
//=============================================================================
Mushroom* MushRangedFactory::createMush()
{
	// return the newly created unit
	return new RangedMushroom();	
}