#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    bool checkOverlap(double x, double y, int toDamage, bool toEat);
    bool checkOverlapSocrates(double x, double y, int damage);
    bool checkOverlapBacteria(double x, double y, int angle, int distOfOverlap);
    bool findFood(int x, int y, Direction& dir);
    bool findSocrates(int x, int y, int& finalX, int& finalY, Direction& dirSoc, int maxDist);
    void addActor(Actor* a);
    
    void changeNumBacteriaAliveBy(int num);
    void decNumPits();
    void increaseSocratesFlame(int num);
    void restoreSocratesHealth();
    virtual ~StudentWorld();
  private:
    std::list<Actor*> m_actors;
    Socrates* m_socrates;
    int numBacteriaAlive;
    int numPits;

};


#endif // STUDENTWORLD_H_
