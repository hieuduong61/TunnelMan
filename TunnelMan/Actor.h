#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
//Actor Declare
class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int hp);
    
    virtual ~Actor();
    
    virtual StudentWorld* getWorld();
    virtual int getHP() const;
    virtual void setHP(int value);
    virtual void doSomething();
    virtual bool m_isDead();
    virtual void annoyedTarget(Actor* target, int hp);

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
    virtual void moveToDir(Direction d);
    virtual bool dig();
    virtual void shoot(Direction d);
    virtual void useSonar();
    virtual void dropGold();
    
    virtual int getWater();
    virtual int getSonarCharge();
    virtual int getGoldNugget();
    virtual int getOil();
    virtual void decOil();
    virtual void setWater(int value);
    virtual void setSonarCharge(int value);
    virtual void setGoldNugget(int value);
    
    
private:
    int m_water; //start with 5
    int m_sonarCharge; //start with 1
    int m_goldNugget; // start with 0
    int m_oil_left;
};
//

//COLLECTIBLE DECLARE
class Collectible: public Actor
{
public:
    Collectible(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int hp, float score);
    virtual ~Collectible();
    virtual int getScore();
private:
    StudentWorld* m_world;
    float m_score;
};
//

//BOULDE DECLARE
class Boulder: public Actor
{
public:
    Boulder(StudentWorld* world, int startX, int startY, int imageID = TID_BOULDER, Direction dir = down, double size = 1.0, unsigned int depth = 1, int hp = 1);
    ~Boulder();
    virtual void doSomething() override;
    bool ifState_stable();
    bool ifState_waiting();
    bool ifState_falling();
    void setState_stable(bool state);
    void setState_waiting(bool state);
    void setState_falling(bool state);
private:
    double damage;
    bool waitingState;
    bool stableState;
    bool fallingState;
    int countDown;
    
};




//EARTH DECLARE
class Earth: public Actor
{
public:
    Earth(StudentWorld* world, int startX, int startY, int imageID = TID_EARTH, Direction dir = right, double size = 0.25, unsigned int depth = 3, int hp = 1);
    virtual ~Earth();
    
    virtual void doSomething() override;
};


//WATER POOL DECLARE
class WaterPool: public Collectible
{
public:
    WaterPool(StudentWorld* world, int startX, int startY, int imageID = TID_WATER_POOL, Direction dir = right, double size = 1.0, unsigned int depth = 2, int hp = 1);
    virtual ~WaterPool();
    virtual void doSomething() override;
    virtual int gettimeToExist();
private:
    int timeToExist;
};


//SQUIRT DECLARE
class Squirt: public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction dir, int imageID = TID_WATER_SPURT, double size = 1.0, unsigned int depth = 1, int hp = 1);
    ~Squirt();
    virtual void doSomething() override;
    
private:
    int m_disTraveled;
};

//


//GOLD NUGGET DECLARE

class GoldNugget: public Collectible
{
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool pickupable, int imageID = TID_GOLD, Direction dir = right, double size = 1.0, unsigned int depth = 2, int hp = 1);
    virtual ~GoldNugget();
    virtual void doSomething() override;
    bool isPickupByProtestor(Actor* protestor);
    void setPickedup(bool val) {isPickup = val;}
    bool isPickedup() {return isPickup;}

private:
    int timetoExist;
    bool m_pickupable;
    bool isPickup = false;
};


//SONAR CHARGE DECLARE

class Sonar:public Collectible
{
public:
    Sonar(StudentWorld* world, int startX, int startY, int imageID = TID_SONAR, Direction dir = right, double size = 1.0, unsigned int depth = 2, int hp = 1);
    virtual ~Sonar();
    virtual void doSomething() override;
private:
    int timeToExist;
};

//OIL DECLARE
class Oil:public Collectible
{
public:
    Oil(StudentWorld* world, int startX, int startY, int imageID = TID_BARREL, Direction dir = right, double size = 1.0, unsigned int depth = 2, int hp = 1);
    virtual ~Oil();
    virtual void doSomething() override;
};

//PROTESTOR DECLARE
class Protestor: public Actor
{
public:
    Protestor(StudentWorld* world, int startX, int startY, int imageID, Direction dir = right, double size = 1.0, unsigned int depth = 0, int hp = 5);
    virtual ~Protestor();
    
    virtual void getStunned();
    virtual void doSomething() override;
    virtual Direction findDirectiontoPlayer();
    virtual void getbribed();
    void move(Direction dir);
    bool leavingoifield() {return leave_oilfield;}
    void set_leavingoifield() {leave_oilfield = true;}
    void reset_currentticks() {currentticks = 0;}
protected:
    bool leave_oilfield;
    int ticksNoyell;
    int ticksToWaitBetweenMoves;
    int currentticks;
    int numSquaresToMoveInCurrentDirection;
    int wait_to_turnTicks;
    void randomSteptoMove();// 8<=   <=60
    bool clearedSightToPlayer(Direction dir);
    bool isFacingPlayer();
    Direction pickNewDir();
    bool isAtIntersection();
    Direction pickRandomDir();
    virtual bool m_isDead() override;

    
};


//HARDCORE PROTESTOR

class Hardcore_Protestor: public Protestor
{
public:
    Hardcore_Protestor(StudentWorld* world, int startX, int startY, int imageID, Direction dir = right, double size = 1.0, unsigned int depth = 0, int hp = 20);
    ~Hardcore_Protestor();
    virtual void getStunned() override;
    virtual void getbribed() override;
    virtual void doSomething() override;
private:
};

#endif // ACTOR_H_
