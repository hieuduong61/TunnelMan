
#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include "algorithm"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//BEGIN ACTOR DEF
///
///
const int MAX_UP = 60;
const int MAX_DOWN = 0;
const int MAX_LEFT = 0;
const int MAX_RIGHT = 60;


//-----------------------------------------Actor----------------------------

Actor::Actor(StudentWorld* world,int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int hp):m_world(world), m_HP(hp), GraphObject(imageID, startX, startY, dir, size, depth){}

Actor::~Actor(){}

StudentWorld* Actor::getWorld(){return m_world;}

int Actor::getHP() const {return m_HP;}

void Actor::setHP(int value) {m_HP = value;}

bool Actor::m_isDead() {return ((m_HP <= 0) ? true:false);}

void Actor::doSomething(){};

void Actor::annoyedTarget(Actor* target, int hp)
{
    int soundID = target->getID();
    
    switch (soundID)
    {
        case TID_PLAYER:
            getWorld()->playSound(SOUND_PLAYER_ANNOYED);
            break;
        case TID_PROTESTER:
            getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
            break;
        case TID_HARD_CORE_PROTESTER:
            getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
            break;
    }
    target->setHP(target->getHP() - hp);
}


//-----------------------------------------END----------------------------






//-----------------------------------------Collectibles----------------------------
Collectible::Collectible(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp, float score):Actor(world, imageID, startX, startY,  dir, size, depth, hp), m_score(score)
{
}

Collectible::~Collectible()
{
}

int Collectible::getScore()
{
    return m_score;
}
//-----------------------------------------END----------------------------





//-----------------------------------------TunnelMan----------------------------
Tunnelman::Tunnelman(StudentWorld* world,int startX, int startY, int imageID,  int hp, int water, int sonarCharge, int goldNugget):Actor(world, imageID, startX, startY, right, 1.0, 0, hp), m_water(water), m_sonarCharge(sonarCharge), m_goldNugget(goldNugget)
{
    setVisible(true);
    m_oil_left = min((int)(2 + getWorld()->getLevel()), 21);
}

int Tunnelman::getWater(){return m_water;}
int Tunnelman::getSonarCharge(){return m_sonarCharge;}
int Tunnelman::getGoldNugget(){return m_goldNugget;}
int Tunnelman::getOil(){return m_oil_left;}
void Tunnelman::setWater(int value){m_water = value;}
void Tunnelman::setSonarCharge(int value){m_sonarCharge = value;}
void Tunnelman::setGoldNugget(int value){m_goldNugget = value;}
void Tunnelman::decOil(){--m_oil_left;}

void Tunnelman::moveToDir(Direction d)
{
    //Move to Dir: if tunnelman is not facing to the dir, then do so. If tunnelman is facing to the dir then move by 1 unit. If it reaches the dead end then it won't move.
    switch(d)
    {
        case none:
            moveTo(getX(),getY());
            break;
        case left:
            if (getDirection() != left)
                setDirection(d);
            else
            {
                if (getX() != MAX_LEFT && !getWorld()->isAnyBoulderInRadius(getX() - 1, getY()))
                    moveTo(getX() - 1, getY());
            }
            break;
        case right:
            if (getDirection() != right )
                setDirection(d);
            else
            {
                if (getX() != MAX_RIGHT && !getWorld()->isAnyBoulderInRadius(getX() + 1, getY()))
                    moveTo(getX() + 1, getY());
            }
            break;
        case up:
            if (getDirection() != up)
                setDirection(d);
            else
            {
                if (getY() != MAX_UP && !getWorld()->isAnyBoulderInRadius(getX(), getY() + 1))
                    moveTo(getX(), getY() + 1);
            }
            break;
        case down:
            if (getDirection() != down)
                setDirection(d);
            else
            {
                if (getY() != MAX_DOWN && !getWorld()->isAnyBoulderInRadius(getX(), getY() - 1))
                    moveTo(getX(), getY() - 1);
            }
            break;
    }
}

bool Tunnelman::dig()
{
    if (getWorld()->isEarth(getX(), getY()))
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                getWorld()->digEarth(i + getX(), j + getY());
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
                if (getWater() > 0)
                {
                    shoot(getDirection());
                    setWater(getWater() - 1);
                }
                break;
            case 'z':
                useSonar();
                break;
            case 'Z':
                useSonar();
                break;
            case KEY_PRESS_ESCAPE:
                //implement
                while (getWorld()->getLives() != 0)
                    getWorld()->decLives();
                break;
            case KEY_PRESS_TAB:
                dropGold();
                //implement
                break;
        }
    }
}

