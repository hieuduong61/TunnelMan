#include "StudentWorld.h"
#include <string>
#include <queue>
using namespace std;

//-----------------------------------------StudentWorld----------------------------
GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::update_stage_info() {
    int score = getScore();
    int level = getLevel();
    int live = getLives();
    int health = m_player->getHP();
    int water = m_player->getWater(); //temporary
    int gold = m_player->getGoldNugget();
    int sonar = m_player->getSonarCharge();
    int barrels = m_player->getOil();

    string info = display_user_interface(score, level, live, health, water, gold, sonar, barrels);
    setGameStatText(info);
}

string StudentWorld::display_user_interface(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels) {
    stringstream s;

    s.fill('0');
    s << "Scr: ";
    s << setw(6) << score;

    s.fill(' ');
    s << " Lvl: ";
    s << setw(2) << level;

    s << " Lives: ";
    s << setw(1) << lives;

    s << "  Hlth: ";
    s << setw(3) << health * 10;
    s << '%';

    s << "  Wtr: ";
    s << setw(2) << squirts;

    s << "  Gld: ";
    s << setw(2) << gold;

    s << "  Sonar: ";
    s << setw(2) << sonar;

    s << "  Oil Left: ";
    s << setw(2) << barrels;

    return s.str();
}

int StudentWorld::init()
{
    maxProtestor = min(15,(int)(2 + getLevel() * 1.5));
    ticktoSpawnProtestor = max(25, (int)(200 - getLevel()));
    
    init_Player();
    init_Earth();
    init_Boulder();
    init_Gold();
    init_Oil();
    init_Protestor(TID_PROTESTER);
    --maxProtestor;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    update_stage_info();
    
    //Dead Check
    
    if (!isGameOver())
    {
        if (m_player->getOil() == 0)
            return GWSTATUS_FINISHED_LEVEL;
        
        if (!m_player->m_isDead())
            m_player->doSomething();
        else
        {
            decLives();
            playSound(SOUND_PLAYER_GIVE_UP);
            return GWSTATUS_PLAYER_DIED;
        }
        
        //add Protestor
        --ticktoSpawnProtestor;
        if (ticktoSpawnProtestor < 0)
        {
            spawnProtestor();
            ticktoSpawnProtestor = max(25, (int)(200 - getLevel()));
        }
        
        
        vector<Actor*>::iterator itr = m_actors.begin();
        while (itr != m_actors.end())
        {
            if (!(*itr)->m_isDead())
                (*itr)->doSomething();
            ++itr;
        }
        
        vector<Actor*>::iterator itrDel = m_actors.begin();
        while (itrDel != m_actors.end())
        {
            if ((*itrDel)->m_isDead())
            {
                delete *itrDel;
                itrDel = m_actors.erase(itrDel);
            }
            else ++itrDel;
        }
        
        spawnSonarandWater();
        
        return GWSTATUS_CONTINUE_GAME;
    }
    else
    {
        return GWSTATUS_PLAYER_DIED;
    }
}

void StudentWorld::cleanUp(){
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 60; y++) {
            delete m_earth[x][y];
            m_earth[x][y] = nullptr;
        }
    }
    
    //destruct Actor
    vector<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end())
    {
        delete (*itr);
        ++itr;
    }
    m_actors.clear();
    
    delete m_player;
    m_player = nullptr;
    
}
//-----------------------------------------END----------------------------





//-----------------------------------------Squirt----------------------------
void StudentWorld::addSquirt(int x, int y, GraphObject::Direction dir)
{
    Actor* squirt = new Squirt(this, x, y, dir);
    m_actors.push_back(squirt);
}

Actor* StudentWorld::isSquirtCollidedwithProtestor(Actor* squirt)
{
    vector<Actor*>::iterator itr = m_actors.begin();

    while (itr != m_actors.end())
    {
        if (inRadius(squirt->getX(), squirt->getY(), (*itr)->getX(), (*itr)->getY(), 3) && ((*itr)->getID() == TID_PROTESTER || (*itr)->getID() == TID_HARD_CORE_PROTESTER))
            {
                return *itr;
            }
        ++itr;
    }
    return nullptr;
}

//-----------------------------------------END----------------------------






