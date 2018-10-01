#ifndef _UNIT_H               // Prevent multiple definitions if this 
#define _UNIT_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <array>
#include <vector>

#include "utilities.h"
#include "entity.h"

/*#include "nematodeBurrowerUnit.h"
#include "nematodeCloseCombatUnit.h"
#include "nematodeParasiteUnit.h"
#include "nematodeRangedUnit.h"
#include "nematodeUnit.h"
#include "mushroomCloseCombatUnit.h"
#include "mushroomColonizerUnit.h"
#include "mushroomParasiteUnit.h"
#include "mushroomRangedUnit.h"
*/

class Tile;

namespace unitNS
{
    const int WIDTH = 32;                   // image width
    const int HEIGHT = 32;                  // image height
	const int FIGHT_WIDTH = 160;			// image width for fighting sprites
	const int FIGHT_HEIGHT = 160;				// image height for fighting sprites
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float ROTATION_RATE = (float)PI/4; // radians per second
    const float SPEED = 10000.0f;
    const float MASS = 300.0f;              // mass
    const int   TEXTURE_COLS = 7;           // texture has 7 columns
	const int	FIGHT_TEXTURE_COLS = 6;		// fighting texture has 6 columns
    //const int   SHIP_START_FRAME = 0;       // ship starts at frame 0
    //const int   SHIP_END_FRAME = 3;         // ship animation frames 0,1,2,3
    const float UNIT_ANIMATION_DELAY = 0.2f; // time between frames
}
 
// abstract class
class Unit : public Entity
{
protected:
	float hp;
	float accuracy;
	float evasion;
	float attackDmg;
	int mvmtRange;
	int atkRange;
	int spreadRadius;
	float accSitMod;
	mushroomWarNS::DIRECTION moveDir;
	bool isMushroomUnit;
	float currDestCoordisMushroomUnit;
	mushroomWarNS::NEMATODE_TYPE nematodeType;
	bool getClosestOpponent;
	std::vector<Tile*> enemyMushTilesChecked;
	std::vector<mushroomWarNS::CHARACTERISTIC_TYPE> bonusChars;
	bool lookingForMushroom;
	Image attackingUnitImg;
	bool isAttacking;
	bool isDeathAnimated;
	int psnEffectiveness;
	int psnSeverity;
	int psnTurnsLeft;
	bool psnDmgThisTurn;
public:
	// constructor
	Unit();
	Unit(float, float, int, int, int, float, float);

	// destructor
	virtual ~Unit();

	virtual void update(float frameTime);

	void wait();
	
	virtual bool canTraverse(Tile*);
	virtual bool canBeAttackedBack() = 0;
	virtual bool attackLands(Entity*, float evsSitMod);
	virtual void attack(Entity*, float accSitMod, float evsSitMod) = 0;

	virtual Tile* setUpPathFinding(Tile*, std::vector<Tile*>&, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&, mushroomWarNS::GAME_PHASE&);

	virtual void resetTurn(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>);

	virtual bool areEnemyMushTilesLeft(std::vector<Tile*>, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>);

	inline virtual void affectAfterDeath(std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&) { return;}

	inline virtual Entity* getEntityToAttack(std::vector<Tile*>) {return NULL;}

	inline virtual Tile* getEntityTileToAttack(std::vector<Tile*>) {return NULL;}

	// getter(s)
	virtual int getStartFrame() = 0;
	virtual int getEndFrame() = 0;
	virtual int getStartFightFrame() = 0;
	virtual int getEndFightFrame() = 0;
	inline bool isThisMushroomUnit() { return isMushroomUnit;}
	inline mushroomWarNS::DIRECTION getMoveDir() {return moveDir;}
	inline int getAtkRange() { return atkRange;}
	inline int getMvmtRange() {return mvmtRange;}
	inline float getAttackDmg() { return attackDmg;}
	inline float getAccuracy() { return accuracy;}
	inline float getEvasion() { return evasion;}
	inline float getHp() {return hp;}
	inline mushroomWarNS::NEMATODE_TYPE getNematodeType() {return nematodeType;}
	inline bool getGetClosestOpponent() { return getClosestOpponent; }
	inline std::vector<Tile*> getEnemyMushTilesChecked() { return enemyMushTilesChecked; }
	inline bool getLookingForMushroom() { return lookingForMushroom; }
	inline bool getIsAttacking() { return isAttacking; }
	inline Image getAttackingUnitImg() { return attackingUnitImg; }
	inline Image& getAttackingUnitImgByRef() { return attackingUnitImg; }
	inline std::vector<mushroomWarNS::CHARACTERISTIC_TYPE> getBonusChars() { return bonusChars; }
	virtual std::string getUnitName() = 0;
	virtual int getSpreadRadius() { return spreadRadius; }
	inline bool getIsDeathAnimated() { return isDeathAnimated;}
	inline int getPsnEffectiveness() { return psnEffectiveness;}
	inline int getPsnSeverity() { return psnSeverity;}
	inline bool getPsnDmgThisTurn() { return psnDmgThisTurn;}
	inline int getPsnTurnsLeft() { return psnTurnsLeft;}

	// setter(s)
	inline void setMoveDir(mushroomWarNS::DIRECTION aDir) {moveDir = aDir;}
	void setHp(float newHp) {hp = newHp;}
	inline void setMvmtRange(int rng) {mvmtRange = rng;}
	inline void setAttackDmg(float atkDmg) { attackDmg = atkDmg;}
	inline void setGetClosestOpponent(bool gtClstOpp) { getClosestOpponent = gtClstOpp;}
	inline void setLookingForMushroom(bool lfm) { lookingForMushroom = lfm; }
	inline void setIsAttacking(bool isAtk) { isAttacking = isAtk; }
	inline void setAttackingUnitImg(Image img) { attackingUnitImg = img; }
	inline void setAccSitMod(float aStMd) {accSitMod = aStMd; }
	inline void setSpreadRadius(int sr) { spreadRadius = sr; }
	inline void setIsDeathAnimated(bool deathAnimated) { isDeathAnimated = deathAnimated;}
	inline void setPsnEffectiveness(int psnEff) { psnEffectiveness = psnEff;}
	inline void setPsnSeverity(int sev) { psnSeverity = sev;}
	
	inline virtual void setPoison(int sev) { return;}
	inline void setPsnDmgThisTurn(bool psnDmg) { psnDmgThisTurn = psnDmg;}
	inline void setPsnTurnsLeft(int psnTurns) { psnTurnsLeft = psnTurns;}

	// other
	inline virtual void clearEnemyMushTilesChecked() { enemyMushTilesChecked.clear(); }
	inline virtual void whileMovingAction(Tile*) { return;};
	inline virtual std::vector<int> movingCompletedAction(Tile*, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&) { std::vector<int> tileIndices; return tileIndices;};
	inline virtual bool pathFoundPostProcessing(bool ultDestReached) { return false;}
};

#endif