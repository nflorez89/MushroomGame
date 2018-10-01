#include "MushroomColonizerUnit.h"

//=============================================================================
// constructor
//=============================================================================
MushroomColonizerUnit::MushroomColonizerUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : Unit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	isMushroomUnit = true;
	//nematodeType = mushroomWarNS::NONE_TYPE;
}

bool MushroomColonizerUnit::canBeAttackedBack()
{
	return true;
}

//=============================================================================
// MushroomColonizerUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: void
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers.
//=============================================================================
void MushroomColonizerUnit::attack(Entity* opposingUnit, float accSitMod, float evsSitMod)
{
	opposingUnit->isAttackedBy(this,1.0f,1.0f);
	if (opposingUnit->getHealth() < 0.0f)
		opposingUnit->setHealth(0.0f);
}

//=============================================================================
// MushroomColonizerUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void MushroomColonizerUnit::isAttackedBy(NematodeParasiteUnit* nemParasiteUnit, float accuracySitMod, float evsSitMod)
{
	// nematode parasites do not attack
	return;
}
void MushroomColonizerUnit::isAttackedBy(NematodeBurrowerUnit* nemBurrowerUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemBurrowerUnit->getAttackDmg());
}
void MushroomColonizerUnit::isAttackedBy(NematodeRangedUnit* nemRangedUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemRangedUnit->getAttackDmg());
}
void MushroomColonizerUnit::isAttackedBy(NematodeCloseCombatUnit* nemCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemCloseCombatUnit->getAttackDmg());
}
void MushroomColonizerUnit::isAttackedBy(MushroomCloseCombatUnit* mccu, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomColonizerUnit::isAttackedBy(MushroomParasiteUnit* mpu, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomColonizerUnit::isAttackedBy(MushroomColonizerUnit* mcu, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomColonizerUnit::isAttackedBy(MushroomRangedUnit* mru, float accuracySitMod, float evsSitMod)
{
	//stub
}

int MushroomColonizerUnit::getStartFrame()
{
	return 18;
}

int MushroomColonizerUnit::getEndFrame()
{
	return 19;
}

int MushroomColonizerUnit::getStartFightFrame()
{
	return COLONIZER_MUSH_ST_FIGHT_FRAME;
}

int MushroomColonizerUnit::getEndFightFrame()
{
	return COLONIZER_MUSH_END_FIGHT_FRAME;
}

int MushroomColonizerUnit::getFlinchFrame()
{
	return COLONIZER_MUSH_FLINCH_FRAME;
}

std::string MushroomColonizerUnit::getUnitName()
{
	return MUSH_COLONIZER_UNIT_NAME;
}

void MushroomColonizerUnit::whileMovingAction(Tile* aTile)
{
	if(!aTile->getIsTaken() && aTile->getTileType() == mushroomWarNS::GRASS)
	{
		aTile->setIsTaken(true);
		Tile::s_tilesTakenOver++;
	}
}

//=============================================================================
// MushroomColonizerUnit::movingCompletedAction
// args: Tile*, array of pointers to tiles
// return value: void
// description: should be called once the colonizer unit has completed moving. 
//				Sets the tiles to taken that are within this unit's spread radius.
//=============================================================================
std::vector<int> MushroomColonizerUnit::movingCompletedAction(Tile* aTile, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid)
{
	Tile* thisTile = NULL;
	int spreadRadius;
	float distance;

	std::vector<int> tileIndices;
	tileIndices.clear();

	// check if the tile is the one this unit is occupying
	if (!aTile->hasOccupyingEntity() || aTile->getOccupyingEnt() != this)
	{
		return tileIndices;
	}

	// for each tile in the field grid, check if it's within this unit's spread radius. If so, set the tile to taken.
	size_t numTiles = fieldGrid.size();
	for (size_t i = 0; i < numTiles; i++)
	{
		thisTile = fieldGrid[i];
		if(!thisTile->getIsTaken() && thisTile->getTileType() == mushroomWarNS::GRASS)
		{
			spreadRadius = getSpreadRadius();

			distance = thisTile->getStraightLineDistance(aTile);

			if(distance <= spreadRadius*unitNS::WIDTH)
			{
				thisTile->setIsTaken(true);
				Tile::s_tilesTakenOver++;
			}
		}
	}

	return tileIndices;
}