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

    ~StudentWorld() {
        cleanUp();
    }

    Actor* get_Tunnelman();

    bool isEarth(int x, int y);

    void digEarth(int x, int y);

    void update_stage_info();

    string display_user_interface(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels);

    virtual int init();

    virtual int move();

    virtual void cleanUp();

private:
    Earth* m_earth[64][64];
    Actor* m_player;
    std::vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
