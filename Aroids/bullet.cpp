#include "bullet.h"

using namespace xroids;

Bullet::Bullet() 
    : Body(false)
{
    radius          = bulletNS::COLLISION_RADIUS;
    mass            = bulletNS::MASS;
    startFrame      = bulletNS::START_FRAME;    // first frame of ship animation
    endFrame        = bulletNS::END_FRAME;      // last frame of ship animation
    setCurrentFrame(startFrame);

    setActive( false );
}

Bullet::~Bullet()
{
}

void Bullet::update(float frameTime)
{
    Body::update( frameTime );
    Body::updatePosition( frameTime );

	if (spriteData.x < -bulletNS::WIDTH || spriteData.x > GAME_WIDTH || spriteData.y < -bulletNS::HEIGHT || spriteData.y > GAME_HEIGHT)
		setActive(false);
}

