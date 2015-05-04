#ifndef BULLET_H
#define BULLET_H

#include "body.h"

namespace bulletNS
{
    const int   WIDTH             = 32;                     // image width
    const int   HEIGHT            = 32;                     // image height

    const float SPEED             = 369.f;                  // pixels per second
    const float MASS              = 300.0f;                 // mass
    const float COLLISION_RADIUS  = 1.f;

    const int   TEXTURE_COLS      = 8;                      // texture has 8 columns
    const int   START_FRAME       = 16;                     // explosion start frame
    const int   END_FRAME         = 19;                     // explosion end frame
    const float ANIMATION_DELAY   = 0.1f;                   // time between frames
}

namespace xroids
{
    class Asteroid;

    class Bullet : public Body
    {
    public:
        // Constructor
        Bullet();

        // Destructor
        virtual ~Bullet();

        virtual void update(float frameTime);

        void doCollide(Asteroid &a) { setActive( false ); }
    };
} // namespace xroids

#endif