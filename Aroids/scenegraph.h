#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <list>
#include <vector>

#include"powerup.h"
#include "ship.h"

namespace xroids
{
    class Asteroid;
    class Bullet;

    class SceneGraph
    {
    private:

        // Active game objects
        std::list<Asteroid*> m_asteroids;
        std::list<Bullet*>   m_bullets;
		
		int score;					// starts 0
		int lives;					// starts at 3, game over at 0

        SceneGraph()
        {
        }

    public:
        Ship ship;                                      
		Powerup				 lifePowerup;
		Powerup				 gunPowerup1;
		Powerup				 gunPowerup2;
		Powerup				 enginePowerup;
		int					 asteroidsDestroyed;
		float				 timeElapsed;

        static SceneGraph &getInstance()
        {
            static SceneGraph instance;
            return instance;
        }

        virtual void draw();
        virtual void update(float frameTime);
        virtual void doCollisions();

        void clear();

        //+
        // **WARNING**  Raw Pointers
        //
        // The scene graph does not own the bodies that are registered with it.
        // Before bodies are destroyed, they should be remove from the scene graph.
        //-
        void addBullet     (Bullet *b);
        void delBullet     (Bullet *b);

        void addAsteroid   (Asteroid *a);
        void delAsteroid   (Asteroid *a);


        unsigned int getAsteroidCount() { return m_asteroids.size(); }

		inline void addToScore(int s)	{	score += s;		}
		inline void setScore(int s)		{	score = s;		}
		inline int getScore()			{	return score;	}

		void setLives(int l)		{	lives = l;		}
		int getLives()				{	return lives;	}
		void decreaseLives()		{	lives--;		}


    };
} // end namespace xroids

#define SG()    xroids::SceneGraph::getInstance()

#endif