// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 6 ship.cpp v1.0

#include <cassert>
#include "scenegraph.h"
#include "ship.h"
////#include "ShipOwnedStates.h"
#include "auxmaths.h"

using namespace xroids;

//=============================================================================
// default constructor
//=============================================================================
Ship::Ship() : Body(true), countdownTimer(0.f), turnLeft(false), turnRight(false)
{
    spriteData.width = shipNS::WIDTH;           // size of Ship
    spriteData.height = shipNS::HEIGHT;
    spriteData.x = (float)shipNS::X;            // location on screen
    spriteData.y = (float)shipNS::Y;
    spriteData.rect.bottom = shipNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = shipNS::WIDTH;

    velocity.x = 0.f;                             // velocity X
    velocity.y = 0.f;                             // velocity Y

    deltaV.x = 0.f;
    deltaV.y = 0.f;

    deltaAngle = 0.f;

    frameDelay = shipNS::SHIP_ANIMATION_DELAY;
    startFrame = shipNS::SHIP_START_FRAME;      // first frame of ship animation
    endFrame     = shipNS::SHIP_END_FRAME;      // last frame of ship animation
    currentFrame = startFrame;
    radius = shipNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;

	////currentState = STATE_SPAWNING::Instance(); //new states
	state = shipNS::STATE_NORMAL;	//old states
    invincible = false;
    waitOnFire = 0.f;
	fireRate = 4.f;
}


//=============================================================================
// copy constructor
//=============================================================================
Ship::Ship(const Ship& other )
    : Body( other )
    , deltaAngle( other.deltaAngle )
    , state( other.state )
    , countdownTimer( other.countdownTimer )
    , explosion( other.explosion )
    , invincible(other.invincible )
{}

