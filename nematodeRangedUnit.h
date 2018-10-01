#ifndef _NEMATODE_RANGED_UNIT_H               // Prevent multiple definitions if this 
#define _NEMATODE_RANGED_UNIT_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "nematodeUnit.h"
#include "nematodeBurrowerUnit.h"
#include "nematodeCloseCombatUnit.h"
#include "nematodeParasiteUnit.h"
#include "mushroomCloseCombatUnit.h"
#include "mushroomColonizerUnit.h"
#include "mushroomParasiteUnit.h"
#include "mushroomRangedUnit.h"

class NematodeRangedUnit : public NematodeUnit
{
protected:
public:
	// constructor
	NematodeRangedUnit();
	NematodeRangedUnit(float, float, int, int, int, float, float);

	bool canTraverse(Tile*);
	virtual bool canBeAttackedBack();

	virtual void attack(Entity*, float, float);

	virtual Entity* getEntityToAttack(std::vector<Tile*>);
	virtual Tile* getEntityTileToAttack(std::vector<Tile*>);

	virtual void isAttackedBy(NematodeParasiteUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeBurrowerUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeRangedUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeCloseCombatUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomCloseCombatUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomRangedUnit*, float accuracySitMod, float atkSitMod);

	virtual Tile* setUpPathFinding(Tile*, std::vector<Tile*>&, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&, mushroomWarNS::GAME_PHASE&);

	virtual void resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>);

	virtual int getStartFrame();
	virtual int getEndFrame();
	virtual int getStartFightFrame();
	virtual int getEndFightFrame();
	virtual int getFlinchFrame();
	virtual std::string getUnitName();
};

#endif