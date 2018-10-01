#include "parasiticMushroom.h"

std::array<LevelRecord,MAX_LEVEL_RECORDS> ParasiticMushroom::s_levelTable = {LevelRecord(30.0f, 60.0f, 20.0f, 5.0f, 3, 2, 0, 1, 0, 80),
LevelRecord(30.0f,60.0f,20.0f,10.0f,3,2,0,2, 40, 160),
LevelRecord(30.0f,65.0f,20.0f,10.0f,3,2,0,3, 80, 240),
LevelRecord(30.0f,65.0f,20.0f,10.0f,3,3,0,4, 120, 320),
LevelRecord(30.0f,65.0f,20.0f,10.0f,4,3,0,5, 160, 400)};

//=============================================================================
// constructor
//=============================================================================
ParasiticMushroom::ParasiticMushroom() : Mushroom()
{
	unitHp = s_levelTable[0].getUnitHp();
	unitAccuracy = s_levelTable[0].getUnitAcc();
	unitEvasion = s_levelTable[0].getUnitEvasion();
	unitDamage = s_levelTable[0].getUnitDamage();
	unitMvmtRange = s_levelTable[0].getUnitMvmtRange();
	unitAtkRange = s_levelTable[0].getUnitAtkRange();
	spreadRadius = s_levelTable[0].getSpreadRadius();
	psnEffectiveness = s_levelTable[0].getPsnEff();

	startFrame = 6;
	endFrame = 8;
}

//=============================================================================
// <factory method>
// ParasiticMushroom::createUnit
// args: none
// return value: pointer to a new unit
//=============================================================================
Unit* ParasiticMushroom::spawnUnit()
{
	return new MushroomParasiteUnit(unitHp, unitDamage, unitAtkRange, unitMvmtRange, spreadRadius, psnEffectiveness, unitAccuracy, unitEvasion);
}

//=============================================================================
// ParasiticMushroom::upgrade
// args: none
// return value: void
// description: increases the stats of this mushroom
//=============================================================================
/*void ParasiticMushroom::upgrade()
{
	// stub
}*/

int ParasiticMushroom::getFlinchFrame()
{
	return -1;
}

int ParasiticMushroom::getStartFlinchFrame()
{
	return PARASITE_MUSHROOM_START_FLINCH_FRAME;
}

int ParasiticMushroom::getEndFlinchFrame()
{
	return PARASITE_MUSHROOM_END_FLINCH_FRAME;
}

std::array<LevelRecord,MAX_LEVEL_RECORDS> ParasiticMushroom::getLevelRecordArr()
{
	return s_levelTable;
}

//=============================================================================
// ParasiticMushroom::isAttackedBy
// args: pointer to child of class Unit, accuracySitMod, atkSitMod
// return value: void
// description: will decrease the hp of the this unit depending on its type
//				and the type of the attacker and the situational modifiers.
//=============================================================================

void ParasiticMushroom::isAttackedBy(MushroomCloseCombatUnit* mushCloseCombatUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void ParasiticMushroom::isAttackedBy(MushroomParasiteUnit* mushParasiteUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void ParasiticMushroom::isAttackedBy(MushroomColonizerUnit* mushColonizerUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

void ParasiticMushroom::isAttackedBy(MushroomRangedUnit* mushRangedUnit, float accuracySitMod, float atkSitMod)
{
	// cannot be attacked by mushroom units
	return;
}

std::string ParasiticMushroom::getMushName()
{
	return MUSH_PARASITE_MUSH_NAME;
}

int ParasiticMushroom::getEndFightFrame()
{
	return PARASITE_MUSHROOM_START_FLINCH_FRAME;
}