//=============================================================================
// assignment operator
//=============================================================================
Ship& Ship::operator=(const Ship& other)
{
    Body::operator=( other );

    deltaAngle = other.deltaAngle;
    state = other.state;
    countdownTimer = other.countdownTimer;
    explosion = explosion;

    return *this;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Ship::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{

    Bullet aBullet;
    aBullet.initialize( gamePtr, bulletNS::WIDTH, bulletNS::HEIGHT, bulletNS::TEXTURE_COLS, textureM);
    aBullet.setFrames( bulletNS::START_FRAME, bulletNS::END_FRAME );
    aBullet.setActive( false );
    aBullet.setLoop( false );

    for( int i = 0; i != shipNS::MAX_AMMO; ++i )
    {
        gun.push_back( aBullet ); // adds a copy of bullet to gun.
    }

    explosion.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
    explosion.setFrames(shipNS::EXPLOSION_START_FRAME, shipNS::EXPLOSION_END_FRAME);
    explosion.setCurrentFrame(shipNS::EXPLOSION_START_FRAME);
    explosion.setFrameDelay(shipNS::EXPLOSION_ANIMATION_DELAY);
    explosion.setLoop(false);   // do not loop animation

	reverseFlag = false;
	numGuns = 1;

    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Ship::update(float frameTime)
{

	//new states
	//currentState->Execute(this, frameTime);


	//old states
    switch( state )
    {
        case shipNS::STATE_NORMAL:
        {
            Body::update( frameTime );     // updates velocity and animation( through calling Image::update)
            updatePhysics( frameTime );
            if( !canFire() )
                waitOnFire += frameTime;
            if( shouldFire && canFire() )
                fire();
        }
        break;

        case shipNS::STATE_EXPLODING:
        {
            explosion.update( frameTime );
            if( explosion.getAnimationComplete() )
            {
				SG().decreaseLives();
				if (SG().getLives() == 0) {

				}
				else
					this->setActive(false); //spawn();
            }
			audio->stopCue(SHIP_THRUST);
			audio->playCue(SHIP_EXPLOSION);
        }
        break;

        case shipNS::STATE_SPAWNING:
        {

            Body::update( frameTime );
            updatePhysics( frameTime );

            if( !canFire() )
                waitOnFire += frameTime;
            if( shouldFire && canFire() )
                fire(); // adds bullet to current frame

            countdownTimer -= frameTime;
            if( timerExpired() ){
                setTimer( 0.f );
                normalise();
            }
        }
        break;
    }
}

void Ship::updatePhysics(float frameTime )
{
    // clamp speed to maximum
    if( Graphics::Vector2Length( &velocity ) > shipNS::SHIP_MAX_SPEED )
    {
        VECTOR2 normal( velocity );                         // copy velocity vector
        Graphics::Vector2Normalize( &normal );              // normalise copy
        velocity = normal * shipNS::SHIP_MAX_SPEED;
    }

    // dampen velocity
    velocity *= 1.f - shipNS::SHIP_DAMPENING_FACTOR * frameTime;

    // apply angular velocity
    if( turnLeft && ! turnRight )
        deltaAngle = +shipNS::ROTATION_RATE;
    else if( !turnLeft && turnRight )
        deltaAngle = -shipNS::ROTATION_RATE;
    else
        deltaAngle = 0.f;
    
    turnLeft = turnRight = false;

    spriteData.angle += frameTime * deltaAngle;        // rotate the ship
    deltaAngle = 0.f;

    updatePosition(frameTime); // applies velocity to position, performing wrap if necessary
}

void Ship::draw()
{
    switch( state )
    {
        case shipNS::STATE_NORMAL:
        case shipNS::STATE_SPAWNING:
        {
            Body::draw();
        }
        break;

        case shipNS::STATE_EXPLODING:
        {
            explosion.draw();
        }
        break;
    }
}

bool Ship::collidesWith(Body &b, VECTOR2 &collisionVector)
{
    if( state == shipNS::STATE_NORMAL )
        return Body::collidesWith( b, collisionVector);

    return false;
}

void Ship::turnClockwise()
{
    turnRight = true;
}

void Ship::turnCounterClockwise()
{
    turnLeft = true;
}

void Ship::thrust()
{
    if( state == shipNS::STATE_NORMAL || state == shipNS::STATE_SPAWNING )
    {
        VECTOR2 v(0,-shipNS::SHIP_ACCELERATION);             // minus because y-axis is "upside down"
        setDeltaV( rotate(v, getRadians()+shipNS::SHIP_THIS_WAY_UP - (float)PI ));
		audio->playCue(SHIP_THRUST);
    }
}

void Ship::reverse()
{
    if( state == shipNS::STATE_NORMAL || state == shipNS::STATE_SPAWNING )
    {
        VECTOR2 v(0,shipNS::SHIP_ACCELERATION);         
        setDeltaV( rotate(v, getRadians()+shipNS::SHIP_THIS_WAY_UP - (float)PI ));
		audio->playCue(SHIP_THRUST);
    }
}

/*	//new states
void Ship::changeState(BaseState* newState) 
{
	currentState->Exit(this);

	currentState = newState;

	currentState->Enter(this);
}*/


// transition to normal state
void Ship::normalise()
{
    state = shipNS::STATE_NORMAL;
    invincible = false;
    setLoop( true );
    setCurrentFrame( shipNS::SHIP_START_FRAME );
    explosion.setAnimationComplete( false );
}

// transition to spawning state
void Ship::spawn()
{
    shouldFire = false;
    state = shipNS::STATE_SPAWNING;
    setVelocity( VECTOR2( 0.f, 0.f) );
    setRadians( shipNS::SHIP_THIS_WAY_UP  );

    invincible = true;
    setLoop( false );
    setCurrentFrame( shipNS::SHIP_START_FRAME );
    setTimer( shipNS::SPAWNING_PERIOD );
    explosion.setAnimationComplete( true );
    setActive( true );
}

void Ship::fire()
{
    for( auto i = gun.begin(); i != gun.end(); ++i )
    {
        if( !i->getActive() )                                                  // bullet not currently in use
        {
            Bullet &b = *i;

            // Calculate bullet velocity in ship's facing direction
            VECTOR2 bv( 0.f, bulletNS::SPEED );
            bv = ::rotate( bv,  shipNS::SHIP_THIS_WAY_UP + getRadians() );
            b.setVelocity( bv );

            // Calculate bullets spawning position
            VECTOR2 centre( getCenterX() , getCenterY()  );                          // center of ship

            // Move bullet so that it is emerging from the top of the ship
            VECTOR2 displacement( 0.f, getScale() * getHeight() );    
            centre += rotate( displacement, shipNS::SHIP_THIS_WAY_UP + getRadians() );

            b.setX( centre.x - b.getWidth() / 2.f );
            b.setY( centre.y - b.getHeight()/ 2.f );

            b.setRadians( getRadians() );
            b.setCurrentFrame( bulletNS::START_FRAME );
            b.activate();

            // add bullet to the scene graph
            SG().addBullet( &b );

            // bullet fired, so must wait before next shot
            waitOnFire = -1.f / fireRate;
            shouldFire = false;

			audio->playCue(LASER_SHOT);

			if (numGuns == 1)
				return;			// wont add extra bullets

			// really messy but not sure how else to find next inactive bullet without checking them all again
			for( auto j = i; j != gun.end(); ++j )
			{
				if( !j->getActive() )   
				{
					Bullet &b2 = *j;
					b2.setX(b.getX());
					b2.setY(b.getY());
					b2.setRadians( getRadians() );
					b2.setCurrentFrame( bulletNS::START_FRAME );
					b2.activate();
					b2.setVelocity(-b.getVelocity());			// shoots behind ship
					SG().addBullet(&b2);						
				}
			}

        } // end if
    } // end for
} // end fire()

// signal that a shot should be fired this frame
void Ship::shoot()
{
    if( canFire()  )
    {
        shouldFire = true;
    }
}

// transition to the exploding state
void Ship::explode()
{
    // enter explodong state
    state = shipNS::STATE_EXPLODING;
    explosion.setX( this->getX() );
    explosion.setY( this->getY() );
    explosion.setCurrentFrame(shipNS::EXPLOSION_START_FRAME);
    explosion.setLoop( false );
    explosion.setAnimationComplete( false );
	input->gamePadVibrateLeft(0, 30000, 1);
	input->gamePadVibrateRight(0, 30000, 1);
}

