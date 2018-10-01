// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 constants.h v1.0

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

//-----------------------------------------------
//                  Constants
//-----------------------------------------------
// graphic images
const char MENU_SPRITES_IMAGE[] = "pictures\\menuSprites.png"; // menu sprites
const char GRID_SPRITES_IMAGE[] = "pictures\\gridSprites.png"; // sprites that go on the grid
const char ATTACK_SPRITES_IMAGE[] = "pictures\\fightingSprites.png"; // sprites that appear on screen during battle
const char MENU_ITEM_SPRITES_IMAGE[]  = "pictures\\menuItemSprites.png";  // menu sprites like buttons
const char DASHBOARD_TEXTURES[] = "pictures\\dashboard.png";
const char BUTTON_TEXTURES[] = "pictures\\buttons.png"; // button images for purchasing mushrooms and ending the player's turn

// window
const char CLASS_NAME[] = "Mushroomwar";
const char GAME_TITLE[] = "Mushroom War";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels
const UINT FULL_GAME_WIDTH = 800;
const UINT FULL_GAME_HEIGHT = 560;

// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 6.67428e-11f;             // gravitational constant
const int FIELD_GRID_WIDTH_TILES = 20;			// field grid width in tiles
const int FIELD_GRID_HEIGHT_TILES = 15;			// field grid height in tiles
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int CLOSE_COMBAT_MUSHROOM_START_FRAME = 0;
const int CLOSE_COMBAT_MUSHROOM_END_FRAME = 2;
const int RANGED_MUSHROOM_START_FRAME = 3;
const int RANGED_MUSHROOM_END_FRAME = 5;
const int PARASITIC_MUSHROOM_START_FRAME = 6;
const int PARASITIC_MUSHROOM_END_FRAME = 8;
const int COLONIZER_MUSHROOM_START_FRAME = 9;
const int COLONIZER_MUSHROOM_END_FRAME = 11;
const int MYCELIUM_FRAME = 31;
const int NUM_ACTIONS = 5;
const int NUM_NODES_TO_EXPLORE_ONE_GO = 10;
const int CLOSE_COMBAT_NEM_FIGHT_ST_FRAME = 0;
const int CLOSE_COMBAT_NEM_FIGHT_END_FRAME = 1;
const int CLOSE_COMBAT_NEM_FLINCH_FRAME = 2;
const int RANGED_NEM_FIGHT_ST_FRAME=3;
const int RANGED_NEM_FIGHT_END_FRAME=4;
const int RANGED_NEM_FLINCH_FRAME=5;
const int PARASITE_NEM_ST_FIGHT_FRAME=6;
const int PARASITE_NEM_FLINCH_FRAME=7;
const int BURROWER_NEM_ST_FIGHT_FRAME=8;
const int BURROWER_NEM_END_FIGHT_FRAME=9;
const int BURROWER_NEM_FLINCH_FRAME=10;
const int CLOSE_COMBAT_MUSH_ST_FIGHT_FRAME=11;
const int CLOSE_COMBAT_MUSH_END_FIGHT_FRAME=12;
const int CLOSE_COMBAT_MUSH_FLINCH_FRAME=13;
const int RANGED_MUSH_ST_FIGHT_FRAME=14;
const int RANGED_MUSH_END_FIGHT_FRAME=15;
const int RANGED_MUSH_FLINCH_FRAME=16;
const int PARASITE_MUSH_ST_FIGHT_FRAME=17;
const int PARASITE_MUSH_END_FIGHT_FRAME=18;
const int PARASITE_MUSH_FLINCH_FRAME=19;
const int COLONIZER_MUSH_ST_FIGHT_FRAME=20;
const int COLONIZER_MUSH_END_FIGHT_FRAME=21;
const int COLONIZER_MUSH_FLINCH_FRAME=22;
const int CLOSE_COMBAT_MUSHROOM_START_FLINCH_FRAME = 23;
const int ClOSE_COMBAT_MUSHROOM_END_FLINCH_FRAME = 24;
const int RANGED_MUSHROOM_START_FLINCH_FRAME = 25;
const int RANGED_MUSHROOM_END_FLINCH_FRAME = 26;
const int PARASITE_MUSHROOM_START_FLINCH_FRAME = 27;
const int PARASITE_MUSHROOM_END_FLINCH_FRAME = 28;
const int COLONIZER_MUSHROOM_START_FLINCH_FRAME = 29;
const int COLONIZER_MUSHROOM_END_FLINCH_FRAME = 30;
const int ENTITY_DEATH_START_FRAME = 39;
const int ENTITY_DEATH_END_FRAME = 40;
const int PARASITE_MUSH_UNIT_DEATH_ST_FRAME = 41;
const int PARASITE_MUSH_UNIT_DEATH_END_FRAME = 42;
const int POISON_INDICATOR_FRAME= 43;
const int MUSH_UPGRADE_ST_FRAME = 44;
const int MUSH_UPGRADE_END_FRAME = 46;
const int ATTACKABLE_TILE_ST_FRAME = 47;
const int ATTACKABLE_TILE_END_FRAME = 48;
const int OPAQUE_ALPHA = 255;
const int MAX_LEVEL_RECORDS = 5;
const int TURNS_BEFORE_MUSH_SPREAD_MYCELLIUM = 3;
const int INPUT_FILE_LINE_LEN = 60;
const char WATER_TILE_NAME[] = "Water";
const char GRASS_TILE_NAME[] = "Grass";
const char STONE_TILE_NAME[] = "Stone";
const char ENV_CHAR_NONE[] = "-";
const char ENV_CHAR_HILL[] = "Hill";
const char ENV_CHAR_BUSH[] = "Bush";
const char ENV_CHAR_ROCK[] = "Rock";
const char ENV_CHAR_C_TREE[] = "Tree";
const char ENV_CHAR_DECOMPOSING_MATERIAL[] = "Decomposing material";
const char ENV_CHAR_LITTER[] = "Litter";
const char ENV_CHAR_MUSHROOM[] = "Mushroom";
const char MUSH_CLOSE_COMBAT_UNIT_NAME[] = "M. Close Combat";
const char MUSH_RANGED_UNIT_NAME[] = "M. Ranged";
const char MUSH_PARASITE_UNIT_NAME[] = "M. Parasite";
const char MUSH_COLONIZER_UNIT_NAME[] = "M. Colonizer";
const char NEMA_CLOSE_COMBAT_UNIT_NAME[] = "N. Close Combat";
const char NEMA_RANGED_UNIT_NAME[] = "N. Ranged";
const char NEMA_BURROWER_UNIT_NAME[] = "N. Burrower";
const char NEMA_PARASITE_UNIT_NAME[] = "N. Parasite";

const char MUSH_CLOSE_COMBAT_MUSH_NAME[] = "Close Combat";
const char MUSH_RANGED_MUSH_NAME[] = "Ranged";
const char MUSH_PARASITE_MUSH_NAME[] = "Parasite";
const char MUSH_COLONIZER_MUSH_NAME[] = "Colonizer";

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;       // escape key
const UCHAR ALT_KEY      = VK_MENU;         // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;       // Enter key

const int POISON_SCALING = 5;

#endif
