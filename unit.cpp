#include "unit.h"
#include "tile.h"

//=============================================================================
// default constructor
//=============================================================================
Unit::Unit() : Entity()
{
	lookingForMushroom = false;
	getClosestOpponent = false;
	isAttacking = false;
}

//=============================================================================
// constructor
//=============================================================================
Unit::Unit(float hp, float atkDmg, int atkRng, int mvmtRng, int spdRd, float acc, float evs) : Entity()
{
	health = hp;
	maxHealth = hp;
	prevHealth = health;
	attackDmg = atkDmg;
	atkRange = atkRng;
	mvmtRange = mvmtRng;
	spreadRadius = spdRd;
	accuracy = acc;
	evasion = evs;
	accSitMod = 1.0f;
	actnThisTurn = false;
	lookingForMushroom = false;
	getClosestOpponent = false;
	isAttacking = false;
	isDeathAnimated = false;
	psnEffectiveness = 0;
	psnSeverity = 0;
	psnTurnsLeft = 0;
	psnDmgThisTurn = false;
	bonusChars.clear();
}

//=============================================================================
// destructor
//=============================================================================
Unit::~Unit()
{
}

//=============================================================================
// Unit::canTraverse
// args: pointer to a tile
// return value: bool
// description: returns false if the tile has an occupying mushroom, true otherwise
//=============================================================================
bool Unit::canTraverse(Tile* aTile)
{
	EnvCharacteristic* envChar;
	mushroomWarNS::CHARACTERISTIC_TYPE ct;

	if(aTile->hasOccupyingEntity() || aTile->hasOccupyingMushroom())
		return false;
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
	if(aTile->getTileType() == mushroomWarNS::WATER)
	{
		// close combat units cannot traverse water tiles
		return false;
	}
	return true;

	return true;
}

//=============================================================================
// Unit::attackLands
// args: Unit*, accuracy situational modifier, and attack situational modifier
// return value: bool
// description: returns true if the attacking unit hits the opposing one based
//				on their accuracy and evasion stats. False if a miss.
//=============================================================================
bool Unit::attackLands(Entity* opposingUnit, float evsSitMod)
{
	// keep track of the unit being attacked's hp before being attacked. Will use this to display during attack animation
	opposingUnit->setPrevHealth(opposingUnit->getHealth());

	bool atkHit = true;
	float oppEv = (opposingUnit->getEvasion()) * evsSitMod;
	float atkAcc = getAccuracy() * accSitMod;
	if(oppEv > -1000000.0f)
	{
		atkHit = Utilities::calcProb(atkAcc, oppEv);
	}

	if(atkHit)
		return true;

	return false;
}

//=============================================================================
// Unit::update
// args: float frame time
// return value: void
// description: typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Unit::update(float frameTime)
{
	mushroomWarNS::DIRECTION dir = getMoveDir();

	switch(dir)
	{
		case mushroomWarNS::D_UP:
			velocity.x = 0.0f;
			velocity.y = frameTime * -(unitNS::SPEED);
			break;
		case mushroomWarNS::D_DOWN:
			velocity.x = 0.0f;
			velocity.y = frameTime * unitNS::SPEED;
			break;
		case mushroomWarNS::D_RIGHT:
			velocity.x = frameTime * unitNS::SPEED;
			velocity.y = 0.0f;
			break;
		case mushroomWarNS::D_LEFT:
			velocity.x = frameTime * -(unitNS::SPEED);
			velocity.y = 0.0f;
			break;
		default:
			velocity.x = 0.0f;
			velocity.y = 0.0f;
	}

	spriteData.x += frameTime * velocity.x;     // move ship along X 
    spriteData.y += frameTime * velocity.y;     // move ship along Y

	Entity::update(frameTime);
}

Tile* Unit::setUpPathFinding(Tile*, std::vector<Tile*>&, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&, mushroomWarNS::GAME_PHASE&)
{
	return NULL;
}

void Unit::resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>)
{
	return;
}

bool Unit::areEnemyMushTilesLeft(std::vector<Tile*>, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>)
{
	return false;
}

//=============================================================================
// Unit::wait
// args: none
// return value: void
// description: will make the unit do nothing this turn
//=============================================================================
void Unit::wait()
{
	// stub
}