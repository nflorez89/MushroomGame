#ifndef _UTILITIES_H               // Prevent multiple definitions if this 
#define _UTILITIES_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;

namespace mushroomWarNS
{
	enum CHARACTERISTIC_TYPE {C_NONE, HILL, BUSH, ROCK, C_TREE, DECOMPOSING_MATERIAL, LITTER, MUSHROOM};
	enum ACTION {NONE, A_UP, A_DOWN, A_LEFT, A_RIGHT};
	enum DIRECTION {D_NONE, D_UP, D_DOWN, D_LEFT, D_RIGHT};
	enum TILE_TYPE {WATER, GRASS, STONE};
	enum DEBRIS_TYPE {TREE, CORPSE};
	enum AXIS {VERT, HORIZ};
	enum GAME_PHASE {PLAYER_CHOOSE, FIND_PATH, ENTITY_MOVING, PLAYER_ATTACK, PLAYER_ATTACK_ANIMATION, PLAYER_HEALTH_BAR_ANIM, PLAYER_MENU_CHOOSE, PLAYER_MENU_CHOOSE_STILL, ENEMY_CHOOSE, ENEMY_FIND_PATH, ENEMY_ATTACK, ENEMY_ATTACK_ANIMATION, ENEMY_HEALTH_BAR_ANIM, ENEMY_POISON_DMG, ENEMY_PSN_BAR};
	enum NEMATODE_TYPE {NONE_TYPE, CLOSE, RANGED, PARASITIC, BURROWER};
}

class Utilities
{
public:
	static int generateRandNum(int);
	static float getStraightLineDistance(float, float, float, float);
	static bool calcProb(float numerator, float denominator);
};

#endif