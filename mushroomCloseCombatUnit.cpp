#include "mushroomCloseCombatUnit.h"

//=============================================================================
// constructor
//=============================================================================
MushroomCloseCombatUnit::MushroomCloseCombatUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : Unit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	isMushroomUnit = true;
	//nematodeType = mushroomWarNS::NONE_TYPE;
}

bool MushroomCloseCombatUnit::canBeAttackedBack()
{
	return true;
}

//=============================================================================
// MushroomCloseCombatUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: bool
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers. Returns true if attack lands, false if a miss.
//=============================================================================
void MushroomCloseCombatUnit::attack(Entity* opposingUnit, float accSitMod, float evsSitMod)
{
		opposingUnit->isAttackedBy(this,1.0f,evsSitMod);
		if (opposingUnit->getHealth() < 0.0f)
			opposingUnit->setHealth(0.0f);
}

//=============================================================================
// MushroomCloseCombatUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void MushroomCloseCombatUnit::isAttackedBy(NematodeParasiteUnit* nemParasiteUnit, float accuracySitMod, float evsSitMod)
{
	// nematode parasites do not attack
	return;
	//nemParasite->wait();
}
void MushroomCloseCombatUnit::isAttackedBy(NematodeBurrowerUnit* nemBurrowerUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemBurrowerUnit->getAttackDmg());
}
void MushroomCloseCombatUnit::isAttackedBy(NematodeRangedUnit* nemRangedUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	// ranged units do half damage to close ranged ones
	float atkDmg = nemRangedUnit->getAttackDmg();
	setHealth(getHealth() - (atkDmg / 2.0f));
}
void MushroomCloseCombatUnit::isAttackedBy(NematodeCloseCombatUnit* nemCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemCloseCombatUnit->getAttackDmg());
}
void MushroomCloseCombatUnit::isAttackedBy(MushroomCloseCombatUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomCloseCombatUnit::isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomCloseCombatUnit::isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomCloseCombatUnit::isAttackedBy(MushroomRangedUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}

int MushroomCloseCombatUnit::getStartFrame()
{
	return 12;
}

int MushroomCloseCombatUnit::getEndFrame()
{
	return 13;
}

int MushroomCloseCombatUnit::getStartFightFrame()
{
	return CLOSE_COMBAT_MUSH_ST_FIGHT_FRAME;
}

int MushroomCloseCombatUnit::getEndFightFrame()
{
	return CLOSE_COMBAT_MUSH_END_FIGHT_FRAME;
}

int MushroomCloseCombatUnit::getFlinchFrame()
{
	return CLOSE_COMBAT_MUSH_FLINCH_FRAME;
}

std::string MushroomCloseCombatUnit::getUnitName()
{
	return MUSH_CLOSE_COMBAT_UNIT_NAME;
}

/*Tile* MushroomCloseCombatUnit::calcDestTile(Tile* srcTile)
{
	std::vector<Tile*> attackableTiles;
	Tile* destTile = NULL;
	attackableTiles = getEnemiesWithinAtkRange(srcTile);
	if(attackableTiles.empty())
	{
		// get the closest tile
		destTile = getClosestOpposingTile(srcTile,false, enemyMushTilesChecked);
		if(!destTile)
		{
			// since no opposing enemy mushroom units on the board, we'll look for closest opposing mushroom to attack
			destTile = getClosestOpposingTile(srcTile,true, enemyMushTilesChecked);
		}

		// find the shortest path (solution) to the closest tile
		gamePhase = mushroomWarNS::ENEMY_FIND_PATH;
	}
	else
	{
		// set game phase to enemy attack to the attack logic commences during the update function
		gamePhase = mushroomWarNS::ENEMY_ATTACK;
	}
}*/