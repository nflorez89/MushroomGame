// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 entity.h v1.0

#ifndef _ENTITY_H               // Prevent multiple definitions if this 
#define _ENTITY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"
#include "utilities.h"

class MushroomCloseCombatUnit;
class MushroomColonizerUnit;
class MushroomRangedUnit;
class MushroomParasiteUnit;
class NematodeBurrowerUnit;
class NematodeParasiteUnit;
class NematodeRangedUnit;
class NematodeCloseCombatUnit;

namespace entityNS
{
    enum COLLISION_TYPE {NONE, CIRCLE, BOX, ROTATED_BOX};
    const float GRAVITY = 6.67428e-11f;         // gravitational constant
}

class Entity : public Image
{
    // Entity properties
  protected:
    entityNS::COLLISION_TYPE collisionType;
    VECTOR2 center;         // center of entity
    float   radius;         // radius of collision circle
    VECTOR2 distSquared;    // used for calculating circle collision
    float   sumRadiiSquared;
    // edge specifies the collision box relative to the center of the entity.
    // left and top are typically negative numbers
    RECT    edge;           // for BOX and ROTATED_BOX collision detection
    VECTOR2 corners[4];     // for ROTATED_BOX collision detection
    VECTOR2 edge01,edge03;  // edges used for projection
    float   edge01Min, edge01Max, edge03Min, edge03Max; // min and max projections
    VECTOR2 velocity;       // velocity
    VECTOR2 deltaV;         // added to velocity during next call to update()
    float   mass;           // Mass of entity
    float   health;         // health 0 to 100
	float maxHealth;
	float	prevHealth;		// used to keep track of health right before being attacked
    float   rr;             // Radius squared variable
    float   force;          // Force of gravity
    float   gravity;        // gravitational constant of the game universe
    Input   *input;         // pointer to the input system
    HRESULT hr;             // standard return type
    bool    active;         // only active entities may collide
    bool    rotatedBoxReady;    // true when rotated collision box is ready
	bool	isSolid;		// determines if a unit is able to occupy the same cell as this entity or not
	bool isFlinching;
	Image flinchingImg;
	Image deathAnimImg;
	bool actnThisTurn;

    // --- The following functions are protected because they are not intended to be
    // --- called from outside the class.
    // Circular collision detection 
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideCircle(Entity &ent, VECTOR2 &collisionVector);
    // Axis aligned box collision detection
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between boxes
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideRotatedBox(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between box and circle
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection helper functions
    void computeRotatedBox();
    bool projectionsOverlap(Entity &ent);
    bool collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector);

  public:
    // Constructor
    Entity();
    // Destructor
    virtual ~Entity() {}

    ////////////////////////////////////////
    //           Get functions            //
    ////////////////////////////////////////

    // Return center of scaled Entity as screen x,y.
    virtual const VECTOR2* getCenter()   
    {
        center = VECTOR2(getCenterX(),getCenterY());
        return &center;
    }

    // Return radius of collision circle.
    virtual float getRadius() const     {return radius;}

    // Return RECT structure used for BOX and ROTATED_BOX collision detection.
    virtual const RECT& getEdge() const {return edge;}

    // Return corner c of ROTATED_BOX
    virtual const VECTOR2* getCorner(UINT c) const
    {
        if(c>=4) 
            c=0;
        return &corners[c]; 
    }

    // Return velocity vector.
    virtual const VECTOR2 getVelocity() const {return velocity;}

    // Return active.
    virtual bool  getActive()         const {return active;}

    // Return mass.
    virtual float getMass()           const {return mass;}

    // Return gravitational constant.
    virtual float getGravity()        const {return gravity;}

    // Return health;
    virtual float getHealth()         const {return health;}

	// Return max health
	virtual float getMaxHealth() const { return maxHealth;}

    // Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
    virtual entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

	virtual bool getIsSolid() const {return isSolid;}

