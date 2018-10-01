#ifndef _TOGGLE_BUTTON_H            // Prevent multiple definitions if this 
#define _TOGGLE_BUTTON_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "image.h"
//#include "constants.h"
#include "textureManager.h"
#include "input.h"
#include "mushFactory.h"

namespace toggleButtonNS
{
	const int	IMAGE_WIDTH = 128;
	const int	IMAGE_HEIGHT = 96;
	const int	TEXTURE_COLS = 5;
	const int   MUSH_CLOSE_UP_FRAME = 0;
	const int   MUSH_CLOSE_DOWN_FRAME = 1;
	const int   MUSH_RANGED_UP_FRAME = 2;
	const int   MUSH_RANGED_DOWN_FRAME = 3;
	const int   MUSH_PARASITIC_UP_FRAME = 4;
	const int   MUSH_PARASITIC_DOWN_FRAME = 5;
	const int   MUSH_COLONIZER_UP_FRAME = 6;
	const int   MUSH_COLONIZER_DOWN_FRAME = 7;
	const int   MUSH_END_TURN_UP_FRAME = 8;
	const int   MUSH_END_TURN_DOWN_FRAME = 9;
	const int	CLOSE_COMBAT_LEFT = 0;
	const int	RANGED_LEFT = 130;
	const int	PARASITIC_LEFT = 260;
	const int	COLONIZER_LEFT = 390;
	const int	END_TURN_LEFT = 660;
	const int	TOP = 485;
	const int	END_TURN_TOP = 523;
	const int	NO_BUTTONS = 5;
	const int	CC_MUSH_COST = 50;
	const int	RANGED_MUSH_COST = 60;
	const int	COLONIZER_MUSH_COST = 90;
	const int	PARASITIC_MUSH_COST = 80;
}

class ToggleButton : public Image
{
    private:
	static bool s_isOneButtonPressed;
    Input   *input;
    HWND    hwnd;
    RECT    switchRect;         // mouse click region
    bool    buttonPressed;           // switch state
    bool    mouseClick;         // track mouse clicks
	int switchOffFrame;
	int switchOnFrame;
	MushFactory* mushFactory;
	Image* mushImg;
	int lastMouseXLoc;
	int lastMouseYLoc;
	int mushCost;

    public:
    // Toggle switch constructor
    ToggleButton();

	// Toggle switch destructor
	~ToggleButton();
    // Initialize the Toggle Switch
    // Pre: *graphics = pointer to Graphics object
    //      *textureM = pointer to TextureManager object
    //      *in = pointer to Input object
    //      hwnd = handle to window
    //      left, top = screen location
    //      scale = scaling (zoom) amount
    bool initialize(Graphics *graphics, TextureManager *textureM, TextureManager *textureM2, Input *in, HWND hwnd,
                    int left, int top, float scale, int, int, MushFactory*, int, int);

	bool initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND hwnd,
                    int left, int top, float scale, int, int);

	// create a mushroom unit
	Mushroom* createMushroomUnit();
    // Override update
    virtual void update(float frameTime, int);
	virtual void update(float frameTime);
    // Get switch state
    bool getButtonPressed()   {return buttonPressed;}
    // Set switch state
    void setButton(bool pressed) {buttonPressed = pressed;}
	
	bool hasUnitFactory();

	static bool getIsOneButtonPressed() { return s_isOneButtonPressed;}
	static void setIsOneButtonPressed(bool isOnePressed) { s_isOneButtonPressed = isOnePressed;}

	Image* getMushImg() { return mushImg;}

	int getMushCost() { return mushCost;}

	inline int getSwitchOffFrame() { return switchOffFrame;}

	// memory cleanup functions
	void cleanUpMem();
	inline void deleteMushFactory() { SAFE_DELETE(mushFactory);}
	inline void deleteMushImg() { SAFE_DELETE(mushImg);}
};

#endif