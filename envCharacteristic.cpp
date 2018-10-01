#include "envCharacteristic.h"

//=============================================================================
// constructor
//=============================================================================

EnvCharacteristic::EnvCharacteristic(mushroomWarNS::CHARACTERISTIC_TYPE ct) : Entity()
{	
	charTp = ct;
	switch(ct)
	{
		case mushroomWarNS::HILL:
			setIsSolid(false);
			isDestructible = false;
			startFrame = 29;
			endFrame = 29;
			break;
		case mushroomWarNS::BUSH:
			setIsSolid(false);
			isDestructible = false;
			startFrame = 30;
			endFrame = 30;
			break;
		case mushroomWarNS::ROCK:
			setIsSolid(true);
			isDestructible = false;
			startFrame = 34;
			endFrame = 34;
			break;
		case mushroomWarNS::C_TREE:
			setIsSolid(true);
			isDestructible = true;
			startFrame = 32;
			endFrame = 32;
			break;
	}
}

//=============================================================================
// destructor
//=============================================================================
EnvCharacteristic::~EnvCharacteristic()
{}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void EnvCharacteristic::update(float frameTime)
{
	// stubb
	Entity::update(frameTime);
}

int EnvCharacteristic::getFlinchFrame()
{
	return -1;
}

//=============================================================================
// getEnvCharNameString
// returns the string representing the environmental characteristic name
//=============================================================================
std::string EnvCharacteristic::getEnvCharNameString(std::string& str)
{
	char tileEnvCharString[25] = { };

	// C_NONE, HILL, BUSH, ROCK, C_TREE, DECOMPOSING_MATERIAL, LITTER, MUSHROOM
	switch(charTp)
	{
	case mushroomWarNS::C_NONE:
		sprintf(tileEnvCharString,ENV_CHAR_NONE);
		break;
	case mushroomWarNS::HILL:
		sprintf(tileEnvCharString, ENV_CHAR_HILL);
		break;
	case mushroomWarNS::BUSH:
		sprintf(tileEnvCharString, ENV_CHAR_BUSH);
		break;
	case mushroomWarNS::ROCK:
		sprintf(tileEnvCharString, ENV_CHAR_ROCK);
		break;
	case mushroomWarNS::C_TREE:
		sprintf(tileEnvCharString, ENV_CHAR_C_TREE);
		break;
	case mushroomWarNS::DECOMPOSING_MATERIAL:
		sprintf(tileEnvCharString, ENV_CHAR_DECOMPOSING_MATERIAL);
		break;
	case mushroomWarNS::LITTER:
		sprintf(tileEnvCharString, ENV_CHAR_LITTER);
		break;
	case mushroomWarNS::MUSHROOM:
		sprintf(tileEnvCharString, ENV_CHAR_MUSHROOM);
		break;
	default:
		sprintf(tileEnvCharString, "???");
	}

	str = tileEnvCharString;

	return tileEnvCharString;
}