void Tunnelman::shoot(Direction d)
{
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
    switch (d) {
        case GraphObject::left:
            if (!getWorld()->isEarth(getX() - 4, getY()) && !getWorld()->isAnyBoulderInRadius(getX()-1, getY()))
            {
                getWorld()->addSquirt(getX() - 4, getY(), d);
            }
            break;
        case GraphObject::right:
            if (!getWorld()->isEarth(getX() + 4, getY()) && !getWorld()->isAnyBoulderInRadius(getX()+ 1, getY()))
            {
                getWorld()->addSquirt(getX() + 4, getY(), d);
            }
            break;
        case GraphObject::up:
            if (!getWorld()->isEarth(getX(), getY()  + 4) && !getWorld()->isAnyBoulderInRadius(getX(), getY()+ 1))
            {
                getWorld()->addSquirt(getX(), getY() + 4, d);
            }
            break;
        case GraphObject::down:
            if (!getWorld()->isEarth(getX(), getY()  - 4) && !getWorld()->isAnyBoulderInRadius(getX(), getY() - 1))
            {
                getWorld()->addSquirt(getX(), getY() - 4, d);
            }
            break;
        case none:
            break;
    }
}

void Tunnelman::useSonar()
{
    if (getSonarCharge() <= 0)
        return;
    getWorld()->useSonar();
    setSonarCharge(getSonarCharge()-1);
}

void Tunnelman::dropGold()
{
    if (getWorld()->get_Tunnelman()->getGoldNugget() > 0)
    {
        getWorld()->dropGold();
        getWorld()->get_Tunnelman()->setGoldNugget(getWorld()->get_Tunnelman()->getGoldNugget() - 1);
    }
}


Tunnelman::~Tunnelman(){}
//-----------------------------------------END----------------------------





//-----------------------------------------Earth----------------------------
Earth::Earth(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Actor(world,imageID, startX, startY, dir, size, depth, hp)
{
    setVisible(true);
};

Earth::~Earth()
{
    setVisible(false);
};

void Earth::doSomething(){}
//-----------------------------------------END----------------------------






//-----------------------------------------WaterPool----------------------------
WaterPool::WaterPool(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Collectible(world, startX, startY, imageID, dir, size, depth, hp, 100)
{
    setVisible(true);
    timeToExist = max(100, int(300 - 10*(getWorld()->getLevel())));
}

WaterPool::~WaterPool()
{
}

void WaterPool::doSomething()
{
    if (m_isDead())
        return;
    
    if (timeToExist == 0)
        setHP(0);
    
    if (inRadius(getX(), getY(), getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), 3))
    {
        getWorld()->get_Tunnelman()->setWater(getWorld()->get_Tunnelman()->getWater() + 5);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(getScore());
        annoyedTarget(this, 1);
        return;
    }
    
    --timeToExist;
}

int WaterPool::gettimeToExist()
{
    return timeToExist;
}

//-----------------------------------------END----------------------------






//-----------------------------------------Squirt----------------------------
Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction dir, int imageID, double size, unsigned int depth, int hp):Actor(world, imageID, startX, startY, dir, size, depth, hp)
{
    setVisible(true);
    m_disTraveled = 0;
}

Squirt::~Squirt()
{
    setVisible(false);
}

void Squirt::doSomething()
{
    if (m_isDead())
        return;
    
    if (m_disTraveled == 4 || getWorld()->isAnyBoulderInRadius(getX(), getY()) || getWorld()->isEarth(getX(), getY())) //Hit by boulder and earth
    {
        setHP(0);
        setVisible(false);
        return;
    }
    
    if (getWorld()->isSquirtCollidedwithProtestor(this)) //If hit protestor
    {
        setHP(0);
        Protestor* protestor = (Protestor*)getWorld()->isSquirtCollidedwithProtestor(this);
        annoyedTarget(protestor, 2);
        
        if (protestor->getHP() <= 0 && !protestor->leavingoifield())
        {
            if (protestor->getID() ==TID_PROTESTER)
                getWorld()->increaseScore(100);
            else if (protestor->getID() == TID_HARD_CORE_PROTESTER)
                getWorld()->increaseScore(250);
            protestor->set_leavingoifield();
            protestor->reset_currentticks();
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            return;
        }
        if (!protestor->leavingoifield() && protestor->getHP() > 0)
            protestor->getStunned();
        return;
    }
    
    switch (getDirection())
    {
        case left:
            if (getX() != MAX_LEFT)
            {
                moveTo(getX() - 1, getY());
                ++m_disTraveled;
            }
            break;
        case right:
            if (getX() != MAX_RIGHT)
            {
                moveTo(getX() + 1, getY());
                ++m_disTraveled;
            }
            break;
        case up:
            if (getX() != MAX_UP)
            {
                moveTo(getX(), getY() + 1);
                ++m_disTraveled;
            }
            break;
        case down:
            if (getX() != MAX_DOWN)
            {
                moveTo(getX(), getY() - 1);
                ++m_disTraveled;
            }
            break;
        case none:
            break;
    }
    
}



