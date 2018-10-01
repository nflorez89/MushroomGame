#include "nematodeBurrowerUnit.h"

//=============================================================================
// constructor
//=============================================================================
NematodeBurrowerUnit::NematodeBurrowerUnit() : NematodeUnit()
{
	//nematodeType = mushroomWarNS::BURROWER;
}

NematodeBurrowerUnit::NematodeBurrowerUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : NematodeUnit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	//nematodeType = mushroomWarNS::BURROWER;
}

//=============================================================================
// NematodeBurrowerUnit::eatMycellium
// args: Tile*, should be the tile this nematode burrower is currently occupying
// return value: void
// description: will set the tile's isTaken member variable to false
//=============================================================================
void NematodeBurrowerUnit::eatMycellium(Tile* currTile)
{
	currTile->setIsTaken(false);
}

//=============================================================================
// NematodeBurrowerUnit::canTraverse
// args: a pointer to a tile
// return value: bool
// description: returns true if this nematode can traverse the given tile type,
//				returns false otherwise.
//=============================================================================
bool NematodeBurrowerUnit::canTraverse(Tile* aTile)
{
	EnvCharacteristic* envChar;
	mushroomWarNS::CHARACTERISTIC_TYPE ct;

	mushroomWarNS::TILE_TYPE tt = aTile->getTileType();
	
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
	
	if(tt == mushroomWarNS::WATER || tt == mushroomWarNS::STONE)
	{
		// close combat units cannot traverse water or stone tiles
		return false;
	}
	
	if(!NematodeUnit::canTraverse(aTile))
	{
		return false;
	}

	return true;
}

bool NematodeBurrowerUnit::canBeAttackedBack()
{
	// stub
	return true;
}

//=============================================================================
// NematodeBurrowerUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: void
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers.
//=============================================================================
void NematodeBurrowerUnit::attack(Entity* opposingUnit, float accuracySitMod, float evsSitMod)
{
	opposingUnit->isAttackedBy(this,1.0f,1.0f);
	if (opposingUnit->getHealth() < 0.0f)
		opposingUnit->setHealth(0.0f);
}

//=============================================================================
// NematodeBurrowerUnit::getEntityToAttack
// args: vector of pointers to tiles (representing list of tiles containing entities that can be attacked)
// return value: pointer to Entity
// description: returns a pointer to an Entity (unit, mushroom, or environmental characteristic)
//				depending on the unit calling this function. The burrower unit looks for mushrooms first.
//				If no mushrooms, then will attack one of the mushroom units.
//=============================================================================
Entity* NematodeBurrowerUnit::getEntityToAttack(std::vector<Tile*> attackableTiles)
{
	size_t size = attackableTiles.size();
	Entity* mushroomToAttack = NULL;
	Entity* mushUnitToAttack = NULL;

	// look through the attackable tiles vector until you find a mushroom, then exit the loop. 
	// Set mushUnitAttack to the first mush unit found in the list if we have not found a mushroom to attack yet.
	for(int ind = 0; ind < size && !mushroomToAttack; ind++)
	{
		if(attackableTiles[ind]->hasOccupyingMushroom())
		{
			mushroomToAttack = attackableTiles[ind]->getOccupyingMushroom();
		}

		if(attackableTiles[ind]->hasOccupyingEntity() && !mushUnitToAttack)
		{
			mushUnitToAttack = attackableTiles[ind]->getOccupyingEnt();
		}
	}

	// if we found a mushroom to attack, then return that, otherwise return the unit we found
	if(mushroomToAttack)
		return mushroomToAttack;

	return mushUnitToAttack;

}

