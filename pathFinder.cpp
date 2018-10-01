#include "pathFinder.h"

PathFinder* PathFinder::pathFinderInst = 0;
//=============================================================================
// constructor
//=============================================================================
PathFinder::PathFinder()
{}

//=============================================================================
// destructor
//=============================================================================
PathFinder::~PathFinder()
{
	//SAFE_DELETE(pathFinderInst);
}

//=============================================================================
// PathFinder::instance
// args: none
// return value: Pathfinder* (instance of Pathfinder)
// description: will create an instance of Pathfinder and return it if this 
//				instance does not exist. If it does exist, then just return this
//				instance.
// remarks: This function allows this class to be responsible for keeping track
//			of its sole instance.
//=============================================================================
PathFinder* PathFinder::instance()
{
	// check if this instance exists or not
	if (pathFinderInst == 0)
	{
		pathFinderInst = new PathFinder;
	}

	return pathFinderInst;
}
//=============================================================================
// PathFinder::result
// args: Node* (parent node), mushroomWarNS::ACTION (an action)
// return value: Node* (child node)
// description: will return the child node of the parent based on the action
//				given.
//=============================================================================
Node* PathFinder::result(Node* parent, Unit* movingUnit, mushroomWarNS::ACTION action, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	Tile* aTile;
	Node* resultingNode = new Node;
	int parentInd = (FIELD_GRID_WIDTH_TILES*parent->tile->getYPos()) + parent->tile->getXPos();
	// check if this action will result in a differently located node or not
	if(movingWithinBounds(parent, action))
	{
		//parentInd = (FIELD_GRID_WIDTH_TILES*parent->tile->getYPos()) + parent->tile->getXPos();

		// get the new position of this new node and set the state (tile) of this node
		switch(action)
		{
			case mushroomWarNS::A_UP:
				aTile = fieldGrid[parentInd - FIELD_GRID_WIDTH_TILES];
				break;
			case mushroomWarNS::A_DOWN:
				aTile = fieldGrid[parentInd + FIELD_GRID_WIDTH_TILES];
				break;
			case mushroomWarNS::A_LEFT:
				aTile = fieldGrid[parentInd - 1];
				break;
			case mushroomWarNS::A_RIGHT:
				aTile = fieldGrid[parentInd + 1];
				break;
			default:
				aTile = fieldGrid[parentInd];
		}

		if(!movingUnit->canTraverse(aTile))
			resultingNode->tile = fieldGrid[parentInd];
		else
			resultingNode->tile = aTile;
	}
	else
	{
		resultingNode->tile = fieldGrid[parentInd];
	}

	// note: no issues seem to be with getting children
	//if(resultingNode->tile->getXPos() == 0 && resultingNode->tile->getYPos() == 0 /*&& parent->tile->getXPos() == 16 && parent->tile->getYPos() == 5*/)
	//{
		//int xp = parent->tile->getYPos();
		//int yp = parent->tile->getXPos();
		//resultingNode->tile->getXPos();
	//}

	return resultingNode;
}

//=============================================================================
// PathFinder::goalTest
// args: Node* (a node), Tile* (a tile)
// return value: boolean
// description: will return true if this is the goal node, false otherwise.
//=============================================================================
bool PathFinder::goalTest(Node* aNode, Tile* goalTile)
{
	if( aNode->tile == goalTile )
	{
		return true;
	}
	
	return false;
}

