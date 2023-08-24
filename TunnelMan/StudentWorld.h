#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "GraphObject.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class GraphObject;
class Earth;
class Tunnelman;

class StudentWorld : public GameWorld
{
public:

    StudentWorld(std::string assetDir) : GameWorld(assetDir), m_player(nullptr) {}

    virtual ~StudentWorld();
    
    void update_stage_info();

    string display_user_interface(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels);

    virtual int init();

    virtual int move();

    virtual void cleanUp();

    Tunnelman* get_Tunnelman();

    
    
    //Squirt
    void addSquirt(int x, int y, GraphObject::Direction dir);
    Actor* isSquirtCollidedwithProtestor(Actor* squirt);

    //Boulder
    bool isEarthBelow(int x, int y);
    Actor* isBoulderCollide(Boulder* boulder);
    bool isAnyBoulderInRadius(Actor* target);
    bool isAnyBoulderInRadius(int x, int y);

    
    //Gold Nugget
    Actor* AnyProtestorPickUpGold(Actor* target);
    void dropGold();
    
    //Sonar
    void useSonar();
    
    //Protestor
    Actor* isAnyProtestorInRadius(Actor* target);
    
    //Game Init
    void init_Player();
    void init_Earth();
    void init_Boulder();
    void init_Protestor(int ID);
    void init_HardCoreProtestor();
    void init_Water();
    void init_Oil();
    void init_Gold();
    void init_Sonar();
    void spawnSonarandWater();
    void spawnProtestor();
    
    //Helper
    bool isGoodPlaceforCollectible(int x, int y);
    bool isEarth(int x, int y);
    void digEarth(int x, int y);
    bool canMoveToHere(int x, int y);
    bool canMoveInDirection(int x, int y, GraphObject::Direction direction);
    
    //Protestor
    void set_leave_oil_field(Protestor* protestor);
    bool isProtestorInRadius(int x, int y);
    GraphObject::Direction smartFindPlayer(Protestor* protestor, int radius);

private:
    Earth* m_earth[64][64];
    Tunnelman* m_player;
    std::vector<Actor*> m_actors;
    int maxProtestor;
    int ticktoSpawnProtestor;
    int m_maze[64][64];
        struct Grid {
            int x;
            int y;
            Grid(int a, int b): x(a),y(b){}
        };
};

bool inRadius(int x_outer, int y_outer, int x_inner, int y_inner, int radius);

#endif // STUDENTWORLD_H_
