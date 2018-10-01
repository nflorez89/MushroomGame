#ifndef _NEMATODE_PARASITE_UNIT_H               // Prevent multiple definitions if this 
#define _NEMATODE_PARASITE_UNIT_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "nematodeUnit.h"
#include "nematodeBurrowerUnit.h"
#include "nematodeCloseCombatUnit.h"
#include "nematodeRangedUnit.h"
#include "mushroomCloseCombatUnit.h"
#include "mushroomColonizerUnit.h"
#include "mushroomParasiteUnit.h"
#include "mushroomRangedUnit.h"

class NematodeParasiteUnit : public NematodeUnit
{
protected:
	short countdown;
	int finalGoalRow, finalGoalCol;
	bool isUltDestReached;
public:
	// constructor
	NematodeParasiteUnit();
	NematodeParasiteUnit(float, float, int, int, int, float, float);

	void countDown();
	bool isTimeToInvade();
	void getFinalGoal(Tile*);
	virtual bool canTraverse(Tile*);
	virtual bool canBeAttackedBack();

	virtual void attack(Entity*, float, float);

	virtual void isAttackedBy(NematodeParasiteUnit*, float , float );
	virtual void isAttackedBy(NematodeBurrowerUnit*, float , float );
	virtual void isAttackedBy(NematodeRangedUnit*, float , float );
	virtual void isAttackedBy(NematodeCloseCombatUnit*, float , float );
	virtual void isAttackedBy(MushroomCloseCombatUnit*, float , float );
	virtual void isAttackedBy(MushroomParasiteUnit*, float , float );
	virtual void isAttackedBy(MushroomColonizerUnit*, float , float );
	virtual void isAttackedBy(MushroomRangedUnit*, float , float );

	virtual Tile* setUpPathFinding(Tile*, std::vector<Tile*>&, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&, mushroomWarNS::GAME_PHASE&);

	virtual void resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>);

	virtual int getStartFrame();
	virtual int getEndFrame();
	virtual int getStartFightFrame();
	virtual int getEndFightFrame();
	virtual int getFlinchFrame();
	virtual std::string getUnitName();
	virtual std::vector<int> movingCompletedAction(Tile*, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&);
	virtual bool pathFoundPostProcessing(bool ultDestReached);
};

#endif