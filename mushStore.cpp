#include "mushStore.h"

MushStore* MushStore::mushStoreInst = 0;

//=============================================================================
//  MushStore Constructor
//=============================================================================
MushStore::MushStore()
{
}

//=============================================================================
//  MushStore Destructor
//=============================================================================
MushStore::~MushStore()
{
	for(size_t i = 0; i < toggleButtons.size(); i++)
	{
		toggleButtons[i]->cleanUpMem();
	}

	SAFE_DELETE(dxFontSmall);
}

//=============================================================================
// MushStore::instance
// args: none
// return value: MushStore* (instance of MushStore)
// description: will create an instance of MushStore and return it if this 
//				instance does not exist. If it does exist, then just return this
//				instance.
// remarks: This function allows this class to be responsible for keeping track
//			of its sole instance.
//=============================================================================
MushStore* MushStore::instance()
{
	// check if this instance exists or not
	if (mushStoreInst == 0)
	{
		mushStoreInst = new MushStore;
	}

	return mushStoreInst;
}

// Initialize the Toggle Switch
// Pre: *graphics = pointer to Graphics object
//      *textureM = pointer to TextureManager object
//      *in = pointer to Input object
//      hwnd = handle to window
void MushStore::initialize(Graphics *graphics, TextureManager *textureM, TextureManager *textureM2, Input *in, HWND hwnd)
{
	isItemBought = false;

	closeCombatMushToggle.initialize(graphics, textureM, textureM2, in, hwnd, toggleButtonNS::CLOSE_COMBAT_LEFT, toggleButtonNS::TOP, 0.8f, toggleButtonNS::MUSH_CLOSE_UP_FRAME, toggleButtonNS::MUSH_CLOSE_DOWN_FRAME, new MushCloseCombatFactory, CLOSE_COMBAT_MUSHROOM_START_FRAME, toggleButtonNS::CC_MUSH_COST);
	rangedMushToggle.initialize(graphics, textureM, textureM2, in, hwnd, toggleButtonNS::RANGED_LEFT, toggleButtonNS::TOP, 0.8f, toggleButtonNS::MUSH_RANGED_UP_FRAME, toggleButtonNS::MUSH_RANGED_DOWN_FRAME, new MushRangedFactory, RANGED_MUSHROOM_START_FRAME, toggleButtonNS::RANGED_MUSH_COST);
	parasiticMushToggle.initialize(graphics, textureM, textureM2, in, hwnd, toggleButtonNS::PARASITIC_LEFT, toggleButtonNS::TOP, 0.8f, toggleButtonNS::MUSH_PARASITIC_UP_FRAME, toggleButtonNS::MUSH_PARASITIC_DOWN_FRAME, new MushParasiticFactory, PARASITIC_MUSHROOM_START_FRAME, toggleButtonNS::PARASITIC_MUSH_COST);
	colonizerMushToggle.initialize(graphics, textureM, textureM2, in, hwnd, toggleButtonNS::COLONIZER_LEFT, toggleButtonNS::TOP, 0.8f, toggleButtonNS::MUSH_COLONIZER_UP_FRAME, toggleButtonNS::MUSH_COLONIZER_DOWN_FRAME, new MushColonizerFactory, COLONIZER_MUSHROOM_START_FRAME, toggleButtonNS::COLONIZER_MUSH_COST);

	// initialize DirectX fonts
    // 15 pixel high Arial
	dxFontSmall	= new TextDX();     // DirectX fonts
    if(dxFontSmall->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font for mushStore"));

	dxFontSmall->setFontColor(graphicsNS::BLACK);

	toggleButtons[0] = &closeCombatMushToggle;
	toggleButtons[1] = &rangedMushToggle;
	toggleButtons[2] = &parasiticMushToggle;
	toggleButtons[3] = &colonizerMushToggle;
}

void MushStore::update(float frameTime, int myceliumScore)
{
	for ( auto it = toggleButtons.begin(); it != toggleButtons.end(); ++it )
	{
		(*it)->update(frameTime, myceliumScore);
	}
}

// name: draw
// inputs: none
// description: draws the toggle buttons and the mushroom image if one of the buttons is pressed
void MushStore::draw()
{
	for ( auto it = toggleButtons.begin(); it != toggleButtons.end(); ++it )
	{
		(*it)->draw();

		// also draw the cost to create a new mushroom
		printCost(*it);
	}
}

// name: draw
// inputs: float for x location and float for y location
// description: draws the toggle buttons and the mushroom image if one of the buttons is pressed
void MushStore::draw(float x, float y)
{
	for ( auto it = toggleButtons.begin(); it != toggleButtons.end(); ++it )
	{
		(*it)->draw();

		// also draw the cost to create a new mushroom
		printCost(*it);

		// check if a button is pressed
		if(ToggleButton::getIsOneButtonPressed())
		{
			if((*it)->getButtonPressed())
			{
				// draw the image of the potential mushroom
				(*it)->getMushImg()->setX(x);
				(*it)->getMushImg()->setY(y);
				(*it)->getMushImg()->draw(graphicsNS::ALPHA50);
			}
		}
	}
}

//=============================================================================
// MushStore::printCost
// args: pointer to a toggle button
// return value: void
// description: prints the cost of the toggleButton on the button
//=============================================================================
void MushStore::printCost(ToggleButton* aTogBtn)
{
	char costStr [10] = { };

	// RECT: left, top, right, bottom
	long left, top, right, bottom;
	left = static_cast<long> (aTogBtn->getX());
	top = static_cast<long> (aTogBtn->getY() + (toggleButtonNS::IMAGE_HEIGHT*0.10));
	right = static_cast<long> (aTogBtn->getX() + toggleButtonNS::IMAGE_WIDTH);
	bottom = static_cast<long> (aTogBtn->getY() + (toggleButtonNS::IMAGE_HEIGHT*0.4));
	RECT costRect = {left, top, right, bottom};
	sprintf_s(costStr, "%d myc", aTogBtn->getMushCost());
	dxFontSmall->print(costStr, costRect, DT_CENTER);
}


//=============================================================================
// MushStore::buyMush
// args: ref to mycelium score
// return value: pointer to Mushroom unit
// description: returns pointer to mushroom unit after subtracting the mushroom
//				cost from the mycelium score passed in.
//=============================================================================
Mushroom* MushStore::buyMush(int& myceliumScore)
{
	ToggleButton* btn = getToggleButtonPressed();

	// if no button is pressed
	if(!btn)
		return NULL;

	// remove the cost from the mycelium score
	myceliumScore -= btn->getMushCost();
	
	// indicate an item has been bought
	isItemBought = true;

	// return a new instance of the mushroom corresponding to the button pressed
	return btn->createMushroomUnit();
}

//=============================================================================
// MushStore::getToggleButtonPressed
// args: none
// return value: pointer to ToggleButton object
// description: returns pointer to ToggleButton object representing the toggle 
//				button that is currently pressed.
//=============================================================================
ToggleButton* MushStore::getToggleButtonPressed()
{
	if(ToggleButton::getIsOneButtonPressed())
	{
		// one of the buttons is pressed, so find which one
		for ( auto it = toggleButtons.begin(); it != toggleButtons.end(); ++it )
		{
			// if this button is pressed then we found the button to return
			if((*it)->getButtonPressed())
				return *it;
		}
	}

	return NULL;
}

//=============================================================================
// MushStore::resetToggleButtons
// args: none
// return value: void
// description: resets the toggle button variables
//=============================================================================
void MushStore::resetToggleButtons()
{
	// reset static var of ToggleButton to false
	ToggleButton::setIsOneButtonPressed(false);

	// for each button, reset the image frame to the off frame and the buttonPressed var to false
	for ( auto it = toggleButtons.begin(); it != toggleButtons.end(); ++it )
	{
		(*it)->setCurrentFrame((*it)->getSwitchOffFrame());
		(*it)->setButton(false);
	}
}