//-----------------------------------------END----------------------------




//-----------------------------------------Boulder----------------------------
Boulder::Boulder(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Actor(world, imageID, startX, startY,  dir, size, depth, hp)
{
    stableState = true;
    waitingState = false;
    fallingState = false;
    setVisible(true);
    countDown = 0;
}
Boulder::~Boulder()
{
    
}

bool Boulder::ifState_stable(){return stableState;}
bool Boulder::ifState_waiting(){return waitingState;}
bool Boulder::ifState_falling(){return fallingState;}
void Boulder::setState_stable(bool state){stableState = state;}
void Boulder::setState_waiting(bool state){waitingState = state;}
void Boulder::setState_falling(bool state){fallingState = state;}


void Boulder::doSomething()
{
    if (m_isDead())
        return;
    
    if (ifState_stable())
    {
        if (!getWorld()->isEarthBelow(getX(), getY()))
        {
            setState_waiting(true);
            setState_stable(false);
            return;
        }
    }
    
    if (ifState_waiting())
    {
        ++countDown;
        if (countDown == 30)
        {
            setState_falling(true);
            setState_waiting(false);
            return;
        }
    }
    
    if (ifState_falling())
    {
        moveTo(getX(), getY() - 1);
        
        if (getWorld()->isBoulderCollide(this) != nullptr)
        {
            Actor* target = getWorld()->isBoulderCollide(this);
            annoyedTarget(target, 100);
            annoyedTarget(this, 1);
            if (target->getID() == TID_PROTESTER)
            {
                getWorld()->increaseScore(500);
                Protestor* pr = (Protestor* ) target;
                pr->set_leavingoifield();
                getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            }
        }
    }
    
    return;
    
    
}

//-----------------------------------------END----------------------------





//-----------------------------------------GOLD NUGGET----------------------------
GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool pickupable, int imageID, Direction dir, double size, unsigned int depth, int hp):Collectible(world, startX, startY, imageID, dir, size, depth, hp, 10), m_pickupable(pickupable)
{
    if (m_pickupable == true)
    {
        setVisible(false);
    }
    else
    {
        setVisible(true);
        timetoExist = 100;
    }
    
}

GoldNugget::~GoldNugget()
{
    
}

void GoldNugget::doSomething()
{
    if(m_pickupable == true)
    {
        if (m_isDead())
        return;
        
        if (inRadius(getX(), getY(), getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), 4) && !isVisible())
        {
            setVisible(true);
            return;
        }
        
        if (inRadius(getX(), getY(), getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), 3) && m_pickupable)
        {
            setHP(0);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(getScore());
            getWorld()->get_Tunnelman()->setGoldNugget(getWorld()->get_Tunnelman()->getGoldNugget() + 1);
        }
    }
    else
    {
        if (m_isDead())
        return;
        if (timetoExist == 0)
            annoyedTarget(this, 1);
        if (getWorld()->AnyProtestorPickUpGold(this))
        {
            Protestor* protestor = (Protestor*)getWorld()->AnyProtestorPickUpGold(this);
            if (protestor->getID() == TID_PROTESTER)
            {
                annoyedTarget(this, 1);
                protestor->getbribed();
                
            }
            if (!isPickedup() && protestor->getID() == TID_HARD_CORE_PROTESTER)
            {
                setPickedup(true);
                protestor->getbribed();
            }
            return;
        }
        
        --timetoExist;
    }
    
    
    
    //Implement for Protestor, if Protestor pick up gold, player got 25 point
}
//-----------------------------------------END----------------------------





