
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

void Actor::doSomething(){};

//END ACTOR DEF


//BEGIN TUNNELMAN DEF
Tunnelman::Tunnelman(StudentWorld* world,int startX, int startY, int imageID,  int hp, int water, int sonarCharge, int goldNugget):Actor(world, imageID, startX, startY, right, 1.0, 0, hp), m_water(water), m_sonarCharge(sonarCharge), m_goldNugget(goldNugget)
{
    setVisible(true);
}

void Tunnelman::moveToDir(Direction d)
{
    const int MAX_UP = 60;
    const int MAX_DOWN = 0;
    const int MAX_LELF = 0;
    const int MAX_RIGHT = 60;
    //Move to Dir: if tunnelman is not facing to the dir, then do so. If tunnelman is facing to the dir then move by 1 unit. If it reaches the dead end then it won't move.
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
    //case top
    if (getWorld()->isEarth(getX(), getY() + 3) || getWorld()->isEarth(getX() + 1, getY() + 3) || getWorld()->isEarth(getX() + 2, getY() + 3) || getWorld()->isEarth(getX() + 3, getY() + 3) || getWorld()->isEarth(getX(), getY()) || getWorld()->isEarth(getX() + 1, getY()) || getWorld()->isEarth(getX() + 2, getY()) || getWorld()->isEarth(getX() + 3, getY()) || getWorld()->isEarth(getX(), getY()) || getWorld()->isEarth(getX(), getY() + 1) || getWorld()->isEarth(getX(), getY() + 2) || getWorld()->isEarth(getX(), getY() + 3) || getWorld()->isEarth(getX()  + 3, getY()) || getWorld()->isEarth(getX() + 3, getY() + 1) || getWorld()->isEarth(getX() + 3, getY() + 2) || getWorld()->isEarth(getX() + 3, getY() + 3))
        {
        getWorld()->digEarth(getX(), getY() + 3);
        getWorld()->digEarth(getX() + 1, getY() + 3);
        getWorld()->digEarth(getX() + 2, getY() + 3);
        getWorld()->digEarth(getX() + 3, getY() + 3);
        getWorld()->digEarth(getX(), getY());
        getWorld()->digEarth(getX() + 1, getY());
        getWorld()->digEarth(getX() + 2, getY());
        getWorld()->digEarth(getX() + 3, getY());
        getWorld()->digEarth(getX(), getY());
        getWorld()->digEarth(getX(), getY() + 1);
        getWorld()->digEarth(getX(), getY() + 2);
        getWorld()->digEarth(getX(), getY() + 3);
        getWorld()->digEarth(getX() + 3, getY());
        getWorld()->digEarth(getX() + 3, getY() + 1);
        getWorld()->digEarth(getX() + 3, getY() + 2);
        getWorld()->digEarth(getX() + 3, getY() + 3);
        return true;
    }
        return false;
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
            case KEY_PRESS_LEFT:
                //implement
                moveToDir(left);
                break;
            case KEY_PRESS_RIGHT:
                //implement
                moveToDir(right);
                break;
            case KEY_PRESS_UP:
                //implement
                moveToDir(up);
                break;
            case KEY_PRESS_DOWN:
                //implement
                moveToDir(down);
                break;
            case KEY_PRESS_SPACE:
                //implement
                break;
            case KEY_PRESS_ESCAPE:
                //implement
                getWorld()->decLives();
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
