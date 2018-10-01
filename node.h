#ifndef _NODE_H               // Prevent multiple definitions if this 
#define _NODE_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "utilities.h"
#include "tile.h"

struct Node
{
	mushroomWarNS::ACTION action;
	int pathCost;
	Node* parent;
	Tile* tile;

	Node()
	{
		action = mushroomWarNS::NONE;
		pathCost = 0;
		parent = NULL;
		tile = NULL;
	}

	// destructor
	~Node()
	{
		//SAFE_DELETE(parent);
		//SAFE_DELETE(tile);
	}
};

#endif