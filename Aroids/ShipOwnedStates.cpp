/*#include "ShipOwnedStates.h"
#include "BaseState.h"
#include "Ship.h"


STATE_SPAWNING* STATE_SPAWNING::Instance()
{
	static STATE_SPAWNING instance;

	return &instance;
}

void STATE_SPAWNING::Enter(Ship* ship)
{

}


void STATE_SPAWNING::Execute(Ship* ship, float frameTime)
{  

}


void STATE_SPAWNING::Exit(Ship* ship)
{
  
}



STATE_NORMAL* STATE_NORMAL::Instance()
{
	static STATE_NORMAL instance;

	return &instance;
}


void STATE_NORMAL::Enter(Ship* ship)
{  
  
}


void STATE_NORMAL::Execute(Ship* ship, float frameTime)
{

	ship->update( frameTime );     // updates velocity and animation( through calling Image::update)
    ship->updatePhysics( frameTime );
    if( !ship->canFire() )
		ship->setWaitOnFire(ship->getWaitOnFire() + frameTime);
        if( ship->getShouldFire() && ship->canFire() )
            ship->fire();

}


void STATE_NORMAL::Exit(Ship* ship)
{
  
}




STATE_EXPLODING* STATE_EXPLODING::Instance()
{
	static STATE_EXPLODING instance;

	return &instance;
}

void STATE_EXPLODING::Enter(Ship* ship)
{
  
}

void STATE_EXPLODING::Execute(Ship* ship, float frameTime)
{ 
  
}

void STATE_EXPLODING::Exit(Ship* ship)
{ 

}

*/