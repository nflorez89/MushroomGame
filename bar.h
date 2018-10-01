#ifndef _BAR_H            // Prevent multiple definitions if this 
#define _BAR_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include "image.h"
#include "constants.h"
#include "textureManager.h"
#include "input.h"

namespace dashboardNS
{
	const int   IMAGE_SIZE = 64;        // each texture size
    const int   TEXTURE_COLS = 4;       // texture has 4 columns
    const int   BAR_FRAME = 0;          // the frame number of each texture
}
class Bar : public Image
{
private:
	float healthBarPercentage;

    public:
    // Initialize the Bar 
    // Pre: *graphics = pointer to Graphics object
    //      *textureM = pointer to TextureManager object
    //      left, top = screen location
    //      scale = scaling (zoom) amount
    //      color = color of bar
    bool initialize(Graphics *graphics, TextureManager *textureM, int left, int top,
                    float scale, COLOR_ARGB color);

	// getter(s)
	inline float getHealthBarPercentage() { return healthBarPercentage;}

    // Set Bar Size
    void set(float percentOn);

	// other
	void decrease(float decrementAmt, float frameTime);
};

#endif