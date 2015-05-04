#include <algorithm>

#include "scenegraph.h"
#include "asteroid.h"
#include "bullet.h"
#include "auxmaths.h"


void xroids::SceneGraph::draw()
{
    // Draw asteroids
    for( auto ai = m_asteroids.begin(); ai != m_asteroids.end(); ++ai)
        if( (*ai)->getActive() ) 
            (*ai)->draw();

    // Draw bullets
    for(auto bi = m_bullets.begin(); bi != m_bullets.end(); ++bi)
    {
        if( (**bi).getActive() ) 
            (**bi).draw();
    }

    ship.draw();

	if (gunPowerup1.getActive())
		gunPowerup1.draw();
	if (gunPowerup2.getActive())
		gunPowerup2.draw();
	if (enginePowerup.getActive())
		enginePowerup.draw();
	if (lifePowerup.getActive())
		lifePowerup.draw();

}

void xroids::SceneGraph::update(float frameTime)
{
	timeElapsed += frameTime;
    ship.update( frameTime );

	if (gunPowerup1.getActive())
		gunPowerup1.update( frameTime );
	if (gunPowerup2.getActive())
		gunPowerup2.update( frameTime );
	if (enginePowerup.getActive())
		enginePowerup.update( frameTime );
	if (lifePowerup.getActive())
		lifePowerup.update( frameTime );

    // asteroids
    for(auto ai = m_asteroids.begin(); ai != m_asteroids.end(); )
    {
        Asteroid &a = **ai;
        a.update( frameTime );
        if( !a.getActive() )
            ai = m_asteroids.erase( ai );
        else
            ++ai;
    }

    // bullets
    for(auto bi = m_bullets.begin(); bi != m_bullets.end(); )
    {
        Bullet &b = **bi;
        b.update( frameTime );
        if( !b.getActive() )
            bi = m_bullets.erase( bi );
        else
            ++bi;
    }

}

void  xroids::SceneGraph::doCollisions()
{
    // collide asteroids with bullets followed by ship
    for(auto ai = m_asteroids.begin(); ai != m_asteroids.end(); )
    {
        Asteroid &a = **ai; 

        // collisions between asteroid and bullets 
        for(auto bi = m_bullets.begin(); bi != m_bullets.end() && a.getActive(); )
        {
            Bullet &b = **bi;
            VECTOR2 v;
            if( b.getActive() && b.collidesWith( a , v) )
            {
                //   ai is an iterator
                //  *ai dereferences the iterator to give a pointer to an Asteroid
                // **ai dereferences the pointer to give the Asteroid
                //
                //  (*x)->y is equivalent to (**x).y
                //
                
                b.doCollide( a );

			//	a.doCollide( b );
				
				SG().asteroidsDestroyed++;
				// If a is a small asteroid, make it inactive
				if (a.getAsteroidSize() == asteroidNS::SMALL) {
					//a.playExplosionSound();		// game crashes here
					a.setActive(false);
					addToScore(100);
				}
				// else decrease its size and make new velocity vector
				else {
					a.setAsteroidSize(a.getAsteroidSize() - 1);
					VECTOR2 newVelocity = normalise(b.getVelocity()) * asteroidNS::MAX_SPEED;

					if (a.getAsteroidSize() == asteroidNS::MEDIUM) {
						a.setScale(0.75f);
						addToScore(20);
						newVelocity *= 5;
					}
					else if (a.getAsteroidSize() == asteroidNS::SMALL) {
						a.setScale(0.4f);
						addToScore(50);
						newVelocity *= 12;
					}

					a.setVelocity( rotate(newVelocity, 1.57f ));	// perpendicular to bullet

					// create another asteroid of same size and opposite velocity
					Asteroid *newA = new Asteroid(a);
					newA->setAsteroidSize(a.getAsteroidSize());
					VECTOR2 v(0.f, (float) asteroidNS::MAX_SPEED);
					newA->setVelocity( -a.getVelocity() );
					newA->setActive(true);
					m_asteroids.push_back(newA);
				}

				
            }

            if( !b.getActive() ) // bullet was inactive or collision has made it so
                bi = m_bullets.erase( bi );
            else
                ++bi;

        }
    
        // collisions between asteroid and ship 
        {
            VECTOR2 v;
            if( a.getActive() && ship.collidesWith( a , v) )
            { 
                a.doCollide( ship );
                ship.doCollide( a );
            }
        }
		
        if( !a.getActive() )                // asteroid was or has become inactive
            ai = m_asteroids.erase( ai );   // remove it from active list
        else
            ++ai;
    }

	
	// Powerup collisions
	VECTOR2 v1;
	if (gunPowerup1.getActive()  && ship.collidesWith( gunPowerup1 , v1) ) {

		if (ship.getNumGuns() == 1)
			ship.setNumGuns(2);				// first powerup gives extra gun on back of ship
		else
			ship.increaseFireRate();		// subsequent powerup increases the firing rate 
			
		gunPowerup1.setActive(false);
		gunPowerup1.playPowerupSound();
	}// end if

	VECTOR2 v2;
	if (gunPowerup2.getActive()  && ship.collidesWith( gunPowerup2 , v2) ) {
		ship.decreaseFireRate();
		gunPowerup2.setActive(false);
		gunPowerup2.playPowerupSound();
	}

	VECTOR2 v3;
	if (lifePowerup.getActive()  && ship.collidesWith( lifePowerup , v3) ) {
		lives++;
		lifePowerup.setActive(false);
		lifePowerup.playPowerupSound();
	}

	VECTOR2 v4;
	if (enginePowerup.getActive()  && ship.collidesWith( enginePowerup , v4) ) {
		ship.setReverse(true);
		enginePowerup.setActive(false);
		enginePowerup.playPowerupSound();
	}

}

void xroids::SceneGraph::clear()
{
    m_asteroids.clear();
    m_bullets.clear();
}

void xroids::SceneGraph::addBullet(Bullet *b)
{
    m_bullets.push_back( b );
}

void xroids::SceneGraph::delBullet(Bullet *b)
{
    auto i = find(m_bullets.begin(), m_bullets.end(), b);
    if( i != m_bullets.end() )
        m_bullets.erase( i ); 
}

void xroids::SceneGraph::addAsteroid(Asteroid *a)
{
    m_asteroids.push_back( a );
}

void xroids::SceneGraph::delAsteroid(Asteroid *a)
{
    auto i = find(m_asteroids.begin(), m_asteroids.end(), a);
    if( i != m_asteroids.end() )
        m_asteroids.erase( i ); 
}

