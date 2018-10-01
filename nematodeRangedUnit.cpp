#include "nematodeRangedUnit.h"

//=============================================================================
// constructor
//=============================================================================
NematodeRangedUnit::NematodeRangedUnit() : NematodeUnit()
{
	//nematodeType = mushroomWarNS::RANGED;
}

NematodeRangedUnit::NematodeRangedUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : NematodeUnit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	bonusChars.push_back(mushroomWarNS::HILL);
	//nematodeType = mushroomWarNS::RANGED;
}

//=============================================================================
// NematodeRangedUnit::canTraverse
// args: mushroomWarNS::TILE_TYPE type
// return value: bool
// description: returns true if this nematode can traverse the given tile type,
//				returns false otherwise.
//=============================================================================
bool NematodeRangedUnit::canTraverse(Tile* aTile)
{
	EnvCharacteristic* envChar;
	mushroomWarNS::CHARACTERISTIC_TYPE ct;
	
	if(aTile->hasOccupyingEnvChar())
	{
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

bool NematodeRangedUnit::canBeAttackedBack()
{
	// stub
	return true;
}

//=============================================================================
// NematodeRangedUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: void
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers.
//=============================================================================
void NematodeRangedUnit::attack(Entity* opposingUnit, float accuracySitMod, float evsSitMod)
{
	opposingUnit->isAttackedBy(this,1.0f,1.0f);
	if (opposingUnit->getHealth() < 0.0f)
		opposingUnit->setHealth(0.0f);
}

//=============================================================================
// NematodeRangedUnit::getEntityToAttack
// args: vector of pointers to tiles (representing list of tiles containing entities that can be attacked)
// return value: pointer to Entity
// description: returns a pointer to an Entity (unit, mushroom, or environmental characteristic)
//				depending on the unit calling this function. The ranged unit attacks whatever is farthest
//				from them at the time, enemy unit or mushroom.
//=============================================================================
Entity* NematodeRangedUnit::getEntityToAttack(std::vector<Tile*> attackableTiles)
{
	size_t size = attackableTiles.size();
	float dist = 0.0f;
	float distMushUnit = -1.0f;
	Entity* farthestMushUnit = NULL;
	float distMushroom = -1.0f;
	Entity* farthestMushroom = NULL;
	Tile* currTile = NULL;
	// get the farthest tile with a mushroom unit and farthest with mushroom
	for(int ind = 0; ind < size; ind++)
	{
		currTile = attackableTiles[ind];
		dist = Utilities::getStraightLineDistance(getCenterX(), getCenterY(), currTile->getCenterX(), currTile->getCenterY());
		if(currTile->hasOccupyingEntity())
		{
			// check if we found a farther away mushroom unit
			if(dist > distMushUnit)
			{
				distMushUnit = dist;
				farthestMushUnit = currTile->getOccupyingEnt();
			}
		}
		else if(currTile->hasOccupyingMushroom())
		{
			// check if we found a farther away mushroom
			if(dist > distMushroom)
			{
				distMushroom = dist;
				farthestMushroom = currTile->getOccupyingMushroom();
			}
		}
	}

	// check which distance is shorter and not < 0
	if(distMushUnit < 0.0f && distMushroom < 0.0f)
		return NULL;
	else if(distMushroom < 0.0f)
		return farthestMushUnit;
	else if(distMushUnit < 0.0f || distMushroom > distMushUnit)
		return farthestMushroom;
	else
		return farthestMushUnit;
}

//=============================================================================
// NematodeRangedUnit::getEntityTileToAttack
// args: vector of pointers to tiles (representing list of tiles containing entities that can be attacked)
// return value: pointer to Entity
// description: returns a pointer to a Tile
//				depending on the unit calling this function. The ranged unit attacks whatever is farthest
//				from them at the time, enemy unit tile or mushroom tile.
//=============================================================================
Tile* NematodeRangedUnit::getEntityTileToAttack(std::vector<Tile*> attackableTiles)
{
	size_t size = attackableTiles.size();
	float dist = 0.0f;
	float distMushUnit = -1.0f;
	Tile* farthestMushUnitTile = NULL;
	float distMushroom = -1.0f;
	Tile* farthestMushroomTile = NULL;
	Tile* currTile = NULL;
	// get the farthest tile with a mushroom unit and farthest with mushroom
	for(int ind = 0; ind < size; ind++)
	{
		currTile = attackableTiles[ind];
		dist = Utilities::getStraightLineDistance(getCenterX(), getCenterY(), currTile->getCenterX(), currTile->getCenterY());
		if(currTile->hasOccupyingEntity())
		{
			// check if we found a farther away mushroom unit
			if(dist > distMushUnit)
			{
				distMushUnit = dist;
				farthestMushUnitTile = currTile;
			}
		}
		else if(currTile->hasOccupyingMushroom())
		{
			// check if we found a farther away mushroom
			if(dist > distMushroom)
			{
				distMushroom = dist;
				farthestMushroomTile = currTile;
			}
		}
	}

	// check which distance is shorter and not < 0
	if(distMushUnit < 0.0f && distMushroom < 0.0f)
		return NULL;
	else if(distMushroom < 0.0f)
		return farthestMushUnitTile;
	else if(distMushUnit < 0.0f || distMushroom > distMushUnit)
		return farthestMushroomTile;
	else
		return farthestMushUnitTile;
}

//=============================================================================
// NematodeRangedUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void NematodeRangedUnit::isAttackedBy(NematodeParasiteUnit* npu, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeRangedUnit::isAttackedBy(NematodeBurrowerUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeRangedUnit::isAttackedBy(NematodeRangedUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeRangedUnit::isAttackedBy(NematodeCloseCombatUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeRangedUnit::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	// mushroom close combat units do double damage to ranged nematodes
	float atkDmg = mushCloseCombatUnit->getAttackDmg();
	setHealth(getHealth() - (atkDmg * 2));
}

void NematodeRangedUnit::isAttackedBy(MushroomParasiteUnit* mushParasiteUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushParasiteUnit->getAttackDmg());
}