//------------------------------------------Boulder---------------------------
Actor* StudentWorld::isBoulderCollide(Boulder* boulder)
{
    vector<Actor*>::iterator itr = m_actors.begin();

    while (itr != m_actors.end()) // if hit someone
    {
        if (inRadius(boulder->getX(), boulder->getY(), (*itr)->getX(), (*itr)->getY(), 3) && ((*itr)->getID() == TID_PROTESTER || (*itr)->getID() == TID_HARD_CORE_PROTESTER))
            return *itr;
        if ((inRadius(boulder->getX(), boulder->getY(), m_player->getX(), m_player->getY(), 3)))
            return m_player;
        if ((*itr)->getID() == TID_BOULDER && (*itr != boulder) && inRadius((*itr)->getX(), (*itr)->getY(), boulder->getX(), boulder->getY(), 3)) // if hit another boulder
        {
            boulder->annoyedTarget(boulder, 1);
            boulder->setState_falling(false);
            return nullptr;
        }
        ++itr;
    }
    if (isEarthBelow(boulder->getX(), boulder->getY()) || boulder->getY() - 1 == 0)// if touch the ground
    {
        boulder->annoyedTarget(boulder, 1);
        boulder->setState_falling(false);
        return nullptr;
    }
    
    return nullptr;
}

bool StudentWorld::isEarthBelow(int x, int y)
{
    for (int i = 0; i < 4; ++i)
        if (m_earth[x+i][y-1]->isVisible())
            return true;
    return false;
}

bool StudentWorld::isAnyBoulderInRadius(Actor* target)
{
    vector<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end())
    {
        if (inRadius(target->getX(), target->getY(), (*itr)->getX(), (*itr)->getY(), 3) && ((*itr)->getID() == TID_BOULDER))
            return true;
        ++itr;
    }
    return false;
}

bool StudentWorld::isAnyBoulderInRadius(int x, int y)
{
    vector<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end())
    {
        if ((*itr)->getID() == TID_BOULDER)
            for (int i = -3; i < 4; ++i)
                for (int j = -3; j < 4; ++j)
                    if(x+i > 0 && x+i < 60 && y+j >0 && y+j < 60)
                        if (x == ((*itr)->getX()+i) && y == (*itr)->getY()+j)
                            return true;
        ++itr;
    }
    return false;
}

//-----------------------------------------END----------------------------







//------------------------------------------Earth---------------------------

bool StudentWorld::isEarth(int x, int y)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (m_earth[x+i][y+j]->isVisible())
                return true;
    return false;
}

void StudentWorld::digEarth(int x, int y)
{
    if (m_earth[x][y]->isVisible()) {
        m_earth[x][y]->setVisible(false);
    }
}

//-----------------------------------------END----------------------------






//------------------------------------------TunnelMan---------------------------

Tunnelman* StudentWorld::get_Tunnelman(){return m_player;}

//-----------------------------------------END----------------------------





//-----------------------------------------GOLD NUGGET----------------------------
Actor* StudentWorld::AnyProtestorPickUpGold(Actor* target)
{
    vector<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end())
    {
        if (inRadius(target->getX(), target->getY(), (*itr)->getX(), (*itr)->getY(), 4) && (((*itr)->getID() == TID_PROTESTER) || (*itr)->getID() == TID_HARD_CORE_PROTESTER))
            return *itr;
        ++itr;
    }
    
    return nullptr;
}

void StudentWorld::dropGold()
{
    m_actors.push_back(new GoldNugget(this, get_Tunnelman()->getX(), get_Tunnelman()->getY(), false));
}


//-----------------------------------------END----------------------------



//-----------------------------------------SONAR----------------------------
void StudentWorld::useSonar()
{
    vector<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end())
    {
        if (inRadius(m_player->getX(), m_player->getY(), (*itr)->getX(), (*itr)->getY(), 12))
            (*itr)->setVisible(true);
        ++itr;
    }
}
//---------------------------------------END------------------------------