//-----------------------------------------SONAR----------------------------
Sonar::Sonar(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Collectible(world, startX, startY, imageID, dir, size, depth, hp, 75)
{
    setVisible(true);
    timeToExist = max(100, int(300 - 10*(getWorld()->getLevel())));
}
Sonar::~Sonar()
{
    
}
void Sonar::doSomething()
{
    if (m_isDead())
        return;
    
    if (timeToExist == 0)
        setHP(0);
    
    if (inRadius(getX(), getY(), getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), 3))
    {
        getWorld()->get_Tunnelman()->setSonarCharge(getWorld()->get_Tunnelman()->getSonarCharge() + 1);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(getScore());
        annoyedTarget(this, 1);
        return;
    }
    
    --timeToExist;
}
//-----------------------------------------END----------------------------



//-----------------------------------------SONAR----------------------------
Oil::Oil(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Collectible(world, startX, startY, imageID, dir, size, depth, hp, 1000)
{
    setVisible(false);
}
Oil::~Oil()
{
}
void Oil::doSomething()
{
    if(m_isDead())
        return;
    if (inRadius(getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), getX(), getY(), 4) && !isVisible())
    {
        setVisible(true);
        return;
    }
    if (inRadius(getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), getX(), getY(), 3))
    {
        annoyedTarget(this, 1);
        getWorld()->increaseScore(getScore());
        getWorld()->get_Tunnelman()->decOil();
        getWorld()->playSound(SOUND_FOUND_OIL);
    }
        
}
//-----------------------------------------END----------------------------



//-----------------------------------------PROTESTOR----------------------------
Protestor::Protestor(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Actor(world, imageID, startX, startY, dir, size, depth, hp)

{
    setVisible(true);
    leave_oilfield = false;
    ticksNoyell = 0;
    ticksToWaitBetweenMoves = max(0, int(3 - getWorld()->getLevel()/4));
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
    currentticks = 0;
    wait_to_turnTicks = 200;
}
Protestor::~Protestor()
{
    
}
    
void Protestor::doSomething()
{
    // if dead
    if (m_isDead())
    {
        return;
    }

    // wating state
    if (currentticks < ticksToWaitBetweenMoves)
    {
        ++currentticks;
        return;
    }
    --wait_to_turnTicks;
    currentticks = 0;
    ++ticksNoyell;
    
    if (leave_oilfield)
    {
        getWorld()->set_leave_oil_field(this);
        return;
    }
    
    //hit Player
    if (inRadius(getX(), getY(), getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), 4))
    {
        if (ticksNoyell > 15)
        {
            annoyedTarget(getWorld()->get_Tunnelman(), 2);
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            ticksNoyell = 0;
        }
        return;
    }
    
    //see player

    if (findDirectiontoPlayer() != none)
    {
        Direction d = findDirectiontoPlayer();
        if (clearedSightToPlayer(d))
        {
            setDirection(d);
            move(d);
            numSquaresToMoveInCurrentDirection = 0;
            return;
        }
    }
    //inInterSection
    if (isAtIntersection())
    {
        if (wait_to_turnTicks <= 0)
        {
            setDirection(pickRandomDir());
            randomSteptoMove();
            move(getDirection());
            wait_to_turnTicks = 200;
            return;
        }
    }
    
    //move randomly
    if (numSquaresToMoveInCurrentDirection <= 0)
    {
        setDirection(pickNewDir());
        randomSteptoMove();
        return;
    }

    move(getDirection());
    --numSquaresToMoveInCurrentDirection;
    
    if (!getWorld()->canMoveInDirection(getX(),getY(),getDirection())) {
            numSquaresToMoveInCurrentDirection = 0;
        }
    //
    
}

void Protestor::move(Direction dir)
{
    switch(dir)
    {
        case left:
            if (getDirection() != left)
                setDirection(left);
            if (getX() > MAX_LEFT && getWorld()->canMoveToHere(getX() - 1, getY()))
                moveTo(getX() - 1, getY());
            else
            {
                setDirection(right);
                randomSteptoMove();
            }
            break;
        case right:
            if (getDirection() != right)
                setDirection(right);
            if (getX() < MAX_RIGHT && getWorld()->canMoveToHere(getX() + 1, getY()))
                    moveTo(getX() + 1, getY());
            else
            {
                setDirection(left);
                randomSteptoMove();
            }
            break;
        case up:
            if (getDirection() != up)
                setDirection(up);
            if (getY() < MAX_UP && getWorld()->canMoveToHere(getX(), getY() + 1))
                    moveTo(getX(), getY() + 1);
            else
            {
                setDirection(down);
                randomSteptoMove();
            }
            break;
        case down:
            if (getDirection() != down)
                setDirection(down);
            if (getY() > MAX_DOWN && getWorld()->canMoveToHere(getX(), getY()-1))
                    moveTo(getX(), getY() - 1);
            else
            {
                setDirection(up);
                randomSteptoMove();
            }
            break;
        case none:
            moveTo(getX(),getY());
            break;
    }
}




