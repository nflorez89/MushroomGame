#include "closeCombatMushroom.h"

std::array<LevelRecord,MAX_LEVEL_RECORDS> CloseCombatMushroom::s_levelTable = {LevelRecord(90.0f, 100.0f, 50.0f, 20.0f, 3, 1, 0, 0,0, 60),
LevelRecord(90.0f,100.0f,55.0f,25.0f,3,1,0,0, 30, 120),
LevelRecord(95.0f,100.0f,55.0f,30.0f,3,2,0,0, 60, 180),
LevelRecord(95.0f,100.0f,55.0f,35.0f,4,2,0,0, 90, 240),
LevelRecord(100.0f,100.0f,55.0f,40.0f,4,2,0,0, 120, 300)};

//=============================================================================
// constructor
//=============================================================================
CloseCombatMushroom::CloseCombatMushroom() : Mushroom()
{
	unitHp = s_levelTable[0].getUnitHp();
	unitAccuracy = s_levelTable[0].getUnitAcc();
	unitEvasion = s_levelTable[0].getUnitEvasion();
	unitDamage = s_levelTable[0].getUnitDamage();
	unitMvmtRange = s_levelTable[0].getUnitMvmtRange();
	unitAtkRange = s_levelTable[0].getUnitAtkRange();
	spreadRadius = s_levelTable[0].getSpreadRadius();
	psnEffectiveness = s_levelTable[0].getPsnEff();

	startFrame = 0;
	endFrame = 2;
}

//=============================================================================
// <factory method>
// CloseCombatMushroom::createUnit
// args: none
// return value: pointer to a new unit
//=============================================================================
Unit* CloseCombatMushroom::spawnUnit()
{
	return new MushroomCloseCombatUnit(unitHp, unitDamage, unitAtkRange, unitMvmtRange, spreadRadius, unitAccuracy, unitEvasion);
}

//=============================================================================
// CloseCombatMushroom::upgrade
// args: none
// return value: void
// description: increases the stats of this mushroom
//=============================================================================
/*void CloseCombatMushroom::upgrade()
{
	// stub
}*/

int CloseCombatMushroom::getFlinchFrame()
{
	return -1;
}

int CloseCombatMushroom::getStartFlinchFrame()
{
	return CLOSE_COMBAT_MUSHROOM_START_FLINCH_FRAME;
}

int CloseCombatMushroom::getEndFlinchFrame()
{
	return ClOSE_COMBAT_MUSHROOM_END_FLINCH_FRAME;
}

std::array<LevelRecord,MAX_LEVEL_RECORDS> CloseCombatMushroom::getLevelRecordArr()
{
	return s_levelTable;
}

//=============================================================================
// CloseCombatMushroom::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, atkSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================

void CloseCombatMushroom::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void CloseCombatMushroom::isAttackedBy(MushroomParasiteUnit* mushParasiteUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void CloseCombatMushroom::isAttackedBy(MushroomColonizerUnit* mushColonizerUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void CloseCombatMushroom::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

std::string CloseCombatMushroom::getMushName()
{
	return MUSH_CLOSE_COMBAT_MUSH_NAME;
}

int CloseCombatMushroom::getEndFightFrame()
{
	return CLOSE_COMBAT_MUSHROOM_START_FLINCH_FRAME;
}