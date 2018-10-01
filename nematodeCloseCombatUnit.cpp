#include "nematodeCloseCombatUnit.h"

//=============================================================================
// constructor
//=============================================================================
NematodeCloseCombatUnit::NematodeCloseCombatUnit() : NematodeUnit()
{
	//nematodeType = mushroomWarNS::CLOSE;
}

NematodeCloseCombatUnit::NematodeCloseCombatUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : NematodeUnit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	//nematodeType = mushroomWarNS::CLOSE;
	getClosestOpponent = true;
}

//=============================================================================
// NematodeCloseCombatUnit::canTraverse
// args: pointer to a tile
// return value: bool
// description: returns true if this nematode can traverse the given tile (based on its tile type and env char type),
//				returns false otherwise.
//=============================================================================
bool NematodeCloseCombatUnit::canTraverse(Tile* aTile)
{
	EnvCharacteristic* envChar;
	mushroomWarNS::CHARACTERISTIC_TYPE ct;
	
	if(aTile->hasOccupyingEnvChar())
	{
		// checking if this unit can walk over the env characteristic on this tile or not
		envChar = aTile->getEnvChar();
		ct = envChar->getType();
		if(envChar->getIsSolid() && (ct == mushroomWarNS::C_TREE || ct == mushroomWarNS::ROCK))
		{
			return false;
		}
	}
	if(aTile->getTileType() == mushroomWarNS::WATER)
	{
		// close combat units cannot traverse water tiles
		return false;
	}
	if(!NematodeUnit::canTraverse(aTile))
	{
		return false;
	}

	return true;
}

bool NematodeCloseCombatUnit::canBeAttackedBack()
{
	return true;
}

//=============================================================================
// NematodeCloseCombatUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: void
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers.
//=============================================================================
void NematodeCloseCombatUnit::attack(Entity* opposingUnit, float accuracySitMod, float evsSitMod)
{
	opposingUnit->isAttackedBy(this,1.0f,1.0f);
	if (opposingUnit->getHealth() < 0.0f)
		opposingUnit->setHealth(0.0f);
}

//=============================================================================
// NematodeCloseCombatUnit::getEntityToAttack
// args: vector of pointers to tiles (representing list of tiles containing entities that can be attacked)
// return value: pointer to Entity
// description: returns a pointer to an Entity (unit, mushroom, or environmental characteristic)
//				depending on the unit calling this function. The Close combat attacks whatever is closest
//				to them at the time, enemy unit or mushroom.
//=============================================================================
Entity* NematodeCloseCombatUnit::getEntityToAttack(std::vector<Tile*> attackableTiles)
{
	size_t size = attackableTiles.size();
	float dist = 0.0f;
	float distMushUnit = -1.0f;
	Entity* closestMushUnit = NULL;
	float distMushroom = -1.0f;
	Entity* closestMushroom = NULL;
	Tile* currTile = NULL;
	// get the closest tile with a mushroom unit and closest with mushroom
	for(int ind = 0; ind < size; ind++)
	{
		currTile = attackableTiles[ind];
		dist = Utilities::getStraightLineDistance(getCenterX(), getCenterY(), currTile->getCenterX(), currTile->getCenterY());
		if(currTile->hasOccupyingEntity())
		{
			// check if this distance is < distMushUnit and distMushUnit is not -1
			if(dist < distMushUnit || distMushUnit < 0.0f)
			{
				distMushUnit = dist;
				closestMushUnit = currTile->getOccupyingEnt();
			}
		}
		else if(currTile->hasOccupyingMushroom())
		{
			if(dist < distMushroom || distMushroom < 0.0f)
			{
				distMushroom = dist;
				closestMushroom = currTile->getOccupyingMushroom();
			}
		}
	}

	// check which distance is shorter and not < 0
	if(distMushUnit < 0.0f && distMushroom < 0.0f)
		return NULL;
	else if(distMushroom < 0.0f)
		return closestMushUnit;
	else if(distMushUnit < 0.0f || distMushroom < distMushUnit)
		return closestMushroom;
	else
		return closestMushUnit;
}

