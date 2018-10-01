#include "mushroomParasiteUnit.h"

//=============================================================================
// constructor
//=============================================================================
MushroomParasiteUnit::MushroomParasiteUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, int psnEff, float acc, float evs) : Unit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	isMushroomUnit = true;
	psnEffectiveness = psnEff;
	//nematodeType = mushroomWarNS::NONE_TYPE;
}

bool MushroomParasiteUnit::canBeAttackedBack()
{
	return true;
}

//=============================================================================
// MushroomParasiteUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: void
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers.
//=============================================================================
void MushroomParasiteUnit::attack(Entity* opposingUnit, float accSitMod, float evsSitMod)
{
	opposingUnit->isAttackedBy(this,1.0f,1.0f);
	
	if (opposingUnit->getHealth() < 0.0f)
		opposingUnit->setHealth(0.0f);
}

//=============================================================================
// MushroomParasiteUnit::affectAfterDeath
// args: array of tile pointers
// return value: void
// description: will set the poison status of enemy units that are within this unit's
//				poison radius
//=============================================================================
void MushroomParasiteUnit::affectAfterDeath(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid)
{
	Tile* thisTile = NULL;
	float distance;
	float x = getX();
	float y = getY();

	// for each tile in the field grid, check if it's within this unit's poison radius. If so, set the unit on the tile to be poisoned
	size_t numTiles = fieldGrid.size();
	for (size_t i = 0; i < numTiles; i++)
	{
		thisTile = fieldGrid[i];

		distance = thisTile->getStraightLineDistance(x, y);

		if( thisTile->hasOccupyingEntity() && distance <= (psnEffectiveness * unitNS::WIDTH) )
		{
			thisTile->getOccupyingEnt()->setPoison(getPsnEffectiveness());
		}
	}
}

//=============================================================================
// MushroomParasiteUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void MushroomParasiteUnit::isAttackedBy(NematodeParasiteUnit* nemParasiteUnit, float accuracySitMod, float evsSitMod)
{
	// nematode parasites do not attack
	return;
}
void MushroomParasiteUnit::isAttackedBy(NematodeBurrowerUnit* nemBurrowerUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemBurrowerUnit->getAttackDmg());
}
void MushroomParasiteUnit::isAttackedBy(NematodeRangedUnit* nemRangedUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemRangedUnit->getAttackDmg());
}
void MushroomParasiteUnit::isAttackedBy(NematodeCloseCombatUnit* nemCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemCloseCombatUnit->getAttackDmg());
}
void MushroomParasiteUnit::isAttackedBy(MushroomCloseCombatUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomParasiteUnit::isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomParasiteUnit::isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomParasiteUnit::isAttackedBy(MushroomRangedUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}

int MushroomParasiteUnit::getStartFrame()
{
	return 16;
}

int MushroomParasiteUnit::getEndFrame()
{
	return 17;
}

int MushroomParasiteUnit::getStartFightFrame()
{
	return PARASITE_MUSH_ST_FIGHT_FRAME;
}

int MushroomParasiteUnit::getEndFightFrame()
{
	return PARASITE_MUSH_END_FIGHT_FRAME;
}

int MushroomParasiteUnit::getFlinchFrame()
{
	return PARASITE_MUSH_FLINCH_FRAME;
}

std::string MushroomParasiteUnit::getUnitName()
{
	return MUSH_PARASITE_UNIT_NAME;
}