#include "colonizerMushroom.h"

std::array<LevelRecord,MAX_LEVEL_RECORDS> ColonizerMushroom::s_levelTable = {LevelRecord(70.0f, 90.0f, 80.0f, 15.0f, 2, 1, 1, 0, 0, 100),
LevelRecord(70.0f,95.0f,85.0f,15.0f,2,1,1,0, 50, 200),
LevelRecord(70.0f,95.0f,90.0f,15.0f,2,1,2,0, 100, 300),
LevelRecord(75.0f,95.0f,90.0f,15.0f,3,1,2,0, 150, 400),
LevelRecord(80.0f,95.0f,90.0f,15.0f,3,1,3,0, 200, 500)};

//=============================================================================
// constructor
//=============================================================================
ColonizerMushroom::ColonizerMushroom() : Mushroom()
{
	unitHp = s_levelTable[0].getUnitHp();
	unitAccuracy = s_levelTable[0].getUnitAcc();
	unitEvasion = s_levelTable[0].getUnitEvasion();
	unitDamage = s_levelTable[0].getUnitDamage();
	unitMvmtRange = s_levelTable[0].getUnitMvmtRange();
	unitAtkRange = s_levelTable[0].getUnitAtkRange();
	spreadRadius = s_levelTable[0].getSpreadRadius();
	psnEffectiveness = s_levelTable[0].getPsnEff();

	startFrame = 9;
	endFrame = 11;
}

//=============================================================================
// <factory method>
// ColonizerMushroom::createUnit
// args: none
// return value: pointer to a new unit
//=============================================================================
Unit* ColonizerMushroom::spawnUnit()
{
	return new MushroomColonizerUnit(unitHp, unitDamage, unitAtkRange, unitMvmtRange, spreadRadius, unitAccuracy, unitEvasion);
}

//=============================================================================
// ColonizerMushroom::upgrade
// args: none
// return value: void
// description: increases the stats of this mushroom
//=============================================================================
/*void ColonizerMushroom::upgrade()
{
	// stub
}*/

int ColonizerMushroom::getFlinchFrame()
{
	return -1;
}

int ColonizerMushroom::getStartFlinchFrame()
{
	return COLONIZER_MUSHROOM_START_FLINCH_FRAME;
}

int ColonizerMushroom::getEndFlinchFrame()
{
	return COLONIZER_MUSHROOM_END_FLINCH_FRAME;
}

std::array<LevelRecord,MAX_LEVEL_RECORDS> ColonizerMushroom::getLevelRecordArr()
{
	return s_levelTable;
}

//=============================================================================
// ColonizerMushroom::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, atkSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================

void ColonizerMushroom::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void ColonizerMushroom::isAttackedBy(MushroomParasiteUnit* mushParasiteUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void ColonizerMushroom::isAttackedBy(MushroomColonizerUnit* mushColonizerUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void ColonizerMushroom::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

std::string ColonizerMushroom::getMushName()
{
	return MUSH_COLONIZER_MUSH_NAME;
}

int ColonizerMushroom::getEndFightFrame()
{
	return COLONIZER_MUSHROOM_START_FLINCH_FRAME;
}