GraphObject::Direction Protestor::findDirectiontoPlayer()
{
    if ((getX() == getWorld()->get_Tunnelman()->getX()))
    {
        if (getY() - getWorld()->get_Tunnelman()->getY() > 0)
            return down;
        else
        {
            return up;
        }
    }
    if ((getY() == getWorld()->get_Tunnelman()->getY()))
    {
        if (getX() - getWorld()->get_Tunnelman()->getX() > 0)
            return left;
        else
        {
            return right;
        }
    }
    return none;
}

void Protestor::randomSteptoMove()
{
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
}

bool Protestor::clearedSightToPlayer(Direction dir)
{
    int x = getWorld()->get_Tunnelman()->getX();
    int y = getWorld()->get_Tunnelman()->getY();
    
    switch (dir) {
        case left:
            for (int i = getX(); i >= x; --i)
                if (!getWorld()->canMoveToHere(i, getY()))
                    return false;
            return true;
            break;
        case right:
            for (int i = getX(); i <= x; ++i)
                if (!getWorld()->canMoveToHere(i, getY()))
                    return false;
            return true;
            break;
        case up:
            for (int i = getY(); i <= y; ++i)
                if (!getWorld()->canMoveToHere(getX(), i))
                    return false;
            return true;
            break;
        case down:
            for (int i = getY(); i >= y; --i)
                if (!getWorld()->canMoveToHere(getX(), i))
                    return false;
            return true;
            break;
        case none:
            return false;
    }
    return false;
}

bool Protestor::isFacingPlayer()
{
    if (getDirection() == left && getX() - getWorld()->get_Tunnelman()->getX() < 0)
        return true;
    if (getDirection() == right && getX() - getWorld()->get_Tunnelman()->getX() > 0)
        return true;
    if (getDirection() == down && getY() - getWorld()->get_Tunnelman()->getY() < 0)
        return true;
    if (getDirection() == up && getY() - getWorld()->get_Tunnelman()->getY() > 0)
        return true;
    return false;
}
    
GraphObject::Direction Protestor::pickNewDir()
{
    vector<Direction> list;
    if (getDirection() == left)
    {
        if(getWorld()->canMoveToHere(getX(), getY() - 1))
            list.push_back(down);
        if(getWorld()->canMoveToHere(getX(), getY() + 1))
            list.push_back(up);
        if(getWorld()->canMoveToHere(getX() +1, getY()))
            list.push_back(right);
    }
    if (getDirection() == right)
    {
        if(getWorld()->canMoveToHere(getX(), getY() - 1))
            list.push_back(down);
        if(getWorld()->canMoveToHere(getX(), getY() + 1))
            list.push_back(up);
        if(getWorld()->canMoveToHere(getX() -1, getY()))
            list.push_back(left);
    }
    if (getDirection() == up)
    {
        if(getWorld()->canMoveToHere(getX() -1, getY()))
            list.push_back(left);
        if(getWorld()->canMoveToHere(getX() +1, getY()))
            list.push_back(right);
        if(getWorld()->canMoveToHere(getX(), getY() - 1))
            list.push_back(down);
    }
        
    if (getDirection() == down)
    {
        if(getWorld()->canMoveToHere(getX() -1, getY()))
            list.push_back(left);
        if(getWorld()->canMoveToHere(getX() +1, getY()))
            list.push_back(right);
        if(getWorld()->canMoveToHere(getX() + 1, getY()))
            list.push_back(up);
    }
    if (list.empty())
        return getDirection();
    else
        return list.at(rand() % list.size());
}

