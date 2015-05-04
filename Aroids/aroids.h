#ifndef AROIDS_H             // Prevent multiple definitions if this
#define AROIDS_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <list>
#include <vector>

#include "game.h"
#include "textureManager.h"
#include "audio.h"
#include "image.h"
#include "body.h"
#include "asteroid.h"
#include "ship.h"
#include "bullet.h"

using std::list;
using std::vector;

namespace xroids
{
    namespace aroidsNS 
    {
        enum GAME_STATES {
            GAME_UNINITIALIZED, GAME_STARTING, GAME_STARTED, GAME_ENDED
        };
    }

    //=============================================================================
    // This class is the core of the game
    //=============================================================================
    class Aroids : public Game
    {
    private:
        // game items
        TextureManager  nebulaTexture;          // nebula texture
        TextureManager  gameTexture;            // texture for sprites
		
		// textures for powerups
		TextureManager livesTexture;
		TextureManager weaponTexture1;
		TextureManager weaponTexture2;
		TextureManager engineTexture;

        Ship            ship;                   // spaceship
        Asteroid        asteroidPrototype;      // All asteroids created will initially be clones of this one
        Image           nebula;                 // backdrop image
		Image			livesSprite;			// image drawn to show num lives left. same sprite as ship

        enum aroidsNS::GAME_STATES state;
        vector<unsigned> levels;
        unsigned        currentLevel;


    public:
        // Constructor
        Aroids();

        // Destructor
        virtual ~Aroids();

        // Game state funcions
        void initialize(HWND hwnd);
        
        void addLevel(unsigned numAsteroids) { levels.push_back( numAsteroids ); }

        void start();           // Sets the scene to enable play
        
        void end();
        
        bool isOver();

        // Game loop action functions

        void update();      // must override pure virtual from Game
        void ai();          // "
        void collisions();  // "
        void render();      // "

        // Resource management functions
        void releaseAll();
        void resetAll();
		
    private:
        void initializeTextures();

    };
}
#endif
