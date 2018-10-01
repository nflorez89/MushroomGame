// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 mushroomWar.h v1.0

#ifndef _MUSHROOMWAR_H             // Prevent multiple definitions if this 
#define _MUSHROOMWAR_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <deque>
#include <array>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "node.h"
#include "pathFinder.h"
#include "nematodeBurrowerUnit.h"
#include "nematodeCloseCombatUnit.h"
#include "nematodeParasiteUnit.h"
#include "nematodeRangedUnit.h"
#include "nematodeUnit.h"
#include "mushroomCloseCombatUnit.h"
#include "mushroomColonizerUnit.h"
#include "mushroomParasiteUnit.h"
#include "mushroomRangedUnit.h"
#include "closeCombatMushroom.h"
#include "rangedMushroom.h"
#include "parasiticMushroom.h"
#include "colonizerMushroom.h"
#include "bar.h"
#include "toggleButton.h"
#include "mushFactory.h"
#include "mushCloseCombatFactory.h"
#include "mushColonizerFactory.h"
#include "mushParasiticFactory.h"
#include "mushRangedFactory.h"
#include "mushStore.h"

using namespace std;

//struct Node;
//class PathFinder;

//=============================================================================
// This class is the core of the game
//=============================================================================
class Mushroomwar : public Game
{
private:
    // game items
	TextDX  *dxFontSmall;       // DirectX font

    TextureManager menuSpritesTexture;   // menu sprites texture
    TextureManager gridSpritesTexture;   // grid sprites texture
	TextureManager fightSpritesTexture; // attack sprites texture
    TextureManager menuItemSpritesTexture;     // menu item sprites texture
	TextureManager  dashboardTextures;
	TextureManager buttonsTexture;		// buttons texture

	Tile* currSolTile;
	Tile* nextSolTile;

	Unit* movingUnit;
	Unit* potentialAttackingUnit;
	Unit* attackingUnit;
	Unit* poisonedUnit;
	Entity* defendingUnit;
	Tile* defendingUnitTile;
	Tile* srcTile;
	Tile* destTile;
	Tile* mushTile;
	Tile* hoverTile;
	int currSolNodeInd;
	size_t totalTilesCreated;
	size_t nextNemaTileInd;
	Image psnIndicator;
	Image withinAtkRngSquare;
	ToggleButton closeCombatMushToggle;
	ToggleButton rangedMushToggle;
	ToggleButton parasiticMushToggle;
	ToggleButton colonizerMushToggle;
	ToggleButton endTurnToggle;

	ToggleButton endTurnButton;

	bool isPathFindingComplete;
	bool unitsDying;
	bool paraNemaSpecialDying;

	//float healthBarPercentage;

	Bar defendingHealthBorderBar;
	Bar defendingHealthBar;

	std::deque<Node*> frontier;
	std::deque<Node*> explored;

	std::vector<Tile*> attackableTiles;
	std::vector<Tile*> withinAttackRngTiles;
	std::vector<Tile*> enemyMushTilesChecked; // this vector is used specifically for nematodes that will need to check for the straight line closest mushroom possibly a few times
	std::vector<Tile*> enemyNematodesOnGrid; // used as a list to keep track of nematodes on grid

	// singleton instances
	PathFinder* pathFinder;
	MushStore* mushStore;
    
	std::array<Tile*,FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES> fieldGrid;

	//std::array<ToggleButton*, toggleButtonNS::NO_BUTTONS> toggleButtons;

	std::deque<Node*> aSolution;

	// factories
	MushCloseCombatFactory mushCCFact;
	MushRangedFactory mushRangedFact;
	MushParasiticFactory mushParaFact;
	MushColonizerFactory mushColFact;

	void createFieldGrid();

	void initializeTileImages();

	void initializeAllUnits();

	void initializeSpecifiedUnitTiles(std::vector<int>);

	void drawTiles();

	mushroomWarNS::DIRECTION calculateDirection(Tile*, Tile*);
	mushroomWarNS::GAME_PHASE gamePhase;

	bool containsChar(std::vector<mushroomWarNS::CHARACTERISTIC_TYPE>, mushroomWarNS::CHARACTERISTIC_TYPE);
	bool destinationReached(mushroomWarNS::DIRECTION, Tile*, Tile*);

	bool finalDestinationReached(Tile*, Unit*);

	void initializeImage(Image* img, int, int, int, int startFrame, int endFrame, float x, float y, TextureManager&);
	//void initializeImage(Image img, int startFrame, int endFrame, float x, float y);

	void takeOverRadius(Tile* aTile);

	bool isWithinRadius(Tile* centerTile, Tile* someTile, int radius = 0);

	int getXPosTile(int mouseX);

	int getTileClickedIndex();
	int getTileHoveredIndex();

	std::deque<Node*> findPath(bool getClosestDest = false);

	bool isNodeInDeq(Node* aNode, std::deque<Node*> deq);

	void setUpMessageDialog(MessageDialog*, float, float, int, const std::string);

	void cleanUpDeque(std::deque<Node*>&);

	void truncateDeque(std::deque<Node*>&, int index);

	std::vector<Tile*> getEnemiesWithinAtkRange(Tile*);

	std::vector<Tile*> getTilesWithinAtkRange(Tile*);

	int getTileIndex(Tile*);

	void initializeUnit(Unit*, Tile*);

	void initializeMushroom(Mushroom*, Tile*);

	void initializeEntity(Entity*, Tile*);

	void initializeActnTurnForAllUnits();

	Tile* getClosestOpposingTile(Tile*, bool, std::vector<Tile*>&);

	bool isTileInVector(Tile*, std::vector<Tile*>);

	float getStraightLineDistance(Tile*, Tile*);

	void removeElem(Node*, std::deque<Node*>&);

	void resetIsWithinAttackRngTiles(std::vector<Tile*>&);

	void decMushroomTurnsLeftSpread();

	//ToggleButton* getToggleButtonPressed();

	// todo: for testing purposes only. Please remove when done
	///////////////////////////////////////////////////////////////
	Unit* aMushroomCloseUnit;
	Unit* mushCloseUnit2;
	Mushroom* aMushroom;
	EnvCharacteristic* envChar1;

	Unit* aNematode1;
	Unit* aNematode2;
	Unit* aNematode3;
	Unit* aNematode4;

	void debugSolution(std::deque<Node*>);
	bool isTurnComplete(bool isPlayerTurn);
	bool isEnemyTurnComplete();
	std::string createTileInfoString(Tile*);
	std::string createUnitInfoString(Tile*);
	std::string createMushroomInfoString(Tile*);

	void resetAttackImageAnimation(Unit*);
	void resetAttackImageAnimation(Entity*);

	Unit* setSitMods(Tile*);

	///////////////////////////////////////////////////////////////

public:
    // Constructor
    Mushroomwar();

    // Destructor
    virtual ~Mushroomwar();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();

	// ai related
	Node* childNode(PathFinder* pf, Node* parent, mushroomWarNS::ACTION action, Unit* movingUnit);
	std::deque<Node*> solution(Node*);

};

#endif
