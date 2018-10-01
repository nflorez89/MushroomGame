#include "tile.h"

int Tile::s_tilesTakenOver = 0;
int Tile::s_myceliumScore = 0;

//=============================================================================
// constructor
//=============================================================================
Tile::Tile(mushroomWarNS::TILE_TYPE tp, int xPosition, int yPosition, float xLocation, float yLocation) : Image()
{
	type = tp;
	envChar = NULL;
	occupyingEnt = NULL;
	occupyingMushroom = NULL;
	isTaken = false;
	xPos = xPosition;
	yPos = yPosition;
	setX(xLocation);
	setY(yLocation);
	mycelium = NULL;
	isWithinAtkRng = false;

	switch(type)
	{
	case mushroomWarNS::WATER:
		startFrame=35;
		endFrame=35;
		break;
	case mushroomWarNS::STONE:
		startFrame=36;
		endFrame=36;
		break;
	default:
		startFrame=28;
		endFrame=28;
	}
}

//=============================================================================
// destructor
//=============================================================================
Tile::~Tile()
{
	SAFE_DELETE(envChar);
	SAFE_DELETE(occupyingMushroom);
	SAFE_DELETE(occupyingEnt);
	SAFE_DELETE(mycelium);
}

//=============================================================================
// Tile::calcMoveDir
// args: source tile, destination tile
// return value: mushroomWarNS::DIRECTION
// description: will return the direction from the source tile to the destination
//				tile. Potential values are UP, DOWN, LEFT, and RIGHT.
//=============================================================================
mushroomWarNS::DIRECTION Tile::calcMoveDir(Tile*, Tile*)
{
	// stub
	return mushroomWarNS::D_UP;
}

//=============================================================================
// Tile::getNextPlanTile
// args: deque<Tile*>, i.e. the path from the original source tile to the final
//			destination tile
// return value: the next tile in the plan from this tile
// description: From this tile will return the adjacent tile in the plan path.
//				returns NULL if this tile is not in the plan.
//=============================================================================
Tile* Tile::getNextPlanTile(std::deque<Tile*> path)
{
	// stub
	return new Tile(mushroomWarNS::GRASS,0,0,0,0);
}

//=============================================================================
// Tile::getGoalPath
// args: Tile*, i.e. the final goal tile
// return value: deque<Tile*>, i.e. container that holds the tiles in sequence
//				from beginning source tile to the final destination tile.
// description: Takes a goal tile and will generate a plan/path of tiles from 
//				this tile to the goal tile. If this tile is not reachable, returns
//				empty deque.
//=============================================================================
std::deque<Tile*> Tile::getGoalPath(Tile* goalTile)
{
	// stub
	std::deque<Tile*> plan;
	return plan;
}

//=============================================================================
// Tile::hasOccupyingEntity
// args: none
// return value: boolean
// description: returns true if there is an occupying entity in this tile. False otherwise.
//=============================================================================
bool Tile::hasOccupyingEntity()
{
	if(occupyingEnt != NULL)
		return true;

	return false;
}

//=============================================================================
// Tile::hasOccupyingMushroom
// args: none
// return value: boolean
// description: returns true if there is an occupying mushroom in this tile. False otherwise.
//=============================================================================
bool Tile::hasOccupyingMushroom()
{
	if(occupyingMushroom != NULL)
		return true;

	return false;
}

//=============================================================================
// Tile::hasOccupyingEnvChar
// args: none
// return value: boolean
// description: returns true if there is an occupying environmental characteristic 
//				in this tile. False otherwise.
//=============================================================================
bool Tile::hasOccupyingEnvChar()
{
	if(envChar != NULL)
		return true;

	return false;
}

//=============================================================================
// Tile::isOccupied
// args: none
// return value: boolean
// description: returns true if there is an occupying environmental characteristic, 
//				mushroom, or unit on this tile. False otherwise.
//=============================================================================
bool Tile::isOccupied()
{
	if(hasOccupyingEntity() || hasOccupyingMushroom() || hasOccupyingEnvChar())
		return true;

	return false;
}