void NematodeRangedUnit::isAttackedBy(MushroomColonizerUnit* mushColonizerUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushColonizerUnit->getAttackDmg());
}

void NematodeRangedUnit::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushRangedUnit->getAttackDmg());
}

Tile* NematodeRangedUnit::setUpPathFinding(Tile* aTile, std::vector<Tile*>& attackableTiles, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid, mushroomWarNS::GAME_PHASE& gamePhase)
{
	if(attackableTiles.empty())
	{
		if(!getLookingForMushroom())
		{
			// if not looking for mushroom, then check if there are any enemy units to path find to
			int size = fieldGrid.size();
			bool foundUnit = false;
			for(int fieldInd = 0; fieldInd < size && !foundUnit; fieldInd++)
			{
				if( fieldGrid[fieldInd]->hasOccupyingEntity() && fieldGrid[fieldInd]->getOccupyingEnt()->isThisMushroomUnit())
					foundUnit = true;
			}

			if(!foundUnit)
			{
				// no units to go after, so have this ranged unit go after mushrooms
				setLookingForMushroom(true);
			}
			
		}
		// get the closest tile with an opposing unit or opposing mushroom
		setGetClosestOpponent(true);

		// find the shortest path (solution) to the closest tile
		gamePhase = mushroomWarNS::ENEMY_FIND_PATH;
	}
	else
	{
		// set game phase to enemy attack to the attack logic commences during the update function
		gamePhase = mushroomWarNS::ENEMY_ATTACK;
	}

	return NULL;
}

void NematodeRangedUnit::resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	setActnThisTurn(false);
}

int NematodeRangedUnit::getStartFrame()
{
	return 22;
}

int NematodeRangedUnit::getEndFrame()
{
	return 23;
}

int NematodeRangedUnit::getStartFightFrame()
{
	return RANGED_NEM_FIGHT_ST_FRAME;
}

int NematodeRangedUnit::getEndFightFrame()
{
	return RANGED_NEM_FIGHT_END_FRAME;
}

int NematodeRangedUnit::getFlinchFrame()
{
	return RANGED_NEM_FLINCH_FRAME;
}

std::string NematodeRangedUnit::getUnitName()
{
	return NEMA_RANGED_UNIT_NAME;
}