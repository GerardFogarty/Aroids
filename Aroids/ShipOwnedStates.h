/*#ifndef SHIP_OWNED_STATES_H
#define SHIP_OWNED_STATES_H

#include "BaseState.h"

class Ship;

class STATE_EXPLODING : public BaseState
{
private:

  STATE_EXPLODING(){}

  STATE_EXPLODING(const STATE_EXPLODING&);
  STATE_EXPLODING& operator=(const STATE_EXPLODING&);

public:

  static STATE_EXPLODING* Instance();
  
  virtual void Enter(Ship* ship);

  virtual void Execute(Ship* ship, float frameTime);

  virtual void Exit(Ship* ship);
};





class STATE_NORMAL : public BaseState
{
private:

  STATE_NORMAL(){}

  STATE_NORMAL(const STATE_NORMAL&);
  STATE_NORMAL& operator=(const STATE_NORMAL&);
  
public:

  static STATE_NORMAL* Instance();

  virtual void Enter(Ship* ship);

  virtual void Execute(Ship* ship, float frameTime);

  virtual void Exit(Ship* ship);
};






class STATE_SPAWNING : public BaseState
{
private:

  STATE_SPAWNING(){}

  STATE_SPAWNING(const STATE_SPAWNING&);
  STATE_SPAWNING& operator=(const STATE_SPAWNING&);

public:

  static STATE_SPAWNING* Instance();

  virtual void Enter(Ship* ship);

  virtual void Execute(Ship* ship, float frameTime);

  virtual void Exit(Ship* ship);
};

#endif*/