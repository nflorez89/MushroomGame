#ifndef _PATHFINDER_H               // Prevent multiple definitions if this 
#define _PATHFINDER_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <array>

#include "node.h"

// singleton
class PathFinder
{
private:
	static PathFinder* pathFinderInst;
	bool movingWithinBounds(Node* aNode, mushroomWarNS::ACTION action);
	bool tileHasMatchingUnitTeam(Tile*, bool isMushroomUnit);
protected:
	PathFinder();
public:
	// destructor
	virtual ~PathFinder();

	// guarantees only 1 instance
	static PathFinder* instance();

	// search problem member functions
	Node* result(Node* parent, Unit* movingUnit, mushroomWarNS::ACTION action, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid);
	bool goalTest(Node*, Tile*);
	bool goalTest(Node*, bool);
	bool goalTestNextTo(Tile*, Node*, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>);
	bool goalTestNextTo(Node*, bool, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>, bool isLookingForMushroom = false);
	std::vector<mushroomWarNS::ACTION> actions(Node*);
	int stepCost(Node* aNode, mushroomWarNS::ACTION action);
};
#endif