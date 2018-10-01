#ifndef _MUSH_UNIT_CLOSE_COMBAT_FACTORY_H               // Prevent multiple definitions if this 
#define _MUSH_UNIT_CLOSE_COMBAT_FACTORY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "mushFactory.h"

class MushCloseCombatFactory : public MushFactory
{
public:
	virtual Mushroom* createMush();
};

#endif