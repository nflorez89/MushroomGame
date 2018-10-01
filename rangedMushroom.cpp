#include "rangedMushroom.h"

std::array<LevelRecord,MAX_LEVEL_RECORDS> RangedMushroom::s_levelTable = {LevelRecord(50.0f, 80.0f, 70.0f, 15.0f, 2, 3, 0, 0, 0, 70),
LevelRecord(50.0f,85.0f,70.0f,15.0f,2,4,0,0, 35, 140),
LevelRecord(50.0f,90.0f,75.0f,15.0f,2,4,0,0, 70, 210),
LevelRecord(55.0f,95.0f,75.0f,15.0f,2,4,0,0, 105, 280),
LevelRecord(55.0f,100.0f,75.0f,20.0f,2,4,0,0, 140, 350)};

//=============================================================================
// constructor
//=============================================================================
RangedMushroom::RangedMushroom() : Mushroom()
{
	unitHp = s_levelTable[0].getUnitHp();
	unitAccuracy = s_levelTable[0].getUnitAcc();
	unitEvasion = s_levelTable[0].getUnitEvasion();
	unitDamage = s_levelTable[0].getUnitDamage();
	unitMvmtRange = s_levelTable[0].getUnitMvmtRange();
	unitAtkRange = s_levelTable[0].getUnitAtkRange();
	spreadRadius = s_levelTable[0].getSpreadRadius();
	psnEffectiveness = s_levelTable[0].getPsnEff();

	startFrame = 3;
	endFrame = 5;
}

//=============================================================================
// <factory method>
// RangedMushroom::createUnit
// args: none
// return value: pointer to a new unit
//=============================================================================
Unit* RangedMushroom::spawnUnit()
{
	return new MushroomRangedUnit(unitHp, unitDamage, unitAtkRange, unitMvmtRange, spreadRadius, unitAccuracy, unitEvasion);
}

//=============================================================================
// RangedMushroom::upgrade
// args: none
// return value: void
// description: increases the stats of this mushroom
//=============================================================================
/*void RangedMushroom::upgrade()
{
	// stub
}*/

int RangedMushroom::getFlinchFrame()
{
	return -1;
}

int RangedMushroom::getStartFlinchFrame()
{
	return RANGED_MUSHROOM_START_FLINCH_FRAME;
}

int RangedMushroom::getEndFlinchFrame()
{
	return RANGED_MUSHROOM_END_FLINCH_FRAME;
}

std::array<LevelRecord,MAX_LEVEL_RECORDS> RangedMushroom::getLevelRecordArr()
{
	return s_levelTable;
}

//=============================================================================
// RangedMushroom::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, atkSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================

void RangedMushroom::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void RangedMushroom::isAttackedBy(MushroomParasiteUnit* mushParasiteUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void RangedMushroom::isAttackedBy(MushroomColonizerUnit* mushColonizerUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void RangedMushroom::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

std::string RangedMushroom::getMushName()
{
	return MUSH_RANGED_MUSH_NAME;
}

int RangedMushroom::getEndFightFrame()
{
	return RANGED_MUSHROOM_START_FLINCH_FRAME;
}