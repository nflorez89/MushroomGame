#include "toggleButton.h"

bool ToggleButton::s_isOneButtonPressed = false;
//=============================================================================
//  ToggleButton Constructor
//=============================================================================
ToggleButton::ToggleButton()
{
    buttonPressed = false;
    mouseClick = true;
}

//=============================================================================
//  ToggleButton Destructor
//=============================================================================
ToggleButton::~ToggleButton()
{
	//SAFE_DELETE(mushImg);
	//SAFE_DELETE(mushFactory);
}

//=============================================================================
// Initialize the Toggle Button
// Pre: *graphics = pointer to Graphics object
//      *textureM = pointer to TextureManager object
//      *in = pointer to Input object
//      hwnd = handle to window
//      left, top = screen location
//      scale = scaling (zoom) amount
// Post: returns true on success, false on error
//=============================================================================
bool ToggleButton::initialize(Graphics *graphics, TextureManager *textureM, TextureManager *textureM2, Input *in, HWND h,
                    int left, int top, float scale, int offFrame, int onFrame, MushFactory* muf, int mushImgFrame, int cost)
{
	lastMouseXLoc = -1;
	lastMouseYLoc = -1;

	mushCost = cost;

	mushImg = new Image();

	mushFactory = muf;

	switchOffFrame = offFrame;
	switchOnFrame = onFrame;

    try {
        Image::initialize(graphics, toggleButtonNS::IMAGE_WIDTH, toggleButtonNS::IMAGE_HEIGHT, 
                          toggleButtonNS::TEXTURE_COLS, textureM);
        setCurrentFrame(switchOffFrame);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        hwnd = h;                       // handle to the window
        input = in;                     // the input object
        switchRect.left = left;
        switchRect.top  = top;
		switchRect.right = (long)(left + (toggleButtonNS::IMAGE_WIDTH * spriteData.scale));
        switchRect.bottom = (long)(top + (spriteData.height * spriteData.scale));

		// initialize the mushroom image
		mushImg->initialize(graphics, unitNS::WIDTH, unitNS::HEIGHT, unitNS::TEXTURE_COLS, textureM2);
		mushImg->setFrames(mushImgFrame,mushImgFrame);
		mushImg->setCurrentFrame(mushImg->getStartFrame());
		mushImg->setLoop(false);
    }
    catch(...)
    {
        return false;
    }
    //return okay
    return true;
}

//=============================================================================
// Initialize the Toggle Button
// Pre: *graphics = pointer to Graphics object
//      *textureM = pointer to TextureManager object
//      *in = pointer to Input object
//      hwnd = handle to window
//      left, top = screen location
//      scale = scaling (zoom) amount
// Post: returns true on success, false on error
//=============================================================================
bool ToggleButton::initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND h,
                    int left, int top, float scale, int offFrame, int onFrame)
{
	lastMouseXLoc = -1;
	lastMouseYLoc = -1;

	mushImg = new Image();

	switchOffFrame = offFrame;
	switchOnFrame = onFrame;

    try {
        Image::initialize(graphics, toggleButtonNS::IMAGE_WIDTH, toggleButtonNS::IMAGE_HEIGHT, 
                          toggleButtonNS::TEXTURE_COLS, textureM);
        setCurrentFrame(switchOffFrame);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        hwnd = h;                       // handle to the window
        input = in;                     // the input object
        switchRect.left = left;
        switchRect.top  = top;
		switchRect.right = (long)(left + (toggleButtonNS::IMAGE_WIDTH * spriteData.scale));
        switchRect.bottom = (long)(top + (spriteData.height * spriteData.scale));
    }
    catch(...)
    {
        return false;
    }
    //return okay
    return true;
}

//=============================================================================
// creates/returns a new mushroom unit
// Post: returns a new mushroom unit
//=============================================================================
Mushroom* ToggleButton::createMushroomUnit()
{
	if(!mushFactory)
		return NULL;

	// call the factory create method
	return mushFactory->createMush();
}

//=============================================================================
// checks if this toggle button has a factory object
// Post: returns true if it has a factory object, false otherwise
//=============================================================================
bool ToggleButton::hasUnitFactory()
{
	if(mushFactory)
		return true;

	return false;
}

//=============================================================================
// Checks for mouse click on switch
//=============================================================================
void ToggleButton::update(float frameTime, int myceliumScore)
{
    if (!initialized || !visible)
        return;

    // calculate screen ratios incase window was resized
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float screenRatioX = (float)FULL_GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)FULL_GAME_HEIGHT / clientRect.bottom;

    if (input->getMouseLButton())       // if mouse left button
    {
        // if mouse clicked inside switch
        if (input->getMouseX()*screenRatioX >= switchRect.left &&
            input->getMouseX()*screenRatioX <= switchRect.right &&
            input->getMouseY()*screenRatioY >= switchRect.top &&
            input->getMouseY()*screenRatioY <= switchRect.bottom)
        {
            if(mouseClick)
            {
				mouseClick = false;

				// the below if/else statement is logic so only one (1) button can be pressed at a time
				if(!s_isOneButtonPressed && myceliumScore >= mushCost)
				{
					buttonPressed = !buttonPressed;       // toggle the switch
					setCurrentFrame(switchOnFrame);
					s_isOneButtonPressed = true;
				}
				else
				{
					if(buttonPressed)
					{
						// we must have pressed the button we already pressed a second time
						buttonPressed = !buttonPressed;       // toggle the switch
						setCurrentFrame(switchOffFrame);
						s_isOneButtonPressed = false;
					}
				}
            }
        }
    } else
        mouseClick = true;
}

//=============================================================================
// Checks for mouse click on switch
//=============================================================================
void ToggleButton::update(float frameTime)
{
    if (!initialized || !visible)
        return;

    // calculate screen ratios incase window was resized
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float screenRatioX = (float)FULL_GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)FULL_GAME_HEIGHT / clientRect.bottom;

    if (input->getMouseLButtonReleaseEndTurn()/*input->getMouseLButton()*/)       // if mouse left button
    {
        // if mouse clicked inside switch
        if (input->getMouseX()*screenRatioX >= switchRect.left &&
            input->getMouseX()*screenRatioX <= switchRect.right &&
            input->getMouseY()*screenRatioY >= switchRect.top &&
            input->getMouseY()*screenRatioY <= switchRect.bottom)
        {
            buttonPressed = true;
        }
    }
}

//=============================================================================
// cleans up all the memory in this toggle button object
//=============================================================================
void ToggleButton::cleanUpMem()
{
	deleteMushFactory();
	deleteMushImg();
}