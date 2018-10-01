#ifndef _ENVCHARACTERISTIC_H               // Prevent multiple definitions if this 
#define _ENVCHARACTERISTIC_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

// inherits from Entity class
class EnvCharacteristic : public Entity
{
protected:

	bool isDestructible;
	mushroomWarNS::CHARACTERISTIC_TYPE charTp;
public:
    // constructor
    EnvCharacteristic(mushroomWarNS::CHARACTERISTIC_TYPE);

	// destructor
	virtual ~EnvCharacteristic();

    // inherited member functions
    void update(float frameTime);

	// getter(s) 
	inline virtual bool getIsDestructible() const {return isDestructible;}

	inline virtual mushroomWarNS::CHARACTERISTIC_TYPE getType() const {return charTp;}

	virtual int getFlinchFrame();

	std::string getEnvCharNameString(std::string&);
};

#endif