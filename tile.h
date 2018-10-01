#ifndef _TILE_H               // Prevent multiple definitions if this 
#define _TILE_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <deque>
#include "image.h"
#include "envCharacteristic.h"
#include "mushroom.h"
#include "unit.h"

class Tile : public Image
{
protected:
	mushroomWarNS::TILE_TYPE type;
	EnvCharacteristic* envChar;
	Mushroom* occupyingMushroom;
	float xLoc, yLoc;
	Unit* occupyingEnt;
	int xPos, yPos;
	bool isTaken;
	bool isWithinAtkRng;
	Image* mycelium;
public:
	// constructor
	Tile(mushroomWarNS::TILE_TYPE, int xPosition, int yPosition, float xLocation, float yLocation);

	// Destructor
    virtual ~Tile();

	// static variable
	static int s_tilesTakenOver;
	static int s_myceliumScore;

	bool hasSolidEntity();
	Tile* getAdjacentTile(mushroomWarNS::ACTION);
	
	mushroomWarNS::DIRECTION calcMoveDir(Tile*, Tile*);
	Tile* getNextPlanTile(std::deque<Tile*> path);	// note, container may want to change its type to x,y locations instead of ptrs to Tile
	std::deque<Tile*> getGoalPath(Tile* goalTile);
	
	/*inline void setXLoc(float xLc) {xLoc = xLc;}
	inline void setYLoc(float yLc) {yLoc = yLc;}
	inline float getXLoc() {return xLoc;}
	inline float getYLoc() {return yLoc;}
	*/
	bool hasOccupyingEntity();
	bool hasOccupyingMushroom();
	bool hasOccupyingEnvChar();
	bool isOccupied();

	Tile* getClosestOpposingTile(bool, std::vector<Tile*>&, std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES>&);
	float getStraightLineDistance(Tile* aTile);
	float getStraightLineDistance(float, float);

	std::string getTileTypeString(std::string&);

	inline void setXPos(int xPs) {xPos = xPs;}
	inline void setYPos(int yPs) {yPos = yPs;}
	inline int getXPos() {return xPos;}
	inline int getYPos() {return yPos;}
	inline bool getIsTaken() { return isTaken;}
	inline void setIsTaken(bool isTkn) { isTaken = isTkn;}
	inline mushroomWarNS::TILE_TYPE getTileType() { return type;}
	inline EnvCharacteristic* getEnvChar(){ return envChar;}
	inline void setEnvChar(EnvCharacteristic* envCh) {envChar = envCh;}
	inline Unit* getOccupyingEnt() { return occupyingEnt;}
	inline Unit** getOccupyingEntRef() { return &occupyingEnt;}
	inline void setOccupyingEnt(Unit* aUnit) { occupyingEnt = aUnit;}
	inline Mushroom* getOccupyingMushroom() { return occupyingMushroom;}
	inline void setOccupyingMushroom(Mushroom* occMush) { occupyingMushroom = occMush;}
	inline Image* getMycelium() { return mycelium;}
	inline void setMycelium(Image* img) { mycelium = img;}
	inline bool getIsWithinAtkRng() { return isWithinAtkRng;}
	inline void setIsWithinAtkRng(bool isWithin) { isWithinAtkRng = isWithin;}

private:
	bool isTileInVector(std::vector<Tile*>&);
};

#endif