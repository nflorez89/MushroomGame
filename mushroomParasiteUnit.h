#ifndef _MUSHROOM_PARASITE_UNIT_H               // Prevent multiple definitions if this 
#define _MUSHROOM_PARASITE_UNIT_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "utilities.h"
#include "unit.h"
#include "nematodeBurrowerUnit.h"
#include "nematodeCloseCombatUnit.h"
#include "nematodeParasiteUnit.h"
#include "nematodeRangedUnit.h"
#include "mushroomCloseCombatUnit.h"
#include "mushroomColonizerUnit.h"
#include "mushroomRangedUnit.h"

class MushroomParasiteUnit : public Unit
{
protected:
public:
	// constructor
	MushroomParasiteUnit(float, float, int, int, int, int, float, float);

	virtual bool canBeAttackedBack();

	virtual void attack(Entity*, float, float);

	void affectAfterDeath(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&);

	virtual void isAttackedBy(NematodeParasiteUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeBurrowerUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeRangedUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeCloseCombatUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomCloseCombatUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomRangedUnit*, float accuracySitMod, float atkSitMod);
	virtual int getStartFrame();
	virtual int getEndFrame();
	virtual int getStartFightFrame();
	virtual int getEndFightFrame();
	virtual int getFlinchFrame();
	inline virtual int getStartDeathFrame() { return PARASITE_MUSH_UNIT_DEATH_ST_FRAME;}
	inline virtual int getEndDeathFrame() { return PARASITE_MUSH_UNIT_DEATH_END_FRAME;}
	virtual std::string getUnitName();
};

#endif