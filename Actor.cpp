#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



//BEGIN ACTOR DEF
///
Actor::Actor(int imageID, int startX, int startY, int hitPoints):m_hitPoints(hitPoints), GraphObject(imageID, startX, startY){}

Actor::~Actor(){}

int Actor::getHitpoints() const {return m_hitPoints;}

void Actor::setHitpoints(int value) {m_hitPoints = value;}
//END ACTOR DEF


//BEGIN TUNNELMAN DEF
Tunnelman::Tunnelman(int startX, int startY, int imageID,  int hitPoints, int water, int sonarCharge, int goldNugget):Actor(imageID, startX, startY, hitPoints), m_water(water), m_sonarCharge(sonarCharge), m_goldNugget(goldNugget)
{
    setVisible(true);
}

void Tunnelman::doSomething()
{
    if (getHitpoints() == 0)
        return;
    
    /*int ch;
    //change something
    if (getWorld()->getKey(ch) == true)// user hit a key this tick!
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
// END DEF TUNNELMAN


//BEGIN DEF EARTH
Earth::Earth(int startX, int startY,int imageID):GraphObject(imageID, startX, startY, left, 0.5, 3)
{
    setVisible(true);
};
Earth::~Earth()
{};

//END DEF EARTH
