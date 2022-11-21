#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//Actor Declare
class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, int hitPoints);
    
    virtual ~Actor();
    
    virtual int getHitpoints() const;
    
    virtual void setHitpoints(int value);
    
    virtual void doSomething() = 0;
    
private:
    int m_hitPoints;
};



//TUNNELMAN DECLARE
const int DEFAULT_HITPOINTS = 10;
const int DEFAULT_WATER = 5;
const int DEFAULT_SONARCHARGE = 1;
const int DEFAULT_GOLDNUGGET = 0;

class Tunnelman: public Actor
{
public:
    Tunnelman(int startX, int startY, int imageID = TID_PLAYER, int hitPoints = DEFAULT_HITPOINTS, int water = DEFAULT_WATER, int sonarCharge = DEFAULT_SONARCHARGE, int goldNugget = DEFAULT_GOLDNUGGET);
    
    virtual ~Tunnelman();
    
    virtual void doSomething();
    
private:
    int m_water; //start with 5
    int m_sonarCharge; //start with 1
    int m_goldNugget; // start with 0
};
//

//COLLECTIBLE DECLARE
class Collectible: public GraphObject
{
    Collectible(int imageID, int startX, int startY);
    
    virtual ~Collectible();
};
//

//EARTH DECLARE
class Earth: public GraphObject
{
    Earth(int imageID, int startX, int startY);
    virtual ~Earth();
};
#endif // ACTOR_H_
