#ifndef ASTEROID_H
#define ASTEROID_H

#pragma region includes
#include <list>

#include "body.h"
#include "auxmaths.h"
#include "GameData.h"
#pragma endregion

namespace asteroidNS
{
    const int   WIDTH = 128;                    // image width
    const int   HEIGHT = 128;                   // image height
    const int   COLLISION_RADIUS = 120/2;       // for circular collision
    const int   X = GAME_WIDTH/2 - WIDTH/2;     // location on screen
    const int   Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float MASS = 1.0e14f;                 // mass
    const float MAX_SPEED    =  9.f;                      // maximum speed
    const int   TEXTURE_COLS = 2;               // texture has 2 columns
    const int   START_FRAME = 1;                // starts at frame 1
    const int   END_FRAME = 1;                  // no animation
	
	// different sizes of asteroids
	enum ASTEROID_SIZE { SMALL, MEDIUM, LARGE };
}

namespace xroids
{
    class Bullet;
    class Ship;

    class Asteroid : public Body
    {
    public:

        // default constructor
        Asteroid()
            : xroids::Body(true)
        {
            radius          = asteroidNS::COLLISION_RADIUS;
            mass            = asteroidNS::MASS;
            startFrame      = asteroidNS::START_FRAME;    // first frame of ship animation
            endFrame        = asteroidNS::END_FRAME;      // last frame of ship animation
            setCurrentFrame(startFrame);
        }

        // copy constructor
        Asteroid(const Asteroid& other) : Body(other)
        {
            // copy any members here
        }

        // destructor
        virtual ~Asteroid() {}

        
        virtual void update(float frameTime)
        {
            Body::update( frameTime );
            Body::updatePosition( frameTime );
        }

		int getAsteroidSize()			{	return size;	}
		void setAsteroidSize(int s)		{	size = s;		}

		void playExplosionSound()				{ audio->playCue(ASTEROID_EXPLOSION);	}

		// instance methods
		virtual void doCollide(Bullet& b) {	}
        virtual void doCollide(Ship &s)   { }


	private:
		int size; // size of asteroid - LARGE, MEDIUM, SMALL
    };

} // namespace xroids
#endif