//---------------------------------------INIT------------------------------
void StudentWorld::init_Player()
{
    m_player = new Tunnelman(this, 30, 60);
}
void StudentWorld::init_Earth()
{
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 64; y++) {
                m_earth[x][y] = new Earth(this, x, y);
                if (((x >= 30 && x <=33) && y > 3) || y > 59)
                    m_earth[x][y]->setVisible(false);
        }
    }
}
void StudentWorld::init_Boulder()
{
    int B = min((int)(getLevel() / 2 + 2), 9);
    for (int  i = 0; i < B; ++i)
    {
        int x = -1;
        int y = -1;
        do
        {
            x = rand() % 61;
            y = rand() % 61;
        } while (!isGoodPlaceforCollectible(x, y) || (x > 26 && x < 34) || (y > 55) || (y < 3));
            
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m_earth[x+i][y+j]->setVisible(false);
        Actor* boulder = new Boulder(this, x, y);
        m_actors.push_back(boulder);
    }
}
void StudentWorld::init_Protestor(int ID)
{
    if (ID == TID_PROTESTER)
        m_actors.push_back(new Protestor(this, 60, 60, ID));
    if (ID == TID_HARD_CORE_PROTESTER)
        m_actors.push_back(new Hardcore_Protestor(this, 60, 60, ID));
}
void StudentWorld::init_HardCoreProtestor()
{
}
void StudentWorld::init_Water()
{
    int x = -1;
    int y = -1;
    
    do
    {
        x = rand() % 61;
        y = rand() % 61;
    }   while (!isGoodPlaceforCollectible(x, y) || isEarth(x, y));
    
    m_actors.push_back(new WaterPool(this, x, y));
}
void StudentWorld::init_Oil()
{
    int L = min((int)(2 + getLevel()), 21);
    for (int i = 0; i < L; ++i)
    {
        int x = -1;
        int y = -1;
        
        do
        {
            x = rand() % 61;
            y = rand() % 61;
        }   while (!isGoodPlaceforCollectible(x, y));
        
        m_actors.push_back(new Oil(this, x, y));
    }
}
void StudentWorld::init_Gold()
{
    int G = max((int)(5-getLevel() / 2), 2);
    for (int i = 0; i < G; ++i)
    {
        int x = -1;
        int y = -1;
        
        do
        {
            x = rand() % 61;
            y = rand() % 61;
        }   while (!isGoodPlaceforCollectible(x, y) || (x > 26 && x < 34) || (y > 55) || (y < 3));
        
        m_actors.push_back(new GoldNugget(this, x, y, true));
    }
}
void StudentWorld::init_Sonar()
{
    int x = -1;
    int y = -1;
    
    do
    {
        x = rand() % 61;
        y = rand() % 61;
    }   while (!isGoodPlaceforCollectible(x, y) || isEarth(x, y));
    
    m_actors.push_back(new Sonar(this, x, y));
}


void StudentWorld::spawnSonarandWater()
{
    int G = getLevel() * 25 + 300;
    if (1 == rand() % G + 1)
    {
        if (rand() % 5+1 == 1)
            init_Sonar();
        else
            init_Water();
    }
}

void StudentWorld::spawnProtestor()
{
    if (maxProtestor > 0)
    {
        int probabilityOfHardcore = min(90, (int)(getLevel() * 10 + 30));
        int randomProb = rand() & 100 + 1;
        if (randomProb <= probabilityOfHardcore)
            init_Protestor(TID_HARD_CORE_PROTESTER);
        else
            init_Protestor(TID_PROTESTER);
        --maxProtestor;
    }
}
//-----------------------------------------END----------------------------


//-----------------------------------------HELPER----------------------------
bool StudentWorld::isGoodPlaceforCollectible(int x, int y)
{
    vector<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end())
    {
        if (inRadius(x, y, (*itr)->getX(), (*itr)->getY(), 6) && ((*itr)->getID() == TID_GOLD || (*itr)->getID() == TID_WATER_POOL || (*itr)->getID() == TID_SONAR || (*itr)->getID() == TID_BARREL || (*itr)->getID() == TID_BOULDER))
            return false;
        ++itr;
    }
        return true;
}

bool inRadius(int x_outer, int y_outer, int x_inner, int y_inner, int radius)
{
    if (((x_outer - x_inner)*(x_outer - x_inner) + (y_outer - y_inner)*(y_outer - y_inner)) <= radius*radius)
        return true;

    return false;
}

bool StudentWorld::canMoveToHere(int x, int y)
{
    if ((x <= 60 && x>=0) && (y <= 60 && y>=0) && !isEarth(x, y) && !isAnyBoulderInRadius(x, y))
        return true;
    return false;
}

bool StudentWorld::canMoveInDirection(int x, int y, GraphObject::Direction direction)
{
    switch (direction) {
    case GraphObject::left:
        return (x!=0&&!isEarth(x - 1, y) && !isAnyBoulderInRadius(x, y));
    case GraphObject::right:
        return (x!=60&& !isEarth(x + 1, y) && !isAnyBoulderInRadius(x, y));
    case GraphObject::up:
        return (y!=60&&!isEarth(x, y + 1) && !isAnyBoulderInRadius(x, y));
    case GraphObject::down:
        return (y!=0&& !isEarth(x, y - 1) && !isAnyBoulderInRadius(x, y));
    case GraphObject::none:
            return false;
    }
    return false;
}


//-----------------------------------------END----------------------------




//-----------------------------------------PROTESTOR---------------------------


