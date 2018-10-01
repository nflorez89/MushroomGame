#ifndef _MUSH_STORE_H               // Prevent multiple definitions if this 
#define _MUSH_STORE_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "toggleButton.h"
#include "mushFactory.h"
#include "mushCloseCombatFactory.h"
#include "mushColonizerFactory.h"
#include "mushParasiticFactory.h"
#include "mushRangedFactory.h"

// singleton
class MushStore
{
protected:
	// default constructor
	MushStore();
private:
	static MushStore* mushStoreInst;

	ToggleButton closeCombatMushToggle;
	ToggleButton rangedMushToggle;
	ToggleButton parasiticMushToggle;
	ToggleButton colonizerMushToggle;

	std::array<ToggleButton*, 4> toggleButtons;
	bool isItemBought;

	TextDX  *dxFontSmall;       // DirectX font

	void printCost(ToggleButton*);

public:

	// MushStore destructor
	~MushStore();

	// guarantees only 1 instance
	static MushStore* instance();

	// Initialize the Toggle Switch
    // Pre: *graphics = pointer to Graphics object
    //      *textureM = pointer to TextureManager object
    //      *in = pointer to Input object
    //      hwnd = handle to window
    void initialize(Graphics *graphics, TextureManager *textureM, TextureManager *textureM2, Input *in, HWND hwnd);

	// Override update
    void update(float frameTime, int);

	// draw
	void draw();
	void draw(float, float);

	Mushroom* buyMush(int&);

	// getter(s)
	inline bool isInTransaction() { return ToggleButton::getIsOneButtonPressed();}
	inline std::array<ToggleButton*, 4> getToggleButtons() { return toggleButtons;}
	inline bool getIsItemBought() { return isItemBought;}

	// setter(s)
	inline void setIsItemBought(bool iib) { isItemBought = iib;}

	// other
	ToggleButton* getToggleButtonPressed();
	void resetToggleButtons();
};

#endif