//=============================================================================
// PathFinder::goalTest
// args: Node* (a node), bool for if the entity you are looking for is on the 
//		 mushroom or nematode side.
// return value: boolean
// description: will return true if this is the goal node, false otherwise.
//=============================================================================
bool PathFinder::goalTest(Node* aNode, bool isMushroomUnit)
{
	Tile*  aTile = aNode->tile;
	// check if this node contains a tile with unit that is on the side we are looking for
	if(isMushroomUnit)
	{
		// so we are looking for an entity on the mushroom side
		if((aTile->hasOccupyingEntity() && aTile->getOccupyingEnt()->isThisMushroomUnit()) ||
			aTile->hasOccupyingMushroom())
		{
			return true;
		}
	}
	else
	{
		// so we are looking for an entity on the nematode side
		if(aTile->hasOccupyingEntity() && !aTile->getOccupyingEnt()->isThisMushroomUnit())
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
// PathFinder::goalTestNextTo
// args: Tile* (destination tile), Node* (a node), array of pointers to tiles
// return value: boolean
// description: will return true if the tile in this node is next to the target tile (destTile), false otherwise
//=============================================================================
bool PathFinder::goalTestNextTo(Tile* destTile, Node* aNode, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	Tile* thisTile = aNode->tile;
	int thisTileInd = (FIELD_GRID_WIDTH_TILES*thisTile->getYPos()) + thisTile->getXPos();

	int currPotentialInd = 0;

	// minus 1 since not including the 'NONE' action
	int potentialGridIndices[NUM_ACTIONS-1];
	potentialGridIndices[currPotentialInd++] = thisTileInd - FIELD_GRID_WIDTH_TILES;
	potentialGridIndices[currPotentialInd++] = thisTileInd + FIELD_GRID_WIDTH_TILES;
	potentialGridIndices[currPotentialInd++] = thisTileInd - 1;
	potentialGridIndices[currPotentialInd++] = thisTileInd + 1;

	int newInd = 0;

	int gridSize = FIELD_GRID_WIDTH_TILES * FIELD_GRID_HEIGHT_TILES;

	// check if the tile next to the tile the node is on is the destination tile
	for(int ind = 0; ind < (NUM_ACTIONS-1); ind++)
	{
			newInd = potentialGridIndices[ind];
			if(newInd < gridSize && newInd >= 0 && movingWithinBounds(aNode, mushroomWarNS::ACTION(ind+1)))
			{
				if(fieldGrid[newInd] == destTile)
					return true;
			}
	}

	// this node is not next to the destination tile
	return false;
}

//=============================================================================
// PathFinder::goalTest
// args: Node* (a node), bool, array of pointers to tiles
// return value: boolean
// description: will return true if the tile in this node is next to the target tile
//=============================================================================
bool PathFinder::goalTestNextTo(Node* aNode, bool isMushroomUnit, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid, bool isLookingForMushroom)
{
	Tile* thisTile = aNode->tile;
	int thisTileInd = (FIELD_GRID_WIDTH_TILES*thisTile->getYPos()) + thisTile->getXPos();

	int currPotentialInd = 0;

	// minus 1 since not including the 'NONE' action
	int potentialGridIndices[NUM_ACTIONS-1];
	potentialGridIndices[currPotentialInd++] = thisTileInd - FIELD_GRID_WIDTH_TILES;
	potentialGridIndices[currPotentialInd++] = thisTileInd + FIELD_GRID_WIDTH_TILES;
	potentialGridIndices[currPotentialInd++] = thisTileInd - 1;
	potentialGridIndices[currPotentialInd++] = thisTileInd + 1;

	int newInd = 0;

	int gridSize = FIELD_GRID_WIDTH_TILES * FIELD_GRID_HEIGHT_TILES;

	if(isLookingForMushroom)
	{
		for(int ind = 0; ind < (NUM_ACTIONS-1); ind++)
		{
			newInd = potentialGridIndices[ind];
			if(newInd < gridSize && newInd >= 0 && movingWithinBounds(aNode, mushroomWarNS::ACTION(ind+1)))
			{
				if(fieldGrid[newInd]->getOccupyingMushroom())
				{
					return true;
				}
			}
		}
	}
	else
	{
		for (int ind = 0; ind < (NUM_ACTIONS-1); ind++)
		{
			newInd = potentialGridIndices[ind];
			if(newInd < gridSize && newInd >= 0)
			{
				if(tileHasMatchingUnitTeam(fieldGrid[newInd], isMushroomUnit))
					return true;
			}
		}
	}

	return false;
}

//=============================================================================
// PathFinder::actions
// args: Node* (a node)
// return value: vector of mushroomWarNS::ACTIONs
// description: will return a vector of all the available actions from the 
//				given node.
//=============================================================================
std::vector<mushroomWarNS::ACTION> PathFinder::actions(Node*)
{
	// stub
	std::vector<mushroomWarNS::ACTION> actionVec;
	return actionVec ;
}

//=============================================================================
// PathFinder::stepCost
// args: mushroomWarNS::ACTION
// return value: integer
// description: will return 0 if the NONE action was taken, 1 otherwise
//=============================================================================
int PathFinder::stepCost(Node* aNode, mushroomWarNS::ACTION action)
{
	// have check if at edge
	if(movingWithinBounds(aNode, action))
		return 1;
	return 0;
}

//=============================================================================
// PathFinder::movingWithinBounds
// args: pointer to node, mushroomWarNS::ACTION
// return value: bool
// description: will return true if the action is moving within the bounds of the grid,
//				false otherwise
//=============================================================================
bool PathFinder::movingWithinBounds(Node* aNode, mushroomWarNS::ACTION action)
{
	bool canMove = false;

	switch(action)
	{
		case mushroomWarNS::A_UP:
			if(aNode->tile->getYPos() > 0)
				canMove = true;
			break;
		case mushroomWarNS::A_DOWN:
			if(aNode->tile->getYPos() < (FIELD_GRID_HEIGHT_TILES-1))
				canMove = true;
			break;
		case mushroomWarNS::A_LEFT:
			if(aNode->tile->getXPos() > 0)
				canMove = true;
			break;
		case mushroomWarNS::A_RIGHT:
			if(aNode->tile->getXPos() < (FIELD_GRID_WIDTH_TILES-1))
				canMove=true;
			break;
	}
	return canMove;
}

//=============================================================================
// PathFinder::tileHasMushroom
// args: pointer to tile, bool
// return value: bool
// description: will return true the tile contains an entity that matches the 
//				"is mushroom unit" status given
//=============================================================================
bool PathFinder::tileHasMatchingUnitTeam(Tile* aTile, bool isMushroomUnit)
{
	if(aTile->hasOccupyingEntity() && isMushroomUnit == aTile->getOccupyingEnt()->isThisMushroomUnit())
		return true;

	return false;
}