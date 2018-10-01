#ifndef _COLONIZER_MUSHROOM_H               // Prevent multiple definitions if this 
#define _COLONIZER_MUSHROOM_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "mushroom.h"
#include "mushroomColonizerUnit.h"

class ColonizerMushroom : public Mushroom
{
public:
	// constructor
	ColonizerMushroom();

	// factory method
	virtual Unit* spawnUnit();
	//virtual void upgrade();

	virtual int   getStartFrame()   {return COLONIZER_MUSHROOM_START_FRAME;}
	virtual int   getEndFrame()     {return COLONIZER_MUSHROOM_END_FRAME;}
	virtual int getFlinchFrame();
	virtual int getStartFlinchFrame();
	virtual int getEndFlinchFrame();
	virtual std::array<LevelRecord,MAX_LEVEL_RECORDS> getLevelRecordArr();

	virtual void isAttackedBy(MushroomCloseCombatUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomRangedUnit*, float accuracySitMod, float atkSitMod);

	virtual std::string getMushName();
	virtual int getEndFightFrame();

	static std::array<LevelRecord,MAX_LEVEL_RECORDS> getLvlTbl() { return s_levelTable;}

private:
	// represents the table indicating the stats of the mushroom units at each level
	static std::array<LevelRecord,MAX_LEVEL_RECORDS> s_levelTable;
};

#endif