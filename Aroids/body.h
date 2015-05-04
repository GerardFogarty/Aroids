#ifndef BODY_H
#define BODY_H

#pragma region includes
#include <list>
#include <vector>
#include <map>

#include "graphics.h"

#include "entity.h"
#pragma endregion

namespace xroids
{
    //+
    // Parent class of all bodies in space e.g. asteroids, ships and bullets
    //
    class Body : public Entity
    {
        bool shouldWrap;                    // Does the body wrap around space or is it destroyed when it leaves?

    public:

        // constructor
        //  Note: defining a constructor stops the compiler from generating a default constructor.
        //      When defining a constructor some experts say also define
        //      o copy constructor
        //      o assignment operator
        //
        Body(bool w) : Entity()
        {
            shouldWrap = w;
        }

        // copy constructor
        Body(const Body& other)
            : Entity( other )                   // base class copy constructor
            , shouldWrap( other.shouldWrap )
        {
        }

        // assignment operator
        Body& operator= (const Body& rval)
        {
            Entity::operator=(rval);            // assignment operator of base class
            shouldWrap = rval.shouldWrap;
            return *this;
        }

        virtual ~Body() {}

        virtual void draw();

        virtual void update(float frmeTime);

        // additional methods

        // ?should be part of kell's Image class?
        inline float getScaledWidth()   { return getWidth() * getScale(); }
        inline float getScaledHeight()  { return getHeight() * getScale(); }

        void updatePosition(float frameTime);                             // taking into account wrapping or otherwise

        //+
        // Collision Detection
        //-

        virtual bool collidesWith(Body &body, VECTOR2 &collisionVector);

        virtual bool collideCircle(Body &body, VECTOR2 &collisionVector);

        //+
        // Collision Response
        //
        // Overload to specialise collision action dependent on the subtype of the 'other' Body.
        //-
        virtual void doCollide(Body& other) {};

    private:
        void drawAt(float px, float py);                                    // helper function to draw body at a position

    }; // end class Body

}
#endif
