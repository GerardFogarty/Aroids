#ifndef _POWERUP_H                 // Prevent multiple definitions if this
#define _POWERUP_H                 // file is included in more than one place


#pragma region includes

#include <vector>

#include "GameData.h"
#include "body.h"

#pragma endregion

namespace xroids
{

    namespace powerupNS
    {
        const int WIDTH                         = 30;                       // image width
        const int HEIGHT                        = 32;                       // image height
        const float SPEED                       = 10.f;                     // pixels per second
        const float MASS                        = 300.0f;                   // mass
        const int   TEXTURE_COLS				= 1;						// texture has 1 columns
        const int   POWERUP_START_FRAME			= 0;						// powerup starts at frame 0
        const int   POWERUP_END_FRAME			= 0;						// powerup animation frames 0
        const float POWERUP_ANIMATION_DELAY     = 0.2f;                     // time between frames
        const float POWERUP_MAX_SPEED           = 200.f;		
    }

    class Powerup : public Body
    {

		float disappearTimer;

    protected:
        // Handle updates to velocity, position and rotation
        void updatePhysics(float frameTime);

    public:
        // constructor
		Powerup();
        Powerup( int type );

        // copy constructor
        Powerup(const Powerup & other);

       
        // destructor
        virtual ~Powerup()                 {}

        // inherited function overrides
        virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                                TextureManager *textureM);

        virtual void update(float frameTime);


        virtual bool collidesWith(Body &b, VECTOR2 &collisionVector);

		void playPowerupSound();

    };

} // namespace xroids
#endif
