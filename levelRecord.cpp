#include "levelRecord.h"

//=============================================================================
// constructor
//=============================================================================
LevelRecord::LevelRecord(float l_unitHp, float l_unitAccuracy, float l_unitEvasion, float l_unitDamage, int l_unitMvmtRange, int l_unitAtkRange, int l_spreadRadius, int l_psnEffectiveness, int l_upgradeCost, int l_spawnCost)
{
	unitHp = l_unitHp;
	unitAccuracy = l_unitAccuracy;
	unitEvasion = l_unitEvasion;
	unitDamage = l_unitDamage;
	unitMvmtRange = l_unitMvmtRange;
	unitAtkRange = l_unitAtkRange;
	spreadRadius = l_spreadRadius;
	psnEffectiveness = l_psnEffectiveness;
	upgradeCost = l_upgradeCost;
	spawnCost = l_spawnCost;
}

//=============================================================================
// destructor
//=============================================================================
LevelRecord::~LevelRecord()
{
}