//=============================================================================
// Tile::getClosestMushroomUnitTile
// args: if isMovingToMushroom is false, then it will look for closest enemy unit
//		 else if it is true, will look for closest mushroom.
//		 A reference to a vector of pointers to tiles which are the tiles we have already 
//		 tried and were not able to path find our way to.
// return value: pointer to tile of the unit that is closest to the given tile
// description: will find the "straight line" closest tile to the given tile that
//				contains a unit of the opposing (mushroom) team.
// note: since it's just getting the "straight line" distance, it's a heuristic
//		 so it will not always find the actual tile that's closest to this tile
//		 path-cost wise.
// note2: will insert into enemy mush tiles checked vector if finds a tile we have not checked before.
//=============================================================================
Tile* Tile::getClosestOpposingTile(bool isMovingToMushroom, std::vector<Tile*>& enemyMushTilesChecked, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid)
{
	// if this tile does not have an occupying entity, return NULL
	if(!hasOccupyingEntity())
		return NULL;

	Tile* fieldTile = NULL;
	int size = fieldGrid.size();
	float smallestDistance = -1;
	Tile* closestTile = NULL;
	bool readyCalcDistance = false;
	
	// using not (!) since we need opposing unit type
	bool isMushUnit = !(getOccupyingEnt()->isThisMushroomUnit());

	// for each tile in the field grid...
	for(int i = 0; i < size; i++)
	{
		readyCalcDistance = false;
		fieldTile = fieldGrid[i];

		if(isMovingToMushroom)
		{
			if(fieldTile->hasOccupyingMushroom() && !fieldTile->isTileInVector(enemyMushTilesChecked))
			{
				enemyMushTilesChecked.push_back(fieldTile);
				readyCalcDistance = true;
			}
		}
		else
		{
			if(fieldTile->hasOccupyingEntity() && fieldTile->getOccupyingEnt()->isThisMushroomUnit() == isMushUnit && !fieldTile->isTileInVector(enemyMushTilesChecked))
			{
				enemyMushTilesChecked.push_back(fieldTile);
				readyCalcDistance = true;
			}
		}

		if(readyCalcDistance)
		{
			float thisDistance = getStraightLineDistance(fieldTile);
			if(smallestDistance == -1)
			{
				smallestDistance = thisDistance;
				closestTile = fieldTile;
			}
			else if(thisDistance < smallestDistance)
			{
				smallestDistance = thisDistance;
				closestTile = fieldTile;
			}
		}
	}

	return closestTile;
}

//=============================================================================
// Tile::isTileInVector
// args: pointer to tile, vector of pointers to tiles
// return value: bool
// description: will return true if the tile given is in the vector, false otherwise
//=============================================================================
bool Tile::isTileInVector(std::vector<Tile*>& aTileVector)
{
	int size = aTileVector.size();
	for(int index = 0; index < size; index++)
	{
		if(this == aTileVector[index])
			return true;
	}
	return false;
}

//=============================================================================
// Tile::getStraightLineDistance
// args: pointers to two tiles
// return value: float representing the distance
// description: will find the "straight line" distance between two tiles using their
//				centers as points.
//=============================================================================
float Tile::getStraightLineDistance(/*Tile* tile1,*/ Tile* aTile)
{
	float tile1CenterX = getX() + TILE_WIDTH/2.0f;
	float tile2CenterX = aTile->getX() + TILE_WIDTH/2.0f;
	float tile1CenterY = getY() + TILE_HEIGHT/2.0f;
	float tile2CenterY = aTile->getY() + TILE_HEIGHT/2.0f;

	// c = square root of a^2 + b^2
	float distance = sqrt(pow((tile1CenterX - tile2CenterX), 2) + pow((tile1CenterY - tile2CenterY), 2));

	return distance;
}

//=============================================================================
// Tile::getStraightLineDistance
// args: x and y coord of the point we are getting distance to
// return value: float representing the distance
// description: will find the "straight line" distance between a tile and a point
//=============================================================================
float Tile::getStraightLineDistance(float x, float y)
{
	float tile1CenterX = getX() + TILE_WIDTH/2.0f;
	float tile2CenterX = x + TILE_WIDTH/2.0f;
	float tile1CenterY = getY() + TILE_HEIGHT/2.0f;
	float tile2CenterY = y + TILE_HEIGHT/2.0f;

	// c = square root of a^2 + b^2
	float distance = sqrt(pow((tile1CenterX - tile2CenterX), 2) + pow((tile1CenterY - tile2CenterY), 2));

	return distance;
}

//=============================================================================
// Tile::getTileTypeString
// args: none
// return value: pointer to char
// description: based on the tile type will return corresponding tile name
//=============================================================================
std::string Tile::getTileTypeString(std::string& str)
{
	char tileTpString[10] = { };

	// 0 -> WATER, 1 -> GRASS, 2 -> STONE
	switch(type)
	{
	case mushroomWarNS::WATER:
		sprintf(tileTpString, WATER_TILE_NAME);
		break;
	case mushroomWarNS::GRASS:
		sprintf(tileTpString, GRASS_TILE_NAME);
		break;
	case mushroomWarNS::STONE:
		sprintf(tileTpString, STONE_TILE_NAME);
		break;
	default:
		sprintf(tileTpString, "???");
	}

	str = tileTpString;
	return tileTpString;
}