void StudentWorld::set_leave_oil_field(Protestor* protestor)
{
    
    for (int i = 0; i < 64; i++){
            for (int j = 0; j < 64; j++){
                m_maze[i][j]=0;
            }
        }
        int a =protestor->getX();
        int b =protestor->getY();
        queue<Grid> q;
        q.push(Grid(60,60));
        m_maze[60][60]=1;
        while (!q.empty()) {
            Grid c = q.front();
            q.pop();
            int x=c.x;
            int y=c.y;

            if(canMoveInDirection(x,y, GraphObject::left)&& m_maze[x-1][y]==0){
                q.push(Grid(x-1,y));
                m_maze[x-1][y] =m_maze[x][y]+1;
            }
            if(canMoveInDirection(x,y, GraphObject::right)&& m_maze[x+1][y]==0){
                q.push(Grid(x+1,y));
                m_maze[x+1][y] =m_maze[x][y]+1;
            }
            if(canMoveInDirection(x,y, GraphObject::up)&& m_maze[x][y+1]==0){
                q.push(Grid(x,y+1));
                m_maze[x][y+1] =m_maze[x][y]+1;
            }
            if(canMoveInDirection(x,y, GraphObject::down)&& m_maze[x][y-1]==0){
                q.push(Grid(x,y-1));
                m_maze[x][y-1] =m_maze[x][y]+1;
            }
        }
        if(canMoveInDirection(a,b, GraphObject::left)&& m_maze[a-1][b]<m_maze[a][b])
            protestor->move(GraphObject::left);
        if(canMoveInDirection(a,b, GraphObject::right)&& m_maze[a+1][b]<m_maze[a][b])
            protestor->move(GraphObject::right);
        if(canMoveInDirection(a,b, GraphObject::up)&& m_maze[a][b+1]<m_maze[a][b])
            protestor->move(GraphObject::up);
        if(canMoveInDirection(a,b, GraphObject::down)&&m_maze[a][b-1]<m_maze[a][b])
            protestor->move(GraphObject::down);
        return ;}

bool StudentWorld::isProtestorInRadius(int x, int y)
{
    vector<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end())
    {
        if ((inRadius(x, y, (*itr)->getX(), (*itr)->getY(), 3) && ((*itr)->getID() == TID_PROTESTER)) || ((*itr)->getID() == TID_HARD_CORE_PROTESTER))
            return true;
        ++itr;
    }
    return false;
}


GraphObject::Direction StudentWorld::smartFindPlayer(Protestor* protestor, int radius)
{
    
    for (int i = 0; i < 64; i++){
        for (int j = 0; j < 64; j++){
            m_maze[i][j]=0;
        }
    }
    int a =protestor->getX();
    int b =protestor->getY();
    queue<Grid> q;
    q.push(Grid(m_player->getX(),m_player->getY()));
    m_maze[m_player->getX()][m_player->getY()]=1;
    while (!q.empty()) {
        Grid c = q.front();
        q.pop();
        int x=c.x;
        int y=c.y;
        
        //left
        if(canMoveInDirection(x,y, GraphObject::left)&& m_maze[x-1][y]==0){
            q.push(Grid(x-1,y));
            m_maze[x-1][y] =m_maze[x][y]+1;
        }
        //right
        if(canMoveInDirection(x,y, GraphObject::right)&& m_maze[x+1][y]==0){
            q.push(Grid(x+1,y));
            m_maze[x+1][y] =m_maze[x][y]+1;
        }
        //up
        if(canMoveInDirection(x,y, GraphObject::up)&& m_maze[x][y+1]==0){
            q.push(Grid(x,y+1));
            m_maze[x][y+1] =m_maze[x][y]+1;
        }
        // down
        if(canMoveInDirection(x,y, GraphObject::down)&& m_maze[x][y-1]==0){
            q.push(Grid(x,y-1));
            m_maze[x][y-1] =m_maze[x][y]+1;
        }
    }
    if(m_maze[a][b] <= radius+1){
        if(canMoveInDirection(a,b, GraphObject::left)&& m_maze[a-1][b]<m_maze[a][b])
            return GraphObject::left;
        if(canMoveInDirection(a,b, GraphObject::right)&& m_maze[a+1][b]<m_maze[a][b])
            return GraphObject::right;
        if(canMoveInDirection(a,b, GraphObject::up)&& m_maze[a][b+1]<m_maze[a][b])
            return GraphObject::up;
        if(canMoveInDirection(a,b, GraphObject::down)&&m_maze[a][b-1]<m_maze[a][b])
            return GraphObject::down;
    }
    return GraphObject::none;
}

//-----------------------------------------END----------------------------

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
