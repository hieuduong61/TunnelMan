#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "StudentWorld.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



//BEGIN ACTOR DEF
///
Actor::Actor(StudentWorld* world,int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int hp):m_world(world), m_HP(hp), GraphObject(imageID, startX, startY, dir, size, depth){}

Actor::~Actor(){}

StudentWorld* Actor::getWorld(){return m_world;}

int Actor::getHP() const {return m_HP;}

void Actor::setHP(int value) {m_HP = value;}

bool Actor::m_isDead() {return ((m_HP <= 0) ? true:false);}

//END ACTOR DEF


//BEGIN TUNNELMAN DEF
Tunnelman::Tunnelman(StudentWorld* world,int startX, int startY, int imageID,  int hp, int water, int sonarCharge, int goldNugget):Actor(world, imageID, startX, startY, right, 1.0, 0, hp), m_water(water), m_sonarCharge(sonarCharge), m_goldNugget(goldNugget)
{
    setVisible(true);
}

void Tunnelman::moveToDir(Direction d)
{
    const int MAX_UP = 63;
    const int MAX_DOWN = 3;
    const int MAX_LELF = 0;
    const int MAX_RIGHT = 59;
    switch(d)
    {
        case none:
            break;
        case left:
            if (getDirection() != left)
                setDirection(d);
            else
            {
                if (getX() != MAX_LELF)
                    moveTo(getX() - 1, getY());
            }
            break;
        case right:
            if (getDirection() != right)
                setDirection(d);
            else
            {
                if (getX() != MAX_RIGHT)
                    moveTo(getX() + 1, getY());
            }
            break;
        case up:
            if (getDirection() != up)
                setDirection(d);
            else
            {
                if (getY() != MAX_UP)
                    moveTo(getX(), getY() + 1);
            }
            break;
        case down:
            if (getDirection() != down)
                setDirection(d);
            else
            {
                if (getY() != MAX_DOWN)
                    moveTo(getX(), getY() - 1);
            }
            break;
    }
}

bool Tunnelman::dig()
{
    return true;
}

void Tunnelman::doSomething()
{
    if (m_isDead())
        return;
    
    if (dig())
    {
        getWorld()->playSound(SOUND_DIG);
    }
    
    int key;
    if (getWorld()->getKey(key) == true)// user hit a key this tick!
    {
        switch (key)
        {
            case none:
                break;
            case left:
                //implement
                moveToDir(left);
                break;
            case right:
                //implement
                moveToDir(right);
                break;
            case up:
                //implement
                moveToDir(up);
                break;
            case down:
                //implement
                moveToDir(up);
                break;
            case KEY_PRESS_SPACE:
                //implement
                break;
            case KEY_PRESS_ESCAPE:
                //implement
                break;
            case KEY_PRESS_TAB:
                //implement
                break;
        }
    }
}

Tunnelman::~Tunnelman(){}
// END DEF TUNNELMAN


//BEGIN DEF EARTH
Earth::Earth(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Actor(world,imageID, startX, startY, dir, size, depth, hp)
{
    setVisible(true);
};

Earth::~Earth()
{
    setVisible(false);
};

void Earth::doSomething(){}

//END DEF EARTH
