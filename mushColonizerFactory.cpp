#include "mushColonizerFactory.h"
#include "colonizerMushroom.h"

//=============================================================================
//  Factory method for creating close combat unit
//=============================================================================
Mushroom* MushColonizerFactory::createMush()
{
	// return the newly created unit
	return new ColonizerMushroom();
	
}