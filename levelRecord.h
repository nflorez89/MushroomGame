#ifndef _LEVEL_RECORD_H               // Prevent multiple definitions if this 
#define _LEVEL_RECORD_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

// represents a record in one of the mushroom upgrade tables
class LevelRecord
{
protected:
	float unitHp;
	float unitAccuracy;
	float unitEvasion;
	float unitDamage;
	int unitMvmtRange;
	int unitAtkRange;
	int unitSpreadRadius;
	int spreadRadius;
	int psnEffectiveness;
	int upgradeCost;
	int spawnCost;
public:
	// constructor
	LevelRecord(float, float, float, float, int, int, int, int, int, int);

	// destructor
	~LevelRecord();

	// getters
	inline float getUnitHp() { return unitHp;}
	inline float getUnitAcc() { return unitAccuracy;}
	inline float getUnitEvasion() { return unitEvasion;}
	inline float getUnitDamage() { return unitDamage;}
	inline int getUnitMvmtRange() { return unitMvmtRange;}
	inline int getUnitAtkRange() { return unitAtkRange;}
	inline int getSpreadRadius() { return spreadRadius;}
	inline int getPsnEff() { return psnEffectiveness;}
	inline int getUpgradeCost() { return upgradeCost;}
	inline int getSpawnCost() { return spawnCost;}

	// setters
	inline void setUnitHp(float hp) { unitHp = hp;}
	inline void setUnitAcc(float acc) { unitAccuracy = acc;}
	inline void setUnitEvasion(float evs) { unitEvasion = evs;}
	inline void setUnitDamage(float dmg) { unitDamage = dmg;}
	inline void setUnitMvmtRange(int mvmtRng) { unitMvmtRange = mvmtRng;}
	inline void setUnitAtkRange(int atkRng) { unitAtkRange = atkRng;}
	inline void setSpreadRadius(int sprdRad) { spreadRadius = sprdRad;}
	inline void setPsnEff(int psnEff) { psnEffectiveness = psnEff;}
	inline void setSpawnCost(int spwnCst) { spawnCost = spwnCst;}
};

#endif