#include "mushroom.h"

Image Mushroom::s_mushroomUpgradeImg;
int Mushroom::s_mushSpreadRadius = 2;
bool Mushroom::s_mushroomsClickable = true;

//=============================================================================
// constructor
//=============================================================================
Mushroom::Mushroom() : Entity()
{
	level = 0;
	spreadRadius = 1;
	spreadMycTurnsLeft = TURNS_BEFORE_MUSH_SPREAD_MYCELLIUM;
	
	animateUpgrade = false;
}

//=============================================================================
// destructor
//=============================================================================
Mushroom::~Mushroom()
{
}

//=============================================================================
// Mushroom::isUpgradeable
// args: int representing how much mycelium the player has
// return value: bool
// description: returns true if the player has enough mycelium to upgrade this mushroom
//=============================================================================
bool Mushroom::canAffordUpgrade(int myceliumScore)
{
	if ( myceliumScore >= getLevelRecordArr()[level+1].getUpgradeCost())
		return true;
	return false;
}

//=============================================================================
// Mushroom::getMushroomUpgradeCost
// args: none
// return value: int
// description: returns the cost to upgrade the mushroom to the next level. Will return
//				-1 if the mushroom is already at max level.
//=============================================================================
int Mushroom::getUpgradeCost()
{
	// if this is true, this mushroom is already at max level
	if(level >= (MAX_LEVEL_RECORDS-1))
		return -1;

	return getLevelRecordArr()[level+1].getUpgradeCost();
}

//=============================================================================
// Mushroom::upgrade
// args: none
// return value: void
// description: increases the stats of this mushroom based on the upgrade table
//=============================================================================
void Mushroom::upgrade(int& myceliumScore)
{
	if(level >= (MAX_LEVEL_RECORDS - 1) )
		return;
	
	// increment the level
	level += 1;
	
	// set the new values for this mushroom's future units
	unitHp = getLevelRecordArr()[level].getUnitHp();
	unitAccuracy = getLevelRecordArr()[level].getUnitAcc();
	unitEvasion = getLevelRecordArr()[level].getUnitEvasion();
	unitDamage = getLevelRecordArr()[level].getUnitDamage();
	unitMvmtRange = getLevelRecordArr()[level].getUnitMvmtRange();
	unitAtkRange = getLevelRecordArr()[level].getUnitAtkRange();
	spreadRadius = getLevelRecordArr()[level].getSpreadRadius();
	psnEffectiveness = getLevelRecordArr()[level].getPsnEff();

	// deplete the mycelium score by the cost for this level
	myceliumScore -= getLevelRecordArr()[level].getUpgradeCost();
	
}

//=============================================================================
// Mushroom::initMushroomStaticVars (static)
// args: none
// return value: void
// description: initializes data related to static member variables in this class
//=============================================================================
void Mushroom::initMushroomStaticVars()
{
	s_mushroomUpgradeImg.setLoop(false);
}

//=============================================================================
// Mushroom::getSpawnCost
// args: none
// return value: int
// description: returns the cost of this mushroom at the level it's at
//=============================================================================
int Mushroom::getSpawnCost()
{
	return getLevelRecordArr()[level].getSpawnCost();
}