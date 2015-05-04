

#include <cassert>
#include "scenegraph.h"

#include "powerup.h"

using namespace xroids;

//=============================================================================
// default constructor
//=============================================================================
Powerup::Powerup() : Body(true)
{
    spriteData.width = powerupNS::WIDTH;           // size of Powerup
    spriteData.height = powerupNS::HEIGHT;
    spriteData.rect.bottom = powerupNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = powerupNS::WIDTH;

    frameDelay = powerupNS::POWERUP_ANIMATION_DELAY;
    startFrame = powerupNS::POWERUP_START_FRAME;      // first frame of powerup animation
    endFrame     = powerupNS::POWERUP_END_FRAME;      // last frame of powerup animation
    currentFrame = startFrame;
    radius = powerupNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;

	velocity.x = 0.f;                             // velocity X
    velocity.y = 0.f;                             // velocity Y

	disappearTimer = 10.f;
}

Powerup::Powerup(int t) : Body(true)
{
    spriteData.width = powerupNS::WIDTH;           // size of Powerup
    spriteData.height = powerupNS::HEIGHT;
    spriteData.rect.bottom = powerupNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = powerupNS::WIDTH;

    frameDelay = powerupNS::POWERUP_ANIMATION_DELAY;
    startFrame = powerupNS::POWERUP_START_FRAME;      // first frame of powerup animation
    endFrame     = powerupNS::POWERUP_END_FRAME;      // last frame of powerup animation
    currentFrame = startFrame;
    radius = powerupNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;

	velocity.x = 0.f;                             // velocity X
    velocity.y = 0.f;                             // velocity Y

	disappearTimer = 10.f;
}


//=============================================================================
// copy constructor
//=============================================================================
Powerup::Powerup(const Powerup& other )
    : Body( other )
{}


//=============================================================================
// Initialize the Powerup.
// Post: returns true if successful, false if failed
//=============================================================================
bool Powerup::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Powerup::update(float frameTime)
{
	disappearTimer -= frameTime;
	if (disappearTimer < 0.f) {
		active = false;
		disappearTimer = 10.f;
	}
    updatePosition(frameTime); // applies velocity to position, performing wrap if necessary
}

bool Powerup::collidesWith(Body &b, VECTOR2 &collisionVector)
{

    return false;
}

void Powerup::playPowerupSound() {

	audio->playCue(POWER_UP);
}