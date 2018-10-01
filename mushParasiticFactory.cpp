#include "mushParasiticFactory.h"
#include "parasiticMushroom.h"

//=============================================================================
//  Factory method for creating close combat unit
//=============================================================================
Mushroom* MushParasiticFactory::createMush()
{
	// return the newly created unit
	return new ParasiticMushroom();
}