#ifndef _MUSH_UNIT_PARASITIC_FACTORY_H               // Prevent multiple definitions if this 
#define _MUSH_UNIT_PARASITIC_FACTORY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "mushFactory.h"

class MushParasiticFactory : public MushFactory
{
public:
	virtual Mushroom* createMush();
};

#endif