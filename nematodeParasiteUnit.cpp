#include "nematodeParasiteUnit.h"

//=============================================================================
// constructor
//=============================================================================
NematodeParasiteUnit::NematodeParasiteUnit() : NematodeUnit()
{
	//nematodeType = mushroomWarNS::PARASITIC;
}

NematodeParasiteUnit::NematodeParasiteUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : NematodeUnit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	//nematodeType = mushroomWarNS::PARASITIC;
	isUltDestReached = false;
}

//=============================================================================
// NematodeParasiteUnit::countDown
// args: None
// return value: void
// description: will decrement the countdown by one
//=============================================================================
void NematodeParasiteUnit::countDown()
{
	--countdown;
}

//=============================================================================
// NematodeParasiteUnit::isTimeToInvade
// args: None
// return value: bool
// description: returns true if the countdown is <= 0, false otherwise
//=============================================================================
bool NematodeParasiteUnit::isTimeToInvade()
{
	if (countdown <= 0)
	{
		return true;
	}

	return false;
}

//=============================================================================
// NematodeParasiteUnit::getFinalGoal
// args: Tile*, should be the tile this nematode parasite is currently on
// return value: void
// description: based on the positions of all the mushrooms, this function will
//				get the center row/column in the middle of them all. It will 
//				set the finalGoalRow and finalGoalCol member variables.
//=============================================================================
void NematodeParasiteUnit::getFinalGoal(Tile* currTile)
{
	// stub
}

//=============================================================================
// NematodeParasiteUnit::canTraverse
// args: a pointer to a tile
// return value: bool
// description: returns true if this nematode parasite can traverse the given
//				type of tile.
//=============================================================================
bool NematodeParasiteUnit::canTraverse(Tile* aTile)
{
	if(!NematodeUnit::canTraverse(aTile))
	{
		return false;
	}
	return true;
}

bool NematodeParasiteUnit::canBeAttackedBack()
{
	// stub
	return true;
}

//=============================================================================
// NematodeParasiteUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: void
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers.
//=============================================================================
void NematodeParasiteUnit::attack(Entity* opposingUnit, float accuracySitMod, float evsSitMod)
{
	opposingUnit->isAttackedBy(this,1.0f,1.0f);
	if (opposingUnit->getHealth() < 0.0f)
		opposingUnit->setHealth(0.0f);
}

//=============================================================================
// NematodeParasiteUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void NematodeParasiteUnit::isAttackedBy(NematodeParasiteUnit* npu, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeParasiteUnit::isAttackedBy(NematodeBurrowerUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeParasiteUnit::isAttackedBy(NematodeRangedUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeParasiteUnit::isAttackedBy(NematodeCloseCombatUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeParasiteUnit::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// close combat units cannot attack the nematode parasite unit
	return;
}

void NematodeParasiteUnit::isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float evsSitMod)
{
	// parasite units cannot attack the nematode parasite unit
	return;
}

void NematodeParasiteUnit::isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float evsSitMod)
{
	// colonizer units cannot attack the nematode parasite unit
	return;
}

void NematodeParasiteUnit::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushRangedUnit->getAttackDmg());
}

Tile* NematodeParasiteUnit::setUpPathFinding(Tile* aTile, std::vector<Tile*>& attackableTiles, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid, mushroomWarNS::GAME_PHASE& gamePhase)
{
	int size = fieldGrid.size();
	std::vector<Tile*> points;
	points.clear();
	Tile* currTile;
	float xVal = 0;
	float yVal = 0;
	int counter = 0;

	gamePhase = mushroomWarNS::ENEMY_FIND_PATH;

	// for each field grid, keep track of the mushroom tiles
	for(int fieldInd = 0; fieldInd < size; fieldInd++)
	{
		currTile = fieldGrid[fieldInd];
		if(currTile->hasOccupyingMushroom())
		{
			points.push_back(currTile);
		}
	}

	// for each mushroom tile, sum the x and y locations
	int sizePts = points.size();
	for(int pointInd = 0; pointInd < sizePts; pointInd++)
	{
		xVal += points[pointInd]->getX() + TILE_WIDTH/2.0f;
		yVal += points[pointInd]->getY() + TILE_HEIGHT/2.0f;
		++counter;
	}

	// get the centroid point
	xVal = xVal/(float)counter;
	yVal = yVal/(float)counter;

	// get the tile index
	int yPos = yVal/unitNS::WIDTH;
	int xPos = xVal/unitNS::WIDTH;
	size_t fieldInd = (size_t)((FIELD_GRID_WIDTH_TILES * yPos) + xPos);
	currTile = fieldGrid[fieldInd];

	// check if this tile is traversable by a close combat nematode unit 
	NematodeParasiteUnit nemPU;
	if(nemPU.canTraverse(currTile))
		return currTile;
	else
	{
		Tile* closestTile = NULL;
		Tile* currFieldTile = NULL;
		float straightLineDist = -1;
		float smallestStraightLineDist = -1;
		// if not, need to find a traversable tile closest to this tile
		for(int fieldInd = 0; fieldInd < size; fieldInd++)
		{
			// for each tile, get the straight line distance to the centroid tile we cannot land on
			currFieldTile = fieldGrid[fieldInd];
			straightLineDist = currFieldTile->getStraightLineDistance(currTile);
			if((smallestStraightLineDist < 0 || straightLineDist < smallestStraightLineDist || (straightLineDist <= smallestStraightLineDist && currFieldTile == aTile) ) && (currFieldTile == aTile || nemPU.canTraverse(currFieldTile)))
			{
				// first time calculating the straight line distance OR found a smaller straight line distance
				smallestStraightLineDist = straightLineDist;
				closestTile = currFieldTile;
			}
		}

		return closestTile;
	}

	return NULL;
}