//=============================================================================
// NematodeCloseCombatUnit::getEntityTileToAttack
// args: vector of pointers to tiles (representing list of tiles containing entities that can be attacked)
// return value: pointer to Tile
// description: returns a pointer to a Tile
//				depending on the unit calling this function. The Close combat attacks whatever is closest
//				to them at the time, tile containing enemy unit or mushroom.
//=============================================================================
Tile* NematodeCloseCombatUnit::getEntityTileToAttack(std::vector<Tile*> attackableTiles)
{
	size_t size = attackableTiles.size();
	float dist = 0.0f;
	float distMushUnit = -1.0f;
	Tile* closestMushUnitTile = NULL;
	float distMushroom = -1.0f;
	Tile* closestMushroomTile = NULL;
	Tile* currTile = NULL;
	// get the closest tile with a mushroom unit and closest with mushroom
	for(int ind = 0; ind < size; ind++)
	{
		currTile = attackableTiles[ind];
		dist = Utilities::getStraightLineDistance(getCenterX(), getCenterY(), currTile->getCenterX(), currTile->getCenterY());
		if(currTile->hasOccupyingEntity())
		{
			// check if this distance is < distMushUnit and distMushUnit is not -1
			if(dist < distMushUnit || distMushUnit < 0.0f)
			{
				distMushUnit = dist;
				closestMushUnitTile = currTile;
			}
		}
		else if(currTile->hasOccupyingMushroom())
		{
			if(dist < distMushroom || distMushroom < 0.0f)
			{
				distMushroom = dist;
				closestMushroomTile = currTile;
			}
		}
	}

	// check which distance is shorter and not < 0
	if(distMushUnit < 0.0f && distMushroom < 0.0f)
		return NULL;
	else if(distMushroom < 0.0f)
		return closestMushUnitTile;
	else if(distMushUnit < 0.0f || distMushroom < distMushUnit)
		return closestMushroomTile;
	else
		return closestMushUnitTile;
}

//=============================================================================
// NematodeCloseCombatUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void NematodeCloseCombatUnit::isAttackedBy(NematodeParasiteUnit* npu, float accuracySitMod, float evsSitMod)
{
	// stub
	//Tile* aTile = NULL;
	//aTile->hasOccupyingEntity();
}

void NematodeCloseCombatUnit::isAttackedBy(NematodeBurrowerUnit* nemBurrowerUnit, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeCloseCombatUnit::isAttackedBy(NematodeRangedUnit* nemRangedUnit, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeCloseCombatUnit::isAttackedBy(NematodeCloseCombatUnit* nemCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeCloseCombatUnit::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushCloseCombatUnit->getAttackDmg());
}

void NematodeCloseCombatUnit::isAttackedBy(MushroomParasiteUnit* mushParasiteUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushParasiteUnit->getAttackDmg());
}

void NematodeCloseCombatUnit::isAttackedBy(MushroomColonizerUnit* mushColonizerUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushColonizerUnit->getAttackDmg());
}

void NematodeCloseCombatUnit::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	// ranged units do half damage to close ranged ones
	float atkDmg = mushRangedUnit->getAttackDmg();
	setHealth(getHealth() - (atkDmg / 2.0f));
}

Tile* NematodeCloseCombatUnit::setUpPathFinding(Tile* aTile, std::vector<Tile*>& attackableTiles, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid, mushroomWarNS::GAME_PHASE& gamePhase)
{
	Tile* destTile = NULL;

	setGetClosestOpponent(true);

	if(attackableTiles.empty())
	{
		// get the closest tile
		destTile = aTile->getClosestOpposingTile(false, enemyMushTilesChecked, fieldGrid);
		if(!destTile)
		{
			// since no opposing enemy mushroom units on the board, we'll look for closest opposing mushroom to attack
			destTile = aTile->getClosestOpposingTile(true, enemyMushTilesChecked, fieldGrid);
		}

		// find the shortest path (solution) to the closest tile
		gamePhase = mushroomWarNS::ENEMY_FIND_PATH;
	}
	else
	{
		// set game phase to enemy attack to the attack logic commences during the update function
		gamePhase = mushroomWarNS::ENEMY_ATTACK;
	}

	return destTile;
}

void NematodeCloseCombatUnit::resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	if(!areEnemyMushTilesLeft(getEnemyMushTilesChecked(), fieldGrid))
	{
		// no other enemies on board to check with. Nothing this unit can do this turn
		setActnThisTurn(false);

		clearEnemyMushTilesChecked();

		lookingForMushroom = false;
	}
}

int NematodeCloseCombatUnit::getStartFrame()
{
	return 20;
}

int NematodeCloseCombatUnit::getEndFrame()
{
	return 21;
}

int NematodeCloseCombatUnit::getStartFightFrame()
{
	return CLOSE_COMBAT_NEM_FIGHT_ST_FRAME;
}

int NematodeCloseCombatUnit::getEndFightFrame()
{
	return CLOSE_COMBAT_NEM_FIGHT_END_FRAME;
}

int NematodeCloseCombatUnit::getFlinchFrame()
{
	return CLOSE_COMBAT_NEM_FLINCH_FRAME;
}

std::string NematodeCloseCombatUnit::getUnitName()
{
	return NEMA_CLOSE_COMBAT_UNIT_NAME;
}