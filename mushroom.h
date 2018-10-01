#ifndef _MUSHROOM_H               // Prevent multiple definitions if this 
#define _MUSHROOM_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "unit.h"
#include "levelRecord.h"

// inherits from EnvCharacteristic class
// abstract class
class Mushroom : public Entity
{
protected:
	int level;
	float unitHp;
	float unitAccuracy;
	float unitEvasion;
	float unitDamage;
	int unitMvmtRange;
	int unitAtkRange;
	static int s_mushSpreadRadius;
	int spreadRadius;
	int psnEffectiveness;
	bool animateUpgrade;
	static Image s_mushroomUpgradeImg;
	int spreadMycTurnsLeft; // the number of turns before the mushroom will spread mycelium again
	static bool s_mushroomsClickable; // keeps track of whether the mushrooms on the field groid are clickable or not
public:

	// constructor
	Mushroom();

	// destructor
	virtual ~Mushroom();

	// factory method
	virtual Unit* spawnUnit() = 0;
	virtual bool canAffordUpgrade(int);
	virtual void upgrade(int&);// = 0;

	// getter(s)/setter(s)
	inline int getLevel() { return level;}
	inline void setLevel(int lvl) { level = lvl;}
	inline float getUnitHp() { return unitHp;}
	inline void setUnitHp(float uHp) { unitHp = uHp;}
	inline float getUnitAccuracy() { return unitAccuracy;}
	inline void setUnitAccuracy(float uAcc) { unitAccuracy = uAcc;}
	inline float getUnitEvasion() { return unitEvasion;}
	inline void setUnitEvasion(float uE) { unitEvasion = uE;}
	inline float getUnitDamage() { return unitDamage;}
	inline void setUnitDamage(float uDmg) { unitDamage = uDmg;}
	inline int getUnitMvmtRange() { return unitMvmtRange;}
	inline void setUnitMvmtRange(int uMR) { unitMvmtRange = uMR;}
	inline int getUnitAtkRange() { return unitAtkRange;}
	inline void setUnitAtkRange(int uAR) { unitAtkRange = uAR;}
	inline int getSpreadRadius() { return spreadRadius;}
	static inline int getMushSpreadRadius() { return s_mushSpreadRadius;}
	inline void setSpreadRadius(int rad) { spreadRadius = rad;}
	inline int getPsnEffectiveness() { return psnEffectiveness; }
	inline void setPsnEffectiveness(int psnEff) { psnEffectiveness = psnEff; }
	inline bool getAnimateUpgrade() { return animateUpgrade; }
	inline void setAnimateUpgrade(bool animUp) { animateUpgrade = animUp; }
	static inline void updateMushroomUpgradeImg(float frameTime) { s_mushroomUpgradeImg.update(frameTime); }
	static inline Image getMushroomUpgradeImg() { return s_mushroomUpgradeImg; }
	static inline Image& getMushroomUpgradeImgByRef() { return s_mushroomUpgradeImg; }
	virtual int getStartFlinchFrame() = 0;
	virtual int getEndFlinchFrame() = 0;
	virtual std::array<LevelRecord,MAX_LEVEL_RECORDS> getLevelRecordArr() = 0;
	static void initMushroomStaticVars();
	inline int getSpreadMycTurnsLeft() { return spreadMycTurnsLeft;}
	inline void setSpreadMycTurnsLeft(int turnsLeft) { spreadMycTurnsLeft = turnsLeft;}
	static inline bool getMushroomsClickable() { return s_mushroomsClickable;}
	static inline void setMushroomsClickable(bool isClickable) { s_mushroomsClickable = isClickable;}

	// other
	virtual std::string getMushName() = 0;
	int getUpgradeCost();
	virtual int getSpawnCost();

};

#endif