bool Protestor::isAtIntersection()
{
    if (getDirection() == left)
        if (getWorld()->canMoveToHere(getX(), getY() - 1) || getWorld()->canMoveToHere(getX(), getY() + 1))
            return true;
    if (getDirection() == right)
        if (getWorld()->canMoveToHere(getX(), getY() - 1) || getWorld()->canMoveToHere(getX(), getY() + 1))
            return true;
    if (getDirection() == up)
        if (getWorld()->canMoveToHere(getX() - 1, getY()) || getWorld()->canMoveToHere(getX() + 1, getY()))
            return true;
    if (getDirection() == left)
        if (getWorld()->canMoveToHere(getX() - 1, getY()) || getWorld()->canMoveToHere(getX() + 1, getY()))
            return true;
    
    return false;
}

GraphObject::Direction Protestor::pickRandomDir()
{
    vector<Direction> list;
    if (getDirection() == left || getDirection() == right)
    {
        if(getWorld()->canMoveToHere(getX(), getY() - 1))
            list.push_back(down);
        if(getWorld()->canMoveToHere(getX(), getY() + 1))
            list.push_back(up);
    }
    if (getDirection() == up || getDirection() == down)
    {
        if(getWorld()->canMoveToHere(getX() -1, getY()))
            list.push_back(left);
        if(getWorld()->canMoveToHere(getX() +1, getY()))
            list.push_back(right);
    }
    if (list.empty())
        return getDirection();
    else
        return list.at(rand() % list.size());
}

void Protestor::getStunned()
{
    currentticks = 0 - max(50, 100 - (int)getWorld()->getLevel()*10);
}

void Protestor::getbribed()
{
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    leave_oilfield = true;
}


bool Protestor::m_isDead()
{
    if (getX() == 60 && getY() == 60 && leave_oilfield)
        return true;
    return false;
}

 // 8<=   <=60
//-----------------------------------------END----------------------------





//-----------------------------------------HARDCORE_PROTESTOR----------------------------
Hardcore_Protestor::Hardcore_Protestor(StudentWorld* world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth, int hp):Protestor(world, startX, startY, imageID)
{
    setHP(20);
    ticksToWaitBetweenMoves = max(0, (int)(3 - getWorld()->getLevel()/4));
}

Hardcore_Protestor::~Hardcore_Protestor()
{
    
}

void Hardcore_Protestor::getStunned()
{
    currentticks = 0 - max(50, (int)(100 - getWorld()->getLevel() * 10));
}

void Hardcore_Protestor::getbribed()
{
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    getStunned();
}


void Hardcore_Protestor::doSomething()
{
    // if dead
    if (m_isDead())
    {
        return;
    }
    if (getHP() <= 0 && !leave_oilfield)
    {
        leave_oilfield = true;
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        return;
    }
    
    
    // wating state
    if (currentticks < ticksToWaitBetweenMoves)
    {
        ++currentticks;
        return;
    }
    
    wait_to_turnTicks--;
    currentticks = 0;
    ++ticksNoyell;
    
    if (leave_oilfield)
    {
        getWorld()->set_leave_oil_field(this);
        return;
    }
    
    //hit Player
    if (inRadius(getX(), getY(), getWorld()->get_Tunnelman()->getX(), getWorld()->get_Tunnelman()->getY(), 4))
    {
        if (ticksNoyell > 15)
        {
            annoyedTarget(getWorld()->get_Tunnelman(), 2);
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            ticksNoyell = 0;
        }
        return;
    }
    
    //see player
    if (findDirectiontoPlayer() != none)
    {
        Direction d = findDirectiontoPlayer();
        if (clearedSightToPlayer(d))
        {
            setDirection(d);
            move(d);
            numSquaresToMoveInCurrentDirection = 0;
            return;
        }
    }
    
    //smart find player
    int radius = 16 + int(getWorld()->getLevel());
    Direction d = getWorld()->smartFindPlayer(this, radius);
    if (d != none)
    {
        move(d);
        return;
    }
    
    //inInterSection
    if (isAtIntersection())
    {
        cout << "Waiting" << wait_to_turnTicks << endl;
        if (wait_to_turnTicks <= 0)
        {
            setDirection(pickRandomDir());
            randomSteptoMove();
            move(getDirection());
            wait_to_turnTicks = 200;
            return;
        }
    }
    
    //move randomly
    if (numSquaresToMoveInCurrentDirection <= 0)
    {
        setDirection(pickNewDir());
        randomSteptoMove();
        return;
    }

    move(getDirection());
    --numSquaresToMoveInCurrentDirection;
    
    if (!getWorld()->canMoveInDirection(getX(),getY(),getDirection())) {
            numSquaresToMoveInCurrentDirection = 0;
        }

    
    
    //
    
}
