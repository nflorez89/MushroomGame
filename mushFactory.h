#ifndef _MUSH_UNIT_FACTORY_H               // Prevent multiple definitions if this 
#define _MUSH_UNIT_FACTORY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "mushroom.h"

class MushFactory
{
public:
	virtual Mushroom* createMush() = 0;
};

#endif