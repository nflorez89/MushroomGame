#include "nematodeUnit.h"

//=============================================================================
// constructor
//=============================================================================
NematodeUnit::NematodeUnit() : Unit()
{
	isMushroomUnit = false;
}

NematodeUnit::NematodeUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : Unit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	isMushroomUnit = false;
}

//=============================================================================
// NematodeUnit::canTraverse
// args: pointer to a tile
// return value: bool
// description: returns false if the tile has an occupying mushroom, true otherwise
//=============================================================================
bool NematodeUnit::canTraverse(Tile* aTile)
{
	if(aTile->hasOccupyingEntity() || aTile->hasOccupyingMushroom())
		return false;

	return true;
}

//=============================================================================
// NematodeUnit::spreadSpores
// args: Tile*, should be the tile that this nematode is currently occupying
// return value: void
// description: will set the adjacent tiles isInfected to true
//=============================================================================
void NematodeUnit::spreadSpores(Tile* currTile)
{
	// stub
}

//=============================================================================
// NematodeUnit::areEnemyMushTilesLeft
// args: vector of pointers to tiles, array of pointers to tiles
// return value: bool
// description: will return true if there are any tiles in the field grid that
//				are mushroom units or mushrooms that are not in the given vector
//=============================================================================
bool NematodeUnit::areEnemyMushTilesLeft(std::vector<Tile*> mushsChecked, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	if(areEnemyMushUnitTilesLeft(mushsChecked, fieldGrid))
		return true;

	if(areEnemyMushroomTilesLeft(mushsChecked, fieldGrid))
	{
		lookingForMushroom = true;
		return true;
	}

	// no enemy tiles of any kind, unit nor mushroom, are left to check
	return false;
}

//=============================================================================
// NematodeUnit::areEnemyMushUnitTilesLeft
// args: vector of pointers to tiles, array of pointers to tiles
// return value: bool
// description: will return true if there are any tiles in the field grid that
//				are mushroom units
//=============================================================================
bool NematodeUnit::areEnemyMushUnitTilesLeft(std::vector<Tile*> mushsChecked, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	int fieldSize = fieldGrid.size();
	int mushsCheckedSize = mushsChecked.size();
	Tile* fieldTile = NULL;
	Tile* mushsTile = NULL;

	for(int fieldInd = 0; fieldInd < fieldSize; fieldInd++)
	{
		fieldTile = fieldGrid[fieldInd];
		if(fieldTile->hasOccupyingEntity() && fieldTile->getOccupyingEnt()->isThisMushroomUnit())
		{
			for(int mushsInd = 0; mushsInd < mushsCheckedSize; mushsInd++)
			{
				mushsTile = mushsChecked[mushsInd];

				// check if the field tile is in the list of tiles we already checked or not
				if(fieldTile != mushsTile)
					return true;
				else
					break;
			}
		}
	}
	return false;
}

//=============================================================================
// NematodeUnit::areEnemyMushroomTilesLeft
// args: vector of pointers to tiles, array of pointers to tiles
// return value: bool
// description: will return true if there are any tiles in the field grid that
//				are mushrooms that are not in the given vector
//=============================================================================
bool NematodeUnit::areEnemyMushroomTilesLeft(std::vector<Tile*> mushsChecked, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	int fieldSize = fieldGrid.size();
	int mushsCheckedSize = mushsChecked.size();
	Tile* fieldTile = NULL;
	Tile* mushsTile = NULL;

	for(int fieldInd = 0; fieldInd < fieldSize; fieldInd++)
	{
		fieldTile = fieldGrid[fieldInd];
		if(fieldTile->hasOccupyingMushroom())
		{
			for(int mushsInd = 0; mushsInd < mushsCheckedSize; mushsInd++)
			{
				mushsTile = mushsChecked[mushsInd];

				// check if the field tile is in the list of tiles we already checked or not
				if(fieldTile != mushsTile)
					return true;
				else
					break;
			}
		}
	}
	return false;
}

//=============================================================================
// NematodeUnit::getEnemyMushroomTile
// args: vector of pointers to tiles
// return value: pointer to tile
// description: will return a pointer to the first tile in the vector 
//				that contains a mushroom. Will return NULL if no tiles in the
//				vector contain a mushroom.
//=============================================================================
Tile* NematodeUnit::getEnemyMushroomTile(std::vector<Tile*> tiles)
{
	Tile* currTile = NULL;
	int size = tiles.size();
	for (int tilesInd = 0; tilesInd < size; tilesInd++)
	{
		currTile = tiles[tilesInd];
		if (currTile->hasOccupyingMushroom())
			return currTile;
	}

	return NULL;
}

void NematodeUnit::setPoison(int sev)
{
	if(psnSeverity <= 0 && psnTurnsLeft <= 0)
	{
		psnSeverity = sev;
		psnTurnsLeft = sev;
	}
}