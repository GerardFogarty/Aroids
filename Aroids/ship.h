// File: ship.h
// Karl Sandison

#ifndef _SHIP_H                 // Prevent multiple definitions if this
#define _SHIP_H                 // file is included in more than one place


#pragma region includes

#include <vector>

#include "GameData.h"
#include "body.h"
#include "bullet.h"

////class BaseState;

#pragma endregion

namespace xroids
{
	
    namespace shipNS
    {
        const int WIDTH                         = 32;                       // image width
        const int HEIGHT                        = 32;                       // image height
        const int X                             = GAME_WIDTH/2 - WIDTH/2;   // location on screen
        const int Y                             = GAME_HEIGHT/2 - HEIGHT/2;
        const float ROTATION_RATE               = (float)PI;                // radians per second
        const float SHIP_THIS_WAY_UP            = 3.f*(float)PI/2.f;        // angle in radians for ship facing towards top of screen
        const float SPEED                       = 10.f;                     //  pixels per second
        const float MASS                        = 300.0f;                   // mass
        const int   TEXTURE_COLS = 8;           // texture has 8 columns
        const int   SHIP_START_FRAME = 0;       // ship starts at frame 0
        const int   SHIP_END_FRAME = 3;         // ship animation frames 0,1,2,3
        const float SHIP_ANIMATION_DELAY        = 0.2f;                     // time between frames
        const float SHIP_ACCELERATION           = 3.f;
        const float SHIP_MAX_SPEED              = 200.f;
        const float SHIP_DAMPENING_FACTOR       = 0.7f;

        const int   EXPLOSION_START_FRAME = 32;         // explosion start frame
        const int   EXPLOSION_END_FRAME = 39;           // explosion end frame
        const float EXPLOSION_ANIMATION_DELAY = 0.1f;   // time between frames

        const float EXPLOSION_DURATION          = 3.0f;
        const float EXPLOSION_MIN_SCALE         = 0.3f;
        const float EXPLOSION_MAX_SCALE         = 2.0f;

        const float SPAWNING_PERIOD             = 3.0f; // how long ship is invincible during spawn

        // should probably be associated with a gun/weapon class
        const int   MAX_AMMO                    = 30;

		//old states
        enum State { STATE_SPAWNING, STATE_NORMAL, STATE_EXPLODING };
    }

    //+
    // C++ Note:                           Forward Class Declarations
    //      
    // Forward class declarations, such as the following
    //
    //          class X;,  
    //
    // allow instances of the class, X, to be passed as parameters. They
    // also be used as pointer or reference type variables, e.g. X *aPointerToAnX.
    //
    // However, no member functions, including constructors, can be called.
    // So the following are not valid because they require a constructor call:
    //
    //          X anX;  // the variable anX is to be initialised by the default constructor of X.
    //                  // The compiler does not know if X has an accessible default constructor.
    //
    //          class Y : public X...   // This requires the constructor of X 
    //                                  // to construct the X part of Y.
    //
    // They are useful in cutting down the number of headers included and thereby reducing build time. 
    //  More importantly, forward declarations help prevent circularly including header files. 
    // Circular inclusion happens when a file includes a file which includes (directly or indirectly) 
    // the first file. For example, file A includes B which includes C which includes A.
    //
    //-

    class Asteroid;                         // Using Asteroid type as reference parameter- so no need to include "asteroids.h" 
                                            // Asteroids have to know about ships. BUT we don't want to include "asteroids.h"
                                            // in "ship.h" and at the same time including "ship.h" in "asteroid.h"!!!
                                            // Instead forward declarations are used for types that would be otherwise included.
                                            // The corresponding implementation file then includes the appropriate header.

	

    class Ship : public Body
    {
		////BaseState*			currentState;	// 
        shipNS::State       state;          // current state

        // data members
        float               deltaAngle;     // the change in angle that should be applied this frame
        bool                turnLeft;       // the ship has been commanded to turn right this frame
        bool                turnRight;      // the ship has been commanded to turn right this frame


        float               countdownTimer; // how long before a delayed state change

        bool                invincible;     // => ship cannot be destroyed (currently only during spawning.)
        
        Image               explosion;      //       - < < * > > -    animation

        std::vector<Bullet> gun;            // this is a memory pool for bullets. Requests for bullets are
                                            // allocated from here.

        float               waitOnFire;     // time for the gun to be ready (1/firing rate).
        bool                shouldFire;     // the gun has been triggered for this frame

        void                fire();         // launch projectile... from top of ship in the direction the ship is facing

		// power up info
		bool				reverseFlag;	// can ship reverse
		int					numGuns;		// level of gun powerup
		float				fireRate;		// rounds per second

    protected:

        // Simple count-down timer used in state transitons
        void setTimer(float duration)   { countdownTimer = duration;    }
        bool timerExpired() const       { return countdownTimer < 0.f;  }

        // Gun/Weapon method
        bool canFire()                  
        { 
			//old states
			return ( state == shipNS::STATE_NORMAL || state == shipNS::STATE_SPAWNING ) 
                &&!( waitOnFire < 0.f);  

			//new states
			//return ( currentState == STATE_NORMAL::Instance() || currentState == STATE_SPAWNING::Instance() ) 
            //    &&!( waitOnFire < 0.f);  
        }  

        // Handle updates to velocity, position and rotation
        void updatePhysics(float frameTime);

    public:
        // constructor
        Ship();

        // copy constructor
        Ship(const Ship & other);

        // assignment operator
        Ship& operator=(const Ship& other);

        // destructor
        virtual ~Ship()                 {}

        // accessor functions
        bool isInvincible()             { return invincible; }

        std::vector<Bullet> &getGun()   { return gun; }

		float getWaitOnFire()			{ return waitOnFire; }

		bool getShouldFire()			{ return shouldFire; }

		// mutator functions

		void setWaitOnFire(float f)		{ waitOnFire = f;	}

        
        // inherited function overrides
        virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                                TextureManager *textureM);

        virtual void update(float frameTime);

        virtual void draw();

        virtual bool collidesWith(Body &b, VECTOR2 &collisionVector);

        //+
        // Overload doCollide for a given Body subtype to respond to collisions.
        //-
        virtual void doCollide(Asteroid &a)         { explode(); }

        //+
        // Actions for this time frame. These essentialy set flags.
        // These requested action is carried out by the update function.
        //-
        void turnClockwise();
        void turnCounterClockwise();
        void shoot();
        void thrust();
		void reverse();

        //+
        // state transition functions
        //-
        void normalise();                       // current state <- Normal
        void spawn();                           // current state <- Spawning
        void explode();                         // current state <- Exploding

		//new states
		////void changeState(BaseState* newState);

		// power-up flag get/set functions
		bool getReverse()				{	return reverseFlag;		}
		void setReverse(bool b)			{	reverseFlag = b;		}	

		int getNumGuns()				{	return numGuns;		}
		void setNumGuns(int n)			{	numGuns = n;		}

		void increaseFireRate()			{ if (fireRate < 8.f)	fireRate += 2.f;	}
		void decreaseFireRate()			{ if (fireRate > 2.f)	fireRate -= 2.f;	}
    };

} // namespace xroids
#endif