	virtual bool getIsFlinching() { return isFlinching; }
	virtual Image getFlinchingImg() { return flinchingImg; }
	virtual Image& getFlinchingImgByRef() { return flinchingImg; }
	virtual Image* getFlinchingImgPtr() { return &flinchingImg; }
	virtual int getFlinchFrame() = 0;
	virtual int getEndFightFrame() { return -1;}

	virtual float getAccuracy() { return -1000000.0f;}
	virtual float getEvasion() { return -1000000.0f;}

	inline virtual Image getDeathAnimImg() { return deathAnimImg;}
	inline virtual Image& getDeathAnimImgByRef() { return deathAnimImg;}

	inline virtual int getStartDeathFrame() { return ENTITY_DEATH_START_FRAME;}
	inline virtual int getEndDeathFrame() { return ENTITY_DEATH_END_FRAME;}

	inline bool getActnThisTurn() {return actnThisTurn;}

    ////////////////////////////////////////
    //           Set functions            //
    ////////////////////////////////////////

    // Set velocity.
    virtual void  setVelocity(VECTOR2 v)    {velocity = v;}

    // Set delta velocity. Added to velocity in update().
    virtual void  setDeltaV(VECTOR2 dv)     {deltaV = dv;}

    // Set active.
    virtual void  setActive(bool a)         {active = a;}

    // Set health.
    virtual void setHealth(float h)         {health = h;}

	// Set max health
	virtual void setMaxHealth(float maxHp) { maxHealth = maxHp;}

	// Set previous health
	virtual void setPrevHealth(float h)		{prevHealth = h;}

    // Set mass.
    virtual void  setMass(float m)          {mass = m;}

    // Set gravitational constant. Default is 6.67428e-11
    virtual void  setGravity(float g)       {gravity = g;}

    // Set radius of collision circle.
    virtual void setCollisionRadius(float r)    {radius = r;}

	virtual void setIsSolid(bool isSld) {isSolid = isSld;}

	virtual void setIsFlinching(bool isFlch) { isFlinching = isFlch; }
	virtual void setFlinchingImg(Image img) { flinchingImg = img; }

	inline virtual void setDeathAnimImg(Image img) { deathAnimImg = img;}

	inline void setActnThisTurn(bool actnTrn) {actnThisTurn = actnTrn;}

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

	virtual void isAttackedBy(NematodeParasiteUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeBurrowerUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeRangedUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(NematodeCloseCombatUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomCloseCombatUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomParasiteUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomColonizerUnit*, float accuracySitMod, float atkSitMod);
	virtual void isAttackedBy(MushroomRangedUnit*, float accuracySitMod, float atkSitMod);

    // Update Entity.
    // typically called once per frame
    // frameTime is used to regulate the speed of movement and animation
    virtual void update(float frameTime);

    // Initialize Entity
    // Pre: *gamePtr = pointer to Game object
    //      width = width of Image in pixels  (0 = use full texture width)
    //      height = height of Image in pixels (0 = use full texture height)
    //      ncols = number of columns in texture (1 to n) (0 same as 1)
    //      *textureM = pointer to TextureManager object
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    // Activate Entity.
    virtual void activate();

    // Empty ai function to allow Entity objects to be instantiated.
    virtual void ai(float frameTime, Entity &ent);

    // Is this entity outside the specified rectangle?
    virtual bool outsideRect(RECT rect);

    // Does this entity collide with ent?
    virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);

    // Damage this Entity with weapon.
    virtual void damage(int weapon);

    // Entity bounces after collision with other Entity
    void bounce(VECTOR2 &collisionVector, Entity &ent);

    // Adds the gravitational force to the velocity vector of this entity
    void gravityForce(Entity *other, float frameTime);

	// updates this entity's death animation
	void updateDeathAnim(float frameTime);

	// returns bool whether the death animation is complete or not
	bool isDeathAnimComplete();
};

#endif
