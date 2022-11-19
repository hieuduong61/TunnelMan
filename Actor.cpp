#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



//Actor Def
///
Actor::Actor(int imageID, int startX, int startY, int hitPoints):m_hitPoints(hitPoints), GraphObject(imageID, startX, startY){}

Actor::~Actor(){}

int Actor::getHitpoints() const {return m_hitPoints;}

void Actor::setHitpoints(int value) {m_hitPoints = value;}


//Tunnelman Def
Tunnelman::Tunnelman(int imageID, int startX, int startY, int hitPoints, int water, int sonarCharge, int goldNugget):Actor(imageID, startX, startY, hitPoints), m_water(water), m_sonarCharge(sonarCharge), m_goldNugget(goldNugget)
{
    setVisible(true);
}

void Tunnelman::doSomething()
{
    if (getHitpoints() == 0)
        return;
    
    int ch;
    //new change
    /*if (getWorld()->getKey(ch) == true)// user hit a key this tick!
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                //implement
                break;
            case KEY_PRESS_RIGHT:
                //implement
                break;
            case KEY_PRESS_UP:
                //implement
                break;
            case KEY_PRESS_DOWN:
                //implement
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
    }*/
}

Tunnelman::~Tunnelman(){}