//=============================================================================
// NematodeBurrowerUnit::getEntityTileToAttack
// args: vector of pointers to tiles (representing list of tiles containing entities that can be attacked)
// return value: pointer to Tile
// description: returns a pointer to a Tile
//				depending on the unit calling this function. The burrower unit looks for mushrooms first.
//				If no mushrooms, then will attack one of the mushroom units.
//=============================================================================
Tile* NematodeBurrowerUnit::getEntityTileToAttack(std::vector<Tile*> attackableTiles)
{
	size_t size = attackableTiles.size();
	Tile* mushroomTileToAttack = NULL;
	Tile* mushUnitTileToAttack = NULL;

	// look through the attackable tiles vector until you find a mushroom, then exit the loop. 
	// Set mushUnitTileAttack to the first tile with a mush unit found in the list if we have not found a mushroom tile to attack yet.
	for(int ind = 0; ind < size && !mushroomTileToAttack; ind++)
	{
		if(attackableTiles[ind]->hasOccupyingMushroom())
		{
			mushroomTileToAttack = attackableTiles[ind];
		}

		if(attackableTiles[ind]->hasOccupyingEntity() && !mushUnitTileToAttack)
		{
			mushUnitTileToAttack = attackableTiles[ind];
		}
	}

	// if we found a mushroom to attack, then return the tile that has that mushroom, otherwise return the tile of the unit we found
	if(mushroomTileToAttack)
		return mushroomTileToAttack;

	return mushUnitTileToAttack;
}

//=============================================================================
// NematodeBurrowerUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void NematodeBurrowerUnit::isAttackedBy(NematodeParasiteUnit* npu, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeBurrowerUnit::isAttackedBy(NematodeBurrowerUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeBurrowerUnit::isAttackedBy(NematodeRangedUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeBurrowerUnit::isAttackedBy(NematodeCloseCombatUnit*, float accuracySitMod, float evsSitMod)
{
	// stub
}

void NematodeBurrowerUnit::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	// mushroom close combat units do 1/2 damage to burrower nematodes
	float atkDmg = mushCloseCombatUnit->getAttackDmg();
	setHealth(getHealth() - (atkDmg / 2.0f));
}

void NematodeBurrowerUnit::isAttackedBy(MushroomParasiteUnit* mushParasiteUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushParasiteUnit->getAttackDmg());
}

void NematodeBurrowerUnit::isAttackedBy(MushroomColonizerUnit* mushColonizerUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - mushColonizerUnit->getAttackDmg());
}

void NematodeBurrowerUnit::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float evsSitMod)
{
	// burrower units cannot be attacked by ranged units
	return;
}

Tile* NematodeBurrowerUnit::setUpPathFinding(Tile* aTile, std::vector<Tile*>& attackableTiles, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid, mushroomWarNS::GAME_PHASE& gamePhase)
{
	// check if there is a mushroom within the attack radius
	Tile* enemyMushTile = getEnemyMushroomTile(attackableTiles);
	if(enemyMushTile == NULL)
	{
		lookingForMushroom = true;
		setGetClosestOpponent(true);
		gamePhase = mushroomWarNS::ENEMY_FIND_PATH;
		return NULL;
	}

	// set game phase to enemy attack to the attack logic commences during the update function
	gamePhase = mushroomWarNS::ENEMY_ATTACK;
	return enemyMushTile;
}

void NematodeBurrowerUnit::resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid)
{
	setActnThisTurn(false);
}

int NematodeBurrowerUnit::getStartFrame()
{
	return 26;
}

int NematodeBurrowerUnit::getEndFrame()
{
	return 27;
}

int NematodeBurrowerUnit::getStartFightFrame()
{
	return BURROWER_NEM_ST_FIGHT_FRAME;
}

int NematodeBurrowerUnit::getEndFightFrame()
{
	return BURROWER_NEM_END_FIGHT_FRAME;
}

int NematodeBurrowerUnit::getFlinchFrame()
{
	return BURROWER_NEM_FLINCH_FRAME;
}

std::string NematodeBurrowerUnit::getUnitName()
{
	return NEMA_BURROWER_UNIT_NAME;
}

void NematodeBurrowerUnit::whileMovingAction(Tile* aTile)
{
	if(aTile->getIsTaken())
	{
		aTile->setIsTaken(false);
		Tile::s_tilesTakenOver--;
	}
}

std::vector<int> NematodeBurrowerUnit::movingCompletedAction(Tile* aTile, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>& fieldGrid)
{
	std::vector<int> tileIndices;
	tileIndices.clear();

	if(aTile->getIsTaken())
	{
		aTile->setIsTaken(false);
		Tile::s_tilesTakenOver--;
	}

	return tileIndices;
}