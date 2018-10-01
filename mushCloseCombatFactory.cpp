#include "mushCloseCombatFactory.h"
#include "closeCombatMushroom.h"

//=============================================================================
//  Factory method for creating close combat unit
//=============================================================================
Mushroom* MushCloseCombatFactory::createMush()
{
	// return the newly created unit
	return new CloseCombatMushroom();
	
}