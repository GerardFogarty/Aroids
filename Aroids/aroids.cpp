// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Draw animated spaceship
// Chapter 6 Aroids.cpp v1.0
// This class is the core of the game

#include <algorithm>
#include <ctime>            // time

#include "aroids.h"
#include "asteroid.h"
#include "graphics.h"
#include "auxmaths.h"
#include "GameData.h"
#include "scenegraph.h"


//=============================================================================
// Constructor
//=============================================================================

Game* ::CreateGame() // KS: Each game must provide an implementation that returns an instance of the game to be played
{
    return new xroids::Aroids;
}

xroids::Aroids::Aroids()
   : state( aroidsNS::GAME_UNINITIALIZED )
   , currentLevel(0)
{}

//=============================================================================
// Destructor
//=============================================================================
xroids::Aroids::~Aroids()
{}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void xroids::Aroids::initialize(HWND hwnd)
{
    if( state != aroidsNS::GAME_UNINITIALIZED )
        return;

    // Seed random generator
    srand( (UINT) time(0) );

    Game::initialize(hwnd); // throws GameError

    initializeTextures();

    addLevel( 3 );
    addLevel( 5 );
    addLevel( 7 );

	SG().setLives(3);
	SG().timeElapsed = 0.f;

    start();
    return;
}

