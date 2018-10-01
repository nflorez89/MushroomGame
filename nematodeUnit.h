#ifndef _NEMATODE_UNIT_H               // Prevent multiple definitions if this 
#define _NEMATODE_UNIT_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "utilities.h"
#include "unit.h"
#include "tile.h"

class NematodeUnit : public Unit
{
protected:
	bool isInfected;
	Tile* getEnemyMushroomTile(std::vector<Tile*>);
public:
	// constructor
	NematodeUnit();
	NematodeUnit(float, float, int, int, int, float, float);

	void spreadSpores(Tile*);

	virtual bool canTraverse(Tile*);

	virtual bool areEnemyMushTilesLeft(std::vector<Tile*>, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>);

	virtual void setPoison(int sev);

	// getter(s)
	inline bool getIsInfected() {return isInfected;}

	// setter(s)
	inline void setIsInfected(bool infctn) { isInfected = infctn;}
private:
	bool areEnemyMushUnitTilesLeft(std::vector<Tile*> mushsChecked, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid);
	bool areEnemyMushroomTilesLeft(std::vector<Tile*> mushsChecked, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid);
};

#endif