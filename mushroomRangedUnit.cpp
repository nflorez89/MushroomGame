#include "mushroomRangedUnit.h"

//=============================================================================
// constructor
//=============================================================================
MushroomRangedUnit::MushroomRangedUnit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : Unit(hp, atkDmg, atkRng, mvmtRng, spdRd, acc, evs)
{
	isMushroomUnit = true;
	nematodeType = mushroomWarNS::NONE_TYPE;
	bonusChars.push_back(mushroomWarNS::HILL);
}

bool MushroomRangedUnit::canBeAttackedBack()
{
	return false;
}

//=============================================================================
// MushroomRangedUnit::attack
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: void
// description: will decrease the hp of the unit passed depending on its type
//				and the situational modifiers.
//=============================================================================
void MushroomRangedUnit::attack(Entity* opposingUnit, float accSitMod, float evsSitMod)
{
	opposingUnit->isAttackedBy(this,1.0f,1.0f);
	if (opposingUnit->getHealth() < 0.0f)
		opposingUnit->setHealth(0.0f);
}

//=============================================================================
// MushroomRangedUnit::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, evsSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================
void MushroomRangedUnit::isAttackedBy(NematodeParasiteUnit* nemParasiteUnit, float accuracySitMod, float evsSitMod)
{
	// nematode parasites do not attack
	return;
}
void MushroomRangedUnit::isAttackedBy(NematodeBurrowerUnit* nemBurrowerUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemBurrowerUnit->getAttackDmg());
}
void MushroomRangedUnit::isAttackedBy(NematodeRangedUnit* nemRangedUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	setHealth(getHealth() - nemRangedUnit->getAttackDmg());
}
void MushroomRangedUnit::isAttackedBy(NematodeCloseCombatUnit* nemCloseCombatUnit, float accuracySitMod, float evsSitMod)
{
	// get the enemy's attack and subtract it from this unit's hp
	// mushroom close combat units do double damage to ranged mushrooms
	float atkDmg = nemCloseCombatUnit->getAttackDmg();
	setHealth(getHealth() - (atkDmg * 2));
}
void MushroomRangedUnit::isAttackedBy(MushroomCloseCombatUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomRangedUnit::isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomRangedUnit::isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}
void MushroomRangedUnit::isAttackedBy(MushroomRangedUnit*, float accuracySitMod, float evsSitMod)
{
	//stub
}

int MushroomRangedUnit::getStartFrame()
{
	return 14;
}

int MushroomRangedUnit::getEndFrame()
{
	return 15;
}

int MushroomRangedUnit::getStartFightFrame()
{
	return RANGED_MUSH_ST_FIGHT_FRAME;
}

int MushroomRangedUnit::getEndFightFrame()
{
	return RANGED_MUSH_END_FIGHT_FRAME;
}

int MushroomRangedUnit::getFlinchFrame()
{
	return RANGED_MUSH_FLINCH_FRAME;
}

std::string MushroomRangedUnit::getUnitName()
{
	return MUSH_RANGED_UNIT_NAME;
}