void xroids::Aroids::initializeTextures()
{
    // nebula texture
    if (!nebulaTexture.initialize(graphics,NEBULA_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

    // game texture
    if (!gameTexture.initialize(graphics,TEXTURES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game sprite texture"));

	// powerup textures
    if (!livesTexture.initialize(graphics,LIVES_POWERUP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing powerup sprite texture"));

	if (!weaponTexture1.initialize(graphics,WEAPON_POWERUP1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing powerup sprite texture"));

	if (!weaponTexture2.initialize(graphics,WEAPON_POWERUP2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing powerup sprite texture"));

	if (!engineTexture.initialize(graphics,ENGINE_POWERUP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing powerup sprite texture"));
}

//=============================================================================
// Prepare game level for play
//=============================================================================
void xroids::Aroids::start()
{
	
    state = aroidsNS::GAME_STARTING;
    SG().clear(); // empty scene graph
	SG().asteroidsDestroyed = 0;

#pragma region Create game objects
    //+
    // Initialise drawable game objects
    //-
        // nebula image
        if (!nebula.initialize(graphics,0,0,0,&nebulaTexture))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

		// lives image
        if (!livesSprite.initialize(graphics,shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTexture))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lives sprite"));
		livesSprite.setDegrees(-90);
		livesSprite.setY(GAME_HEIGHT - shipNS::HEIGHT -10);

        // Initialise ship
        {
            // Alias ship in scene graph
            Ship &ship = SG().ship;

            // ship
            if (!ship.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTexture))
                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship from game texture"));

            ship.setFrames(shipNS::SHIP_START_FRAME, shipNS::SHIP_END_FRAME);
            ship.setCurrentFrame(shipNS::SHIP_START_FRAME);
            ship.setX(GAME_WIDTH/4 - shipNS::WIDTH);
            ship.setY(GAME_HEIGHT/2 - shipNS::HEIGHT);
            ship.setRadians(shipNS::SHIP_THIS_WAY_UP);
			ship.setReverse(false);
            ship.spawn();
        }

        // prototypical asteroid
        if (!asteroidPrototype.initialize(this, asteroidNS::WIDTH, asteroidNS::HEIGHT, asteroidNS::TEXTURE_COLS, &gameTexture))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing asteroid from game texture"));

        // create asteroids
        for(unsigned i = 0; i != levels[currentLevel]; ++i)
        {
            Asteroid *anAsteroid= new Asteroid( asteroidPrototype );

            anAsteroid->setX( float( rand() % GAME_WIDTH ) );
            anAsteroid->setY( float( rand() % GAME_HEIGHT ) );
            anAsteroid->setDegrees( float( rand() % 360 ) );
            VECTOR2 v( 0.f, (float) asteroidNS::MAX_SPEED );
            anAsteroid->setVelocity( rotate( v, anAsteroid->getRadians() ) );
			anAsteroid->setAsteroidSize(asteroidNS::LARGE);


            SG().addAsteroid( anAsteroid );
        }

		if (!SG().gunPowerup1.initialize(this, powerupNS::WIDTH, powerupNS::HEIGHT, powerupNS::TEXTURE_COLS, &weaponTexture1))
                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship from game texture"));
		if (!SG().gunPowerup2.initialize(this, powerupNS::WIDTH, powerupNS::HEIGHT, powerupNS::TEXTURE_COLS, &weaponTexture2))
                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship from game texture"));
		if (!SG().lifePowerup.initialize(this, powerupNS::WIDTH, powerupNS::HEIGHT, powerupNS::TEXTURE_COLS, &livesTexture))
                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship from game texture"));
		if (!SG().enginePowerup.initialize(this, powerupNS::WIDTH, powerupNS::HEIGHT, powerupNS::TEXTURE_COLS, &engineTexture))
                throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship from game texture"));

		
		SG().gunPowerup1.setActive(false);
		SG().gunPowerup2.setActive(false);
		SG().lifePowerup.setActive(false);
		SG().enginePowerup.setActive(false);

#pragma endregion  Create game objects

    state = aroidsNS::GAME_STARTED;
}

//=============================================================================
// Update all game items
//=============================================================================
void xroids::Aroids::update()
{

    if( Game::input->isKeyDown(VK_ESCAPE) )
        Game::exitGame();

    if( state == aroidsNS::GAME_ENDED )
    {
		
		if (Game::getInput()->isKeyDown(VK_SPACE)) {
			start(); 
			SG().setLives(3);
			SG().setScore(0);
		}
        return;
    }

    if( state != aroidsNS::GAME_STARTED )
        return;

    Ship &ship = SG().ship;

    if( !ship.getActive() ) // ship dead
    {
        start();
        return;
    }

	if (SG().getLives() < 0)
		state = aroidsNS::GAME_ENDED;

	// spawn powerups

	// lives powerup - after 2000 score
	if (SG().getScore() > 2000 && SG().getScore() < 1100 && !SG().lifePowerup.getActive()) {
		SG().lifePowerup.setActive(true);
		SG().lifePowerup.setX(GAME_WIDTH - (GAME_WIDTH/4));
		SG().lifePowerup.setY(GAME_HEIGHT/2);
	}

	// Gun powerup 1 - after 5 asteroids destroyed
	if (SG().asteroidsDestroyed == 5 && !SG().gunPowerup1.getActive()) {
		SG().gunPowerup1.setActive(true);
		SG().gunPowerup1.setX(GAME_WIDTH - 3*(GAME_WIDTH/4));
		SG().gunPowerup1.setY(GAME_HEIGHT/5);
		SG().gunPowerup1.setVelocity(VECTOR2( (rand()%300)-150, (rand()%300)-150));
	}

	// Gun powerup 2 - after 25 seconds
	if (SG().timeElapsed > 25.f && SG().timeElapsed < 26.f && !SG().gunPowerup2.getActive()) {
		SG().gunPowerup2.setActive(true);
		SG().gunPowerup2.setX(GAME_WIDTH - 3*(GAME_WIDTH/4));
		SG().gunPowerup2.setY(GAME_HEIGHT/5);
		SG().gunPowerup2.setVelocity(VECTOR2( (rand()%300)-150, (rand()%300)-150));
	}

	// Engine powerup - when asteroids destroyed is greater than asteroids remaining
	if (SG().getAsteroidCount() < SG().asteroidsDestroyed && !SG().enginePowerup.getActive() && !ship.getReverse()) {
		SG().enginePowerup.setActive(true);
		SG().enginePowerup.setX(GAME_WIDTH/2);
		SG().enginePowerup.setY(GAME_HEIGHT - GAME_HEIGHT/5);
		SG().enginePowerup.setVelocity(VECTOR2( (rand()%300)-150, (rand()%300)-150));
	}

    if( SG().getAsteroidCount() == 0 ) // cleared level
    {
		state = aroidsNS::GAME_ENDED;
        ++currentLevel;
        if( currentLevel == levels.size() )
            currentLevel = 0;
        start();
        return;
    }

    // Handle player controls for ship
        // turn:
        if( Game::getInput()->isKeyDown(VK_LEFT) || Game::input->getGamepadThumbLX(0) < -5000)
            ship.turnClockwise();
        else if( Game::getInput()->isKeyDown(VK_RIGHT) || Game::input->getGamepadThumbLX(0) > 5000  )
            ship.turnCounterClockwise();

        // accelerate:
        if( Game::getInput()->isKeyDown(VK_UP) || Game::input->getGamepadThumbLY(0) > 5000)
            ship.thrust();

		// reverse
		else if (ship.getReverse() && (Game::getInput()->isKeyDown( VK_DOWN ) || Game::input->getGamepadThumbLY(0) < -5000))
				ship.reverse();
		else
			audio->stopCue(SHIP_THRUST);	// Cue is started playing in reverse()

        // shoot:
        if( Game::getInput()->isKeyDown( VK_SPACE ) || Game::input->getGamepadRightTrigger(0))
            ship.shoot();

		// used while testing to change different variables
		if (Game::getInput()->isKeyDown( VK_ADD ))
			ship.increaseFireRate();
		if (Game::getInput()->isKeyDown( VK_SUBTRACT ))
			ship.decreaseFireRate();

    SG().update(frameTime);

}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void xroids::Aroids::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void xroids::Aroids::collisions()
{
    // Delegate collision detetion to the scene graph
    SG().doCollisions();
}


//=============================================================================
// Render game items
//=============================================================================
void xroids::Aroids::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    nebula.draw();                          // add the orion nebula to the scene

    SG().draw();
	
	// draw lives icons
	for (int i=1; i<=SG().getLives(); i++) {
		livesSprite.setX( (i*shipNS::WIDTH) +45.f );
		livesSprite.draw();
	}

	char buffer[128];
	sprintf(buffer,"%s%d","Score: ",SG().getScore());
	dxFont.print(buffer, 10, GAME_HEIGHT-45);
	sprintf(buffer,"%s%d","Lives: ", SG().getLives());
	dxFont.print(buffer, 10, GAME_HEIGHT-30);
	sprintf(buffer,"%s%f","Time: ", SG().timeElapsed);
	dxFont.print(buffer, 10, GAME_HEIGHT-60);

	if (state ==  aroidsNS::GAME_ENDED) {
		sprintf(buffer,"%s%d","GAME OVER\nYOUR SCORE: ",SG().getScore());
		dxFontLarge.print(buffer, GAME_WIDTH/2 - 150, GAME_HEIGHT/2 - 50);
	}

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void xroids::Aroids::releaseAll()
{
    gameTexture.onLostDevice();
    nebulaTexture.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void xroids::Aroids::resetAll()
{
    nebulaTexture.onResetDevice();
    gameTexture.onResetDevice();

    Game::resetAll();
    return;
}
