#include "bar.h"

//=============================================================================
// Initialize the Bar 
// Pre: *graphics = pointer to Graphics object
//      *textureM = pointer to TextureManager object
//      left, top = screen location
//      scale = scaling (zoom) amount
//      color = color of bar
// Post: returns true on success, false on error
//=============================================================================
bool Bar::initialize(Graphics *graphics, TextureManager *textureM, int left,
                     int top, float scale, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
    }
    catch(...)
    {
        return false;
    }
    //return okay
    return true;
}

//=============================================================================
// set the Bar size
//=============================================================================
void Bar::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;

	healthBarPercentage = p;
    spriteData.rect.right = spriteData.rect.left + (LONG)(spriteData.width*p/100);
}

//=============================================================================
// Decrease length of the bar
// Pre: currPercentage = how much of bar is filled before decreasing
//      decrementAmt = how much to decrease the bar based on frame time
//		frameTime = frame time
// Post: void
//=============================================================================
void Bar::decrease(float decrementAmt, float frameTime)
{
	// decrease the health bar a little each frame
	healthBarPercentage -= frameTime*decrementAmt;

	if (healthBarPercentage <= 0.0f)
		healthBarPercentage = 0.0f;

	set(healthBarPercentage);
}