#include "entity.h"
#include "body.h"

void xroids::Body::draw()
{
    Image::draw();                              // original position

    if( !shouldWrap )
        return;

    float x = getX();
    float y = getY();

    drawAt(x+GAME_WIDTH,y);                     // right
    drawAt(x+GAME_WIDTH,y+GAME_HEIGHT);         // bottom right
    drawAt(x, y+GAME_HEIGHT);                   // bottom
    drawAt(x-GAME_WIDTH,y+GAME_HEIGHT);         // bottom left
    drawAt(x-GAME_WIDTH,y);                     // left
    drawAt(x-GAME_WIDTH,y-GAME_HEIGHT);         // top left
    drawAt(x, y-GAME_HEIGHT);                   // top
    drawAt(x+GAME_WIDTH,y-GAME_HEIGHT);         // top right
}

void xroids::Body::drawAt(float px, float py)
{
    // save current position
    float x = getX();
    float y = getY();

    // change to drawing position
    setX( px );
    setY( py );

    Image::draw();

    // restore saved position
    setX( x );
    setY( y );
}

void xroids::Body::update(float frameTime)
{
    Entity::update( frameTime );                            // invoke super class version which deals with 
                                                            // adding acceleration to velocity and updating animation
}

void xroids::Body::updatePosition(float frameTime)          // take care of wrapping around in space or not
{
    spriteData.x += frameTime * velocity.x;                 // move ship along X
    spriteData.y += frameTime * velocity.y;                 // move ship along Y

    if( shouldWrap )
    {
        // Wrap around screen horizontally
        if (spriteData.x > GAME_WIDTH)                          // passed right screen edge
        {
            spriteData.x -= GAME_WIDTH;                         // reposition left by screen width
        }
        else if (spriteData.x < -getScaledWidth())              // passed left screen edge
        {
            spriteData.x += GAME_WIDTH;                         // reposition right by screen width
        }

        // Wrap around screen vertically
        if (spriteData.y > GAME_HEIGHT)                         // passed bottom screen edge
        {
            spriteData.y -= GAME_HEIGHT;                        // reposition downwards by screen height
        }
        else if (spriteData.y < -getScaledHeight())             // passed top of screen
        {
            spriteData.y += GAME_HEIGHT;                        // repostion upwards by height of screen
        }
    }
    else // does not wrap => inactivate if outside screen
    {
        if ( spriteData.x > GAME_WIDTH 
          || spriteData.x < -getScaledWidth()
          || spriteData.y > GAME_HEIGHT
          || spriteData.y < -getScaledHeight()
          )             // passed top of screen
        {
            setActive( false );
        }
    }
}

bool xroids::Body::collidesWith(Body &body, VECTOR2 &collisionVector)
{
    // if either body is not active then no collision may occcur
    if (!active || !body.getActive())
        return false;

    // If both bodies are using CIRCLE collision
    if (collisionType == entityNS::CIRCLE && body.getCollisionType() == entityNS::CIRCLE)
        return collideCircle(body, collisionVector);

    if( !this->shouldWrap && !body.shouldWrap )
        return Entity::collidesWith( body, collisionVector );

    throw "Collision detection undefined for these collision types in toroidal space.";
}

// Overload the circle collision check because the wrapping behviour of some bodies
bool xroids::Body::collideCircle(Body &body, VECTOR2 &collisionVector)
{
    collisionVector = *(body.getCenter()) - *getCenter();

    float dx = collisionVector.x;
    float dy = collisionVector.y;

    if( shouldWrap || body.shouldWrap )
    {
        float dwx = GAME_WIDTH - abs(dx);
        if ( dwx < abs(dx) )  // wrapped in x-
        {                                            //( dwx opposite sign to dx)
            collisionVector.x =  body.getCenter()->x + ( dx < 0.f ? +dwx : -dwx) - getCenterX();
            dx = dwx;
        }

        float dhy = GAME_HEIGHT - abs(dy);
        if( dhy < abs(dy) )   // wrapped in y-
        {
            collisionVector.y =  body.getCenter()->y + ( dy < 0.f ? +dhy : -dhy) - getCenterY();
            dy = dhy;
        }

    }

    float distanceSq = dx*dx + dy*dy;

    float sumrad     = getRadius() * getScale() 
                     + body.getRadius() * body.getScale();

    return distanceSq < sumrad*sumrad;
}
