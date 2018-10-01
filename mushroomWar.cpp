// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Draw animated spaceship
// Chapter 6 spacewar.cpp v1.0
// This class is the core of the game

#include "mushroomWar.h"

//=============================================================================
// Constructor
//=============================================================================
Mushroomwar::Mushroomwar()
{}

//=============================================================================
// Destructor
//=============================================================================
Mushroomwar::~Mushroomwar()
{
	// release memory from the grid
	for(size_t i = 0; i < totalTilesCreated/*fieldGrid.size()*/; i++)
	{
		SAFE_DELETE(fieldGrid[i]);
	}

    releaseAll();           // call onLostDevice() for every graphics item
	SAFE_DELETE(dxFontSmall);
	SAFE_DELETE(pathFinder);
	SAFE_DELETE(mushStore);
	endTurnButton.deleteMushImg();
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Mushroomwar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

	aSolution.clear();

	attackableTiles.clear();
	withinAttackRngTiles.clear();

	enemyNematodesOnGrid.clear();

	srand(static_cast<unsigned int>(time(0)));

	srcTile=NULL;
	destTile=NULL;

	attackingUnit = NULL;
	defendingUnit = NULL;
	defendingUnitTile = NULL;

	currSolTile = NULL;
	nextSolTile = NULL;
	hoverTile = NULL;

	currSolNodeInd = 0;
	nextNemaTileInd = 0;
	totalTilesCreated = 0;

	defendingHealthBar.set(100.0f);
	defendingHealthBorderBar.set(100.0f);

	frontier.clear();
	explored.clear();

	pathFinder = PathFinder::instance();

	mushStore = MushStore::instance();

	isPathFindingComplete = false;

	unitsDying = false;

	//paraNemaSpecialDying = false;

	gamePhase = mushroomWarNS::PLAYER_CHOOSE;

	dxFontSmall	= new TextDX();     // DirectX fonts

    // menu sprites texture
    if (!menuSpritesTexture.initialize(graphics,MENU_SPRITES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu sprites texture"));

    // grid sprites texture
    if (!gridSpritesTexture.initialize(graphics,GRID_SPRITES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing grid sprites texture"));

	// buttons texture
	if (!buttonsTexture.initialize(graphics,BUTTON_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing buttons texture"));

	// grid attack sprites texture
	if (!fightSpritesTexture.initialize(graphics,ATTACK_SPRITES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack sprites texture"));

    // menu item sprites texture
    if (!menuItemSpritesTexture.initialize(graphics,MENU_ITEM_SPRITES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu item sprites texture"));

	// initialize DirectX fonts
    // 15 pixel high Arial
    if(dxFontSmall->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	// gauge textures
    if (!dashboardTextures.initialize(graphics, DASHBOARD_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dashboard textures"));

	defendingHealthBorderBar.initialize(graphics, &dashboardTextures, 0, 0, 1.0f, graphicsNS::BLACK);
	defendingHealthBar.initialize(graphics, &dashboardTextures, 0, 0, 1.0f, graphicsNS::LIME);

	// create the field grid
	createFieldGrid();

	// initialize each tile image
	initializeTileImages();

	initializeImage(&psnIndicator, unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, POISON_INDICATOR_FRAME, POISON_INDICATOR_FRAME, 0.0f, 0.0f, gridSpritesTexture);
	initializeImage(&withinAtkRngSquare, unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, ATTACKABLE_TILE_ST_FRAME, ATTACKABLE_TILE_END_FRAME, 0.0f, 0.0f, gridSpritesTexture);
	initializeImage(&Mushroom::getMushroomUpgradeImgByRef(), unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, MUSH_UPGRADE_ST_FRAME, MUSH_UPGRADE_END_FRAME, 0.0f, 0.0f, gridSpritesTexture);

	endTurnButton.initialize(graphics, &buttonsTexture, input, hwnd, toggleButtonNS::END_TURN_LEFT, toggleButtonNS::END_TURN_TOP, 0.5f, toggleButtonNS::MUSH_END_TURN_UP_FRAME, toggleButtonNS::MUSH_END_TURN_DOWN_FRAME);

	// init dialog box
	messageDialog->setBorderColor(graphicsNS::LTGRAY);
    messageDialog->setBackColor(SETCOLOR_ARGB(255,50,50,90));

	// initialize the mushroom store (toggle buttons)
	mushStore->initialize(graphics, &buttonsTexture, &gridSpritesTexture, input, hwnd);

	initializeAllUnits();

	initializeActnTurnForAllUnits();

	Mushroom::initMushroomStaticVars();
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Mushroomwar::update()
{
	int index = 0;
	int hoverIndex = 0;
	Tile* aTile = NULL;

	if(!unitsDying /*&& !paraNemaSpecialDying*/)
	{
		//if(input->isKeyDown(ESC_KEY) && gamePhase == mushroomWarNS::PLAYER_CHOOSE)
		if((input->wasKeyPressed(ESC_KEY) || endTurnButton.getButtonPressed()) && gamePhase == mushroomWarNS::PLAYER_CHOOSE )
		{
			gamePhase = mushroomWarNS::ENEMY_CHOOSE;

			// go through all mushrooms in play and decrement their turns left before spreading (reset if at 0)
			decMushroomTurnsLeftSpread();

			// set the button pressed to false
			endTurnButton.setButton(false);
		}

		hoverIndex = getTileHoveredIndex();

		if (gamePhase == mushroomWarNS::PLAYER_CHOOSE)
		{
			if (hoverIndex >= 0)
				hoverTile = fieldGrid[hoverIndex];
			else
				hoverTile = NULL;
		}

		// check if the player clicked on something, but only if it is in the player choose phase
		if (gamePhase == mushroomWarNS::PLAYER_CHOOSE)
		{
			index = getTileClickedIndex();

			// reset the isWithinAttackRng member var for each tile in the withinAttackRngTiles vector
			if(!attackingUnit && withinAttackRngTiles.size()){
				resetIsWithinAttackRngTiles(withinAttackRngTiles);
				withinAttackRngTiles.clear();
			}
			
			if(index >= 0)
			{
			aTile = fieldGrid[index];

			// check if the player is choosing an enemy to attack
			if(attackingUnit)
			{
				if(aTile->hasOccupyingEntity())
				{
					defendingUnitTile = aTile;
					defendingUnit = aTile->getOccupyingEnt();
					gamePhase = mushroomWarNS::PLAYER_ATTACK;
					attackingUnit->setIsAttacking(true);
					Image* atkingImg = &(attackingUnit->getAttackingUnitImgByRef());
					atkingImg->setAnimTimer(0.0f);
				}
			}
			else
			{
				// clear attackable tiles for next unit to use
				attackableTiles.clear();

				if(!srcTile && mushStore->isInTransaction())
				{
					ToggleButton* tb = mushStore->getToggleButtonPressed();
					Mushroom* newMush = NULL;
					if(tb && !aTile->isOccupied() && aTile->getTileType() == mushroomWarNS::GRASS && aTile->getIsTaken())
					{
						// buy a mushroom and place it on the field grid
						newMush = mushStore->buyMush(Tile::s_myceliumScore);

						aTile->setOccupyingMushroom(newMush);
						initializeMushroom(newMush, aTile);
						
						mushStore->resetToggleButtons();
					}
				}
				// check if the tile clicked on contains a mushroom or a mushroom unit
				else if (aTile->hasOccupyingEntity() && aTile != srcTile && aTile->getOccupyingEnt()->getActnThisTurn() && aTile->getOccupyingEnt()->isThisMushroomUnit())
				{
					srcTile = aTile;
				}
				else if((!aTile->hasOccupyingEntity() && !aTile->hasOccupyingMushroom() && (!aTile->hasOccupyingEnvChar() || !aTile->getEnvChar()->getIsSolid())&& srcTile))
				{
					Unit* occEnt = srcTile->getOccupyingEnt();

					// check if the dest clicked on is within the entity's movement range
					if(isWithinRadius(srcTile, aTile, occEnt->getMvmtRange()))
					{
						// check if the mushroom unit can traverse over the potential destination tile
						NematodeCloseCombatUnit nccu;
						if(nccu.canTraverse(aTile))
						{
							// have the unit move to location
							gamePhase = mushroomWarNS::FIND_PATH;
							destTile = aTile;
							movingUnit = occEnt;

							// mark this unit as already actioned upon
							occEnt->setActnThisTurn(false);
						}
					}
				}
				else if(aTile == srcTile)
				{
					gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE;

					attackableTiles = getEnemiesWithinAtkRange(aTile);

					messageDialogNS::MENU_TYPE menuTp = messageDialogNS::UNIT;

					withinAttackRngTiles = getTilesWithinAtkRange(aTile);

					// if enemies are within the radius of the unit, then bring up the attack button also
					if(attackableTiles.size() > 0)
					{
						menuTp = messageDialogNS::UNIT_ATK;
						potentialAttackingUnit = setSitMods(aTile);
					}

					setUpMessageDialog(messageDialog, aTile->getX(), aTile->getY(), menuTp, "Unit Actions");

					// this is specifically for the colonizer unit. Should call movingCompletedAction even if we don't move the unit to take over it's radius
					if(srcTile->hasOccupyingEntity())
						srcTile->getOccupyingEnt()->movingCompletedAction(srcTile,fieldGrid);

					// mark this unit as already actioned upon
					srcTile->getOccupyingEnt()->setActnThisTurn(false);

					srcTile = NULL;
					destTile = NULL;
					movingUnit = NULL;
					currSolNodeInd = NULL;
				}
				else if(aTile->hasOccupyingMushroom() && Mushroom::getMushroomsClickable() /*aTile->getOccupyingMushroom()->getActnThisTurn()*/)
				{
					mushTile= aTile;
					srcTile = NULL;
					destTile = NULL;
					//firstClicked = false;
					// bring up menu for the mushroom chosen
					//messageDialog->setMenuType(messageDialogNS::MUSHROOM);
					/*messageDialog->setX(aTile->getX() + unitNS::WIDTH);
					messageDialog->setY(aTile->getY()-messageDialogNS::HEIGHT);
					messageDialog->setButtonType(0);
					messageDialog->print("Mushroom Actions");
					*/
					setUpMessageDialog(messageDialog, aTile->getX(), aTile->getY(), messageDialogNS::MUSHROOM, "Mushroom Actions");
					gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE;
				}
				
			}
			}
		}

		if(gamePhase == mushroomWarNS::ENEMY_CHOOSE)
		{
			// todo: for each nematode unit, have it do a move

			// clear the attackableTiles vector
			attackableTiles.clear();

			if(enemyNematodesOnGrid.empty())
			{
				Tile* aTile;

				// get the list
				for(size_t ind = 0; ind < fieldGrid.size(); ind++)
				{
					aTile = fieldGrid[ind];
					if(aTile->hasOccupyingEntity() && !aTile->getOccupyingEnt()->isThisMushroomUnit())
					{
						enemyNematodesOnGrid.push_back(aTile);
					}
				}
			}

			if(nextNemaTileInd < enemyNematodesOnGrid.size())
			{
				// set the source tile to the next tile to use
				srcTile = enemyNematodesOnGrid[nextNemaTileInd];
				nextNemaTileInd++;
			}
		}

		if(gamePhase == mushroomWarNS::ENEMY_ATTACK)
		{
			Tile* defendingTile = attackingUnit->getEntityTileToAttack(attackableTiles);

			if(defendingUnit == NULL)
			{
				// choose a random enemy to attack
				//int randInd = Utilities::generateRandNum(attackableTiles.size()) - 1;

				//defendingUnit = attackableTiles[randInd]->getOccupyingEnt();

				if(defendingTile->hasOccupyingEntity())
				{
					defendingUnit = defendingTile->getOccupyingEnt();
				}
				else
				{
					defendingUnit = defendingTile->getOccupyingMushroom();
				}
				defendingUnit->setIsFlinching(true);

				Image* flnchingImg = &(defendingUnit->getFlinchingImgByRef());
				flnchingImg->setAnimTimer(0.0f);
			}

			float beforeAttackHp = defendingUnit->getHealth();
			defendingHealthBar.set( (beforeAttackHp/defendingUnit->getMaxHealth()) * 100.0f);

			defendingHealthBorderBar.setX(defendingUnit->getFlinchingImg().getX() + defendingUnit->getFlinchingImg().getWidth() - defendingHealthBar.getWidth());
			defendingHealthBorderBar.setY(defendingUnit->getFlinchingImg().getY() + defendingUnit->getFlinchingImg().getHeight());
			defendingHealthBar.setX(defendingUnit->getFlinchingImg().getX() + defendingUnit->getFlinchingImg().getWidth() - defendingHealthBar.getWidth());
			defendingHealthBar.setY(defendingUnit->getFlinchingImg().getY() + defendingUnit->getFlinchingImg().getHeight());

			float atkSitMod = 1.0f;
			float accSitMod = 1.0f;
			float evsSitMod = 1.0f;

			// check if the tile the defending unit is on is a bush
			if(defendingTile->hasOccupyingEnvChar())
			{
				if(defendingTile->getEnvChar()->getType() == mushroomWarNS::BUSH)
				{
					evsSitMod = 2.0f;
				}
			}

			// check if the attack will land based on the accuracy and evasion stats
			bool atkHit = attackingUnit->attackLands(defendingUnit, evsSitMod);

			if(atkHit)
			{
				// have call to attack logic here
				attackingUnit->attack(defendingUnit, 1.0f, evsSitMod);
			}

			// clear attackableTiles since we are no longer using it for this unit on this turn
			attackableTiles.clear();

			gamePhase = mushroomWarNS::ENEMY_ATTACK_ANIMATION;

		}

		if(gamePhase == mushroomWarNS::ENEMY_ATTACK_ANIMATION)
		{
			// change gamephase to ENEMY CHOOSE only once animation has completed
			if(attackingUnit->getAttackingUnitImg().getAnimationComplete() && defendingUnit->getFlinchingImg().getAnimationComplete())
			{
				gamePhase = mushroomWarNS::ENEMY_HEALTH_BAR_ANIM;
			}
		}

		if(gamePhase == mushroomWarNS::ENEMY_HEALTH_BAR_ANIM)
		{
			// change gamephase to ENEMY CHOOSE only once animation has completed
			if( (defendingHealthBar.getHealthBarPercentage()/100.0f) <= (defendingUnit->getHealth()/defendingUnit->getMaxHealth()) )
			{
				// check if set psn dmg to true
				if(attackingUnit->getPsnTurnsLeft() > 0)
				{
					attackingUnit->setPsnDmgThisTurn(true);
					gamePhase = mushroomWarNS::ENEMY_POISON_DMG;
					poisonedUnit = attackingUnit;
				}
				else
				{
					gamePhase = mushroomWarNS::ENEMY_CHOOSE;
				}

				attackingUnit->setIsAttacking(false);
				defendingUnit->setIsFlinching(false);
				attackingUnit->setActnThisTurn(false);

				resetAttackImageAnimation(attackingUnit);
				resetAttackImageAnimation(defendingUnit);

				attackingUnit = NULL;

				if(defendingUnit->getHealth() <= 0.0f)
				{
					//defendingUnit->updateDeathAnim(frameTime);
					unitsDying = true;
				}

				defendingUnit = NULL;

				srcTile = NULL;
				destTile = NULL;
				movingUnit = NULL;
				currSolNodeInd = NULL;
			}

			// decrease the health bar a little each frame
			defendingHealthBar.decrease(20.0f, frameTime);
		}

		if(gamePhase == mushroomWarNS::ENEMY_POISON_DMG)
		{
			// keep track of the unit being attacked's hp before being attacked. Will use this to display during attack animation
			float beforeAttackHp = poisonedUnit->getHealth();
			defendingHealthBar.set( (beforeAttackHp/poisonedUnit->getMaxHealth()) * 100.0f );

			defendingHealthBorderBar.setX(poisonedUnit->getX());
			defendingHealthBorderBar.setY(poisonedUnit->getY() + poisonedUnit->getHeight());
			defendingHealthBar.setX(poisonedUnit->getX());
			defendingHealthBar.setY(poisonedUnit->getY() + poisonedUnit->getHeight());

			// do the damage here
			poisonedUnit->setHealth(poisonedUnit->getHealth() - (poisonedUnit->getPsnSeverity() * POISON_SCALING));

			if(poisonedUnit->getHealth() < 0.0f)
				poisonedUnit->setHealth(0.0f);

			poisonedUnit->setPsnTurnsLeft(poisonedUnit->getPsnTurnsLeft() - 1);

			if(poisonedUnit->getPsnTurnsLeft() <= 0)
				poisonedUnit->setPsnSeverity(0);

			gamePhase = mushroomWarNS::ENEMY_PSN_BAR;
		}

		if(gamePhase == mushroomWarNS::ENEMY_PSN_BAR)
		{
			if( (defendingHealthBar.getHealthBarPercentage()/100.0f) <= (poisonedUnit->getHealth()/poisonedUnit->getMaxHealth()) )
			{
				if(poisonedUnit->getHealth() <= 0.0f)
				{
					//poisonedUnit->updateDeathAnim(frameTime);
					unitsDying = true;
				}

				poisonedUnit->setPsnDmgThisTurn(false);

				gamePhase = mushroomWarNS::ENEMY_CHOOSE;
			}

			// decrease the health bar a little each frame
			defendingHealthBar.decrease(20.0f, frameTime);
		}

		messageDialog->update();    // check for button events
		//if(gamePhase == mushroomWarNS::PLAYER_MENU_CHOOSE)
		//{
			if(messageDialog->getButtonClicked() != 0)
			{
				/*if(messageDialog->getButtonClicked() == 1)
				{
					if(messageDialog->getButtonType() == 1)
					{
						// todo: mark this entity as actioned upon

					}
				}
				else*/ if(messageDialog->getButtonClicked() == 2)
				{
					int btnTp = messageDialog->getButtonType();
					if(btnTp == 2)
					{
						// attack button clicked
						// todo: replace. have to let player choose which enemy to attack
						//messageDialog->setButtonType(0);
						//messageDialog->print("Attack button clicked");

						messageDialog->setButtonClicked(0);

						// set gamephase to player choosing enemy to attack
						gamePhase = mushroomWarNS::PLAYER_CHOOSE;
						attackingUnit = potentialAttackingUnit;
						//attackingUnit->setIsAttacking(true);
						potentialAttackingUnit = NULL;
					}
					else if(btnTp == 0)
					{
						// keep track if a space was found to spawn a unit or not
						bool unitSpawned = true;

						int mycCost = mushTile->getOccupyingMushroom()->getSpawnCost();

						// check if the player has enough myc score to pay for another unit
						if(Tile::s_myceliumScore >= mycCost)
						{
							// mushroom spawn button clicked
							// create a unit
							Unit* aUnit = mushTile->getOccupyingMushroom()->spawnUnit();

							// place the unit on the grid
							// based on the src tile, find an empty space in an immediate surrounding tile.
							// if no empty space, then cannot spawn
							// a popup window should say that no unit can spawn since no tile available
							int currInd = getTileIndex(mushTile);
							if(currInd-1 >= 0 && !fieldGrid[currInd-1]->hasOccupyingEntity() && !fieldGrid[currInd-1]->hasOccupyingMushroom() && mushTile->getXPos() > 0)
							{
								// found an empty space to the left of the mushroom
								initializeUnit(aUnit,fieldGrid[currInd-1]);
								messageDialog->setButtonClicked(0);

								// todo: decide if we want this for mushrooms also
								// mark this unit as already actioned upon
								//mushTile->getOccupyingMushroom()->setActnThisTurn(false);

							}
							else if(currInd+1 < FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES && !fieldGrid[currInd+1]->hasOccupyingEntity() && !fieldGrid[currInd+1]->hasOccupyingMushroom() && mushTile->getXPos() < (FIELD_GRID_WIDTH_TILES-1))
							{
								// found an empty space to the right of the mushroom
								initializeUnit(aUnit,fieldGrid[currInd+1]);
								messageDialog->setButtonClicked(0);
							}
							else if(currInd-FIELD_GRID_WIDTH_TILES >= 0 && !fieldGrid[currInd-FIELD_GRID_WIDTH_TILES]->hasOccupyingEntity() && !fieldGrid[currInd-FIELD_GRID_WIDTH_TILES]->hasOccupyingMushroom() && mushTile->getYPos() > 0)
							{
								// found an empty space to the top of the mushroom
								initializeUnit(aUnit,fieldGrid[currInd-FIELD_GRID_WIDTH_TILES]);
								messageDialog->setButtonClicked(0);
							}
							else if(currInd+FIELD_GRID_WIDTH_TILES < FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES && !fieldGrid[currInd+FIELD_GRID_WIDTH_TILES]->hasOccupyingEntity() && !fieldGrid[currInd+FIELD_GRID_WIDTH_TILES]->hasOccupyingMushroom() && mushTile->getYPos() < (FIELD_GRID_HEIGHT_TILES - 1))
							{
								// found an empty space to the bottom of the mushroom
								initializeUnit(aUnit,fieldGrid[currInd+FIELD_GRID_WIDTH_TILES]);
								messageDialog->setButtonClicked(0);
							}
							else
							{
								// have menu show up that says no room for another unit at this time
								setUpMessageDialog(messageDialog, GAME_WIDTH/4.0f, GAME_HEIGHT/4.0f, messageDialogNS::NO_SPAWN_SPACE, "No space to spawn unit!");
								SAFE_DELETE(aUnit);

								gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE_STILL;

								unitSpawned = false;
							}
						}
						else
						{
							char infoStrng [100] = { };
							sprintf_s(infoStrng, "Need %d myc", mycCost);

							// have menu show up that says need x amount of myc to pay for this unit
							setUpMessageDialog(messageDialog, GAME_WIDTH/4.0f, GAME_HEIGHT/4.0f, messageDialogNS::NO_SPAWN_SPACE, infoStrng);

							gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE_STILL;
							unitSpawned = false;
						}

						// set action flag so this mushroom cannot be actioned upon any more this turn
						if(unitSpawned)
						{
							// subtract the cost from the player's score
							Tile::s_myceliumScore -= mycCost;
							
							Mushroom::setMushroomsClickable(false);
						}
			
					}
					//else
					//{
						// todo: incomplete code. Replace with actual else
						//messageDialog->setButtonType(0);
						//messageDialog->print("Button 2 clicked");
					//}
				}
				else if(messageDialog->getButtonClicked() == 3)
				{
					if(mushTile && mushTile->hasOccupyingMushroom() && mushTile->getOccupyingMushroom()->getLevel() < (MAX_LEVEL_RECORDS - 1))
					{
						if(mushTile->getOccupyingMushroom()->canAffordUpgrade(Tile::s_myceliumScore))
						{
							Mushroom* currMushroom = mushTile->getOccupyingMushroom();
							// upgrade the mushroom
							currMushroom->upgrade(Tile::s_myceliumScore);

							// set flag so all mushroom cannot be actioned upon this turn
							//currMushroom->setActnThisTurn(false);
							Mushroom::setMushroomsClickable(false);

							// set flag to animate the upgrade animation
							currMushroom->setAnimateUpgrade(true);
						}
						else
						{
							char infoStrng [100] = { };
							sprintf_s(infoStrng, "Need %d myc", mushTile->getOccupyingMushroom()->getUpgradeCost());

							messageDialog->print(infoStrng);
							gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE_STILL;
						}
					}
					else
					{
						messageDialog->print("Already at Max Lvl");
						gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE_STILL;
					}

					messageDialog->setButtonClicked(0);
				}

				if(gamePhase == mushroomWarNS::PLAYER_MENU_CHOOSE)
					gamePhase = mushroomWarNS::PLAYER_CHOOSE;
				else if(gamePhase == mushroomWarNS::PLAYER_MENU_CHOOSE_STILL)
					gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE;

				// todo: check if we have performed action on this entity or not
				//if()
			}
		//}

		if(gamePhase == mushroomWarNS::PLAYER_ATTACK)
		{
			defendingUnit->setIsFlinching(true);

			Image* flnchingImg = &(defendingUnit->getFlinchingImgByRef());
			flnchingImg->setAnimTimer(0.0f);

			float atkSitMod = 1.0f;
			float accSitMod = 1.0f;
			float evsSitMod = 1.0f;

			if(defendingUnitTile->hasOccupyingEnvChar())
			{
				if(defendingUnitTile->getEnvChar()->getType() == mushroomWarNS::BUSH)
				{
					evsSitMod = 2.0f;
				}
			}

			// keep track of the unit being attacked's hp before being attacked. Will use this to display during attack animation
			float beforeAttackHp = defendingUnit->getHealth();
			defendingHealthBar.set( (beforeAttackHp/defendingUnit->getMaxHealth()) * 100.0f );

			defendingHealthBorderBar.setX(defendingUnit->getFlinchingImg().getX() + defendingUnit->getFlinchingImg().getWidth() - defendingHealthBar.getWidth());
			defendingHealthBorderBar.setY(defendingUnit->getFlinchingImg().getY() + defendingUnit->getFlinchingImg().getHeight());
			defendingHealthBar.setX(defendingUnit->getFlinchingImg().getX() + defendingUnit->getFlinchingImg().getWidth() - defendingHealthBar.getWidth());
			defendingHealthBar.setY(defendingUnit->getFlinchingImg().getY() + defendingUnit->getFlinchingImg().getHeight());

			// check if the attack will land based on the accuracy and evasion stats
			bool atkHit = attackingUnit->attackLands(defendingUnit, evsSitMod);

			if(atkHit)
			{
				// have call to attack logic here
				attackingUnit->attack(defendingUnit, 1.0f, evsSitMod);
			}

			// clear attackableTiles since we are no longer using it for this unit on this turn
			attackableTiles.clear();

			gamePhase = mushroomWarNS::PLAYER_ATTACK_ANIMATION;
		}

		if(gamePhase == mushroomWarNS::PLAYER_ATTACK_ANIMATION)
		{
			if(attackingUnit->getAttackingUnitImg().getAnimationComplete() && defendingUnit->getFlinchingImg().getAnimationComplete())
			{
				gamePhase = mushroomWarNS::PLAYER_HEALTH_BAR_ANIM;
			}
		}

		if(gamePhase == mushroomWarNS::PLAYER_HEALTH_BAR_ANIM)
		{
			// change gamephase to PLAYER CHOOSE only once animations have completed
			if( (defendingHealthBar.getHealthBarPercentage()/100.0f) <= (defendingUnit->getHealth()/defendingUnit->getMaxHealth()) )
			{
				gamePhase = mushroomWarNS::PLAYER_CHOOSE;
				attackingUnit->setIsAttacking(false);
				defendingUnit->setIsFlinching(false);

				resetAttackImageAnimation(attackingUnit);
				resetAttackImageAnimation(defendingUnit);
			
				attackingUnit = NULL;

				if(defendingUnit->getHealth() <= 0.0f)
				{
					//defendingUnit->updateDeathAnim(frameTime);
					unitsDying = true;
				}

				defendingUnit = NULL;
				defendingUnitTile = NULL;
			}

			// decrease the health bar a little each frame
			defendingHealthBar.decrease(20.0f, frameTime);
		}

		if(gamePhase == mushroomWarNS::ENTITY_MOVING)
		{
			if(movingUnit)
			{
				if(aSolution.size() > 1 && !currSolTile && !nextSolTile)
				{

					currSolTile = aSolution[currSolNodeInd]->tile;
					nextSolTile = aSolution[currSolNodeInd + 1]->tile;

					// calculate direction
					movingUnit->setMoveDir(calculateDirection(nextSolTile, currSolTile));
				}

				// set the next and current tiles unit pointer appropriately
				if (aSolution.size() > 1 && destinationReached(movingUnit->getMoveDir(), nextSolTile, currSolTile))
				{
					nextSolTile->setOccupyingEnt(movingUnit);
					currSolTile->setOccupyingEnt(NULL);
		
					// set the unit's (x,y) coords
					movingUnit->setX(nextSolTile->getX());
					movingUnit->setY(nextSolTile->getY());

					// set the unit's move dir to none
					movingUnit->setMoveDir(mushroomWarNS::D_NONE);

					// perform this unit's specific action after it has moved a space
					movingUnit->whileMovingAction(nextSolTile);

					currSolNodeInd++;
					currSolTile = NULL;
					nextSolTile = NULL;
				}
			
				// have code to check when unit has gotten to destination
				if(finalDestinationReached(destTile, movingUnit) || srcTile == destTile || aSolution.size() <= 0)
				{
					if(movingUnit->isThisMushroomUnit())
					{
						gamePhase = mushroomWarNS::PLAYER_MENU_CHOOSE;

						if(movingUnit->isThisMushroomUnit() && aSolution.size() <= 0)
						{
							destTile = srcTile;
						}

						// bring up menu for the entity chosen
						attackableTiles = getEnemiesWithinAtkRange(destTile);

						messageDialogNS::MENU_TYPE menuTp;

						withinAttackRngTiles = getTilesWithinAtkRange(destTile);

						// if enemies are within the radius of the unit, then bring up the attack button also
						if(attackableTiles.size() <= 0)
							menuTp = messageDialogNS::UNIT;
						else
						{
							menuTp = messageDialogNS::UNIT_ATK;
							potentialAttackingUnit = setSitMods(destTile);
						}

						setUpMessageDialog(messageDialog, destTile->getX(), destTile->getY(), menuTp, "Unit Actions");

					}
					else
					{
						attackableTiles = getEnemiesWithinAtkRange(destTile);

						if(!attackableTiles.empty())
						{
							attackingUnit = destTile->getOccupyingEnt();
							attackingUnit->setIsAttacking(true);

							Image* atkingImg = &(attackingUnit->getAttackingUnitImgByRef());
							atkingImg->setAnimTimer(0.0f);

							gamePhase = mushroomWarNS::ENEMY_ATTACK;

							if(destTile->hasOccupyingEnvChar())
							{
								if(destTile->getEnvChar()->getType() == mushroomWarNS::HILL && containsChar(attackingUnit->getBonusChars(), mushroomWarNS::HILL))
								{
									attackingUnit->setAccSitMod(2.0f);
								}
								else
								{
									attackingUnit->setAccSitMod(1.0f);
								}
							}
						}
						else
						{
							// check if set psn dmg to true
							if(movingUnit->getPsnTurnsLeft() > 0)
							{
								movingUnit->setPsnDmgThisTurn(true);
								gamePhase = mushroomWarNS::ENEMY_POISON_DMG;
								poisonedUnit = movingUnit;
							}
							else
							{
								gamePhase = mushroomWarNS::ENEMY_CHOOSE;
							}
						}
					}

					std::vector<int> gridIdxs;

					// the unit will do the specific action it should do after it has reached its destination
					gridIdxs = movingUnit->movingCompletedAction(destTile, fieldGrid);

					// initialize the units of the tiles corresponding to the indices (this is specific to the nematode parasite units)
					initializeSpecifiedUnitTiles(gridIdxs);

					if(gridIdxs.size() > 0)
						unitsDying = true;

					movingUnit->setGetClosestOpponent(false);
					srcTile = NULL;
					destTile = NULL;
					movingUnit = NULL;
					currSolNodeInd = NULL;

					cleanUpDeque(frontier);
					cleanUpDeque(explored);

					// todo: this is temporary. Put back after we add destructors
					//cleanUpDeque(aSolution);
					aSolution.clear();
				}

				/*if(movingUnit)
				{
					// needed here since when destination is reached, moving unit is set to null
					movingUnit->update(frameTime);
				}*/

				/*
				int size = fieldGrid.size();
				for (int ind = 0; ind < size; ind++)
				{
					aTile = fieldGrid[ind];
					if(aTile->hasOccupyingEntity())
						aTile->getOccupyingEnt()->update(frameTime);
					if(aTile->hasOccupyingMushroom())
						aTile->getOccupyingMushroom()->update(frameTime);
				}
				*/

			}
		}
	} // end !unitsDying if block

	int size = fieldGrid.size();
	for (int ind = 0; ind < size; ind++)
	{
		aTile = fieldGrid[ind];
		if(aTile->hasOccupyingEntity())
		{
			Unit* thisUnit = aTile->getOccupyingEnt();
			
			aTile->getOccupyingEnt()->update(frameTime);
			if(aTile->getOccupyingEnt()->getIsAttacking())
			{
				Image* atkingImg = &(aTile->getOccupyingEnt()->getAttackingUnitImgByRef());
				atkingImg->flipHorizontal(false);
				atkingImg->update(frameTime);
			}
			else if(aTile->getOccupyingEnt()->getIsFlinching())
			{
				Image* flnchingImg = &(aTile->getOccupyingEnt()->getFlinchingImgByRef());
				flnchingImg->flipHorizontal(true);
				flnchingImg->update(frameTime);
			}

			if(thisUnit->getHealth() <= 0.0f && (gamePhase == mushroomWarNS::PLAYER_CHOOSE || gamePhase == mushroomWarNS::ENEMY_CHOOSE || gamePhase == mushroomWarNS::ENEMY_PSN_BAR) && !thisUnit->getPsnDmgThisTurn())
			{
				// update (or begin) the death animation
				//Image* deathImg = &(thisUnit->getDeathAnimImgByRef());
				//deathImg->update(frameTime);
				thisUnit->updateDeathAnim(frameTime);

				if(thisUnit->isDeathAnimComplete())
				{
					// since this unit is dead, perform it's death action
					thisUnit->affectAfterDeath(fieldGrid);

					// remove this unit from the game
					SAFE_DELETE(thisUnit);
					aTile->setOccupyingEnt(NULL);

					// unit done dying. Can continue with regular processing
					unitsDying = false;
					/*paraNemaSpecialDying = false;*/
				}
			}
		}
		if(aTile->hasOccupyingMushroom())
		{
			Mushroom* thisMushroom = aTile->getOccupyingMushroom();
			aTile->getOccupyingMushroom()->update(frameTime);

			// check if it's time for this mushroom to take over its radius
			int sprdMycTurnsLeft = thisMushroom->getSpreadMycTurnsLeft();
			if(sprdMycTurnsLeft <= 0 && gamePhase == mushroomWarNS::PLAYER_CHOOSE)
			{
				takeOverRadius(aTile);
				thisMushroom->setSpreadMycTurnsLeft(TURNS_BEFORE_MUSH_SPREAD_MYCELLIUM);
			}

			if(thisMushroom->getAnimateUpgrade())
			{
				if(Mushroom::getMushroomUpgradeImg().getAnimationComplete())
				{
					thisMushroom->setAnimateUpgrade(false);
					Mushroom::getMushroomUpgradeImgByRef().setCurrentFrame(MUSH_UPGRADE_ST_FRAME);
				}
				else
				{
					// update the upgrade image animation, including its x,y location
					Mushroom::getMushroomUpgradeImgByRef().setX(thisMushroom->getX());
					Mushroom::getMushroomUpgradeImgByRef().setY(thisMushroom->getY());
					Mushroom::updateMushroomUpgradeImg(frameTime);
				}
			}

			if(thisMushroom->getIsFlinching())
			{
				Image* flnchingImg = &(aTile->getOccupyingMushroom()->getFlinchingImgByRef());
				flnchingImg->flipHorizontal(true);
				flnchingImg->update(frameTime);
			}
			if(thisMushroom->getHealth() <= 0.0f && (gamePhase == mushroomWarNS::PLAYER_CHOOSE || gamePhase == mushroomWarNS::ENEMY_CHOOSE || gamePhase == mushroomWarNS::ENEMY_PSN_BAR))
			{
				thisMushroom->updateDeathAnim(frameTime);

				if(thisMushroom->isDeathAnimComplete())
				{
					// remove this mushroom from the game
					SAFE_DELETE(thisMushroom);
					aTile->setOccupyingMushroom(NULL);

					// unit done dying. Can continue with regular processing
					unitsDying = false;
				}
			}
		}

	}

	withinAtkRngSquare.update(frameTime);

	// update the toggle buttons if it's the player's turn
	if(gamePhase == mushroomWarNS::PLAYER_CHOOSE && !srcTile && !attackingUnit)
	{
		endTurnButton.update(frameTime);

		if(!mushStore->getIsItemBought())
			mushStore->update(frameTime, Tile::s_myceliumScore);
	}

	// check if the enemy's turn is complete if it is currently their turn
	if(isTurnComplete(false) && gamePhase == mushroomWarNS::ENEMY_CHOOSE)
	{
		// set the turn to the player again and initialize all units to be ready to move again.
		gamePhase = mushroomWarNS::PLAYER_CHOOSE;

		// set the new score based on mycellium since beginning of player's turn
		Tile::s_myceliumScore += Tile::s_tilesTakenOver;

		initializeActnTurnForAllUnits();

		// reset variables here
		nextNemaTileInd = 0;
		enemyNematodesOnGrid.clear();
		mushStore->setIsItemBought(false);
		mushStore->resetToggleButtons();
	}
	
	/////////////////////////////////////////////////////////
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Mushroomwar::ai()
{
	if(!srcTile)
		return;

	if(gamePhase == mushroomWarNS::ENEMY_CHOOSE)
	{
		attackableTiles = getEnemiesWithinAtkRange(srcTile);
		destTile = srcTile->getOccupyingEnt()->setUpPathFinding(srcTile, attackableTiles, fieldGrid, gamePhase);

		if(gamePhase == mushroomWarNS::ENEMY_ATTACK)
		{
			attackingUnit = srcTile->getOccupyingEnt();
			attackingUnit->setIsAttacking(true);

			Image* atkingImg = &(attackingUnit->getAttackingUnitImgByRef());
			atkingImg->setAnimTimer(0.0f);
		}

	}

	if(gamePhase == mushroomWarNS::FIND_PATH || gamePhase == mushroomWarNS::ENEMY_FIND_PATH)
	{
		aSolution = findPath(srcTile->getOccupyingEnt()->getGetClosestOpponent());
	}

	if(gamePhase == mushroomWarNS::ENEMY_FIND_PATH && isPathFindingComplete)
	{
		Unit* aUnit = srcTile->getOccupyingEnt();

		if(!aSolution.empty())
		{
			// we found a solution so set the action this turn to false
			aUnit->setActnThisTurn(false);

			// need to limit the solution to just the number of steps this unit can move
			int mvmtRng = srcTile->getOccupyingEnt()->getMvmtRange();

			movingUnit = srcTile->getOccupyingEnt();

			if(aSolution.size() > mvmtRng + 1)
			{
				movingUnit->pathFoundPostProcessing(false);
				truncateDeque(aSolution, mvmtRng+1);
			}
			else
			{
				movingUnit->pathFoundPostProcessing(true);
			}

			destTile = aSolution[aSolution.size()-1]->tile;
		}
		else
		{
			// (ranged and burrower) no solution for the nematodes that try to find the closest mushroom unit to attack. Set the action to false since nothing they can do.
			// (close and parasitic) no solution for the nematodes that use different method of finding enemy. Check here is any mushroom things left on board. 
			// If not, then set actn this turn to false as nothing this one can do.

			/////////////////////////////////////////////
			std::vector<int> gridIdxs;

			unitsDying = srcTile->getOccupyingEnt()->pathFoundPostProcessing(true);

			srcTile->getOccupyingEnt()->setActnThisTurn(false);

			//paraNemaSpecialDying = true;
			//unitsDying = true;

			// the unit will do the specific action it should do after it has reached its destination
			gridIdxs = srcTile->getOccupyingEnt()->movingCompletedAction(srcTile, fieldGrid);

			// initialize the units of the tiles corresponding to the indices (this is specific to the nematode parasite units)
			if(unitsDying)
				initializeSpecifiedUnitTiles(gridIdxs);
			/////////////////////////////////////////////

			isPathFindingComplete = false;
			gamePhase = mushroomWarNS::ENEMY_CHOOSE;

			// check if set psn dmg to true
			if(aUnit->getPsnTurnsLeft() > 0)
			{
				aUnit->setPsnDmgThisTurn(true);
				gamePhase = mushroomWarNS::ENEMY_POISON_DMG;
				poisonedUnit = aUnit;
			}

			aUnit->resetTurn(fieldGrid);

			srcTile = false;
			destTile = NULL;
			cleanUpDeque(frontier);
			cleanUpDeque(explored);

		}
	}

	if(isPathFindingComplete)
	{
		gamePhase = mushroomWarNS::ENTITY_MOVING;

		// reset for the next time we need to path find
		isPathFindingComplete = false;
	}
}

bool Mushroomwar::isNodeInDeq(Node* aNode, std::deque<Node*> deq)
{
	int size = deq.size();
	Node* thisNode;
	int thisXPos, thisYPos, anXPos, aYPos;

	for ( int i = 0; i < size; i++)
	{
		thisNode = deq[i];
		thisXPos = thisNode->tile->getXPos();
		thisYPos = thisNode->tile->getYPos();
		anXPos = aNode->tile->getXPos();
		aYPos = aNode->tile->getYPos();

		if(thisXPos == anXPos && thisYPos == aYPos)
			return true;
	}

	return false;
}
//=============================================================================
// Mushroomwar::findPath
// args: none
// return value: a deque of pointers to nodes representing the solution.
// description: will return a deque of pointers to nodes if a path is found,  
//				an empty solution if there is no path
//=============================================================================
std::deque<Node*> Mushroomwar::findPath(bool getClosestPath)
{
	Unit* aUnit = srcTile->getOccupyingEnt();
	Node* aNode;
	Node* aChildNode;
	
	int numNodesExploredThisTime = 0;

	if(explored.empty())
	{
		aNode = new Node();
		
		// initialize initial state node
		aNode->tile = srcTile;

		// initialize frontier using initial state of problem
		frontier.push_back(aNode);

		// check if initial state is goal
		if(!getClosestPath)
		{
			// if we were not able to get a destination tile in the previous step, no need to try and find a path in the first place
			if(!destTile)
			{
				return solution(NULL);
			}

			if(pathFinder->goalTest(frontier.front(), destTile))
			{
				isPathFindingComplete = true;
				return solution(frontier.front());
			}
		}
	}

	while(!frontier.empty() && numNodesExploredThisTime < NUM_NODES_TO_EXPLORE_ONE_GO)
	{
		// choose a leaf node and remove it from the frontier
		aNode = frontier.front();
		frontier.pop_front();

		// add node to explored set
		explored.push_back(aNode);

		// expand the chosen node, adding the resulting nodes to the frontier
		for(int i = 0; i < NUM_ACTIONS; i++)
		{
			aChildNode = childNode(pathFinder, aNode, static_cast<mushroomWarNS::ACTION>(i), aUnit);

			// add the child node to the frontier if not in frontier or explored set
			if(!isNodeInDeq(aChildNode,frontier) && !isNodeInDeq(aChildNode, explored))
			{
				frontier.push_back(aChildNode);

				// perform goal test to check if this is the goal
				if(!getClosestPath)
				{
					if(pathFinder->goalTest(aChildNode, destTile))
					{
						isPathFindingComplete = true;

						// return solution
						return solution(aChildNode);
					}
				}
				else
				{
					if(destTile)
					{
						if(pathFinder->goalTestNextTo(destTile, aChildNode, fieldGrid))
						{
							isPathFindingComplete = true;

							// return solution
							return solution(aChildNode);
						}
					}
					else
					{
						if(pathFinder->goalTestNextTo(aChildNode, !srcTile->getOccupyingEnt()->isThisMushroomUnit(), fieldGrid, srcTile->getOccupyingEnt()->getLookingForMushroom()))
						{
							isPathFindingComplete = true;

							// return solution
							return solution(aChildNode);
						}
					}
				}
			}
			else
			{
				// delete this node
				SAFE_DELETE(aChildNode);
			}
		}

		numNodesExploredThisTime++;
	}

	if(frontier.empty())
		isPathFindingComplete = true;

	// frontier is empty so return with failure
	return solution(NULL);
}

//=============================================================================
// Handle collisions
//=============================================================================
void Mushroomwar::collisions()
{}

//=============================================================================
// Mushroomwar::childNode
// args: path finder object pointer, node, action
// return value: Node* (representing the child node)
// description: will return the child node of the parent node given based on 
//				the action given.
//=============================================================================
Node* Mushroomwar::childNode(PathFinder* pf, Node* parent, mushroomWarNS::ACTION action, Unit* movingUnit)
{
	// get the resulting node (child) from performing the given action on the given parent node
	Node* resultingNode = pf->result(parent, movingUnit, action, fieldGrid);

	// set this node's parent, action, and path costs
	resultingNode->parent = parent;
	resultingNode->action = action;

	resultingNode->pathCost = parent->pathCost + pf->stepCost(parent, action);

	return resultingNode;
}

//=============================================================================
// Mushroomwar::solution
// args: node pointer
// return value: a deque of node pointers representing the path from beginning to end
// description: Takes a node and will create a deque of nodes based on the given
//				node's ancestors, i.e. the deque will contain the path from the source
//				node to the destination/given node.
//=============================================================================
std::deque<Node*> Mushroomwar::solution(Node* aNode)
{
	std::deque<Node*> solutionPath;
	solutionPath.clear();

	// put destination node in solution path
	if(aNode != NULL)
	{
		solutionPath.push_front(aNode);

		// follow the parents back to source node to create solution path
		Node* parentNode = aNode->parent;
		while(parentNode != NULL)
		{
			solutionPath.push_front(parentNode);

			parentNode = parentNode->parent;
		}

	}
	
	return solutionPath;
}

//=============================================================================
// Render game items
//=============================================================================
void Mushroomwar::render()
{
	std::vector<Image*> images;
    graphics->spriteBegin();                // begin drawing sprites

	dxFontSmall->setFontColor(graphicsNS::BLACK);

	// draw tiles
	drawTiles();

	int size = fieldGrid.size();
	Tile* aTile = NULL;
	Unit* aUnit = NULL;
	Mushroom* aMush = NULL;
	for(int ind = 0; ind < size; ind++)
	{
		aTile = fieldGrid[ind];
		if(aTile->hasOccupyingEnvChar())
			aTile->getEnvChar()->draw();
		if(aTile->hasOccupyingEntity())
		{
			aUnit = aTile->getOccupyingEnt();
			
			// check if the unit is dead
			if(aUnit->getHealth() <= 0.0f && (unitsDying /*|| paraNemaSpecialDying*/))
			{
				Image* img = &(aUnit->getDeathAnimImgByRef());
				img->setX(aUnit->getX());
				img->setY(aUnit->getY());
				aUnit->getDeathAnimImg().draw();
			}
			else
			{
				if(aUnit->getActnThisTurn())
					aUnit->draw();
				else
					aUnit->draw(graphicsNS::GRAY);

				// if the unit is poisoned, draw the poison indicator on the unit
				if(aUnit->getPsnSeverity() > 0)
				{
					psnIndicator.setX(aUnit->getX());
					psnIndicator.setY(aUnit->getY());
					psnIndicator.draw();
				}

				if(aUnit->getIsAttacking())
				{
					images.push_back(&(aUnit->getAttackingUnitImg()));
				}
				else if(aUnit->getIsFlinching())
				{
					images.push_back(&(aUnit->getFlinchingImg()));
				}
			}
		}
		if(aTile->hasOccupyingMushroom())
		{
			aMush = aTile->getOccupyingMushroom();

			if(aMush->getHealth() <= 0.0f && unitsDying)
			{
				Image* img = &(aMush->getDeathAnimImgByRef());
				img->setX(aMush->getX());
				img->setY(aMush->getY());
				aMush->getDeathAnimImg().draw();
			}
			else
			{
				if(Mushroom::getMushroomsClickable())
					aTile->getOccupyingMushroom()->draw();
				else
					aTile->getOccupyingMushroom()->draw(graphicsNS::GRAY);

				if(aMush->getAnimateUpgrade())
				{
					// draw the animation for upgrading the mushroom
					Mushroom::getMushroomUpgradeImg().draw();
				}

				if(aMush->getIsFlinching())
				{
					images.push_back(&(aMush->getFlinchingImg()));
				}
			}
		}

		if(aTile->getIsWithinAtkRng())
		{
			withinAtkRngSquare.setX(aTile->getX());
			withinAtkRngSquare.setY(aTile->getY());
			withinAtkRngSquare.draw();
		}

	}

	// todo: testing only. Please remove
	/////////////////////////////////////////////////////////
	//aMushroomCloseUnit->draw();
	//mushCloseUnit2->draw();
	//aNematode1->draw();
	//aNematode2->draw();
	//aNematode3->draw();
	//aNematode4->draw();
	//aMushroom->draw();

	// left, top, right, bottom
	RECT scoreRect = {GAME_WIDTH,0,FULL_GAME_WIDTH,19};
	RECT scoreRectNum = {GAME_WIDTH,19,FULL_GAME_WIDTH,38};
	RECT infoRect = {GAME_WIDTH,38,FULL_GAME_WIDTH,57};
	RECT infoTileHdrRect = {GAME_WIDTH,57,FULL_GAME_WIDTH,76};
	RECT infoTilRect = {GAME_WIDTH,76,FULL_GAME_WIDTH,114};
	RECT infoUnitHdrRect = {GAME_WIDTH,114,FULL_GAME_WIDTH,133};
	RECT infoUnitRect = {GAME_WIDTH,133,FULL_GAME_WIDTH,285};
	RECT infoMushroomHdrRect = {GAME_WIDTH,281,FULL_GAME_WIDTH,300};//{GAME_WIDTH,266,FULL_GAME_WIDTH,285};
	RECT infoMushroomRect = {GAME_WIDTH,300,FULL_GAME_WIDTH,547};//{GAME_WIDTH,285,FULL_GAME_WIDTH,475};

	char infoStrng [100] = { };
	dxFontSmall->print("=== MYC SCORE ===",scoreRect,DT_CENTER);
	sprintf_s(infoStrng, "%d\n", Tile::s_myceliumScore);
	dxFontSmall->print(infoStrng,scoreRectNum,DT_CENTER);

	dxFontSmall->print("=== INFORMATION ===",infoRect,DT_CENTER);
	dxFontSmall->print("Tile:",infoTileHdrRect,DT_CENTER);

	//??? char* initialized correctly?
	std::string infoString;
	infoString = createTileInfoString(hoverTile);
	dxFontSmall->print(infoString, infoTilRect, DT_LEFT);

	dxFontSmall->print("Unit:",infoUnitHdrRect,DT_CENTER);

	infoString = createUnitInfoString(hoverTile);
	dxFontSmall->print(infoString, infoUnitRect, DT_LEFT);

	dxFontSmall->print("Mushroom:", infoMushroomHdrRect,DT_CENTER);

	infoString = createMushroomInfoString(hoverTile);
	dxFontSmall->print(infoString, infoMushroomRect, DT_LEFT);

	messageDialog->draw();      // display dialog box
	/////////////////////////////////////////////////////////

	// check if there are fighting units right now
	size = images.size();
	if (size == 2)
	{
		for(int i = 0; i < size; i++)
			images[i]->draw();
	}

	if(gamePhase == mushroomWarNS::ENEMY_HEALTH_BAR_ANIM || gamePhase == mushroomWarNS::PLAYER_HEALTH_BAR_ANIM || gamePhase == mushroomWarNS::ENEMY_PSN_BAR)
	{
		defendingHealthBorderBar.draw(graphicsNS::FILTER);
		defendingHealthBar.draw(graphicsNS::FILTER);
	}

	// draw the toggle buttons only if it's the player's turn
	if(gamePhase == mushroomWarNS::PLAYER_CHOOSE && !srcTile && !attackingUnit)
	{
		endTurnButton.draw();

		if(!mushStore->getIsItemBought())
		{

			// if one of the toggle buttons is pressed then we may need to draw the mushroom image on the field grid
			if(ToggleButton::getIsOneButtonPressed())
			{
				int hoverToggleIndex = getTileHoveredIndex();
				float x,y;
				if (hoverToggleIndex >= 0)
				{
					// draw the buttons and the mushroom image
					x = fieldGrid[hoverToggleIndex]->getX();
					y = fieldGrid[hoverToggleIndex]->getY();
					mushStore->draw(x,y);
				}
				else
				{
					// just draw the buttons
					mushStore->draw();
				}
			}
			else
			{
				// just draw the buttons
				mushStore->draw();
			}
		}
	}

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// Mushroomwar::createFieldGrid
// args: none
// return value: void
// description: Will initialize the field grid array with Tiles.
//=============================================================================
void Mushroomwar::createFieldGrid()
{
	int totalTiles = FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES;
	int row;
	int col;

	// todo: change to relative path
	std::string line;
	std::ifstream myfile ("inputFile.txt");
	char* token;
	char * cstr;
	char tileType;
	char entityType;
	Tile* currTile;

	EnvCharacteristic* envCh = NULL;

	int i = 0;

	if (myfile.is_open())
	{
		// process each line of the input file
		while ( getline (myfile,line) )
		{
			
			// check if each line is 60 characters long. If not then we should throw an error
			if(line.length() != INPUT_FILE_LINE_LEN)
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error: every line needs to be exactly 60 chars long"));

			cstr = new char [line.length()+1];
			std::strcpy (cstr, line.c_str());
			token = strtok(cstr, "|");

			std::string tokenStdStr = std::string(token);
			if(tokenStdStr.length() != 2)
			{
				SAFE_DELETE(cstr);
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error: input file not in correct format"));
			}

			while (token != NULL)
			{

				row = i/FIELD_GRID_WIDTH_TILES;
				col = i%FIELD_GRID_WIDTH_TILES;

				// get the type type and create it
				tileType = token[0];
				if(tileType == 'g')
					currTile = new Tile(mushroomWarNS::GRASS, col, row, (float)col*TILE_WIDTH, (float)row*TILE_WIDTH);
				else if(tileType == 's')
					currTile = new Tile(mushroomWarNS::STONE, col, row, (float)col*TILE_WIDTH, (float)row*TILE_WIDTH);
				else if(tileType == 'w')
					currTile = new Tile(mushroomWarNS::WATER, col, row, (float)col*TILE_WIDTH, (float)row*TILE_WIDTH);
				else if(tileType == 'h')
				{
					// attach the hill to this tile
					currTile = new Tile(mushroomWarNS::GRASS, col, row, (float)col*TILE_WIDTH, (float)row*TILE_WIDTH);
					envCh = new EnvCharacteristic(mushroomWarNS::HILL);
					currTile->setEnvChar(envCh);
					initializeEntity(envCh, currTile);
				}
				else if(tileType == 'b')
				{
					// attach the bush to this tile
					currTile = new Tile(mushroomWarNS::GRASS, col, row, (float)col*TILE_WIDTH, (float)row*TILE_WIDTH);
					envCh = new EnvCharacteristic(mushroomWarNS::BUSH);
					currTile->setEnvChar(envCh);
					initializeEntity(envCh, currTile);
				}
				else
				{
					// default
					currTile = new Tile(mushroomWarNS::GRASS, col, row, (float)col*TILE_WIDTH, (float)row*TILE_WIDTH);
				}

				// add the entity to the tile if there is one
				entityType = token[1];
				if(entityType == '1')
				{
					// create close combat mush unit and put on this tile
					LevelRecord lvlRec = CloseCombatMushroom::getLvlTbl()[0];
					currTile->setOccupyingEnt(new MushroomCloseCombatUnit(lvlRec.getUnitHp(), lvlRec.getUnitDamage(), lvlRec.getUnitAtkRange(), lvlRec.getUnitMvmtRange(), lvlRec.getSpreadRadius(), lvlRec.getUnitAcc(), lvlRec.getUnitEvasion()));
				}
				else if(entityType == '2')
				{
					// create ranged mush unit and put on this tile
					LevelRecord lvlRec = RangedMushroom::getLvlTbl()[0];
					currTile->setOccupyingEnt(new MushroomRangedUnit(lvlRec.getUnitHp(), lvlRec.getUnitDamage(), lvlRec.getUnitAtkRange(), lvlRec.getUnitMvmtRange(), lvlRec.getSpreadRadius(), lvlRec.getUnitAcc(), lvlRec.getUnitEvasion()));
				}
				else if(entityType == '3')
				{
					// create colonizer mush unit and put on this tile
					LevelRecord lvlRec = ColonizerMushroom::getLvlTbl()[0];
					currTile->setOccupyingEnt(new MushroomColonizerUnit(lvlRec.getUnitHp(), lvlRec.getUnitDamage(), lvlRec.getUnitAtkRange(), lvlRec.getUnitMvmtRange(), lvlRec.getSpreadRadius(), lvlRec.getUnitAcc(), lvlRec.getUnitEvasion()));
				}
				else if(entityType == '4')
				{
					// create parasitic mush unit and put on this tile
					LevelRecord lvlRec = ParasiticMushroom::getLvlTbl()[0];
					currTile->setOccupyingEnt(new MushroomParasiteUnit(lvlRec.getUnitHp(), lvlRec.getUnitDamage(), lvlRec.getUnitAtkRange(), lvlRec.getUnitMvmtRange(), lvlRec.getSpreadRadius(), lvlRec.getPsnEff(), lvlRec.getUnitAcc(), lvlRec.getUnitEvasion()));
				}
				else if(entityType == '5')
				{
					// create cc nema unit and put on this tile
					currTile->setOccupyingEnt(new NematodeCloseCombatUnit(80.0f, 25.0f, 2,1, 0, 90.0f, 100.0f));
				}
				else if(entityType == '6')
				{
					// create ranged nema unit and put on this tile
					currTile->setOccupyingEnt(new NematodeRangedUnit(50.0f, 15.0f, 4,2, 0, 85.0f, 65.0f));
				}
				else if(entityType == '7')
				{
					// create burrower nema unit and put on this tile
					currTile->setOccupyingEnt(new NematodeBurrowerUnit(70.0f, 20.0f, 1,2, 0, 75.0f, 100.0f));
				}
				else if(entityType == '8')
				{
					// create parasitic nema unit and put on this tile
					currTile->setOccupyingEnt(new NematodeParasiteUnit(30.0f, 0.0f, 0,3, 0, 0.0f, 100.0f));
				}
				else if(entityType == 'c')
				{
					// create cc mushroom and put on this tile
					currTile->setOccupyingMushroom(mushCCFact.createMush());
				}
				else if(entityType == 'r')
				{
					// create ranged mushroom and put on this tile
					currTile->setOccupyingMushroom(mushRangedFact.createMush());
				}
				else if(entityType == 'l')
				{
					// create colonizer mushroom and put on this tile
					currTile->setOccupyingMushroom(mushColFact.createMush());
				}
				else if(entityType == 'p')
				{
					// create parasitic mushroom and put on this tile
					currTile->setOccupyingMushroom(mushParaFact.createMush());
				}
				else if(entityType == 'o' && tileType != 'b' && tileType != 'h')
				{
					// attach a rock to this tile if there is not already an environmental char on this tile
					envCh = new EnvCharacteristic(mushroomWarNS::ROCK);
					currTile->setEnvChar(envCh);
					initializeEntity(envCh, currTile);
				}
				else if(entityType == 't' && tileType != 'b' && tileType != 'h')
				{
					// attach a tree to this tile if there is not already an environmental char on this tile
					envCh = new EnvCharacteristic(mushroomWarNS::C_TREE);
					currTile->setEnvChar(envCh);
					initializeEntity(envCh, currTile);
				}

				fieldGrid[i] = currTile;
				// move onto next token
				token = strtok (NULL, "|");
				++i;

				totalTilesCreated++;
			}

			SAFE_DELETE(cstr);
		}
		myfile.close();
	}
	else
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: could not open level layout file"));
}


//=============================================================================
// Mushroomwar::initializeTileImages
// args: none
// return value: void
// description: Will initialize each tile image in the array of tiles of the field grid
//=============================================================================
void Mushroomwar::initializeTileImages()
{
	int totalTiles = FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES;
	Tile* aTile = 0;
	for (int i = 0; i < totalTiles; i++)
	{
		aTile = fieldGrid[i];
		if (!aTile->initialize(this->getGraphics(), unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, &gridSpritesTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
		aTile->setFrames(aTile->getStartFrame(), aTile->getEndFrame());
		aTile->setCurrentFrame(aTile->getStartFrame());

		// initialize the mycelium image
		aTile->setMycelium(new Image);
		initializeImage(aTile->getMycelium(), unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, MYCELIUM_FRAME, MYCELIUM_FRAME, aTile->getX(), aTile->getY(), gridSpritesTexture);
	}
}

//=============================================================================
// Mushroomwar::initializeAllUnits
// args: none
// return value: void
// description: Will initialize all units on the grid at the time of calling this function.
//=============================================================================
void Mushroomwar::initializeAllUnits()
{
	int totalTiles = FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES;
	Tile* aTile = 0;
	for (int i = 0; i < totalTiles; i++)
	{
		aTile = fieldGrid[i];
		Unit* aUnit = aTile->getOccupyingEnt();

		// if a unit exists on this tile, initialize it
		if(aUnit)
		{
			initializeUnit(aUnit, aTile);
		}
		else
		{
			Mushroom* aMush = aTile->getOccupyingMushroom();

			// if a mushroom exists on this tile, initialize it
			if(aMush)
			{
				initializeMushroom(aMush, aTile);
			}
		}
	}
}

//=============================================================================
// Mushroomwar::initializeSpecifiedUnitTiles
// args: vector of integers
// return value: void
// description: Will initialize all units on the tiles specified by the indices
//				in the vector passed.
//=============================================================================
void Mushroomwar::initializeSpecifiedUnitTiles(std::vector<int> idxVec)
{
	Tile* aTile = 0;
	for(std::vector<int>::iterator it = idxVec.begin(); it != idxVec.end(); ++it)
	{
		aTile = fieldGrid[*it];
		if(aTile->hasOccupyingEntity())
		{
			initializeUnit(aTile->getOccupyingEnt(), aTile);
		}
	}
}

//=============================================================================
// Mushroomwar::drawTiles
// args: none
// return value: void
// description: loops through tiles in field grid and draws each one
//=============================================================================
void Mushroomwar::drawTiles()
{
	Tile* thisTile = 0;
	int totalTiles = FIELD_GRID_WIDTH_TILES*FIELD_GRID_HEIGHT_TILES;
	COLOR_ARGB color;

	for (int i = 0; i < totalTiles; i++)
	{
		color = graphicsNS::WHITE;
		thisTile = fieldGrid[i];

		if(gamePhase == mushroomWarNS::PLAYER_CHOOSE && srcTile != NULL && destTile == NULL && srcTile->hasOccupyingEntity())
		{
			// check if this tile is within the radius of the src tile
			if(isWithinRadius(srcTile, thisTile, srcTile->getOccupyingEnt()->getMvmtRange()))
			{
				// set the tile's filter to orange
				color = graphicsNS::ORANGE;
			}
		}
		thisTile->draw(color);

		// check if this tile has a mushroom
		if(thisTile->hasOccupyingMushroom())
		{
			thisTile->getOccupyingMushroom()->draw();
		}

		// check if this tile is taken
		if(thisTile->getIsTaken())
		{
			thisTile->getMycelium()->draw();
		}
	}
}

//=============================================================================
// Mushroomwar::calculateDirection
// args: next tile and current tile (pointers)
// return value: direction (none, up, down, left, or right)
// description: will return the direction from the current tile to the next tile
//=============================================================================
mushroomWarNS::DIRECTION Mushroomwar::calculateDirection(Tile* nextTile, Tile* currTile)
{
	mushroomWarNS::DIRECTION dir = mushroomWarNS::D_NONE;

	int sum = nextTile->getXPos() - currTile->getXPos();
	if(sum != 0)
	{
		if(sum > 0)
		{
			// set direction to right
			dir = mushroomWarNS::D_RIGHT;
		}
		else
		{
			// set direction to left
			dir = mushroomWarNS::D_LEFT;
		}
	}
	else
	{
		sum = nextTile->getYPos() - currTile->getYPos();
		if(sum != 0)
		{
			if(sum > 0)
			{
				// set dir to down
				dir = mushroomWarNS::D_DOWN;
			}
			else
			{
				// set dir to up
				dir = mushroomWarNS::D_UP;
			}
		}
	}

	return dir;
}

//=============================================================================
// Mushroomwar::destinationReached
// args: the direction the unit is moving in, next tile pointer, and current tile pointer
// return value: void
// description: will return true if this unit has gotten to the destination tile, false otherwise
//=============================================================================
bool Mushroomwar::destinationReached(mushroomWarNS::DIRECTION dir, Tile* nextTile, Tile* currTile)
{
	// shorthand to avoid writing with scope operator
	mushroomWarNS::DIRECTION up = mushroomWarNS::D_UP;
	mushroomWarNS::DIRECTION down = mushroomWarNS::D_DOWN;
	mushroomWarNS::DIRECTION left = mushroomWarNS::D_LEFT;
	mushroomWarNS::DIRECTION right = mushroomWarNS::D_RIGHT;
	mushroomWarNS::DIRECTION none = mushroomWarNS::D_NONE;

	Unit* unit = currTile->getOccupyingEnt();

	if( (dir==down && unit->getY() > nextTile->getY()) || 
		(dir==up && unit->getY() < nextTile->getY()) || 
		(dir==left && unit->getX() < nextTile->getX()) || 
		(dir==right && unit->getX() > nextTile->getX()) )
	{
		return true;
	}

	return false;
}

//=============================================================================
// Mushroomwar::finalDestinationReached
// args: tile pointer for the final destination tile. Unit pointer for the entity that is moving to the final destination.
// return value: bool
// description: will return true if this unit has gotten to the final destination tile, false otherwise
//=============================================================================
bool Mushroomwar::finalDestinationReached(Tile* finalTile, Unit* aUnit)
{
	if(finalTile->getOccupyingEnt() == aUnit)
		return true;
	else
		return false;
}

//=============================================================================
// Mushroomwar::initializeImage
// args: start frame, end frame, (x,y) coords
// return value: void
// description: will initialize an image object
//=============================================================================
void Mushroomwar::initializeImage(Image* img, int width, int height, int textureCols, int startFrame, int endFrame, float x, float y, TextureManager& spritesTexture)
{
	if (!img->initialize(this->graphics, width, height, textureCols, &spritesTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image"));
	img->setFrames(startFrame, endFrame);
	img->setCurrentFrame(startFrame);
	img->setX(x);
	img->setY(y);
}

//=============================================================================
// Mushroomwar::initializeImage
// args: start frame, end frame, (x,y) coords
// return value: void
// description: will initialize an image object
//=============================================================================
/*void Mushroomwar::initializeImage(Image img, int startFrame, int endFrame, float x, float y)
{
	if (!img.initialize(this->graphics, unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, &gridSpritesTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image"));
	img.setFrames(startFrame, endFrame);
	img.setCurrentFrame(startFrame);
	img.setX(x);
	img.setY(y);
}*/

//=============================================================================
// Mushroomwar::takeOverRadius
// args: pointer to tile which is center of radius
// return value: void
// description: will "take" the tiles surrounding the given tile (which has a mushroom)
//				on it).
//=============================================================================
void Mushroomwar::takeOverRadius(Tile* centerTile)
{
	Tile* thisTile = NULL;
	

	if (!centerTile->hasOccupyingMushroom())
	{
		// this tile does not contain a mushroom. Cannot take over any tiles from here. Return.
		return;
	}
	
	size_t numTiles = fieldGrid.size();
	for (size_t i = 0; i < numTiles; i++)
	{
		thisTile = fieldGrid[i];
		if(!thisTile->getIsTaken())
		{
			
			if (isWithinRadius(centerTile, thisTile) && thisTile->getTileType() == mushroomWarNS::GRASS)
			{
				thisTile->setIsTaken(true);
				//thisTile->setMycelium(new Image);
				//initializeImage(thisTile->getMycelium(), unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, MYCELIUM_FRAME, MYCELIUM_FRAME, thisTile->getX(), thisTile->getY(), gridSpritesTexture);
				Tile::s_tilesTakenOver++;
			}
		}
	}
}

//=============================================================================
// Mushroomwar::isWithinRadius
// args: pointer to tile which is center of radius, pointer to another tile
// return value: bool
// description: will return true if someTile is within centerTile's mushroom's radius.
// remark: the centerTile must have an occupying mushroom OR an occupying entity
//=============================================================================
bool Mushroomwar::isWithinRadius(Tile* centerTile, Tile* someTile, int radius)
{
	int spreadRadius;
	
	if(radius > 0)
		spreadRadius = radius;
	else if(centerTile->getOccupyingMushroom() != NULL)
		spreadRadius = Mushroom::getMushSpreadRadius();//centerTile->getOccupyingMushroom()->getSpreadRadius();
	else
		spreadRadius = centerTile->getOccupyingEnt()->getAtkRange();

	/*float someTileCenterX = someTile->getX() + TILE_WIDTH/2.0f;
	float centerTileCenterX = centerTile->getX() + TILE_WIDTH/2.0f;
	float someTileCenterY = someTile->getY() + TILE_HEIGHT/2.0f;
	float centerTileCenterY = centerTile->getY() + TILE_HEIGHT/2.0f;

	// c = square root of a^2 + b^2
	float distance = sqrt(pow((someTileCenterX - centerTileCenterX), 2) + pow((someTileCenterY - centerTileCenterY), 2));
	*/

	float distance = someTile->getStraightLineDistance(centerTile);

	// check if someTile is within the radius of the mushroom
	if(distance <= spreadRadius*unitNS::WIDTH)
		return true;

	return false;
}

//=============================================================================
// Mushroomwar::getXPosTile
// args: mouseX location on screen
// return value: int
// description: will return the x position of the tile clicked on
//=============================================================================
int Mushroomwar::getXPosTile(int mouseX)
{
	return mouseX/unitNS::WIDTH;
}

//=============================================================================
// Mushroomwar::getTileClickedIndex
// args: none
// return value: int representing an index in fieldGrid
// description: will return the tile index of the tile clicked on
//=============================================================================
int Mushroomwar::getTileClickedIndex()
{
	int mouseX = 1;
	int mouseY = 0;
	int posX = 0;
	int posY = 0;
	int index = -1;

	int tileYPos = 0;
	int tileXPos = 0;
	
	if(input->getMouseLButtonRelease()/*lMouseDown && !input->getMouseLButton()*/)
	{
		// left mouse button has just been released
		mouseX = input->getMouseX();
		mouseY = input->getMouseY();

		// check if click is within the bounds of the field grid
		if(mouseX < 0 || mouseX > GAME_WIDTH || mouseY < 0 || mouseY > GAME_HEIGHT)
			return index;

		posX = mouseX/unitNS::WIDTH;
		posY = mouseY/unitNS::WIDTH;

		// get the tile from the field grid based on the pos x and y
		index = (FIELD_GRID_WIDTH_TILES * posY) + posX;

		// testing purposes
		//tileXPos = fieldGrid[index]->getXPos();
		//tileYPos = fieldGrid[index]->getYPos();
	}

	return index;
}

//=============================================================================
// Mushroomwar::getTileHoveredIndex
// args: none
// return value: int representing an index in fieldGrid
// description: will return the tile index of the tile hovered over
//=============================================================================
int Mushroomwar::getTileHoveredIndex()
{
	int mouseX = 1;
	int mouseY = 0;
	int posX = 0;
	int posY = 0;
	int index = -1;

	// left mouse button has just been released
	mouseX = input->getMouseX();
	mouseY = input->getMouseY();

	if(mouseX >= 0 && mouseX < GAME_WIDTH && mouseY >= 0 && mouseY < GAME_HEIGHT)
	{
		posX = mouseX/unitNS::WIDTH;
		posY = mouseY/unitNS::WIDTH;

		// get the tile from the field grid based on the pos x and y
		index = (FIELD_GRID_WIDTH_TILES * posY) + posX;
	}

	return index;
}

//=============================================================================
// Mushroomwar::setUpMessageDialog
// args: message dialog pointer, x val, y val, int (button type), and message dialog title string
// return value: void
// description: will re-configure the message dialog with the x,y,button type, and dialog title
//=============================================================================
void Mushroomwar::setUpMessageDialog(MessageDialog* messageDialog, float x, float y, int buttonType, const std::string dialogTitle)
{
	float xVal = x + unitNS::WIDTH;
	float yVal = y-messageDialogNS::HEIGHT;

	// check if the message dialog will go out of bounds
	if(xVal+messageDialog->getWidth() > GAME_WIDTH)
		xVal = x - messageDialog->getWidth();

	if(yVal < 0)
		yVal = y+unitNS::HEIGHT;

	messageDialog->setX(xVal);
	messageDialog->setY(yVal);
	messageDialog->setButtonType(buttonType);
	messageDialog->print(dialogTitle);
}

//=============================================================================
// Mushroomwar::getEnemiesWithinAtkRange
// args: pointer to tile
// return value: vector of pointers to tiles
// description: will return a vector of tiles, each of which contain an enemy to
//				the entity in the given center tile, that are within the center tile 
//				unit's attack range.
//=============================================================================
std::vector<Tile*> Mushroomwar::getEnemiesWithinAtkRange(Tile* centerTile)
{
	std::vector<Tile*> attackableTiles;
	attackableTiles.clear();
	Unit* aUnit;
	Mushroom* aMushroom;

	// set enemyIsMushUnit to opposite bool value of center tile occupying entity
	bool enemyIsMushUnit = !centerTile->getOccupyingEnt()->isThisMushroomUnit();

	Tile* aTile = NULL;

	size_t numTiles = fieldGrid.size();
	for (size_t tileInd = 0; tileInd < numTiles; tileInd++)
	{
		aTile = fieldGrid[tileInd];

		// check if this tile contains a unit
		aUnit = aTile->getOccupyingEnt();
		if(aUnit != NULL)
		{
			// check if this tile contains an enemy unit
			if(aUnit->isThisMushroomUnit() == enemyIsMushUnit)
			{
				// check if this tile is within radius of center tile
				if(isWithinRadius(centerTile, aTile))
				{
					// add this tile to list of tiles within radius for unit to attack
					attackableTiles.push_back(aTile);
				}
			}
		}
		else
		{
			// no occupying entity there. Check if there is a mushroom there
			aMushroom = aTile->getOccupyingMushroom();
			if(!centerTile->getOccupyingEnt()->isThisMushroomUnit() && aMushroom != NULL)
			{
				// the unit in the center tile is a nematode and there is a mushroom at the tile we are looking at
				if(isWithinRadius(centerTile, aTile))
				{
					// add this tile to list of tiles within radius for unit to attack
					attackableTiles.push_back(aTile);
				}
			}
		}
	}

	return attackableTiles;
}

//=============================================================================
// Mushroomwar::getTilesWithinAtkRange
// args: pointer to tile
// return value: vector of pointers to tiles
// description: will return a vector of tiles, each of which are within the center tile 
//				unit's attack range. Sets each tile's isWithinAtkRng to true that is within the radius.
//=============================================================================
std::vector<Tile*> Mushroomwar::getTilesWithinAtkRange(Tile* centerTile)
{
	std::vector<Tile*> attackableTiles;
	attackableTiles.clear();

	Tile* aTile = NULL;

	size_t numTiles = fieldGrid.size();
	for (size_t tileInd = 0; tileInd < numTiles; tileInd++)
	{
		aTile = fieldGrid[tileInd];

		// check if this tile is within radius of center tile
		if(isWithinRadius(centerTile, aTile))
		{
			// set the isWithinAtkRng to true for this tile
			aTile->setIsWithinAtkRng(true);

			// add this tile to list of tiles within unit's attack radius
			attackableTiles.push_back(aTile);
		}
	}

	return attackableTiles;
}

//=============================================================================
// Mushroomwar::getTileIndex
// args: pointer to tile
// return value: integer
// description: will return an integer representing the tile's index in the 
//				field grid.
//=============================================================================
int Mushroomwar::getTileIndex(Tile* aTile)
{
	return (FIELD_GRID_WIDTH_TILES*aTile->getYPos()) + aTile->getXPos();
}

//=============================================================================
// Mushroomwar::initializeUnit
// args: pointer to unit and pointer to tile
// return value: void
// description: will initialize an unit  onto the tile given
//=============================================================================
void Mushroomwar::initializeUnit(Unit* aUnit, Tile* aTile)
{
	if(aTile->getOccupyingEnt() == NULL)
	{
		aTile->setOccupyingEnt(aUnit);
	}

	// check if this tile has a hll on it
	if(aTile->hasOccupyingEnvChar() && aTile->getEnvChar()->getType() == mushroomWarNS::HILL && containsChar(aUnit->getBonusChars(), mushroomWarNS::HILL))
	{
		// set this unit's accuracy modifier
		aUnit->setAccSitMod(2.0f);
	}

	// initialize the entities textures, frames, and (x,y) location
	initializeEntity(aUnit, aTile);

	// initialize the attack and flinch sprite textures
	initializeImage(&aUnit->getAttackingUnitImgByRef(), unitNS::FIGHT_WIDTH, unitNS::FIGHT_HEIGHT, unitNS::FIGHT_TEXTURE_COLS, aUnit->getStartFightFrame(), aUnit->getEndFightFrame(),GAME_WIDTH/4.0f, GAME_HEIGHT/10.0f, fightSpritesTexture);
	(&(aUnit->getAttackingUnitImgByRef()))->setLoop(false);
	initializeImage(&aUnit->getFlinchingImgByRef(), unitNS::FIGHT_WIDTH, unitNS::FIGHT_HEIGHT, unitNS::FIGHT_TEXTURE_COLS, aUnit->getEndFightFrame(), aUnit->getFlinchFrame(),(GAME_WIDTH*3.0f)/4.0f, GAME_HEIGHT/10.0f, fightSpritesTexture);
	(&(aUnit->getFlinchingImgByRef()))->setLoop(false);
	initializeImage(&aUnit->getDeathAnimImgByRef(), unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, aUnit->getStartDeathFrame(), aUnit->getEndDeathFrame(), 0.0f, 0.0f, gridSpritesTexture);
	(&(aUnit->getDeathAnimImgByRef()))->setLoop(false);
}

//=============================================================================
// Mushroomwar::initializeUnit
// args: pointer to unit and pointer to tile
// return value: void
// description: will initialize an unit  onto the tile given
//=============================================================================
void Mushroomwar::initializeMushroom(Mushroom* aMushroom, Tile* aTile)
{
	// initialize the entities textures, frames, and (x,y) location
	initializeEntity(aMushroom, aTile);

	// initialize the flinch sprite textures
	initializeImage(&aMushroom->getFlinchingImgByRef(), unitNS::FIGHT_WIDTH, unitNS::FIGHT_HEIGHT, unitNS::FIGHT_TEXTURE_COLS, aMushroom->getStartFlinchFrame(), aMushroom->getEndFlinchFrame(), (GAME_WIDTH*3.0f)/4.0f, GAME_HEIGHT/10.0f, fightSpritesTexture);
	(&(aMushroom->getFlinchingImgByRef()))->setLoop(false);
	initializeImage(&aMushroom->getDeathAnimImgByRef(), unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, aMushroom->getStartDeathFrame(), aMushroom->getEndDeathFrame(), 0.0f, 0.0f, gridSpritesTexture);
	(&(aMushroom->getDeathAnimImgByRef()))->setLoop(false);
}

//=============================================================================
// Mushroomwar::initializeEntity
// args: pointer to an Entity and pointer to tile
// return value: void
// description: will initialize an Entity  onto the tile given
//=============================================================================
void Mushroomwar::initializeEntity(Entity* anEntity, Tile* aTile)
{
	if (!anEntity->initialize(this, unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, &gridSpritesTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing unit"));
	anEntity->setFrames(anEntity->getStartFrame(), anEntity->getEndFrame());
	anEntity->setCurrentFrame(anEntity->getStartFrame());
    anEntity->setX(aTile->getX());
    anEntity->setY(aTile->getY());
}

//=============================================================================
// Mushroomwar::cleanUpDeque
// args: deque of pointers to nodes
// return value: void
// description: will deallocate memory currently allocated for the nodes in the 
//				given deque argument
//=============================================================================
void Mushroomwar::cleanUpDeque(std::deque<Node*>& dq)
{
	int size = dq.size();
	
	for(int nodeInd = 0; nodeInd < size; nodeInd++)
	{
		SAFE_DELETE(dq[nodeInd]);
	}

	dq.clear();
}

//=============================================================================
// Mushroomwar::cleanUpDeque
// args: deque of pointers to nodes, index
// return value: void
// description: will deallocate memory currently allocated for the nodes in the 
//				given deque argument starting at the given index
//=============================================================================
void Mushroomwar::truncateDeque(std::deque<Node*>& dq, int index)
{
	std::deque<Node*>::iterator it;

	for(it = dq.begin()+index; it != dq.end(); it++)
	{
		// check if this node exists in the frontier
		removeElem(*it, frontier);
		removeElem(*it, explored);

		SAFE_DELETE(*it);
	}

	dq.erase(dq.begin()+index, dq.end());
}

//=============================================================================
// Mushroomwar::initializeActnTurnForAllUnits
// args: none
// return value: void
// description: will initiate all actn turn vars to true for every unit on the board
//=============================================================================
void Mushroomwar::initializeActnTurnForAllUnits()
{
	int size = fieldGrid.size();
	Tile* aTile = NULL;

	for (int index = 0; index < size; index++)
	{
		aTile = fieldGrid[index];
		if(aTile->hasOccupyingEntity())
		{
			aTile->getOccupyingEnt()->clearEnemyMushTilesChecked();
			aTile->getOccupyingEnt()->setActnThisTurn(true);
		}
		if(aTile->hasOccupyingMushroom())
		{
			Mushroom::setMushroomsClickable(true);
			aTile->getOccupyingMushroom()->setActnThisTurn(true);
		}
	}
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Mushroomwar::releaseAll()
{
	dxFontSmall->onLostDevice();
    menuSpritesTexture.onLostDevice();
    gridSpritesTexture.onLostDevice();
    menuItemSpritesTexture.onLostDevice();
	messageDialog->onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Mushroomwar::resetAll()
{
	dxFontSmall->onResetDevice();
    menuSpritesTexture.onResetDevice();
    gridSpritesTexture.onResetDevice();
    menuItemSpritesTexture.onResetDevice();
	messageDialog->onResetDevice();

    Game::resetAll();
    return;
}

//=============================================================================
// Mushroomwar::isPlayersTurnComplete
// args: bool. True for if the player's turn. False if the enemy's turn.
// return value: bool
// description: will return true if all unit's on the players side have taken an action, 
//				false otherwise.
//=============================================================================
bool Mushroomwar::isTurnComplete(bool isPlayerTurn)
{
	int size = fieldGrid.size();
	Tile* aTile = NULL;

	for (int index = 0; index < size; index++)
	{
		aTile = fieldGrid[index];

		// check if there is a unit here
		if(aTile->hasOccupyingEntity())
		{
			// check if this unit is able to make a move or not
			Unit* aUnit = aTile->getOccupyingEnt();
			if(isPlayerTurn)
			{
				if(aUnit->isThisMushroomUnit() && aUnit->getActnThisTurn())
				{
					return false;
				}
			}
			else
			{
				if(!aUnit->isThisMushroomUnit() && aUnit->getActnThisTurn())
				{
					return false;
				}
			}
		}
	}

	// none of the units on the player's (or enemy's) side have an existing move to use
	return true;
}

//=============================================================================
// Mushroomwar::removeElem
// args: pointer to node, deque of pointers to nodes
// return value: void
// description: will remove the node from the deque if it exists in the deque
//=============================================================================
void Mushroomwar::removeElem(Node* someNode, std::deque<Node*>& someDeq)
{
	std::deque<Node*>::iterator it;

	for(it = someDeq.begin(); it != someDeq.end(); it++)
	{
		if(someNode == *it)
		{
			// remove this element from the frontier
			someDeq.erase(it);

			return;
		}
	}
}

//=============================================================================
// Mushroomwar::resetIsWithinAttackRngTiles
// args: reference to vector of pointers to tiles
// return value: void
// description: will set the isWithinAtkRng member variable of each tile to false
//=============================================================================
void Mushroomwar::resetIsWithinAttackRngTiles(std::vector<Tile*>& vec)
{
	int size = vec.size();
	for(int i = 0; i < size; i++)
	{
		vec[i]->setIsWithinAtkRng(false);
	}
}

//=============================================================================
// Mushroomwar::decMushroomTurnsLeftSpread
// args: none
// return value: void
// description: will decrement (or reset if at 0) the number of turns left until
//				the mushroom will spread mycelium again for each mushroom on the
//				field.
//=============================================================================
void Mushroomwar::decMushroomTurnsLeftSpread()
{
	Tile* aTile = NULL;
	Mushroom* aMush = NULL;

	// get the list
	for(size_t ind = 0; ind < fieldGrid.size(); ind++)
	{
		aTile = fieldGrid[ind];

		if(aTile->hasOccupyingMushroom())
		{
			aMush = aTile->getOccupyingMushroom();
			int turnsLeft = aMush->getSpreadMycTurnsLeft();

			if(turnsLeft <= 0)
				aMush->setSpreadMycTurnsLeft(TURNS_BEFORE_MUSH_SPREAD_MYCELLIUM);
			else
				aMush->setSpreadMycTurnsLeft(--turnsLeft);
		}

	} // end for
}

bool Mushroomwar::containsChar(std::vector<mushroomWarNS::CHARACTERISTIC_TYPE> charList, mushroomWarNS::CHARACTERISTIC_TYPE ct)
{
	int size = charList.size();
	for(int i = 0; i < size; i++)
	{
		if(charList[i] == ct)
			return true;
	}

	return false;
}

//=============================================================================
// Mushroomwar::createInfoString
// args: pointer to tile
// return value: string
// description: will create and return a string of info on the tile
//=============================================================================
std::string Mushroomwar::createTileInfoString(Tile* aTile)
{
	char infoString [100] = { };
	char infoStrTmp[100] = { };
	std::string tmp;

	if(!aTile)
	{
		return "";
	}
	aTile->getTileTypeString(tmp);
	sprintf_s(infoString, "type: %s\n", tmp.c_str());

	if(aTile->hasOccupyingEnvChar())
	{
		aTile->getEnvChar()->getEnvCharNameString(tmp);
		sprintf_s(infoStrTmp, "env char: %s", tmp.c_str());
	}
	else
	{
		sprintf_s(infoStrTmp, "env char: -");
	}
	strcat_s(infoString, infoStrTmp);

	return infoString;
}

//=============================================================================
// Mushroomwar::createUnitInfoString
// args: pointer to tile
// return value: string
// description: will create and return a string of info on the unit on the given tile
//=============================================================================
std::string Mushroomwar::createUnitInfoString(Tile* aTile)
{
	char infoString [800] = { };
	char infoStrTmp[100] = { };
	std::string tmp;
	Unit* aUnit = NULL;

	if(!aTile || !aTile->hasOccupyingEntity())
	{
		return "";
	}

	aUnit = aTile->getOccupyingEnt();

	tmp = aUnit->getUnitName();
	sprintf_s(infoString, "unit type: %s\n", tmp.c_str());

	sprintf_s(infoStrTmp, "hp: %.2f/%.2f\n", aUnit->getHealth(), aUnit->getMaxHealth());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "accuracy: %.2f\n", aUnit->getAccuracy());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "evasion: %.2f\n", aUnit->getEvasion());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "attack dmg: %.2f\n", aUnit->getAttackDmg());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "movement range: %d\n", aUnit->getMvmtRange());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "attack range: %d\n", aUnit->getAtkRange());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "spread rad: %d\n", aUnit->getSpreadRadius());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "psn sev: %d\n", aUnit->getPsnSeverity());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "psn eff: %d\n", aUnit->getPsnEffectiveness());
	strcat_s(infoString, infoStrTmp);

	return infoString;
}

//=============================================================================
// Mushroomwar::createMushroomInfoString
// args: pointer to tile
// return value: string
// description: will create and return a string of info on the mushroom on the given tile
//=============================================================================
std::string Mushroomwar::createMushroomInfoString(Tile* aTile)
{
	char infoString [800] = { };
	char infoStrTmp[100] = { };
	std::string tmp;
	Mushroom* aMush = NULL;

	if(!aTile || !aTile->hasOccupyingMushroom())
	{
		return "";
	}

	aMush = aTile->getOccupyingMushroom();

	tmp = aMush->getMushName();
	sprintf_s(infoString, "type: %s\n", tmp.c_str());

	sprintf_s(infoStrTmp, "level: %d\n", aMush->getLevel()+1);
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "hp: %.2f/%.2f\n", aMush->getHealth(), aMush->getMaxHealth());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "unit hp: %.2f\n", aMush->getUnitHp());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "unit acc: %.2f\n", aMush->getUnitAccuracy());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "unit evasion: %.2f\n", aMush->getUnitEvasion());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "unit damage: %.2f\n", aMush->getUnitDamage());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "unit mvmt rng: %d\n", aMush->getUnitMvmtRange());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "unit atk rng: %d\n", aMush->getUnitAtkRange());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "unit spread rad: %d\n", aMush->getSpreadRadius());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "psn Eff: %d\n", aMush->getPsnEffectiveness());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "Mush Sprd turns left: %d\n", aMush->getSpreadMycTurnsLeft());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "Upgrade cost: %d\n", aMush->getUpgradeCost());
	strcat_s(infoString, infoStrTmp);

	sprintf_s(infoStrTmp, "Unit spawn cost: %d\n", aMush->getSpawnCost());
	strcat_s(infoString, infoStrTmp);

	return infoString;
}

//=============================================================================
// Mushroomwar::resetAttackImageAnimation
// args: pointer to Unit
// return value: void
// description: will reset image isAnimComplete and current frame of the attacking image
//=============================================================================
void Mushroomwar::resetAttackImageAnimation(Unit* aUnit)
{
	aUnit->getAttackingUnitImgByRef().setCurrentFrame(attackingUnit->getStartFightFrame());
	aUnit->getAttackingUnitImgByRef().setAnimationComplete(false);
}

//=============================================================================
// Mushroomwar::resetAttackImageAnimation
// args: pointer to an entity
// return value: void
// description: will reset image isAnimComplete and current frame of the flinching image
//=============================================================================
void Mushroomwar::resetAttackImageAnimation(Entity* anEntity)
{
	anEntity->getFlinchingImgByRef().setCurrentFrame(defendingUnit->getEndFightFrame());
	anEntity->getFlinchingImgByRef().setAnimationComplete(false);
}

//=============================================================================
// Mushroomwar::setSitMods
// args: pointer to a Tile
// return value: a pointer to the unit we are setting the situational modifiers to
// description: will set the unit on the tile's attack and/or accuracy modifier
///				if the unit is on a tile that grants it a bonus.
//=============================================================================
Unit* Mushroomwar::setSitMods(Tile* aTile)
{
	if(!aTile->hasOccupyingEntity())
		return NULL;

	Unit* thisUnit = aTile->getOccupyingEnt();

	if(aTile->hasOccupyingEnvChar())
	{
		if(aTile->getEnvChar()->getType() == mushroomWarNS::HILL && containsChar(thisUnit->getBonusChars(), mushroomWarNS::HILL))
		{
			thisUnit->setAccSitMod(2.0f);
		}
		else
		{
			thisUnit->setAccSitMod(1.0f);
		}
	}

	return thisUnit;
}