void NematodeParasiteUnit::resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	if(!areEnemyMushTilesLeft(getEnemyMushTilesChecked(), fieldGrid))
	{
		// no other enemies on board to check with. Nothing this unit can do this turn
		setActnThisTurn(false);

		clearEnemyMushTilesChecked();
	}
}

int NematodeParasiteUnit::getStartFrame()
{
	return 24;
}

int NematodeParasiteUnit::getEndFrame()
{
	return 25;
}

int NematodeParasiteUnit::getStartFightFrame()
{
	return PARASITE_NEM_ST_FIGHT_FRAME;
}

int NematodeParasiteUnit::getEndFightFrame()
{
	// the parasite nematode does not have an attack animation, so the end frame is the same as the start frame
	return PARASITE_NEM_ST_FIGHT_FRAME;
}

int NematodeParasiteUnit::getFlinchFrame()
{
	return PARASITE_NEM_FLINCH_FRAME;
}

std::string NematodeParasiteUnit::getUnitName()
{
	return NEMA_PARASITE_UNIT_NAME;
}

std::vector<int> NematodeParasiteUnit::movingCompletedAction(Tile* aTile, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid)
{
	std::vector<int> tileIndices;

	if(!isUltDestReached)
		return tileIndices;

	NematodeCloseCombatUnit nccu;

	int count = 0;

	int currInd = (FIELD_GRID_WIDTH_TILES*aTile->getYPos()) + aTile->getXPos();

	if(currInd-1 >= 0 && !fieldGrid[currInd-1]->hasOccupyingEntity() && !fieldGrid[currInd-1]->hasOccupyingMushroom() && aTile->getXPos() > 0 && count < 3 && nccu.canTraverse(fieldGrid[currInd-1]))
	{
		// found an empty space to the left of the mushroom
		count++;
		fieldGrid[currInd-1]->setOccupyingEnt(new NematodeCloseCombatUnit(80.0f, 25.0f, 2,1, 0, 90.0f, 100.0f));
		tileIndices.push_back(currInd-1);
	}
	
	if(currInd+1 < FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES && !fieldGrid[currInd+1]->hasOccupyingEntity() && !fieldGrid[currInd+1]->hasOccupyingMushroom() && aTile->getXPos() < (FIELD_GRID_WIDTH_TILES-1) && count < 3 && nccu.canTraverse(fieldGrid[currInd+1]))
	{
		// found an empty space to the right of the mushroom
		count++;
		fieldGrid[currInd+1]->setOccupyingEnt(new NematodeCloseCombatUnit(80.0f, 25.0f, 2,1, 0, 90.0f, 100.0f));
		tileIndices.push_back(currInd+1);
	}

	if(currInd-FIELD_GRID_WIDTH_TILES >= 0 && !fieldGrid[currInd-FIELD_GRID_WIDTH_TILES]->hasOccupyingEntity() && !fieldGrid[currInd-FIELD_GRID_WIDTH_TILES]->hasOccupyingMushroom() && aTile->getYPos() > 0 && count < 3 && nccu.canTraverse(fieldGrid[currInd-FIELD_GRID_WIDTH_TILES]))
	{
		count++;
		fieldGrid[currInd-FIELD_GRID_WIDTH_TILES]->setOccupyingEnt(new NematodeCloseCombatUnit(80.0f, 25.0f, 2,1, 0, 90.0f, 100.0f));
		tileIndices.push_back(currInd-FIELD_GRID_WIDTH_TILES);
	}
	
	if(currInd+FIELD_GRID_WIDTH_TILES < FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES && !fieldGrid[currInd+FIELD_GRID_WIDTH_TILES]->hasOccupyingEntity() && !fieldGrid[currInd+FIELD_GRID_WIDTH_TILES]->hasOccupyingMushroom() && aTile->getYPos() < (FIELD_GRID_HEIGHT_TILES - 1) && count < 3 && nccu.canTraverse(fieldGrid[currInd+FIELD_GRID_WIDTH_TILES]))
	{
		count++;
		fieldGrid[currInd+FIELD_GRID_WIDTH_TILES]->setOccupyingEnt(new NematodeCloseCombatUnit(80.0f, 25.0f, 2,1, 0, 90.0f, 100.0f));
		tileIndices.push_back(currInd+FIELD_GRID_WIDTH_TILES);
	}

	// this unit should die once it releases the parasites it's hosting
	setHealth(0.0f);

	return tileIndices;
}

// return value indicates if this unit should now be dead or not
bool NematodeParasiteUnit::pathFoundPostProcessing(bool ultDestReached)
{
	isUltDestReached = ultDestReached;
	return true;
}