#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
//Actor Declare
class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int hitPoints);
    
    virtual ~Actor();
    
    virtual StudentWorld* getWorld();
    virtual int getHP() const;
    virtual void setHP(int value);
    virtual void doSomething();
    virtual bool m_isDead();

private:
    StudentWorld* m_world;
    int m_HP;
};



//TUNNELMAN DECLARE
const int DEFAULT_HITPOINTS = 10;
const int DEFAULT_WATER = 5;
const int DEFAULT_SONARCHARGE = 1;
const int DEFAULT_GOLDNUGGET = 0;

class Tunnelman: public Actor
{
public:
    Tunnelman(StudentWorld* world, int startX, int startY, int imageID = TID_PLAYER, int hp = DEFAULT_HITPOINTS, int water = DEFAULT_WATER, int sonarCharge = DEFAULT_SONARCHARGE, int goldNugget = DEFAULT_GOLDNUGGET);
    
    virtual ~Tunnelman();
    virtual void doSomething() override;
    virtual bool dig();
    virtual void moveToDir(Direction d);
    
private:
    int m_water; //start with 5
    int m_sonarCharge; //start with 1
    int m_goldNugget; // start with 0
};
//


//ENEMY DECLARE
class Enemy: public Actor
{
public:
    Enemy(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hitPoints);
    virtual void doSomething() override;
    virtual ~Enemy();
};
//

//BOULDE DECLARE
class Boulder: public Enemy
{
public:
    Boulder(StudentWorld* world, int startX, int startY, int imageID = TID_BOULDER, Direction dir = down, double size = 1.0, unsigned int depth = 1, int hitPoints = 1);
    ~Boulder();
    virtual void doSomething() override;
    bool ifState_stable();
    bool ifState_waiting();
    bool ifState_falling();
    void setState_stable();
    void setState_wating();
    void setState_falling();
private:
    double radius;
    double damage;
    bool watingState;
    bool stableState;
    bool fallingState;
};

//COLLECTIBLE DECLARE
class Collectible: public GraphObject
{
public:
    Collectible(StudentWorld* world, int imageID, int startX, int startY);
    virtual ~Collectible();
};
//


//EARTH DECLARE
class Earth: public Actor
{
public:
    Earth(StudentWorld* world, int startX, int startY, int imageID = TID_EARTH, Direction dir = right, double size = 0.25, unsigned int depth = 3, int hitPoints = 1);
    virtual ~Earth();
    
    virtual void doSomething();
};
#endif // ACTOR_H_
